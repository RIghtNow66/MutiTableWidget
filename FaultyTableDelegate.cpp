#include "FaultyTableDelegate.h"
#include "MutiTableWidget.h"
#include <QPainter>
#include <QApplication>

const int FAULTY_STATUS_COLUMN = 2; // 定义状态列

FaultyTableDelegate::FaultyTableDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

void FaultyTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // 步骤 1: 创建样式选项副本并进行默认初始化
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    // 步骤 2: ★★★ 关键修正点 ★★★
    // 从父控件（表格）获取动态字体大小
    if (const BaseDataTable* table = qobject_cast<const BaseDataTable*>(parent())) {
        QFont font = viewOption.font;
        font.setPointSize(table->getCurrentPointSize());
        viewOption.font = font;
    }

    // 步骤 3: 判断是否为需要特殊绘制的状态列
    if (index.column() == FAULTY_STATUS_COLUMN) {
        // a. 清空文本，只显示圆点
        viewOption.text = QString();

        // b. 先绘制背景、选中框等
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &viewOption, painter, viewOption.widget);

        // c. 在背景之上绘制自定义的圆点
        QString status = index.data(Qt::DisplayRole).toString();
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);

        // d. 计算圆点的位置和大小
        int margin = 5;
        QRectF ellipseRect = option.rect.adjusted(margin, margin, -margin, -margin);
        int diameter = qMin(ellipseRect.width(), ellipseRect.height());
        ellipseRect.setSize(QSizeF(diameter, diameter));
        ellipseRect.moveCenter(option.rect.center());

        // e. 根据状态值设置颜色
        if (status == "2") { // 紧急
            painter->setBrush(QBrush(QColor("#dc3545"))); // 红色
        }
        else if (status == "1") { // 警告
            painter->setBrush(QBrush(QColor("#ffc107"))); // 黄色
        }
        else { // 正常或未知状态 ("0" 等)
            painter->setBrush(QBrush(QColor("#007bff"))); // 蓝色
        }

        painter->setPen(Qt::NoPen);
        painter->drawEllipse(ellipseRect);
        painter->restore();
    }
    else {
        // 对于所有其他普通列，使用我们更新了字体大小的 viewOption 进行标准绘制
        QStyledItemDelegate::paint(painter, viewOption, index);
    }
}