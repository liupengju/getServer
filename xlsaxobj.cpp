#pragma execution_character_set("GBK")

#include "xlsaxobj.h"
#include "QDebug"


xlsAxObj::xlsAxObj()
{
    excel.setControl("Excel.Application");
    work_books = excel.querySubObject("WorkBooks");
    work_book = excel.querySubObject("ActiveWorkBook");
}

void xlsAxObj::setVisible(bool visible)
{
    excel.setProperty("Visible",visible);
}

void xlsAxObj::open(const QString filename)
{
    work_books->dynamicCall("Open (const QString &)",filename);
}

void xlsAxObj::close( )
{
    work_books->dynamicCall("Close");
}

QString xlsAxObj::getTitle()
{
    QVariant title_value = excel.property("Caption");
    return title_value.toString();
}

int xlsAxObj::getSheetsCount()
{
    work_book = excel.querySubObject("ActiveWorkBook");
    work_sheets = work_book->querySubObject("Sheets");
    int i =  work_sheets->property("Count").toInt();  //获取工作表数目
    return i;
}

QAxObject *xlsAxObj::getSheet(int index)
{
    if( (index<1)||(index>getSheetsCount() ))
    {
        qDebug()<<QString("sheetNo over ride [1-%1]").arg(getSheetsCount())<<__FILE__<<__LINE__;
        qDebug()<<"exit getSheet";
        return NULL;
    }
    qDebug()<<"exit getSheet";
    return work_book->querySubObject("Sheets(int)", index);  //Sheets(int)也可换用Worksheets(int)
}

QString xlsAxObj::getSheetName(int index)
{
    if( (index<1)||(index>getSheetsCount() ))
    {
        qDebug()<<QString("sheetNo over ride[1-%1]").arg(getSheetsCount())<<__FILE__<<__LINE__;
        return NULL;
    }
    work_sheet = work_book->querySubObject("Sheets(int)", index);  //Sheets(int)也可换用Worksheets(int)
    return work_sheet->property("Name").toString();  //获取工作表名称
}

QVariant xlsAxObj::getCell(int row, int column)
{
    QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", row, column);
    QVariant cell_value = cell->property("Value");  //获取单元格内容
    return cell_value;
}


int xlsAxObj::getRowCount(QAxObject *work_sheet)
{
    used_range = work_sheet->querySubObject("UsedRange");
    return used_range->querySubObject("Rows")->property("Count").toInt();
}
int xlsAxObj::getColumnCount(QAxObject *work_sheet)
{
    used_range = work_sheet->querySubObject("UsedRange");
    return used_range->querySubObject("Columns")->property("Count").toInt();
}

int xlsAxObj::getRowStart(QAxObject *work_sheet)
{
    used_range = work_sheet->querySubObject("UsedRange");
    return used_range->property("Row").toInt();
}
int xlsAxObj::getColumnStart(QAxObject *work_sheet)
{
    used_range = work_sheet->querySubObject("UsedRange");
    return used_range->property("Column").toInt();
}
int xlsAxObj::activeSheet(QString sheetName)
{
    for(int i = 1;i <= this->getSheetsCount();i++)
    {
        if(this->getSheetName(i) == sheetName)
        {
            QAxObject *currentSheet = this->getSheet(i);
            currentSheet->querySubObject("Select");
            return 0;
        }
    }
    qDebug()<<"can`t find sheet["<<sheetName<<"]";
    return -1;
}
