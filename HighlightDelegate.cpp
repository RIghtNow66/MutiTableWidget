#include "HighlightDelegate.h"
#include <QStyleOptionViewItem>
#include <QStyle>
#include <QColor>
#include <QBrush>
#include <QApplication>
#include <QPainter>
#include "MutiTableWidget.h"


HighlightDelegate::HighlightDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}


void HighlightDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // 步骤 1: 创建一个可修改的样式选项副本
    QStyleOptionViewItem viewOption(option);

    // 步骤 2: 【关键修复】执行默认的初始化。
    // 这一步会从模型中获取数据，并将文本、对齐方式等填充到 viewOption 中。
    // 这是让文字重新出现的关键！
    initStyleOption(&viewOption, index);

    // 步骤 3: 【功能恢复】从父控件获取动态字体大小并应用。
    // 这是让字体能够自适应缩放的关键。
    if (const BaseDataTable* table = qobject_cast<const BaseDataTable*>(parent())) {
        QFont font = viewOption.font;
        font.setPointSize(table->getCurrentPointSize());
        viewOption.font = font;
    }

    // 步骤 4: 开始根据列进行自定义绘制
    if (index.column() == STATUS_COLUMN_INDEX) {
        // 对于状态列，我们不希望显示 "0" 或 "1" 这些文字，只显示椭圆。
        // 所以，在绘制前，我们手动清空文本。
        viewOption.text = QString();

        // 先绘制单元格的背景、选中框等（此时已无文本）。
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &viewOption, painter, viewOption.widget);

        // 然后，在背景之上绘制我们自定义的椭圆。
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
            painter->setBrush(QBrush(QColor("#28a745"))); // 绿色
        }
        else if (status == "1") {
            painter->setBrush(QBrush(QColor("#dc3545"))); // 红色
        }

        painter->setPen(Qt::NoPen);
        painter->drawEllipse(ellipseRect);
        painter->restore();

    }
    else {
        // 对于所有其他普通列，我们已经完成了初始化和字体设置，
        // 直接调用基类的 paint 方法，让它用我们准备好的 viewOption 来完成标准绘制即可。
        QStyledItemDelegate::paint(painter, viewOption, index);
    }
}

void HighlightDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    // 1. 必須首先調用基類的實現，它會應用QSS中的樣式（例如藍色選中背景）
    QStyledItemDelegate::initStyleOption(option, index);

    // 2. 獲取我們在 My_QTableWidget 中設置的“是否為異常值”狀態
    bool isOutlier = index.data(Qt::UserRole).toBool();

    // 3. 定義樣式優先級：
    //    如果單元格是異常值，並且它當前“沒有”被選中...
    if (isOutlier && !(option->state & QStyle::State_Selected))
    {
        // ...那麼我們就應用粉色背景和紅色粗體字樣式。
        option->backgroundBrush = QBrush(QColor("#FFEAEA"));
        option->palette.setColor(QPalette::Text, QColor("#8B0000"));
        option->font.setBold(true);
    }
    // 對於其他所有情況（普通單元格、被選中的異常單元格），我們不做任何操作，
    // 完全交給基類和QSS去處理，這樣就能正確顯示白色背景或藍色選中背景。
}