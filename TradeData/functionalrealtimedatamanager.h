///===========================================================================
/// \date 2016-12-22
/// \author ���
/// \copyright Copyright ? 2015 - 2017 by YiChao Yang. All Rights Reserved.
///===========================================================================

#ifndef FUNCTIONALREALTIMEDATAMANAGER_H
#define FUNCTIONALREALTIMEDATAMANAGER_H

#include <QObject>

/*!
 * \brief ʵʱ��������������ڹ����Զ����ɵ�ʵʱ�������ݡ�
 */
class FunctionalRealTimeDataManager : public QObject
{
    Q_OBJECT
public:
    explicit FunctionalRealTimeDataManager(QObject *parent = 0);

signals:

public slots:
};

#endif // FUNCTIONALREALTIMEDATAMANAGER_H
