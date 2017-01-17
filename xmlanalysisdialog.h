#ifndef XMLANALYSISDIALOG_H
#define XMLANALYSISDIALOG_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QDialog>
#include <QStringList>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include "xlsaxobj.h"
#include <QFile>

namespace Ui {
class xmlAnalysisDialog;
}

class xmlAnalysisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit xmlAnalysisDialog(QStringList sFileList,QWidget *parent = 0);
    ~xmlAnalysisDialog();
    void analysisSerFile();
    void getSerInfoFromxls(QXmlStreamWriter &xml,QString fileName);
    void updateProgressBar(int currentVal,int maxVal);

private slots:
    void on_cancelBtn_clicked();

private:
    Ui::xmlAnalysisDialog *ui;
    QStringList fileList;
    xlsAxObj *xlsObj;
};

#endif // XMLANALYSISDIALOG_H
