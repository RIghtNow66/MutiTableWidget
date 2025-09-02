#pragma execution_character_set("utf-8")

#include "MutiTableWidget.h"
#include <QHeaderView>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <algorithm>
#include <QScrollBar>
#include <numeric>
#include <QFontMetrics> 
#include <QMenu>
#include <QSet>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QDialog>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QScrollArea>

const int MINIMUM_SECTION_WIDTH = 30;

BaseDataTable::BaseDataTable(QWidget* parent)
    : QTableWidget(parent)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    NonSortingHeaderView* newHeader = new NonSortingHeaderView(Qt::Horizontal, this);
    setHorizontalHeader(newHeader);
    horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    border-top: 0px solid #D3D3D3;"
        "    border-left: 0px solid #D3D3D3;"
        "    border-right: 3px solid #D3D3D3;"
        "    border-bottom: 2px solid #D3D3D3;"
        "    background-color: #E8F0FE;"
        "    padding: 4px;"
        "    padding-right: 20px;"
        "    padding-left: 20px;"
        "    text-align: center;"
        "    font-weight: bold; "
        "}"
    );
    this->setShowGrid(false);
    this->setStyleSheet(
        "QTableWidget { gridline-color: #E8E8E8; }"
        "QTableWidget::item { padding: 1px; }"
        "QTableWidget::item:selected {"
        "    background-color: #E6F7FF;"
        "    color: #000000;"
        "}"
        "QTableWidget::item:selected:focus {"
        "    background-color: #B3E0FF;"
        "}"
    );
    setSelectionBehavior(QAbstractItemView::SelectItems);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_resizeThrottleTimer.start();
    connect(horizontalHeader(), &QHeaderView::sectionResized, this, &BaseDataTable::onSectionResized);

    // 连接表头的筛选信号到处理槽
    connect(newHeader, &NonSortingHeaderView::filterTriggered, this, &BaseDataTable::applyFilter);

    connect(newHeader, &NonSortingHeaderView::clearFilterTriggered, this, &BaseDataTable::clearFilter);
}

void BaseDataTable::clearFilter(int logicalIndex)
{
    if (m_activeFilters.contains(logicalIndex)) {
        m_activeFilters.remove(logicalIndex);

        auto* header = qobject_cast<NonSortingHeaderView*>(horizontalHeader());
        if (header) {
            header->setFilterState(logicalIndex, false);
        }
        applyFiltersAndDisplay();
    }
}

void BaseDataTable::onSectionResized(int logicalIndex, int oldSize, int newSize)
{
    if (signalsBlocked() || columnCount() <= 1 || logicalIndex == columnCount() - 1) {
        // 如果是程序内部调整、只有一列或正在拖动最后一列，则不执行此逻辑
        return;
    }

    horizontalHeader()->blockSignals(true);

    int delta = newSize - oldSize;
    long long rightTotalWidth = 0;

    // 1. 计算所有右侧列的总宽度
    for (int i = logicalIndex + 1; i < columnCount(); ++i) {
        rightTotalWidth += horizontalHeader()->sectionSize(i);
    }

    if (rightTotalWidth > 0) {
        int appliedDelta = 0;
        // 2. 按比例重新分配delta到右侧各列
        for (int i = logicalIndex + 1; i < columnCount(); ++i) {
            int sectionSize = horizontalHeader()->sectionSize(i);
            double proportion = static_cast<double>(sectionSize) / rightTotalWidth;
            int change = static_cast<int>(delta * proportion);

            int newSectionSize = sectionSize - change;

            // 施加最小宽度限制
            if (newSectionSize < MINIMUM_SECTION_WIDTH) {
                change = sectionSize - MINIMUM_SECTION_WIDTH;
                newSectionSize = MINIMUM_SECTION_WIDTH;
            }

            horizontalHeader()->resizeSection(i, newSectionSize);
            appliedDelta += change;
        }

        // 3. 修正被拖动列的宽度，以确保总宽度不变
        //    这可以防止因取整误差或最小宽度限制导致的跳动
        horizontalHeader()->resizeSection(logicalIndex, newSize - (delta - appliedDelta));
    }

    // 4. 操作完成后，更新全局的列宽比例，用于窗口的整体缩放
    m_isInitialLayout = false;
    long long totalWidth = 0;
    for (int i = 0; i < columnCount(); ++i) {
        totalWidth += horizontalHeader()->sectionSize(i);
    }
    if (totalWidth > 0) {
        m_columnProportions.resize(columnCount());
        for (int i = 0; i < columnCount(); ++i) {
            m_columnProportions[i] = static_cast<double>(horizontalHeader()->sectionSize(i)) / totalWidth;
        }
    }

    horizontalHeader()->blockSignals(false);
}

void BaseDataTable::updateColumnLayout()
{
    if (columnCount() == 0 || m_columnProportions.isEmpty() || m_columnProportions.size() != columnCount()) return;

    horizontalHeader()->blockSignals(true);

    // 多次尝试获取正确的可用宽度
    int availableWidth = viewport()->width();

    // 如果viewport宽度为0或异常小，尝试其他方式获取
    if (availableWidth <= 0) {
        availableWidth = width() - verticalHeader()->width() - 2; // 减去垂直表头和边框
        if (availableWidth <= 0) {
            horizontalHeader()->blockSignals(false);
            return;
        }
    }

    long long totalNewWidth = 0;
    QVector<int> newWidths(columnCount());

    for (int c = 0; c < columnCount(); ++c) {
        int newWidth = static_cast<int>(availableWidth * m_columnProportions[c]);
        newWidth = std::max(newWidth, MINIMUM_SECTION_WIDTH); // 确保不小于最小宽度
        newWidths[c] = newWidth;
        totalNewWidth += newWidth;
    }

    // 处理总宽度超出或不足的情况
    if (totalNewWidth != availableWidth && columnCount() > 0) {
        int diff = availableWidth - totalNewWidth;
        // 将差值分配给最后一列
        newWidths[columnCount() - 1] = std::max(newWidths[columnCount() - 1] + diff, MINIMUM_SECTION_WIDTH);
    }

    // 应用新宽度
    for (int c = 0; c < columnCount(); ++c) {
        horizontalHeader()->resizeSection(c, newWidths[c]);
    }

    horizontalHeader()->blockSignals(false);

    // 强制重绘以确保视觉更新
    update();
}

// 弹出筛选对话框的槽函数
void BaseDataTable::applyFilter(int logicalIndex)
{
    // ★ 关键改动：移除了函数开头的 if 判断，现在此函数只负责打开对话框
    auto* header = qobject_cast<NonSortingHeaderView*>(horizontalHeader());

    QSet<QString> uniqueValues;
    for (const auto& rowData : m_fullData) {
        if (logicalIndex < rowData.size()) {
            uniqueValues.insert(rowData[logicalIndex]);
        }
    }

    QDialog filterDialog(this);
    filterDialog.setWindowTitle(QString("筛选: %1").arg(m_headers[logicalIndex]));

    // ... (对话框的剩余代码保持不变) ...
    QVBoxLayout* layout = new QVBoxLayout(&filterDialog);
    QList<QCheckBox*> checkBoxes;
    QWidget* checkBoxContainer = new QWidget;
    QVBoxLayout* checkBoxLayout = new QVBoxLayout(checkBoxContainer);
    QCheckBox* selectAllCheckBox = new QCheckBox("全选", this);
    checkBoxLayout->addWidget(selectAllCheckBox);

    for (const QString& value : uniqueValues) {
        QCheckBox* checkBox = new QCheckBox(value, &filterDialog);
        if (!m_activeFilters.contains(logicalIndex) || m_activeFilters[logicalIndex].contains(value)) {
            checkBox->setChecked(true);
        }
        checkBoxes.append(checkBox);
        checkBoxLayout->addWidget(checkBox);
    }

    connect(selectAllCheckBox, &QCheckBox::stateChanged, this, [=](int state) {
        for (auto* box : checkBoxes) {
            box->setChecked(state == Qt::Checked);
        }
        });

    QScrollArea* scrollArea = new QScrollArea(&filterDialog);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(checkBoxContainer);
    layout->addWidget(scrollArea);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Reset, &filterDialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &filterDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &filterDialog, &QDialog::reject);

    // Reset 按钮的逻辑已经是正确的，它调用 clearFilter
    connect(buttonBox->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, [this, logicalIndex, &filterDialog]() {
        clearFilter(logicalIndex);
        filterDialog.reject();
        });

    layout->addWidget(buttonBox);

    if (filterDialog.exec() == QDialog::Accepted) {
        QSet<QString> selectedValues;
        for (const auto& checkBox : checkBoxes) {
            if (checkBox->isChecked()) {
                selectedValues.insert(checkBox->text());
            }
        }

        if (selectedValues.isEmpty() || selectedValues.size() == uniqueValues.size()) {
            m_activeFilters.remove(logicalIndex);
            if (header) {
                header->setFilterState(logicalIndex, false);
            }
        }
        else {
            m_activeFilters[logicalIndex] = selectedValues;
            if (header) {
                header->setFilterState(logicalIndex, true);
            }
        }
        applyFiltersAndDisplay();
    }
}

// 应用所有筛选器并刷新表格
void BaseDataTable::applyFiltersAndDisplay()
{
    if (m_activeFilters.isEmpty()) {
        m_filteredData = m_fullData;
    }
    else {
        m_filteredData.clear();
        for (const auto& rowData : m_fullData) {
            bool pass = true;
            for (auto it = m_activeFilters.constBegin(); it != m_activeFilters.constEnd(); ++it) {
                int col = it.key();
                const QSet<QString>& filterValues = it.value();
                if (col >= rowData.size() || !filterValues.contains(rowData[col])) {
                    pass = false;
                    break;
                }
            }
            if (pass) {
                m_filteredData.append(rowData);
            }
        }
    }

    if (m_filteredData.isEmpty()) {
        m_totalPages = 1;
    }
    else {
        m_totalPages = (m_filteredData.size() + m_pageSize - 1) / m_pageSize;
    }
    goToPage(1);
}


void BaseDataTable::setFullData(const QVector<QVector<QString>>& data, const QStringList& headers)
{
    m_fullData = data;
    m_headers = headers;
    m_isInitialLayout = true;

    // 清除旧的筛选条件和所有图标
    auto* header = qobject_cast<NonSortingHeaderView*>(horizontalHeader());
    if (header) {
        for (int colIndex : m_activeFilters.keys()) {
            header->setFilterState(colIndex, false);
        }
    }
    m_activeFilters.clear();

    setColumnCount(m_headers.size());
    setHorizontalHeaderLabels(m_headers);

    applyFiltersAndDisplay();
}

void BaseDataTable::displayCurrentPage()
{
    bool oldState = blockSignals(true);
    clearContents();
    setRowCount(0);

    if (m_filteredData.isEmpty()) {
        blockSignals(oldState);
        emit pageChanged(m_currentPage, m_totalPages);
        QTimer::singleShot(0, this, [this]() {
            updateGeometries();
            updateColumnLayout();
            });
        return;
    }

    // 分页计算基于 m_filteredData
    int startRow = (m_currentPage - 1) * m_pageSize;
    int endRow = std::min(startRow + m_pageSize, m_filteredData.size());
    setRowCount(endRow - startRow);

    for (int i = startRow; i < endRow; ++i) {
        // 从 m_filteredData 获取行数据
        const QVector<QString>& rowData = m_filteredData[i];
        int displayRow = i - startRow;

        for (int c = 0; c < m_headers.size(); ++c) {
            QString cellText = c < rowData.size() ? rowData[c] : QString();
            QTableWidgetItem* it = new QTableWidgetItem(cellText);
            it->setTextAlignment(Qt::AlignCenter);
            setItem(displayRow, c, it);
        }
        checkAndHighlightRow(displayRow);
    }

    if (m_isInitialLayout) {
        calculateMinimumColumnWidthsAndProportions();
    }

    QTimer::singleShot(0, this, [this]() {
        updateGeometries();
        updateColumnLayout();
        });

    blockSignals(oldState);
    adjustRowHeightAndFont();
    emit pageChanged(m_currentPage, m_totalPages);
}

void BaseDataTable::resizeEvent(QResizeEvent* event)
{
    QTableWidget::resizeEvent(event);

    // 立即处理窗口状态变化（如最大化/最小化）
    if (event->oldSize().width() == 0 || event->oldSize().height() == 0 ||
        abs(event->size().width() - event->oldSize().width()) > 100 ||
        abs(event->size().height() - event->oldSize().height()) > 100) {
        // 窗口大小变化较大，可能是状态变化，强制更新
        adjustRowHeightAndFont();
        updateColumnLayout();
        m_resizeThrottleTimer.restart();
        return;
    }

    if (m_resizeThrottleTimer.elapsed() < m_throttleIntervalMs) {
        // 使用QTimer::singleShot延迟执行，确保不会丢失最终的布局更新
        QTimer::singleShot(m_throttleIntervalMs + 10, this, [this]() {
            adjustRowHeightAndFont();
            updateColumnLayout();
            });
        return;
    }

    adjustRowHeightAndFont();
    updateColumnLayout();
    m_resizeThrottleTimer.restart();
}

void BaseDataTable::calculateMinimumColumnWidthsAndProportions()
{
    if (columnCount() == 0) return;
    horizontalHeader()->blockSignals(true);
    QVector<int> minWidths;
    minWidths.reserve(columnCount());
    long long totalMinWidth = 0;
    for (int c = 0; c < columnCount(); ++c) {
        horizontalHeader()->setSectionResizeMode(c, QHeaderView::ResizeToContents);
        int width = horizontalHeader()->sectionSize(c);
        minWidths.append(width);
        totalMinWidth += width;
        horizontalHeader()->setSectionResizeMode(c, QHeaderView::Interactive);
    }
    if (totalMinWidth > 0) {
        m_columnProportions.resize(columnCount());
        for (int c = 0; c < columnCount(); ++c) {
            m_columnProportions[c] = static_cast<double>(minWidths[c]) / totalMinWidth;
        }
    }
    horizontalHeader()->blockSignals(false);
}

void BaseDataTable::adjustRowHeightAndFont()
{
    if (rowCount() == 0) return;
    int oldPointSize = m_currentPointSize;
    int availableHeight = this->height();
    int newPointSize = static_cast<int>((availableHeight / (double)m_pageSize) * 0.25);
    newPointSize = std::min(std::max(newPointSize, 8), 12);
    m_currentPointSize = newPointSize;
    if (oldPointSize != m_currentPointSize) {
        m_isInitialLayout = true;
        calculateMinimumColumnWidthsAndProportions();
    }
    QFont font;
    font.setPointSize(m_currentPointSize);
    QFontMetrics fm(font);
    int verticalPadding = 16;
    int comfortableMinHeight = fm.height() + verticalPadding;
    int fillHeight = viewport()->height() / m_pageSize;
    int finalRowHeight = std::max(comfortableMinHeight, fillHeight);
    for (int i = 0; i < rowCount(); ++i) {
        this->setRowHeight(i, finalRowHeight);
    }
    QFont headerFont = horizontalHeader()->font();
    if (headerFont.pointSize() != m_currentPointSize) {
        headerFont.setPointSize(m_currentPointSize);
        horizontalHeader()->setFont(headerFont);
    }
}

void BaseDataTable::goToPage(int page) { if (page >= 1 && page <= m_totalPages) { m_currentPage = page; displayCurrentPage(); } }
void BaseDataTable::goToNextPage() { goToPage(m_currentPage + 1); }
void BaseDataTable::goToPreviousPage() { goToPage(m_currentPage - 1); }
void BaseDataTable::goToFirstPage() { goToPage(1); }
void BaseDataTable::goToLastPage() { goToPage(m_totalPages); }
int BaseDataTable::getCurrentPointSize() const { return m_currentPointSize; }


void BaseDataTable::paintEvent(QPaintEvent* event)
{
    QTableWidget::paintEvent(event);
    QPainter painter(viewport());
    QPen pen(QColor("#D3D3D3"));
    pen.setWidth(2);
    painter.setPen(pen);
    for (int row = 0; row < this->rowCount(); ++row) {
        for (int col = 0; col < this->columnCount(); ++col) {
            QRect cellRect = this->visualRect(this->model()->index(row, col));
            if (event->rect().intersects(cellRect)) {
                painter.drawLine(cellRect.topRight().x() - 1, cellRect.topRight().y(), cellRect.bottomRight().x() - 1, cellRect.bottomRight().y());
                painter.drawLine(cellRect.bottomLeft().x(), cellRect.bottomLeft().y() - 1, cellRect.bottomRight().x(), cellRect.bottomRight().y() - 1);
            }
        }
    }
}

BaseDataTable::~BaseDataTable() {}
