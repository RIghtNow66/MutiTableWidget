#include "FaultyTableDelegate.h"
#include "MutiTableWidget.h"
#include <QPainter>
#include <QApplication>

const int FAULTY_STATUS_COLUMN = 2; // ����״̬��

FaultyTableDelegate::FaultyTableDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

void FaultyTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // ���� 1: ������ʽѡ���������Ĭ�ϳ�ʼ��
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    // ���� 2: ���� �ؼ������� ����
    // �Ӹ��ؼ�����񣩻�ȡ��̬�����С
    if (const BaseDataTable* table = qobject_cast<const BaseDataTable*>(parent())) {
        QFont font = viewOption.font;
        font.setPointSize(table->getCurrentPointSize());
        viewOption.font = font;
    }

    // ���� 3: �ж��Ƿ�Ϊ��Ҫ������Ƶ�״̬��
    if (index.column() == FAULTY_STATUS_COLUMN) {
        // a. ����ı���ֻ��ʾԲ��
        viewOption.text = QString();

        // b. �Ȼ��Ʊ�����ѡ�п��
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &viewOption, painter, viewOption.widget);

        // c. �ڱ���֮�ϻ����Զ����Բ��
        QString status = index.data(Qt::DisplayRole).toString();
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);

        // d. ����Բ���λ�úʹ�С
        int margin = 5;
        QRectF ellipseRect = option.rect.adjusted(margin, margin, -margin, -margin);
        int diameter = qMin(ellipseRect.width(), ellipseRect.height());
        ellipseRect.setSize(QSizeF(diameter, diameter));
        ellipseRect.moveCenter(option.rect.center());

        // e. ����״ֵ̬������ɫ
        if (status == "2") { // ����
            painter->setBrush(QBrush(QColor("#dc3545"))); // ��ɫ
        }
        else if (status == "1") { // ����
            painter->setBrush(QBrush(QColor("#ffc107"))); // ��ɫ
        }
        else { // ������δ֪״̬ ("0" ��)
            painter->setBrush(QBrush(QColor("#007bff"))); // ��ɫ
        }

        painter->setPen(Qt::NoPen);
        painter->drawEllipse(ellipseRect);
        painter->restore();
    }
    else {
        // ��������������ͨ�У�ʹ�����Ǹ����������С�� viewOption ���б�׼����
        QStyledItemDelegate::paint(painter, viewOption, index);
    }
}