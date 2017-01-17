#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "config.h"
#include "ui_config.h"
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>


Config::Config(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);
    QDomDocument doc;
    QDomElement domElement;
    //获取默认路劲
    getElement(doc,"Root/LastDir",domElement);
    defaultDir = domElement.text();
    //获取ESB-->P文件列表
    getElement(doc,"Root/epFile",domElement);
    if(domElement.hasChildNodes())
    {
        QDomNodeList childList;
        childList = domElement.childNodes();
        int childCount = childList.count();
        for(int i = 0;i<childCount;i++)
        {
            epFileList.append(childList.at(i).toElement().text());
        }
    }
    ui->epFileListWidget->addItems(epFileList);
}

Config::~Config()
{
    delete ui;
}

bool Config::getElement(QDomDocument &doc,QString nodeName,QDomElement &root)
{
    QFile xmlDocument(":/src/fileList.xml");
    if(!xmlDocument.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<"fileList.xml oepn failed";
        qDebug()<<"./cfg/路劲下不存在[fileList.xml],系统将自动生成该文件";
        //return false;
    }
    //QDomDocument doc;
    QString errStr;
    int errLine;
    int errColumn;
    if(!doc.setContent(&xmlDocument,&errStr,&errLine,&errColumn))
    {
        qDebug()<<"Error:Paese error at line "<<errLine<<","
               <<"Column "<<errColumn<<":"<<qPrintable(errStr);
        return false;
    }
    // QDomElement root = doc.documentElement();
    root = doc.documentElement();
    int index = 1;
    QString tagName;

    while(root.hasChildNodes())//"SECU/MacBuf/Special/Tran/MacField1"
    {
        tagName = nodeName.section('/',index,index);

        for(int i = 0;i<root.childNodes().count();i++)
        {
            if(root.childNodes().at(i).toElement().tagName() == tagName)
            {
                root = root.childNodes().at(i).toElement();
                if(nodeName.section('/',index+1,index+1)==NULL)
                {
                    xmlDocument.close();
                    return true;
                }
                break;
            }
        }
        if(nodeName.section('/',index+1,index+1)==NULL)
        {
            root.clear();
            xmlDocument.close();
            return false;
        }
        index++;
    }
    xmlDocument.close();
    return true;
}


void Config::on_deleteBtn_clicked()
{
    if(ui->epFileListWidget->selectedItems().isEmpty())
        QMessageBox::information(this,"删除","没有文件被选中",QMessageBox::Ok,QMessageBox::Ok);
    else
    {
        for(int i =0; i <(ui->epFileListWidget->count());i++)
        {
            if(ui->epFileListWidget->itemAt(0,i)->isSelected())
            {
                ui->epFileListWidget->takeItem(i);
                ui->epFileListWidget->removeItemWidget(ui->epFileListWidget->itemAt(0,i));
            }
        }
    }
}

void Config::on_addBtn_clicked()
{
    for(int i = 0;i < redItem.count();i++)
    {
        QString sIndex = redItem.at(i);
        int iIndex = sIndex.toInt();
        ui->epFileListWidget->item(iIndex)->setTextColor(Qt::black);
    }
    redItem.clear();
    QStringList fileList = QFileDialog::getOpenFileNames(this,"选择接口文件",this->defaultDir,"Images (*.xls *.xlsc *.jpg);;ALL(*.*)");
    QString alreadyExitFile;
    for( int i = 0; i < fileList.count(); i++ )
    {
        int ret = addValue(fileList.at(i));
        if(ret == 88)
        {
            alreadyExitFile.append(fileList.at(i));
            alreadyExitFile.append('\n');
        }
    }
    if(!alreadyExitFile.isEmpty())
    {
        QMessageBox::warning(this,"添加错误",QString("以下文件已经存在[红色标示]\n%1").arg(alreadyExitFile),QMessageBox::Yes,QMessageBox::Yes);
    }
}



int Config::addValue(const QString sfileName)
{
    try
    {
        QString name = sfileName.section('/',-1);
        for(int i = 0;i < epFileList.count();i++)
        {
            QString epfileName = epFileList.at(i);
            if(!epfileName.isNull() && name == epfileName.section('/',-1))
            {
                ui->epFileListWidget->item(i)->setTextColor(Qt::red);
                QString sIndex;
                sIndex.setNum(i);
                redItem.append(sIndex);
                return 88;
            }
        }

        QString fileName("G:/SystemFile/Documents/GitHub/getServer/src/fileList.xml");
        QDomDocument doc;

        QFile xmlDocument(fileName);
        if(!xmlDocument.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            qDebug()<<"fileList.xml oepn failed";
        }
        QString errStr;
        int errLine;
        int errColumn;
        if(!doc.setContent(&xmlDocument,&errStr,&errLine,&errColumn))
        {
            qDebug()<<"Error:Paese error at line "<<errLine<<","
                   <<"Column "<<errColumn<<":"<<qPrintable(errStr);
            return -1;
        }
        xmlDocument.close();
        QDomElement docElem = doc.documentElement();
        QDomNodeList epFileNode = docElem.elementsByTagName("epFile");
        QDomElement e1 = epFileNode.at(0).toElement();
        QDomElement fileElement = doc.createElement("file");
        QDomText text = doc.createTextNode(sfileName);
        fileElement.appendChild(text);
        e1.appendChild(fileElement);

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return -1;
        }
        QTextStream out(&file);
        doc.save(out,4);
        epFileList.append(sfileName);
        ui->epFileListWidget->addItem(sfileName);
        ui->epFileListWidget->item(ui->epFileListWidget->count()-1)->setTextColor(Qt::green);
    }
    catch (...)
    {
        qDebug()<<"catch false";
        return -1;
    }
}
