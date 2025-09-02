#include "StateTableDelegate.h"
#include "MutiTableWidget.h"
#include <QPainter>
#include <QApplication>

const int COMBINED_STATUS_COLUMN = 2;

StateTableDelegate::StateTableDelegate(QObject* parent) : QStyledItemDelegate(parent) {}


void StateTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    if (const BaseDataTable* table = qobject_cast<const BaseDataTable*>(parent())) {
        QFont font = viewOption.font;
        font.setPointSize(table->getCurrentPointSize());
        viewOption.font = font;
    }

    if (index.column() == COMBINED_STATUS_COLUMN) {
        viewOption.text = QString();
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &viewOption, painter, viewOption.widget);

        QString status = index.data(Qt::DisplayRole).toString();
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        int margin = 4;
        QRectF ellipseRect = option.rect.adjusted(margin, margin, -margin, -margin);
        int diameter = qMin(ellipseRect.width(), ellipseRect.height());
        ellipseRect.setWidth(diameter);
        ellipseRect.setHeight(diameter);
        ellipseRect.moveCenter(option.rect.center());

        if (status == "0") {
            painter->setBrush(QBrush(Qt::blue)); // Blue
        }
        else if (status == "1") {
            painter->setBrush(QBrush(QColor("#dc3545"))); // Red
        }

        painter->setPen(Qt::NoPen);
        painter->drawEllipse(ellipseRect);
        painter->restore();
    }
    else {
        QStyledItemDelegate::paint(painter, viewOption, index);
    }
}