#pragma once
#include <QStyledItemDelegate>

class StateTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StateTableDelegate(QObject* parent = nullptr);
protected:
    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};