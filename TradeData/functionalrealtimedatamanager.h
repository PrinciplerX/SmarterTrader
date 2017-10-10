///===========================================================================
/// \date 2016-12-22
/// \author 杨翌超
/// \copyright Copyright ? 2015 - 2017 by YiChao Yang. All Rights Reserved.
///===========================================================================

#ifndef FUNCTIONALREALTIMEDATAMANAGER_H
#define FUNCTIONALREALTIMEDATAMANAGER_H

#include <QObject>

/*!
 * \brief 实时行情管理器，用于管理自动生成的实时行情数据。
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
