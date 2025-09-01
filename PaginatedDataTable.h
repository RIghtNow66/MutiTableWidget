#pragma once
#pragma execution_character_set("utf-8")


#include <QWidget>
#include <QVector>
#include <QString>
#include <QStringList>

class BaseDataTable;
class QPushButton;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;

class PaginatedDataTable : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 改造后的构造函数，接收一个 BaseDataTable 指针。
     * @param tableToHost 要在窗口中托管和显示的表格实例。
     * @param parent 父窗口。
     */
    explicit PaginatedDataTable(BaseDataTable* tableToHost, QWidget* parent = nullptr);

    void setData(const QVector<QVector<QString>>& data, const QStringList& headers);

private slots:
    void updateNavigationControls(int currentPage, int totalPages);

private:
    QVBoxLayout* m_mainLayout;
    BaseDataTable* m_table; // 使用基类指针

    QWidget* m_navControlsWidget;
    QHBoxLayout* m_navControlsLayout;
    QPushButton* m_firstPageButton;
    QPushButton* m_prevPageButton;
    QPushButton* m_nextPageButton;
    QPushButton* m_lastPageButton;
    QLabel* m_pageLabel;
};