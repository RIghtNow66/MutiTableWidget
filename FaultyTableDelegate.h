#pragma once
#include <QStyledItemDelegate>

class FaultyTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FaultyTableDelegate(QObject* parent = nullptr);
protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
