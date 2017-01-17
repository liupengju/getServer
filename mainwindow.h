#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>
#include <QList>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "config.h"
#include "xlsaxobj.h"
//#include <QCryptographicHash>
#include <QMessageBox>
#include <QList>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QProgressDialog>
#include "xmlanalysisthread.h"
#include "xmlanalysisdialog.h"

//typedef struct server
//{
//    QString serCode;
//    QString serScene;
//    QString serVersion;
//    QString serLink;
//}server;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void getSerInfoFromxls(QXmlStreamWriter &xml,QString fileName);
    int upDateVerList(QString verCode,QString verScene,QString verVersion,QString fileName);
    int updSer2Xml();
    int updateSerXml();

private:
    Ui::MainWindow *ui;
    Config *_mConfig;
    //QList<server> serverList;
    QStringList fileList;
    xlsAxObj *xls;
    QStandardItemModel *model ;
    xmlAnalysisThread *xmlThread;
    xmlAnalysisDialog *xmlDialog;

private slots:
    void showConfig();
    void analysisSerFile();
    void on_analysFileBtn_clicked();
    void searchSer();
    void on_serchBtn_clicked();
    void on_serListTree_doubleClicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
