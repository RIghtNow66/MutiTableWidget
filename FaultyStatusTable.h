#pragma once
#include "MutiTableWidget.h"

/**
 * @class FaultyStatusTable
 * @brief 继承自 BaseDataTable，用于显示故障状态。
 *
 * 其独特功能是：将状态列（第3列）的数字 "0", "1", "2" 绘制成彩色圆点。
 * 它本身没有行高亮逻辑。
 */
class FaultyStatusTable : public BaseDataTable {
    Q_OBJECT
public:
    explicit FaultyStatusTable(QWidget* parent = nullptr);
    // 无需重写 checkAndHighlightRow，将自动使用基类的空实现。
};