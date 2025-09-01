#pragma once
#include <QStyledItemDelegate>
#include <QSet>

class UniversalDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit UniversalDelegate(QObject* parent = nullptr);

    // 新增：配置哪些列需要被绘制成状态圆点
    void setStatusCircleColumn(int column, bool enabled = true);

    void removeStatusCircleColumn(int column);
protected:
    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QSet<int> m_statusCircleColumns; // 存储需要画圆点的列索引
};
