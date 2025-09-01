#pragma once

#include <QHeaderView>
#include <QMap>
#include <QMenu>
#include <QAction>
#include <QMouseEvent> 


class NonSortingHeaderView : public QHeaderView
{
    Q_OBJECT

public:
    explicit NonSortingHeaderView(Qt::Orientation orientation, QWidget* parent = nullptr);

    // 用于从外部(Data_table)更新筛选状态的公共函数
    void setFilterState(int logicalIndex, bool isActive);

signals:
    // 这个信号保持不变
    void filterTriggered(int logicalIndex);

protected:
    // 重写 paintSection 来绘制图标
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;

    // 重写 mousePressEvent 来处理对图标的点击
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;

private slots:
    // 用于显示右键菜单的槽函数
    void showContextMenu(const QPoint& pos);

private:
    // 存储每个列的筛选激活状态
    QMap<int, bool> m_filterStates;

    // 获取指定列的图标绘制区域
    QRect getIconRect(const QRect& sectionRect, int logicalIndex) const;
};
