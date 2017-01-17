#ifndef XLSAXOBJ_H
#define XLSAXOBJ_H

#pragma execution_character_set("GBK")

#include <QAxObject>
#include <QString>

class xlsAxObj
{
public:
    xlsAxObj();

    void setVisible(bool visible);
    void open(const QString filename);
    QString getTitle();
    int getSheetsCount();
    QAxObject *getSheet(int index);
    QString getSheetName(int sheetNo);
    QVariant getCell(int row,int column);
    int getRowCount(QAxObject *work_sheet);
    int getColumnCount(QAxObject *work_sheet);
    int getRowStart(QAxObject *work_sheet);
    int getColumnStart(QAxObject *work_sheet);
    int activeSheet(QString sheetName);
    void close();



public:
    QAxObject excel;//("Excel.Application");
    QAxObject *work_books;
    QAxObject *work_book;
    QAxObject *work_sheets;
    QAxObject *work_sheet;
    QAxObject *used_range;
    QAxObject *cell;

};

#endif // XLSAXOBJ_H
