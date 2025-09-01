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
    virtual ~BaseDataTable(); // ������������ӦΪ�麯��

    // --- ���й����ӿںͲۺ��� ---
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
    // --- ���й����ı�����Ա���¼����� ---
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    // �� ģ�巽��������������Ľӿڣ�������ʵ�־����߼�
    virtual void checkAndHighlightRow(int row) {
        // ����Ĭ��ʵ��Ϊ�գ��������κθ�������
    }

    QVector<QVector<QString>> m_fullData;
    QStringList m_headers;
    int m_currentPage = 1;
    int m_totalPages = 0;
    const int m_pageSize = 15;

    int m_currentPointSize = 12;

private:
    // --- ����˽�з����ͳ�Ա���� ---
    void displayCurrentPage(); // displayCurrentPage�ڲ�������麯��checkAndHighlightRow
    void adjustRowHeightAndFont();
    void updateColumnLayout();
    void calculateMinimumColumnWidthsAndProportions();
    void applyFiltersAndDisplay();

    // ... ����������ͬ��˽�г�Ա ...
    QElapsedTimer m_resizeThrottleTimer;
    QVector<double> m_columnProportions;
    bool m_isInitialLayout = true;
    QVector<QVector<QString>> m_filteredData;
    QMap<int, QSet<QString>> m_activeFilters;

    const int m_throttleIntervalMs = 35;
};

