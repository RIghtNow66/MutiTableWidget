#include "UniversalDelegate.h"
#include "MutiTableWidget.h"   

#include <QPainter>
#include <QStyleOptionViewItem>
#include <QApplication>
#include <QStyle>
#include <QColor>
#include <QBrush>


UniversalDelegate::UniversalDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

void UniversalDelegate::setStatusCircleColumn(int column, bool enable)
{
    m_statusCircleColumns.insert(column);
}

void UniversalDelegate::removeStatusCircleColumn(int column)
{
    m_statusCircleColumns.remove(column);
}

void UniversalDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    // 1. 必须首先调用基类的实现，它会设置所有默认样式，包括 QSS 中的样式（例如选中背景色）。
    QStyledItemDelegate::initStyleOption(option, index);

    // 2. 获取我们在表格类中通过 setData(Qt::UserRole, ...) 设置的“是否为异常值”状态。
    bool isOutlier = index.data(Qt::UserRole).toBool();

    // 3. 定义样式优先级：
    //    如果单元格是异常值，并且它当前“没有”被选中...
    if (isOutlier && !(option->state & QStyle::State_Selected))
    {
        // ...那么我们就应用粉色背景和红色粗体字样式。
        option->backgroundBrush = QBrush(QColor("#FFEAEA")); // 淡粉色背景
        option->palette.setColor(QPalette::Text, QColor("#8B0000")); // 暗红色文字
        option->font.setBold(true);
    }
    // 对于其他所有情况（普通单元格、被选中的异常单元格），我们不做任何操作，
    // 完全交给基类和QSS去处理，这样就能正确显示默认背景或蓝色选中背景。
}

void UniversalDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // 步骤 1: 创建一个可修改的样式选项副本。我们将在绘制前对其进行修改。
    QStyleOptionViewItem viewOption(option);

    // 步骤 2: 调用我们重写的 initStyleOption。
    // 这一步至关重要，它会将行高亮逻辑（粉色背景等）应用到 viewOption 中。
    initStyleOption(&viewOption, index);

    // 步骤 3: 从父控件（表格）获取动态字体大小并应用。
    // 这使得单元格字体能随着表格的缩放而平滑地变化。
    if (const BaseDataTable* table = qobject_cast<const BaseDataTable*>(parent())) {
        QFont font = viewOption.font;
        font.setPointSize(table->getCurrentPointSize());
        viewOption.font = font;
    }

    // 步骤 4: 判断当前列是否需要进行特殊绘制（状态圆点）。
    if (m_statusCircleColumns.contains(index.column())) {
        // --- 状态圆点列的自定义绘制逻辑 ---

        // a. 在绘制前，手动清空文本，因为我们只想显示圆点。
        viewOption.text = QString();

        // b. 使用 QSS 和我们的样式（但无文本）先绘制单元格的背景、选中框等。
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &viewOption, painter, viewOption.widget);

        // c. 在背景之上绘制我们自定义的圆点。
        QString status = index.data(Qt::DisplayRole).toString();
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true); // 开启抗锯齿，使圆点更平滑

        // d. 计算圆点的绘制区域，使其在单元格内居中并保持圆形。
        int margin = 5; // 圆点距离单元格边界的距离
        QRectF ellipseRect = option.rect.adjusted(margin, margin, -margin, -margin);
        int diameter = qMin(ellipseRect.width(), ellipseRect.height()); // 取宽高最小值确保是圆形
        ellipseRect.setWidth(diameter);
        ellipseRect.setHeight(diameter);
        ellipseRect.moveCenter(option.rect.center()); // 移动到单元格中心

        // e. 根据 status 的值选择不同的颜色。
        //    这里整合了您所有表格中的状态颜色，使其真正“通用”。
        if (status == "0") {
            painter->setBrush(QBrush(QColor("#28a745"))); // 正常/提示 (绿色)
        }
        else if (status == "1") {
            painter->setBrush(QBrush(QColor("#ffc107"))); // 警告 (黄色)
        }
        else if (status == "2") {
            painter->setBrush(QBrush(QColor("#dc3545"))); // 紧急/错误 (红色)
        }
        else {
            painter->setBrush(QBrush(QColor("#007bff"))); // 其他/未知 (蓝色)
        }

        painter->setPen(Qt::NoPen); // 我们不希望圆点有边框
        painter->drawEllipse(ellipseRect);
        painter->restore();

    }
    else {
        // --- 其他所有普通列的绘制逻辑 ---

        // 直接调用基类的 paint 方法，让它用我们准备好的、包含了
        // 正确高亮样式和字体大小的 viewOption 来完成标准绘制即可。
        QStyledItemDelegate::paint(painter, viewOption, index);
    }
}