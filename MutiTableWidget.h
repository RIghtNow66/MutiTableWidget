#pragma once

#include <QTableWidget>
#include "NonSortingHeaderView.h"
#include <QElapsedTimer>
#include <QVector>
#include <QTimer>
#include <QMap>
#include <QSet>

class BaseDataTable : public QTableWidget
{
    Q_OBJECT

public:
    explicit BaseDataTable(QWidget* parent = nullptr);
    virtual ~BaseDataTable(); // 基类析构函数应为虚函数

    // --- 所有公共接口和槽函数 ---
    int getCurrentPointSize() const;
    void setFullData(const QVector<QVector<QString>>& data, const QStringList& headers);

public slots:
    void goToPage(int page);
    void goToNextPage();
    void goToPreviousPage();
    void goToFirstPage();
    void goToLastPage();
    void onSectionResized(int logicalIndex, int oldSize, int newSize);
    void applyFilter(int logicalIndex);

    void clearFilter(int logicalIndex);

signals:
    void pageChanged(int currentPage, int totalPages);

protected:
    // --- 所有公共的保护成员和事件处理 ---
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    // ★ 模板方法：定义高亮检查的接口，由子类实现具体逻辑
    virtual void checkAndHighlightRow(int row) {
        // 基类默认实现为空，即不做任何高亮处理
    }

    QVector<QVector<QString>> m_fullData;
    QStringList m_headers;
    int m_currentPage = 1;
    int m_totalPages = 0;
    const int m_pageSize = 15;

    int m_currentPointSize = 12;

private:
    // --- 所有私有方法和成员变量 ---
    void displayCurrentPage(); // displayCurrentPage内部会调用虚函数checkAndHighlightRow
    void adjustRowHeightAndFont();
    void updateColumnLayout();
    void calculateMinimumColumnWidthsAndProportions();
    void applyFiltersAndDisplay();

    // ... 其他所有相同的私有成员 ...
    QElapsedTimer m_resizeThrottleTimer;
    QVector<double> m_columnProportions;
    bool m_isInitialLayout = true;
    QVector<QVector<QString>> m_filteredData;
    QMap<int, QSet<QString>> m_activeFilters;

    const int m_throttleIntervalMs = 35;
};

