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
     * @brief �����Ĺ��캯��������һ�� BaseDataTable ָ�롣
     * @param tableToHost Ҫ�ڴ������йܺ���ʾ�ı��ʵ����
     * @param parent �����ڡ�
     */
    explicit PaginatedDataTable(BaseDataTable* tableToHost, QWidget* parent = nullptr);

    void setData(const QVector<QVector<QString>>& data, const QStringList& headers);

private slots:
    void updateNavigationControls(int currentPage, int totalPages);

private:
    QVBoxLayout* m_mainLayout;
    BaseDataTable* m_table; // ʹ�û���ָ��

    QWidget* m_navControlsWidget;
    QHBoxLayout* m_navControlsLayout;
    QPushButton* m_firstPageButton;
    QPushButton* m_prevPageButton;
    QPushButton* m_nextPageButton;
    QPushButton* m_lastPageButton;
    QLabel* m_pageLabel;
};