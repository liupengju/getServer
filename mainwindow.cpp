#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateSerXml();//从fileList.xml 获取所有的文件名称保存到成员变量filelist中。
    model = new QStandardItemModel(ui->serListTree);
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("接口文件"));
    connect(ui->setBtn,SIGNAL(clicked(bool)),this,SLOT(showConfig()));
    xmlThread = new xmlAnalysisThread(fileList);
    xmlDialog = new xmlAnalysisDialog(fileList,this);
    xls = new xlsAxObj();
    _mConfig = new Config;
    // updSer2Xml();
}

void MainWindow::analysisSerFile()
{
    QXmlStreamWriter xml;
    QFile file("G:\\SystemFile\\Documents\\GitHub\\getServer\\src\\serSet.xml");
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug()<<QString("Cannot write file.");
        return ;
    }
    xml.setDevice(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("Root");

    QProgressDialog *progressDlg=new QProgressDialog(this);
    QFont font("ZYSong18030",12);
    progressDlg->setFont(font);
    progressDlg->setWindowModality(Qt::WindowModal);
    progressDlg->setMinimumDuration(5);
    progressDlg->setWindowTitle(tr("please wait"));
    progressDlg->setLabelText(tr("正在分析......      "));
    progressDlg->setCancelButtonText(tr("取消"));
    int num = fileList.count();
    progressDlg->setRange(0,num);

    for(int i = 0;i < fileList.count();i++)
    {
        QString fileName = fileList.at(i);
        xml.writeStartElement("InterFaceFile");
        xml.writeAttribute("FileName",fileName);
        getSerInfoFromxls(xml,fileName);
        progressDlg->setValue(i);
        xml.writeEndElement();
    }
    QMessageBox::information(this,"","接口文件分析结束",QMessageBox::Yes,QMessageBox::Yes);
    xml.writeEndDocument();
}

void MainWindow::getSerInfoFromxls(QXmlStreamWriter &xml,QString fileName)
{
    qDebug()<<fileName;
    xls->open(fileName);
   // xls->setVisible(true);
    int sheetCount = xls->getSheetsCount();
    for(int i = 1;i < sheetCount+1;i++)        //因为表格数量并不多。没必要设置标志位在找到INDEX表格后退出。
    {
        if("INDEX" == xls->getSheetName(i))
        {
            int columnCount = xls->getColumnCount(xls->getSheet(i));
            if(columnCount > 20)
                columnCount = 20;
            int rowCount = xls->getRowCount(xls->getSheet(i));
            int columnStart = xls->getColumnStart(xls->getSheet(i));
            int rowStart = xls->getRowStart(xls->getSheet(i));

            //获取服务代码，服务场景，服务版本，服务链接所在列
            int codeColumn = 0;
            int sceneColumn = 0;
            int versionColumn = 0;
            int linkColumn = 0;
            bool gotServerColumn = false; //标志位:是否已经找到SERVER code,scene,version,link所在列
            int contentStartRow = 0; //正文起始行，xsl中INDEX表中标题行["识别码"，"交易名称"...所在那一行]下一行
            for(int row = 0;row < rowCount; row++ )
            {
                if(gotServerColumn)
                    break;
                for(int column = 0; column < columnCount; column ++)
                {
                    if( xls->getCell(row+rowStart,column+columnStart).toString().contains("服务名称"))
                        codeColumn = column+columnStart-1;
                    if("服务场景" == xls->getCell(row+rowStart,column+columnStart).toString())
                        sceneColumn = column+columnStart-1;
                    if("服务版本" == xls->getCell(row+rowStart,column+columnStart).toString())
                        versionColumn = column+columnStart-1;
                    if("链接" == xls->getCell(row+rowStart,column+columnStart).toString())
                    {
                        linkColumn = column+columnStart-1;
                        gotServerColumn = true;
                        contentStartRow = row+1;
                        break;
                    }
                }
            }
            QString sCode;
            QString sScene;
            QString sVersion;
            QString sLink;
            for(int row = contentStartRow;row < rowCount; row++ )
            {

                for(int column = 0; column < columnCount; column++)
                {
                    if(column == codeColumn)
                        sCode = xls->getCell(row+rowStart,column+columnStart).toString().section('\n',0,0);
                    else if(column == sceneColumn)
                        sScene = xls->getCell(row+rowStart,column+columnStart).toString().section('\n',0,0);

                    else if(column == versionColumn)
                        sVersion = xls->getCell(row+rowStart,column+columnStart).toString();
                    else if(column == linkColumn)
                        sLink = xls->getCell(row+rowStart,column+columnStart).toString();
                    else if(column < columnCount-1)
                    {
                        if(!sCode.isEmpty())
                        {
                            //插入数据到xml配置文件中，函数有问题，暂时不用
                            //upDateVerList(sCode,sScene,sVersion,fileName);
                            {
                                xml.writeStartElement("SERVER");
                                xml.writeAttribute("SerCode",sCode);
                                xml.writeAttribute("SerScene",sScene);
                                xml.writeAttribute("SerVersion",sVersion);
                                xml.writeAttribute("fildLink",sLink);
                                xml.writeEndElement();
                                sCode.clear();
                                sScene.clear();
                                sVersion.clear();
                                sLink.clear();
                            }
                        }
                    }
                }

            }
        }
    }
    xls->close();
}

int MainWindow::updateSerXml()//从filelist中获取接口文件名称并保存在fileList中
{
    QXmlStreamReader xml;
    QFile file("G:\\SystemFile\\Documents\\GitHub\\getServer\\src\\fileList.xml");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<QString("Cannot write file.");
        return -1;
    }
    xml.setDevice(&file);

    // xml.StartDocument();
    while(!xml.atEnd())
    {
        xml.readNext();
        if(xml.isStartElement())
        {
            if(xml.name() == "file")
            {
                QString str = xml.readElementText();
                fileList.append(str);
            }
        }
    }
    return 0;
}

void MainWindow::searchSer()
{
    model->clear();
    ui->serListTree->setModel(model);
    QStandardItem* itemFile;

    QString serCode = ui->serCodeEdit->text();
    QString serScene = ui->serSceneEdit->text();
    QString serVersion = ui->serVersionEdit->text();
    bool serFileItemCreated = false;
    bool searchCondition;
    QString fileName;
    QString serFile;
    if(serCode.isEmpty())
    {
        QMessageBox::information(this,"","服务代码不能为空",QMessageBox::Yes,QMessageBox::Yes);
        return;
    }
    if(serScene.isEmpty())
    {
        QMessageBox::information(this,"","服务场景不能为空",QMessageBox::Yes,QMessageBox::Yes);
        return;
    }

    QXmlStreamReader xml;
    QString xmlFileName = QDir::currentPath()+"/src/serSet.xml";
    qDebug()<<"fileName"<<xmlFileName;
    QFile file(xmlFileName);
    //QFile file("G:\\SystemFile\\Documents\\GitHub\\getServer\\src\\serSet.xml");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<QString("Cannot open file.");
        return ;
    }
    xml.setDevice(&file);
    while(!xml.atEnd())
    {
        xml.readNext();
        if(xml.isStartElement())
        {
            if(xml.name() == "InterFaceFile")
            {
                serFile = xml.attributes().at(0).value().toString();
                serFileItemCreated = false;
            }
            if(xml.name() == "SERVER")
            {
                if(ui->serVersionEdit->text().isEmpty())
                {
                    searchCondition =  (xml.attributes().at(0).value() == serCode)
                            &&(xml.attributes().at(1).value() == serScene);
                }
                else
                {
                    searchCondition = (xml.attributes().at(0).value() == serCode)
                            &&(xml.attributes().at(1).value() == serScene)
                            &&(xml.attributes().at(2).value() == serVersion);
                }
                if( searchCondition )
                {
                    //如果文件名称节点还没有创建，则先创建文件名称节点。
                    if(serFileItemCreated == false)
                    {
                        itemFile = new QStandardItem(serFile);
                        model->appendRow(itemFile);
                        serFileItemCreated = true;
                    }
                    QString linkName;
                    if(ui->serVersionEdit->text().isEmpty() && !xml.attributes().at(2).value().toString().isEmpty())
                        linkName = "服务版本:  "+xml.attributes().at(2).value().toString()
                                +"\t\t服务链接:  "+xml.attributes().at(3).value().toString();
                    else
                        linkName = "服务链接:  "+xml.attributes().at(3).value().toString();
                    itemFile->appendRow(new QStandardItem(linkName));
                }
            }
        }
    }
    return;

}

void MainWindow::showConfig()
{
     //全屏显示，没有菜单栏，标题栏
   // this->_mConfig->showFullScreen();
    this->_mConfig->showMaximized();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_analysFileBtn_clicked()
{

    QMessageBox message;
    QMessageBox::StandardButton stdBtn = message.information(this,"","本操作将重新分析所有存在于设置中的接口文件",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if(stdBtn == QMessageBox::Yes)
    {
        //xmlThread->start();
        analysisSerFile();
        //xmlDialog->exec();
    }
    else
        return;
}

void MainWindow::on_serchBtn_clicked()
{
    searchSer();
}


void MainWindow::on_serListTree_doubleClicked(const QModelIndex &index)
{
    if(index.parent().child(0,0).data().toString().isEmpty())
    {
        qDebug()<<"双击顶层节点";

    }
    else
    {
        qDebug()<<index.parent().child(index.row(),0).data().toString();
        QString fileName = index.parent().data().toString();
        QString linkName  = index.parent().child(index.row(),0).data().toString();
        qDebug()<<"FileName"<<fileName<<"\n"<<"LinkName"<<linkName;
        //处理linkName
        {
            int index = linkName.indexOf("链接");
            linkName = linkName.right(linkName.length()-index-5);
            qDebug()<<"LinkName"<<linkName;
        }
        xls->open(fileName);
        xls->setVisible(true);
        xls->activeSheet(linkName);
    }
}
