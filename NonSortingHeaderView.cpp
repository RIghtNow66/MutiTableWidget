#pragma execution_character_set("utf-8")

#include "NonSortingHeaderView.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTableWidget>
#include <QPixmap>
#include <QCursor>
#include <QFontMetrics>


NonSortingHeaderView::NonSortingHeaderView(Qt::Orientation orientation, QWidget* parent)
    : QHeaderView(orientation, parent)
{
    // �����Ҽ��˵����ԣ�������ʾ��ɸѡ��ѡ��
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QHeaderView::customContextMenuRequested, this, &NonSortingHeaderView::showContextMenu);

    setMouseTracking(true);
}

void NonSortingHeaderView::mouseMoveEvent(QMouseEvent* event)
{
    // 1. �����ȵ��û����ʵ�֡�
    //    ���û����л����ȸ������λ�����ú�Ĭ�ϵĹ�꣬
    //    �������зָ�����ʱ���������ú� Qt::SplitHCursor��
    QHeaderView::mouseMoveEvent(event);

    // 2. Ȼ������ֻ����Ҫʱ�š����ǡ�����ľ�����
    int logicalIndex = logicalIndexAt(event->pos());

    // �������Ƿ���ĳ������ɸѡͼ����е�ͼ��������
    if (logicalIndex != -1 && m_filterStates.value(logicalIndex, false))
    {
        int x = sectionPosition(logicalIndex);
        int width = sectionSize(logicalIndex);
        QRect sectionRect(x, 0, width, height());

        if (getIconRect(sectionRect, logicalIndex).contains(event->pos())) {
            // ֻ�������־�ȷ������£����ǲŰѹ��ǿ����Ϊ����
            setCursor(Qt::PointingHandCursor);
        }
    }
    // ����Ҫ������ɾ���� else ��֧��
    // ����ζ�ţ������겻�����ǵ�ͼ���ϣ�����ʲôҲ������
    // �Ӷ������˻����ڵ�1�������õ��κι�꣨�����Ǽ�ͷ������ק��꣩��
}

// ��������뿪�¼�
void NonSortingHeaderView::leaveEvent(QEvent* event)
{
    // ȷ������뿪������ͷ����ʱ�����ָ�����
    setCursor(Qt::ArrowCursor);
    QHeaderView::leaveEvent(event);
}

// ���ⲿ���ã�����ĳһ�е�ɸѡ״̬
void NonSortingHeaderView::setFilterState(int logicalIndex, bool isActive)
{
    if (isActive) {
        m_filterStates[logicalIndex] = true;
    }
    else {
        m_filterStates.remove(logicalIndex);
    }
    // ֪ͨ��ͼ�ػ�˱�ͷ����
    updateSection(logicalIndex);
}

// ���Ĺ��ܣ��ػ��ͷ
void NonSortingHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
    painter->save();
    // 1. �ȵ��û���Ļ��Ʒ���������QSS��ʽ�����ֵ���Ȼ��Ч
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();

    // 2. �����ǰ�д���ɸѡ״̬������������ӻ������ǵ�ͼ��
    if (m_filterStates.value(logicalIndex, false))
    {
        QPixmap pixmap(":/MutiTableWidget/m_icon/hourglass.png"); // ����Դ�ļ�����ͼ��
        if (!pixmap.isNull())
        {
            QRect iconRect = getIconRect(rect, logicalIndex);
            painter->drawPixmap(iconRect, pixmap);
        }
    }
}

// ���Ĺ��ܣ����������
void NonSortingHeaderView::mousePressEvent(QMouseEvent* event)
{
    // ֻ����������
    if (event->button() == Qt::LeftButton) {
        int logicalIndex = logicalIndexAt(event->pos());
        if (logicalIndex != -1) {
            // ��鵱ǰ���Ƿ���ɸѡͼ��
            if (m_filterStates.value(logicalIndex, false)) {
                // ʹ�� sectionPosition �� sectionSize ����ȫ�ع�������
                int x = sectionPosition(logicalIndex);
                int width = sectionSize(logicalIndex);
                QRect sectionRect(x, 0, width, height()); // ��������ǰ��ͷ�����������

                // �����ͼ��ľ�������
                QRect iconRect = getIconRect(sectionRect, logicalIndex);

                // �ж������λ���Ƿ���ͼ��������
                if (iconRect.contains(event->pos())) {
                    // ����ǣ�����ɸѡ�źţ���ֹͣ�¼�����
                    emit filterTriggered(logicalIndex);
                    event->accept();
                    return;
                }
            }
        }
    }

    // �������Ĳ���ͼ�꣬���߲����������ִ�л����Ĭ����Ϊ������ק�С�ѡ���еȣ�
    QHeaderView::mousePressEvent(event);
}

// ������ʾ�Ҽ��˵��Ĳۺ���
void NonSortingHeaderView::showContextMenu(const QPoint& pos)
{
    int logicalIndex = logicalIndexAt(pos);
    if (logicalIndex != -1) {
        QMenu contextMenu(this);
        QAction* action;

        // �� �ؼ��Ķ�������״̬���ӵ���ͬ���ź�
        if (m_filterStates.value(logicalIndex, false)) {
            // �����ɸѡ���˵���Ϊ��ȡ��ɸѡ��
            action = contextMenu.addAction("ȡ��ɸѡ");
            // ���ӵ������ɸѡ����ר���ź�
            connect(action, &QAction::triggered, [this, logicalIndex]() {
                emit clearFilterTriggered(logicalIndex);
                });
        }
        else {
            // ���δɸѡ���˵���Ϊ��ɸѡ��
            action = contextMenu.addAction("ɸѡ");
            // ���ӵ�����/�༭ɸѡ�����ź�
            connect(action, &QAction::triggered, [this, logicalIndex]() {
                emit filterTriggered(logicalIndex);
                });
        }

        contextMenu.exec(mapToGlobal(pos));
    }
}

// ��������������ͼ��Ӧ�ñ��������ĸ�λ��
QRect NonSortingHeaderView::getIconRect(const QRect& sectionRect, int logicalIndex) const
{
    // ��ȡ��ͷ����������
    QString text = model()->headerData(logicalIndex, Qt::Horizontal, Qt::DisplayRole).toString();

    // �������ֵ����ؿ��
    QFontMetrics fm(font());
    int textWidth = fm.horizontalAdvance(text);

    // �������ֻ��Ƶ���ʼX���꣨QHeaderViewĬ�Ͼ��ж��룩
    int textStartX = sectionRect.x() + (sectionRect.width() - textWidth) / 2;

    // �������ֵĽ���X����
    int textEndX = textStartX + textWidth;

    int iconSize = 22; // ͼ��ߴ� 
    int textPadding = 4; // ͼ��������֮��ļ��

    // ����y����ʹ�䴹ֱ����
    int y = sectionRect.y() + (sectionRect.height() - iconSize) / 2;
    // ����x���꣬ʹ����������Ҳ�
    int x = textEndX + textPadding;

    // ��ȫ��飺ȷ��ͼ�겻�ᳬ����ͷ���ұ߽�
    if (x + iconSize > sectionRect.right()) {
        x = sectionRect.right() - iconSize - 2; // ���Ҫ���������ǿ��Ҷ���
    }

    return QRect(x, y, iconSize, iconSize);
}

bool NonSortingHeaderView::getFilterState(int logicalIndex) const
{
    return m_filterStates.value(logicalIndex, false);
}