#include "PaginatedDataTable.h"
#include "MutiTableWidget.h" // 包含基类头文件
#include <QPushButton>
#include <QLabel>


PaginatedDataTable::PaginatedDataTable(BaseDataTable* tableToHost, QWidget* parent)
    : QWidget(parent), m_table(tableToHost) // 直接使用传入的指针
{
    QFont globalFont("Microsoft YaHei");
    globalFont.setStyleHint(QFont::SansSerif);
    this->setFont(globalFont);

    m_firstPageButton = new QPushButton("<< 首页");
    m_prevPageButton = new QPushButton("< 上一页");
    m_pageLabel = new QLabel("第 1 页 / 共 1 页");
    m_nextPageButton = new QPushButton("下一页 >");
    m_lastPageButton = new QPushButton("尾页 >>");


    // --- 步骤 2: 创建并设置布局 ---
    auto* navLayout = new QHBoxLayout();
    navLayout->setContentsMargins(10, 5, 10, 5); // 增加一些边距，让UI更好看
    navLayout->addStretch();
    navLayout->addWidget(m_firstPageButton);
    navLayout->addWidget(m_prevPageButton);
    navLayout->addWidget(m_pageLabel);
    navLayout->addWidget(m_nextPageButton);
    navLayout->addWidget(m_lastPageButton);
    navLayout->addStretch();

    // 设置主窗口的垂直布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // --- 步骤 3: 将已创建的控件按正确关系添加到主布局中 ---

    // 关键布局：表格占据所有可用空间(拉伸因子=1)，导航条保持固定高度(拉伸因子=0)
    m_mainLayout->addWidget(m_table, 1);
    m_mainLayout->addLayout(navLayout, 0);


    // --- 步骤 4: 设置窗口属性和连接信号槽 ---

    this->setWindowTitle("实时数据表格");
    this->setWindowIcon(QIcon(":/state_table/table.png"));

    // 连接按钮点击信号到表格的翻页槽
    connect(m_firstPageButton, &QPushButton::clicked, m_table, &BaseDataTable::goToFirstPage);
    connect(m_prevPageButton, &QPushButton::clicked, m_table, &BaseDataTable::goToPreviousPage);
    connect(m_nextPageButton, &QPushButton::clicked, m_table, &BaseDataTable::goToNextPage);
    connect(m_lastPageButton, &QPushButton::clicked, m_table, &BaseDataTable::goToLastPage);

    // 连接表格的页码变化信号到导航条的更新槽
    connect(m_table, &BaseDataTable::pageChanged, this, &PaginatedDataTable::updateNavigationControls);

    // 初始化UI状态
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
