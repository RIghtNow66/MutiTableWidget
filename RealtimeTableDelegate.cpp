#include "RealtimeTableDelegate.h"
#include <QBrush>
#include <QColor>

RealtimeTableDelegate::RealtimeTableDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

void RealtimeTableDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
    if (index.data(Qt::UserRole).toBool() && !(option->state & QStyle::State_Selected)) {
        option->backgroundBrush = QBrush(QColor("#FFEAEA"));
        option->palette.setColor(QPalette::Text, QColor("#8B0000"));
        option->font.setBold(true);
    }
}