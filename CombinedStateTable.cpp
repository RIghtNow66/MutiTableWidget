#include "CombinedStateTable.h"
#include "UniversalDelegate.h"
#include <QTableWidgetItem>
#include <QStringList>

CombinedStateTable::CombinedStateTable(QWidget* parent) : BaseDataTable(parent)
{
    auto* delegate = new UniversalDelegate(this);

    // 配置1: 告诉委托，第3列（索引为2）需要被绘制成状态圆点。
    delegate->setStatusCircleColumn(2);

    setItemDelegate(delegate);
}

void CombinedStateTable::checkAndHighlightRow(int displayRow, const QVector<QString>& rowData)
{
    // 定义包含持续时间的列索引（第5列，索引为4）。
    const int durationColumn = 4;

    if (durationColumn >= rowData.size()) return;

    QString durationStr = rowData[durationColumn]; // "HH:mm:ss"
    QStringList parts = durationStr.split(':');

    bool isOutlier = false;

    if (parts.size() == 3) {
        bool okH, okM, okS;
        int hours = parts[0].toInt(&okH);
        int minutes = parts[1].toInt(&okM);
        int seconds = parts[2].toInt(&okS);

        if (okH && okM && okS) {
            long totalSeconds = hours * 3600 + minutes * 60 + seconds;
            // 核心判断逻辑：如果总秒数大于1200秒，则为异常。
            if (totalSeconds > 1200) {
                isOutlier = true;
            }
        }
    }

    if (isOutlier) {
        for (int c = 0; c < columnCount(); ++c) {
            if (auto currentItem = item(displayRow, c)) {
                currentItem->setData(Qt::UserRole, true);
            }
        }
    }
}
