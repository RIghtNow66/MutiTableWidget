#include "FaultyStatusTable.h"
#include "UniversalDelegate.h"

FaultyStatusTable::FaultyStatusTable(QWidget* parent) : BaseDataTable(parent)
{
    auto* delegate = new UniversalDelegate(this);

    // 关键配置：告诉委托，第3列（索引为2）需要被绘制成状态圆点。
    delegate->setStatusCircleColumn(2);

    setItemDelegate(delegate);
}