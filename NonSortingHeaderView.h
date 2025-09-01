#pragma once

#include <QHeaderView>
#include <QMap>
#include <QMenu>
#include <QAction>
#include <QMouseEvent> 


class NonSortingHeaderView : public QHeaderView
{
    Q_OBJECT

public:
    explicit NonSortingHeaderView(Qt::Orientation orientation, QWidget* parent = nullptr);

    // ���ڴ��ⲿ(Data_table)����ɸѡ״̬�Ĺ�������
    void setFilterState(int logicalIndex, bool isActive);

signals:
    // ����źű��ֲ���
    void filterTriggered(int logicalIndex);

protected:
    // ��д paintSection ������ͼ��
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;

    // ��д mousePressEvent �������ͼ��ĵ��
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;

private slots:
    // ������ʾ�Ҽ��˵��Ĳۺ���
    void showContextMenu(const QPoint& pos);

private:
    // �洢ÿ���е�ɸѡ����״̬
    QMap<int, bool> m_filterStates;

    // ��ȡָ���е�ͼ���������
    QRect getIconRect(const QRect& sectionRect, int logicalIndex) const;
};
