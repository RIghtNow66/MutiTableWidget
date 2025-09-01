#include "UniversalDelegate.h"
#include "MutiTableWidget.h"   

#include <QPainter>
#include <QStyleOptionViewItem>
#include <QApplication>
#include <QStyle>
#include <QColor>
#include <QBrush>


UniversalDelegate::UniversalDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

void UniversalDelegate::setStatusCircleColumn(int column, bool enable)
{
    m_statusCircleColumns.insert(column);
}

void UniversalDelegate::removeStatusCircleColumn(int column)
{
    m_statusCircleColumns.remove(column);
}

void UniversalDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    // 1. �������ȵ��û����ʵ�֣�������������Ĭ����ʽ������ QSS �е���ʽ������ѡ�б���ɫ����
    QStyledItemDelegate::initStyleOption(option, index);

    // 2. ��ȡ�����ڱ������ͨ�� setData(Qt::UserRole, ...) ���õġ��Ƿ�Ϊ�쳣ֵ��״̬��
    bool isOutlier = index.data(Qt::UserRole).toBool();

    // 3. ������ʽ���ȼ���
    //    �����Ԫ�����쳣ֵ����������ǰ��û�С���ѡ��...
    if (isOutlier && !(option->state & QStyle::State_Selected))
    {
        // ...��ô���Ǿ�Ӧ�÷�ɫ�����ͺ�ɫ��������ʽ��
        option->backgroundBrush = QBrush(QColor("#FFEAEA")); // ����ɫ����
        option->palette.setColor(QPalette::Text, QColor("#8B0000")); // ����ɫ����
        option->font.setBold(true);
    }
    // �������������������ͨ��Ԫ�񡢱�ѡ�е��쳣��Ԫ�񣩣����ǲ����κβ�����
    // ��ȫ���������QSSȥ��������������ȷ��ʾĬ�ϱ�������ɫѡ�б�����
}

void UniversalDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // ���� 1: ����һ�����޸ĵ���ʽѡ��������ǽ��ڻ���ǰ��������޸ġ�
    QStyleOptionViewItem viewOption(option);

    // ���� 2: ����������д�� initStyleOption��
    // ��һ��������Ҫ�����Ὣ�и����߼�����ɫ�����ȣ�Ӧ�õ� viewOption �С�
    initStyleOption(&viewOption, index);

    // ���� 3: �Ӹ��ؼ�����񣩻�ȡ��̬�����С��Ӧ�á�
    // ��ʹ�õ�Ԫ�����������ű������Ŷ�ƽ���ر仯��
    if (const BaseDataTable* table = qobject_cast<const BaseDataTable*>(parent())) {
        QFont font = viewOption.font;
        font.setPointSize(table->getCurrentPointSize());
        viewOption.font = font;
    }

    // ���� 4: �жϵ�ǰ���Ƿ���Ҫ����������ƣ�״̬Բ�㣩��
    if (m_statusCircleColumns.contains(index.column())) {
        // --- ״̬Բ���е��Զ�������߼� ---

        // a. �ڻ���ǰ���ֶ�����ı�����Ϊ����ֻ����ʾԲ�㡣
        viewOption.text = QString();

        // b. ʹ�� QSS �����ǵ���ʽ�������ı����Ȼ��Ƶ�Ԫ��ı�����ѡ�п�ȡ�
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &viewOption, painter, viewOption.widget);

        // c. �ڱ���֮�ϻ��������Զ����Բ�㡣
        QString status = index.data(Qt::DisplayRole).toString();
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true); // ��������ݣ�ʹԲ���ƽ��

        // d. ����Բ��Ļ�������ʹ���ڵ�Ԫ���ھ��в�����Բ�Ρ�
        int margin = 5; // Բ����뵥Ԫ��߽�ľ���
        QRectF ellipseRect = option.rect.adjusted(margin, margin, -margin, -margin);
        int diameter = qMin(ellipseRect.width(), ellipseRect.height()); // ȡ�����Сֵȷ����Բ��
        ellipseRect.setWidth(diameter);
        ellipseRect.setHeight(diameter);
        ellipseRect.moveCenter(option.rect.center()); // �ƶ�����Ԫ������

        // e. ���� status ��ֵѡ��ͬ����ɫ��
        //    ���������������б���е�״̬��ɫ��ʹ��������ͨ�á���
        if (status == "0") {
            painter->setBrush(QBrush(QColor("#28a745"))); // ����/��ʾ (��ɫ)
        }
        else if (status == "1") {
            painter->setBrush(QBrush(QColor("#ffc107"))); // ���� (��ɫ)
        }
        else if (status == "2") {
            painter->setBrush(QBrush(QColor("#dc3545"))); // ����/���� (��ɫ)
        }
        else {
            painter->setBrush(QBrush(QColor("#007bff"))); // ����/δ֪ (��ɫ)
        }

        painter->setPen(Qt::NoPen); // ���ǲ�ϣ��Բ���б߿�
        painter->drawEllipse(ellipseRect);
        painter->restore();

    }
    else {
        // --- ����������ͨ�еĻ����߼� ---

        // ֱ�ӵ��û���� paint ����������������׼���õġ�������
        // ��ȷ������ʽ�������С�� viewOption ����ɱ�׼���Ƽ��ɡ�
        QStyledItemDelegate::paint(painter, viewOption, index);
    }
}