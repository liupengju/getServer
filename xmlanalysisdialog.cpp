#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "xmlanalysisdialog.h"
#include "ui_xmlanalysisdialog.h"
#include <QDebug>
#include <QTime>
//#include <QTest>


xmlAnalysisDialog::xmlAnalysisDialog(QStringList sFileList,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::xmlAnalysisDialog),
    fileList(sFileList)
{
    ui->setupUi(this);
    xlsObj = new xlsAxObj();
}

xmlAnalysisDialog::~xmlAnalysisDialog()
{
    delete ui;
}

void xmlAnalysisDialog::updateProgressBar(int currentVal, int maxVal)
{
   // ui->progressBar->setMaximum(maxVal);
   // ui->progressBar->setValue(currentVal);
    ui->progressBar->setRange(0,1000);
    int val = 0;
    while(1)
    {
        if(val <= 1000)
        {
            ui->progressBar->setValue(val);
            val+=1;
           // QTest::qSleep(1000);
            qDebug()<<"val"<<"--->"<<val;
        }
        else
            return;
    }
}

void xmlAnalysisDialog::on_cancelBtn_clicked()
{
    updateProgressBar(1,1);
    //this->close();
}

void xmlAnalysisDialog::analysisSerFile()
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

    for(int i = 0;i < fileList.count();i++)
    {
        QString fileName = fileList.at(i);
        xml.writeStartElement("InterFaceFile");
        xml.writeAttribute("FileName",fileName);
        getSerInfoFromxls(xml,fileName);
        xml.writeEndElement();
    }
    QMessageBox::information(this,"","接口文件分析结束",QMessageBox::Yes,QMessageBox::Yes);
    xml.writeEndDocument();
}

void xmlAnalysisDialog::getSerInfoFromxls(QXmlStreamWriter &xml,QString fileName)
{
    xlsObj->open(fileName);
    xlsObj->setVisible(true);
    int sheetCount = xlsObj->getSheetsCount();
    for(int i = 1;i < sheetCount+1;i++)        //因为表格数量并不多。没必要设置标志位在找到INDEX表格后退出。
    {
        if("INDEX" == xlsObj->getSheetName(i))
        {
            int columnCount = xlsObj->getColumnCount(xlsObj->getSheet(i));
            if(columnCount > 20)
                columnCount = 20;
            int rowCount = xlsObj->getRowCount(xlsObj->getSheet(i));
            int columnStart = xlsObj->getColumnStart(xlsObj->getSheet(i));
            int rowStart = xlsObj->getRowStart(xlsObj->getSheet(i));

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
                    if( xlsObj->getCell(row+rowStart,column+columnStart).toString().contains("服务名称"))
                        codeColumn = column+columnStart-1;
                    if("服务场景" == xlsObj->getCell(row+rowStart,column+columnStart).toString())
                        sceneColumn = column+columnStart-1;
                    if("服务版本" == xlsObj->getCell(row+rowStart,column+columnStart).toString())
                        versionColumn = column+columnStart-1;
                    if("链接" == xlsObj->getCell(row+rowStart,column+columnStart).toString())
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
                        sCode = xlsObj->getCell(row+rowStart,column+columnStart).toString().section('\n',0,0);
                    else if(column == sceneColumn)
                        sScene = xlsObj->getCell(row+rowStart,column+columnStart).toString().section('\n',0,0);

                    else if(column == versionColumn)
                        sVersion = xlsObj->getCell(row+rowStart,column+columnStart).toString();
                    else if(column == linkColumn)
                        sLink = xlsObj->getCell(row+rowStart,column+columnStart).toString();
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
    xlsObj->close();
}
