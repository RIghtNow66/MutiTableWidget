#include "FaultyStatusTable.h"
#include "UniversalDelegate.h"

FaultyStatusTable::FaultyStatusTable(QWidget* parent) : BaseDataTable(parent)
{
    auto* delegate = new UniversalDelegate(this);

    // �ؼ����ã�����ί�У���3�У�����Ϊ2����Ҫ�����Ƴ�״̬Բ�㡣
    delegate->setStatusCircleColumn(2);

    setItemDelegate(delegate);
}