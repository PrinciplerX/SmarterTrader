#ifndef LOGHELPER_H
#define LOGHELPER_H

#include "qwt_scale_map.h"

class LogHelper{
public:
    static QString toString(const QwtScaleMap& map, QString name = QString("QwtScaleMap")){
        return QString("%1: scale(%2, %3), paint(%4, %5)").arg(name,
                                                               QString::number(map.s1()),
                                                               QString::number(map.s2()),
                                                               QString::number(map.p1()),
                                                               QString::number(map.p2()));
    }
};

#endif // LOGHELPER_H
