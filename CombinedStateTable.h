#pragma once
#include "MutiTableWidget.h"

/**
 * @class CombinedStateTable
 * @brief �̳��� BaseDataTable���ǹ�����ȫ���״̬���
 *
 * ����ع����ǣ�
 * 1. ��״̬�У���3�У����Ƴɲ�ɫԲ�㡣
 * 2. ������ʱ�䣨��5�У�����20���ӣ�1200�룩�����и�����
 */
class CombinedStateTable : public BaseDataTable {
    Q_OBJECT
public:
    explicit CombinedStateTable(QWidget* parent = nullptr);
};