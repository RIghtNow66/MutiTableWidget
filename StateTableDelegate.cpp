#include "StateTableDelegate.h"
#include <QPainter>
#include <QApplication>
#include <QBrush>
#include <QColor>

const int COMBINED_STATUS_COLUMN = 2;

StateTableDelegate::StateTableDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

// ���� RealtimeDataDelegate �ĸ����߼�
void StateTableDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
    if (index.data(Qt::UserRole).toBool() && !(option->state & QStyle::State_Selected)) {
        option->backgroundBrush = QBrush(QColor("#FFEAEA"));
        // ...
    }
}

// ���� FaultyStatusDelegate �� paint �߼�
void StateTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index); // Ӧ�ø���

    if (index.column() == COMBINED_STATUS_COLUMN) {
        // ... �˴�ճ�� State_table �Ļ�Բ����루��/�죩...
    }
    else {
        QStyledItemDelegate::paint(painter, viewOption, index);
    }
}