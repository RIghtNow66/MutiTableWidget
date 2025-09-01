#include "CombinedStateTable.h"
#include "UniversalDelegate.h"
#include <QTableWidgetItem>
#include <QStringList>

CombinedStateTable::CombinedStateTable(QWidget* parent) : BaseDataTable(parent)
{
    auto* delegate = new UniversalDelegate(this);

    // ����1: ����ί�У���3�У�����Ϊ2����Ҫ�����Ƴ�״̬Բ�㡣
    delegate->setStatusCircleColumn(2);

    setItemDelegate(delegate);
}

void CombinedStateTable::checkAndHighlightRow(int displayRow, const QVector<QString>& rowData)
{
    // �����������ʱ�������������5�У�����Ϊ4����
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
            // �����ж��߼����������������1200�룬��Ϊ�쳣��
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
