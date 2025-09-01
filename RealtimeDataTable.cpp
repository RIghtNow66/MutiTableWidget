#include "RealtimeDataTable.h"
#include "RealtimeTableDelegate.h"
#include <QTableWidgetItem>


RealtimeDataTable::RealtimeDataTable(QWidget* parent) : BaseDataTable(parent) {
    // ����ί�У�������Ҫ���û�Բ��
    setItemDelegate(new RealtimeTableDelegate(this));
}

//void RealtimeDataTable::checkAndHighlightRow(int displayRow, const QVector<QString>& rowData) {
//    // ճ�� Data_table �м����ֵ��Χ���߼�
//    const int valueColumn = 2;
//    if (valueColumn >= rowData.size()) return;
//
//    bool isNumeric;
//    double value = rowData[valueColumn].toDouble(&isNumeric);
//    bool isOutlier = isNumeric && (value > 520.0 || value < 0.0);
//
//    if (isOutlier) {
//        for (int c = 0; c < columnCount(); ++c) {
//            if (auto currentItem = item(displayRow, c)) {
//                currentItem->setData(Qt::UserRole, true);
//            }
//        }
//    }
//}

void RealtimeDataTable::checkAndHighlightRow(int row)
{
    const int valueColumn = 2; // Value is in the 3rd column
    if (row < 0 || row >= rowCount()) return;

    QTableWidgetItem* valueItem = item(row, valueColumn);
    if (!valueItem) return;

    bool isNumeric;
    double value = valueItem->text().toDouble(&isNumeric);

    // Specific highlighting logic for this table
    bool isOutlier = isNumeric && (value > 520.0 || value < 0.0);

    for (int c = 0; c < columnCount(); ++c) {
        if (auto currentItem = item(row, c)) {
            currentItem->setData(Qt::UserRole, isOutlier);
        }
    }
}