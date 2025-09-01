#include "CombinedStateTable.h"
#include "StateTableDelegate.h"
#include <QTableWidgetItem>
#include <QStringList>

CombinedStateTable::CombinedStateTable(QWidget* parent) : BaseDataTable(parent)
{
    setItemDelegate(new StateTableDelegate(this));
}
