#pragma once
#include "MutiTableWidget.h"

/**
 * @class FaultyStatusTable
 * @brief �̳��� BaseDataTable��������ʾ����״̬��
 *
 * ����ع����ǣ���״̬�У���3�У������� "0", "1", "2" ���Ƴɲ�ɫԲ�㡣
 * ������û���и����߼���
 */
class FaultyStatusTable : public BaseDataTable {
    Q_OBJECT
public:
    explicit FaultyStatusTable(QWidget* parent = nullptr);
    // ������д checkAndHighlightRow�����Զ�ʹ�û���Ŀ�ʵ�֡�
};