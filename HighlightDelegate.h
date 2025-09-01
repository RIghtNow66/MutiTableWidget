#pragma once

#ifndef HIGHLIGHTDELEGATE_H
#define HIGHLIGHTDELEGATE_H

#include <QStyledItemDelegate>

const int STATUS_COLUMN_INDEX = 2;

class HighlightDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit HighlightDelegate(QObject* parent = nullptr);

protected:
    // 重写 initStyleOption 来根据数据自定义样式
    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const override;

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // HIGHLIGHTDELEGATE_H
