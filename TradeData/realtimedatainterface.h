#ifndef REALTIMEDATAINTERFACE_H
#define REALTIMEDATAINTERFACE_H

#include "tradedata_global.h"
#include "tradedata.h"

/*!
 * \brief ʵʱ�����������͵Ľӿڡ�
 */
class TRADEDATASHARED_EXPORT RealTimeDataInterface
{
public:
    virtual ~RealTimeDataInterface() {}

    /*!
     * \brief setData ������ʷ���ݣ�����ָ����ֵ���С�
     * \param data ��ʷ���ݡ�
     */
    virtual void setData(const TradeDataSeries& data) = 0;

    /*!
     * \brief ����һ�����ڵ����ݣ�������Ϊָ�굱ǰ��ʾ�������ڷ�Χ֮�����һ���ڡ�
     * \param data �����ڵĽ����������ݡ�
     */
    virtual void append(const TradeData& data) = 0;

    /*!
     * \brief ����ָ�굱ǰ���Ƿ�Χ�ڵ��������ݣ����¼���ָ�������ֵ��
     *
     * �������������һ�����󣺵����µ�һ��������δ����ʱ������߼ۡ���ͼۡ����̼��Լ��ɽ������ڲ�ͣ�仯���ڸ�������
     * �ļ۸����仯ʱ������ָ����Ҫ�������������ظ���������ָ����ֵ����������ڽ���ʱ��OHLVC����ȫ��ȷ��������ָ��
     * ��ֵҲ�����Ƕ�̬�仯�ģ���Ϊ��һ���̶���ֵ����ʱ����ʹ��append���������һ���ڵĽ������ݣ����������ڵ�ָ�꣩��
     *
     * \param data �������ڵĽ����������ݡ�
     */
    virtual void update(const TradeData& data) = 0;
};

#endif // REALTIMEDATAINTERFACE_H
