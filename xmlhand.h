/****************************************************************************
**时间：2016-10-13
**描述：该类主要用来处理xml文件的增删查改操作。基于QDom来实现，不适用于处理大型xml文件
**     大型xml文件最好用QXmlStreamReader和QxmlStreamWrite来时实现
****************************************************************************/
#ifndef XMLHAND_H
#define XMLHAND_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>

class xmlHand
{
public:
    xmlHand();
    xmlHand(QString fileName);

    bool createXml(QString fileName = NULL);
    bool checkXml(QString fileName = NULL);
    QString getData(QString fileName = NULL,QString tag = "all");
    bool setData(QString fileName = NULL,QString tag = "all");

private:
    QString sFileName;

};

#endif // XMLHAND_H
