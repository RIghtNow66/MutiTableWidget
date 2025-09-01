#include "HighlightDelegate.h"
#include <QStyleOptionViewItem>
#include <QStyle>
#include <QColor>
#include <QBrush>
#include <QApplication>
#include <QPainter>
#include "MutiTableWidget.h"


HighlightDelegate::HighlightDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}


void HighlightDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // ���� 1: ����һ�����޸ĵ���ʽѡ���
    QStyleOptionViewItem viewOption(option);

    // ���� 2: ���ؼ��޸���ִ��Ĭ�ϵĳ�ʼ����
    // ��һ�����ģ���л�ȡ���ݣ������ı������뷽ʽ����䵽 viewOption �С�
    // �������������³��ֵĹؼ���
    initStyleOption(&viewOption, index);

    // ���� 3: �����ָܻ����Ӹ��ؼ���ȡ��̬�����С��Ӧ�á�
    // �����������ܹ�����Ӧ���ŵĹؼ���
    if (const BaseDataTable* table = qobject_cast<const BaseDataTable*>(parent())) {
        QFont font = viewOption.font;
        font.setPointSize(table->getCurrentPointSize());
        viewOption.font = font;
    }

    // ���� 4: ��ʼ�����н����Զ������
    if (index.column() == STATUS_COLUMN_INDEX) {
        // ����״̬�У����ǲ�ϣ����ʾ "0" �� "1" ��Щ���֣�ֻ��ʾ��Բ��
        // ���ԣ��ڻ���ǰ�������ֶ�����ı���
        viewOption.text = QString();

        // �Ȼ��Ƶ�Ԫ��ı�����ѡ�п�ȣ���ʱ�����ı�����
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &viewOption, painter, viewOption.widget);

        // Ȼ���ڱ���֮�ϻ��������Զ������Բ��
        QString status = index.data(Qt::DisplayRole).toString();
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        int margin = 4;
        QRectF ellipseRect = option.rect.adjusted(margin, margin, -margin, -margin);
        int diameter = qMin(ellipseRect.width(), ellipseRect.height());
        ellipseRect.setWidth(diameter);
        ellipseRect.setHeight(diameter);
        ellipseRect.moveCenter(option.rect.center());

        if (status == "0") {
            painter->setBrush(QBrush(QColor("#28a745"))); // ��ɫ
        }
        else if (status == "1") {
            painter->setBrush(QBrush(QColor("#dc3545"))); // ��ɫ
        }

        painter->setPen(Qt::NoPen);
        painter->drawEllipse(ellipseRect);
        painter->restore();

    }
    else {
        // ��������������ͨ�У������Ѿ�����˳�ʼ�����������ã�
        // ֱ�ӵ��û���� paint ����������������׼���õ� viewOption ����ɱ�׼���Ƽ��ɡ�
        QStyledItemDelegate::paint(painter, viewOption, index);
    }
}

void HighlightDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    // 1. ��������{�û�Č��F����������QSS�еĘ�ʽ�������{ɫ�x�б�����
    QStyledItemDelegate::initStyleOption(option, index);

    // 2. �@ȡ�҂��� My_QTableWidget ���O�õġ��Ƿ�鮐��ֵ����B
    bool isOutlier = index.data(Qt::UserRole).toBool();

    // 3. ���x��ʽ���ȼ���
    //    �����Ԫ���Ǯ���ֵ���K������ǰ���]�С����x��...
    if (isOutlier && !(option->state & QStyle::State_Selected))
    {
        // ...���N�҂��͑��÷�ɫ�����ͼtɫ���w�֘�ʽ��
        option->backgroundBrush = QBrush(QColor("#FFEAEA"));
        option->palette.setColor(QPalette::Text, QColor("#8B0000"));
        option->font.setBold(true);
    }
    // �������������r����ͨ��Ԫ�񡢱��x�еĮ�����Ԫ�񣩣��҂������κβ�����
    // ��ȫ���o���QSSȥ̎���@�Ӿ������_�@ʾ��ɫ�������{ɫ�x�б�����
}