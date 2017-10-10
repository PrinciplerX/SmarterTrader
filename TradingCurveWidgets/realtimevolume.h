#ifndef REALTIMEVOLUME_H
#define REALTIMEVOLUME_H

#include "volume.h"
#include <tradedata.h>

class TRADINGCURVEWIDGETSSHARED_EXPORT RealTimeVolume : public Volume
{
public:
    RealTimeVolume(AbstractHistoryData *data,
                   const QPen& pen = QPen(Qt::yellow));
    virtual ~RealTimeVolume(){}

    virtual QPen getRaisePen() const override;
    virtual QPen getFallPen() const override;

    virtual QString titleLabelString(int index) override;

private:
    QPen pen;
};

#endif // REALTIMEVOLUME_H
