#include "RealtimeTableDelegate.h"
#include "MutiTableWidget.h"
#include <QPainter>
#include <QApplication>

RealtimeTableDelegate::RealtimeTableDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

void RealtimeTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    if (const BaseDataTable* table = qobject_cast<const BaseDataTable*>(parent())) {
        QFont font = viewOption.font;
        font.setPointSize(table->getCurrentPointSize());
        viewOption.font = font;
    }

    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &viewOption, painter, viewOption.widget);
}

void RealtimeTableDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
    bool isOutlier = index.data(Qt::UserRole).toBool();

    if (isOutlier && !(option->state & QStyle::State_Selected))
    {
        option->backgroundBrush = QBrush(QColor("#FFEAEA"));
        option->palette.setColor(QPalette::Text, QColor("#8B0000"));
        option->font.setBold(true);
    }
}