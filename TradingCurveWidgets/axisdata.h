#ifndef AXISDATA_H
#define AXISDATA_H

#include "tradingcurvewidgets_global.h"
#include "rectangularaxisdraw.h"
#include <qwt_text.h>
#include <qwt_color_map.h>
#include <qwt_interval.h>

/*!
 * \brief ����һ����������ʹ�õ��������ݡ�
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AxisData
{
public:
    AxisData();

    virtual ~AxisData();

    /*! ʹ��ָ�뱣�棬֧�ֶ�̬���ԡ�*/
    RectangularAxisDraw *scaleDraw = nullptr;

    // ȷ��������������ʾ����
    int basePos;        /*! ��������������λ�ã������Ӧ�����꣬�����Ӧ�����ꡣ*/
    int scaleLength;    /*! �������������ĳ��ȣ������Ӧ���᷽��ߴ磬�����Ӧ���᷽��ߴ硣*/

    // ����ʱ��ע�����Ϣ��
    int margin;

    int spacing;
};

#endif // AXISDATA_H
