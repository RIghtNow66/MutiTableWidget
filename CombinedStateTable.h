#pragma once
#include "MutiTableWidget.h"

/**
 * @class CombinedStateTable
 * @brief 继承自 BaseDataTable，是功能最全面的状态表格。
 *
 * 其独特功能是：
 * 1. 将状态列（第3列）绘制成彩色圆点。
 * 2. 将持续时间（第5列）超过20分钟（1200秒）的整行高亮。
 */
class CombinedStateTable : public BaseDataTable {
    Q_OBJECT
public:
    explicit CombinedStateTable(QWidget* parent = nullptr);
};