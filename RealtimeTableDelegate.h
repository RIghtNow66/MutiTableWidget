#pragma once


#include <QStyledItemDelegate>

class RealtimeTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit RealtimeTableDelegate(QObject* parent = nullptr);
protected:
    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const override;
};
