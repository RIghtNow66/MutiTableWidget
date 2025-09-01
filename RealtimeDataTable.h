#pragma once
#include "MutiTableWidget.h"


class RealtimeDataTable : public BaseDataTable {
    Q_OBJECT
public:
    explicit RealtimeDataTable(QWidget* parent = nullptr);
protected:
    //void checkAndHighlightRow(int displayRow, const QVector<QString>& rowData) override;
    void checkAndHighlightRow(int row) override;
};