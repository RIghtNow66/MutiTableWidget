#include "FaultyStatusTable.h"
#include "FaultyTableDelegate.h"

FaultyStatusTable::FaultyStatusTable(QWidget* parent) : BaseDataTable(parent)
{
    setItemDelegate(new FaultyTableDelegate(this));
}