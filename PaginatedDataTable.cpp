#include "PaginatedDataTable.h"
#include "MutiTableWidget.h" // ��������ͷ�ļ�
#include <QPushButton>
#include <QLabel>


PaginatedDataTable::PaginatedDataTable(BaseDataTable* tableToHost, QWidget* parent)
    : QWidget(parent), m_table(tableToHost) // ֱ��ʹ�ô����ָ��
{
    QFont globalFont("Microsoft YaHei");
    globalFont.setStyleHint(QFont::SansSerif);
    this->setFont(globalFont);

    m_firstPageButton = new QPushButton("<< ��ҳ");
    m_prevPageButton = new QPushButton("< ��һҳ");
    m_pageLabel = new QLabel("�� 1 ҳ / �� 1 ҳ");
    m_nextPageButton = new QPushButton("��һҳ >");
    m_lastPageButton = new QPushButton("βҳ >>");


    // --- ���� 2: ���������ò��� ---
    auto* navLayout = new QHBoxLayout();
    navLayout->setContentsMargins(10, 5, 10, 5); // ����һЩ�߾࣬��UI���ÿ�
    navLayout->addStretch();
    navLayout->addWidget(m_firstPageButton);
    navLayout->addWidget(m_prevPageButton);
    navLayout->addWidget(m_pageLabel);
    navLayout->addWidget(m_nextPageButton);
    navLayout->addWidget(m_lastPageButton);
    navLayout->addStretch();

    // ���������ڵĴ�ֱ����
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // --- ���� 3: ���Ѵ����Ŀؼ�����ȷ��ϵ��ӵ��������� ---

    // �ؼ����֣����ռ�����п��ÿռ�(��������=1)�����������̶ֹ��߶�(��������=0)
    m_mainLayout->addWidget(m_table, 1);
    m_mainLayout->addLayout(navLayout, 0);


    // --- ���� 4: ���ô������Ժ������źŲ� ---

    this->setWindowTitle("ʵʱ���ݱ��");
    this->setWindowIcon(QIcon(":/state_table/table.png"));

    // ���Ӱ�ť����źŵ����ķ�ҳ��
    connect(m_firstPageButton, &QPushButton::clicked, m_table, &BaseDataTable::goToFirstPage);
    connect(m_prevPageButton, &QPushButton::clicked, m_table, &BaseDataTable::goToPreviousPage);
    connect(m_nextPageButton, &QPushButton::clicked, m_table, &BaseDataTable::goToNextPage);
    connect(m_lastPageButton, &QPushButton::clicked, m_table, &BaseDataTable::goToLastPage);

    // ���ӱ���ҳ��仯�źŵ��������ĸ��²�
    connect(m_table, &BaseDataTable::pageChanged, this, &PaginatedDataTable::updateNavigationControls);

    // ��ʼ��UI״̬
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
