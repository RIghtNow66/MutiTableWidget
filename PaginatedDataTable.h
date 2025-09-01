#pragma once
#pragma execution_character_set("utf-8")


#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

// ǰ���������������������ͷ�ļ�
class BaseDataTable;
class QPushButton;
class QLabel;

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
    QHBoxLayout* navLayout;

    BaseDataTable* m_table; // ʹ�û���ָ��
    QPushButton* m_firstPageButton;
    QPushButton* m_prevPageButton;
    QLabel* m_pageLabel;
    QPushButton* m_nextPageButton;
    QPushButton* m_lastPageButton;
};