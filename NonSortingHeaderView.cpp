#pragma execution_character_set("utf-8")

#include "NonSortingHeaderView.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTableWidget>
#include <QPixmap>
#include <QCursor>
#include <QFontMetrics>


NonSortingHeaderView::NonSortingHeaderView(Qt::Orientation orientation, QWidget* parent)
    : QHeaderView(orientation, parent)
{
    // 开启右键菜单策略，用于显示“筛选”选项
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QHeaderView::customContextMenuRequested, this, &NonSortingHeaderView::showContextMenu);

    setMouseTracking(true);
}

void NonSortingHeaderView::mouseMoveEvent(QMouseEvent* event)
{
    // 1. 总是先调用基类的实现。
    //    这让基类有机会先根据鼠标位置设置好默认的光标，
    //    比如在列分隔线上时，它会设置好 Qt::SplitHCursor。
    QHeaderView::mouseMoveEvent(event);

    // 2. 然后，我们只在需要时才“覆盖”基类的决定。
    int logicalIndex = logicalIndexAt(event->pos());

    // 检查鼠标是否在某个带有筛选图标的列的图标区域内
    if (logicalIndex != -1 && m_filterStates.value(logicalIndex, false))
    {
        int x = sectionPosition(logicalIndex);
        int width = sectionSize(logicalIndex);
        QRect sectionRect(x, 0, width, height());

        if (getIconRect(sectionRect, logicalIndex).contains(event->pos())) {
            // 只有在这种精确的情况下，我们才把光标强制设为手型
            setCursor(Qt::PointingHandCursor);
        }
    }
    // 【重要】我们删除了 else 分支。
    // 这意味着，如果鼠标不在我们的图标上，我们什么也不做，
    // 从而保留了基类在第1步中设置的任何光标（无论是箭头还是拖拽光标）。
}

// 处理鼠标离开事件
void NonSortingHeaderView::leaveEvent(QEvent* event)
{
    // 确保鼠标离开整个表头区域时，光标恢复正常
    setCursor(Qt::ArrowCursor);
    QHeaderView::leaveEvent(event);
}

// 供外部调用，更新某一列的筛选状态
void NonSortingHeaderView::setFilterState(int logicalIndex, bool isActive)
{
    if (isActive) {
        m_filterStates[logicalIndex] = true;
    }
    else {
        m_filterStates.remove(logicalIndex);
    }
    // 通知视图重绘此表头区域
    updateSection(logicalIndex);
}

// 核心功能：重绘表头
void NonSortingHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
    painter->save();
    // 1. 先调用基类的绘制方法，这样QSS样式、文字等依然生效
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();

    // 2. 如果当前列处于筛选状态，则在上面叠加绘制我们的图标
    if (m_filterStates.value(logicalIndex, false))
    {
        QPixmap pixmap(":/MutiTableWidget/m_icon/hourglass.png"); // 从资源文件加载图标
        if (!pixmap.isNull())
        {
            QRect iconRect = getIconRect(rect, logicalIndex);
            painter->drawPixmap(iconRect, pixmap);
        }
    }
}

// 核心功能：处理鼠标点击
void NonSortingHeaderView::mousePressEvent(QMouseEvent* event)
{
    // 只处理左键点击
    if (event->button() == Qt::LeftButton) {
        int logicalIndex = logicalIndexAt(event->pos());
        if (logicalIndex != -1) {
            // 检查当前列是否有筛选图标
            if (m_filterStates.value(logicalIndex, false)) {
                // 使用 sectionPosition 和 sectionSize 来安全地构建矩形
                int x = sectionPosition(logicalIndex);
                int width = sectionSize(logicalIndex);
                QRect sectionRect(x, 0, width, height()); // 构建出当前表头项的完整矩形

                // 计算出图标的矩形区域
                QRect iconRect = getIconRect(sectionRect, logicalIndex);

                // 判断鼠标点击位置是否在图标区域内
                if (iconRect.contains(event->pos())) {
                    // 如果是，则发射筛选信号，并停止事件处理
                    emit filterTriggered(logicalIndex);
                    event->accept();
                    return;
                }
            }
        }
    }

    // 如果点击的不是图标，或者不是左键，则执行基类的默认行为（如拖拽列、选中列等）
    QHeaderView::mousePressEvent(event);
}

// 用于显示右键菜单的槽函数
void NonSortingHeaderView::showContextMenu(const QPoint& pos)
{
    int logicalIndex = logicalIndexAt(pos);
    if (logicalIndex != -1) {
        QMenu contextMenu(this);
        QAction* action;

        // ★ 关键改动：根据状态连接到不同的信号
        if (m_filterStates.value(logicalIndex, false)) {
            // 如果已筛选，菜单项为“取消筛选”
            action = contextMenu.addAction("取消筛选");
            // 连接到“清除筛选”的专属信号
            connect(action, &QAction::triggered, [this, logicalIndex]() {
                emit clearFilterTriggered(logicalIndex);
                });
        }
        else {
            // 如果未筛选，菜单项为“筛选”
            action = contextMenu.addAction("筛选");
            // 连接到“打开/编辑筛选”的信号
            connect(action, &QAction::triggered, [this, logicalIndex]() {
                emit filterTriggered(logicalIndex);
                });
        }

        contextMenu.exec(mapToGlobal(pos));
    }
}

// 辅助函数，计算图标应该被绘制在哪个位置
QRect NonSortingHeaderView::getIconRect(const QRect& sectionRect, int logicalIndex) const
{
    // 获取表头的文字内容
    QString text = model()->headerData(logicalIndex, Qt::Horizontal, Qt::DisplayRole).toString();

    // 计算文字的像素宽度
    QFontMetrics fm(font());
    int textWidth = fm.horizontalAdvance(text);

    // 计算文字绘制的起始X坐标（QHeaderView默认居中对齐）
    int textStartX = sectionRect.x() + (sectionRect.width() - textWidth) / 2;

    // 计算文字的结束X坐标
    int textEndX = textStartX + textWidth;

    int iconSize = 22; // 图标尺寸 
    int textPadding = 4; // 图标与文字之间的间距

    // 计算y坐标使其垂直居中
    int y = sectionRect.y() + (sectionRect.height() - iconSize) / 2;
    // 计算x坐标，使其紧贴文字右侧
    int x = textEndX + textPadding;

    // 安全检查：确保图标不会超出表头的右边界
    if (x + iconSize > sectionRect.right()) {
        x = sectionRect.right() - iconSize - 2; // 如果要超出，则还是靠右对齐
    }

    return QRect(x, y, iconSize, iconSize);
}

bool NonSortingHeaderView::getFilterState(int logicalIndex) const
{
    return m_filterStates.value(logicalIndex, false);
}