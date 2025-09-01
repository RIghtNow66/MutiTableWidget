#include "StateTableDelegate.h"
#include <QPainter>
#include <QApplication>
#include <QBrush>
#include <QColor>

const int COMBINED_STATUS_COLUMN = 2;

StateTableDelegate::StateTableDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

// 复制 RealtimeDataDelegate 的高亮逻辑
void StateTableDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
    if (index.data(Qt::UserRole).toBool() && !(option->state & QStyle::State_Selected)) {
        option->backgroundBrush = QBrush(QColor("#FFEAEA"));
        // ...
    }
}

// 复制 FaultyStatusDelegate 的 paint 逻辑
void StateTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index); // 应用高亮

    if (index.column() == COMBINED_STATUS_COLUMN) {
        // ... 此处粘贴 State_table 的画圆点代码（绿/红）...
    }
    else {
        QStyledItemDelegate::paint(painter, viewOption, index);
    }
}