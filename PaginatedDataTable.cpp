#include "PaginatedDataTable.h"
#include "MutiTableWidget.h" // ��������ͷ�ļ�
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QFont>

PaginatedDataTable::PaginatedDataTable(BaseDataTable* tableToHost, QWidget* parent)
    : QWidget(parent), m_table(tableToHost) // ֱ��ʹ�ô����ָ��
{
    m_navControlsWidget = new QWidget(this);
    m_firstPageButton = new QPushButton("<< ��ҳ", m_navControlsWidget);
    m_prevPageButton = new QPushButton("< ��һҳ", m_navControlsWidget);
    m_pageLabel = new QLabel("�� 1 ҳ / �� 1 ҳ", m_navControlsWidget);
    m_nextPageButton = new QPushButton("��һҳ >", m_navControlsWidget);
    m_lastPageButton = new QPushButton("βҳ >>", m_navControlsWidget);

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

    this->setWindowTitle("ͨ�����ݱ��");
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
    m_pageLabel->setText(QString("�� %1 ҳ / �� %2 ҳ").arg(currentPage).arg(totalPages));
    bool canGoBack = currentPage > 1;
    m_firstPageButton->setEnabled(canGoBack);
    m_prevPageButton->setEnabled(canGoBack);
    bool canGoForward = currentPage < totalPages;
    m_nextPageButton->setEnabled(canGoForward);
    m_lastPageButton->setEnabled(canGoForward);
}
