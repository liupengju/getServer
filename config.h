#ifndef CONFIG_H
#define CONFIG_H
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QDebug>
#include <QFile>
#include <QMessageBox>

namespace Ui {
class Config;
}

class Config : public QWidget
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0);
    ~Config();

private slots:
    void on_deleteBtn_clicked();
    void on_addBtn_clicked();

private:
    Ui::Config *ui;
    QString defaultDir;
    QStringList epFileList;
    QStringList redItem;

    bool getElement(QDomDocument &doc,QString nodeName,QDomElement &root);
    bool setTValue(const QString &key, const QVariant &value);
    //key是从根节点到要添加的节点的组合，如[Root/epFile/file]
    int addValue(const QString fileName);
};

#endif // CONFIG_H
