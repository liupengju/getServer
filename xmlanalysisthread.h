#ifndef XMLANALYSISTHREAD_H
#define XMLANALYSISTHREAD_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QThread>
#include <QStringList>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include "xlsaxobj.h"
#include <QMainWindow>

class xmlAnalysisThread : public QThread,public QMainWindow
{
    //Q_OBJECT

public:
    xmlAnalysisThread(const QStringList &sFileList);
    ~xmlAnalysisThread();
    //QStringList fileList;
protected:
    virtual void run();
private:
    QStringList fileList;
    xlsAxObj *xlsObj;
    void getSerInfoFromxls(QXmlStreamWriter &xml,QString fileName);
};

#endif // XMLANALYSISTHREAD_H
