#pragma once
#include <QStyledItemDelegate>
#include <QSet>

class UniversalDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit UniversalDelegate(QObject* parent = nullptr);

    // ������������Щ����Ҫ�����Ƴ�״̬Բ��
    void setStatusCircleColumn(int column, bool enabled = true);

    void removeStatusCircleColumn(int column);
protected:
    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QSet<int> m_statusCircleColumns; // �洢��Ҫ��Բ���������
};
