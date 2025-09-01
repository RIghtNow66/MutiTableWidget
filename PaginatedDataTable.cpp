#include "PaginatedDataTable.h"
#include "MutiTableWidget.h" // 包含基类头文件
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QFont>

PaginatedDataTable::PaginatedDataTable(BaseDataTable* tableToHost, QWidget* parent)
    : QWidget(parent), m_table(tableToHost) // 直接使用传入的指针
{
    m_navControlsWidget = new QWidget(this);
    m_firstPageButton = new QPushButton("<< 首页", m_navControlsWidget);
    m_prevPageButton = new QPushButton("< 上一页", m_navControlsWidget);
    m_pageLabel = new QLabel("第 1 页 / 共 1 页", m_navControlsWidget);
    m_nextPageButton = new QPushButton("下一页 >", m_navControlsWidget);
    m_lastPageButton = new QPushButton("尾页 >>", m_navControlsWidget);

    m_navControlsLayout = new QHBoxLayout(m_navControlsWidget);
    m_navControlsLayout->setContentsMargins(10, 5, 10, 5);
    m_navControlsLayout->addStretch();
    m_navControlsLayout->addWidget(m_firstPageButton);
    m_navControlsLayout->addWidget(m_prevPageButton);
    m_navControlsLayout->addWidget(m_pageLabel);
    m_navControlsLayout->addWidget(m_nextPageButton);
    m_navControlsLayout->addWidget(m_lastPageButton);
    m_navControlsLayout->addStretch();

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    m_table->setParent(this); // Set parent
    m_mainLayout->addWidget(m_table, 1);
    m_mainLayout->addWidget(m_navControlsWidget, 0);

    this->setWindowTitle("通用数据表格");
    this->setWindowIcon(QIcon(":/MutiTableWidget/m_icon/table.png")); 

    connect(m_firstPageButton, &QPushButton::clicked, m_table, &BaseDataTable::goToFirstPage);
    connect(m_prevPageButton, &QPushButton::clicked, m_table, &BaseDataTable::goToPreviousPage);
    connect(m_nextPageButton, &QPushButton::clicked, m_table, &BaseDataTable::goToNextPage);
    connect(m_lastPageButton, &QPushButton::clicked, m_table, &BaseDataTable::goToLastPage);
    connect(m_table, &BaseDataTable::pageChanged, this, &PaginatedDataTable::updateNavigationControls);

    updateNavigationControls(1, 1);
}

void PaginatedDataTable::setData(const QVector<QVector<QString>>& data, const QStringList& headers)
{
    m_table->setFullData(data, headers);
}

void PaginatedDataTable::updateNavigationControls(int currentPage, int totalPages)
{
    if (totalPages <= 0) totalPages = 1;
    m_pageLabel->setText(QString("第 %1 页 / 共 %2 页").arg(currentPage).arg(totalPages));
    bool canGoBack = currentPage > 1;
    m_firstPageButton->setEnabled(canGoBack);
    m_prevPageButton->setEnabled(canGoBack);
    bool canGoForward = currentPage < totalPages;
    m_nextPageButton->setEnabled(canGoForward);
    m_lastPageButton->setEnabled(canGoForward);
}
