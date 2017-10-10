#include "asi.h"
#include "tradedataoperations.h"

const QString ASI::mCategory = QString("ASI");

ASI::ASI(AbstractHistoryData *data, int period)
{
    // 输入
    CLOSE close(data);
    HIGH high(data);
    LOW low(data);
    OPEN open(data);
    REF<double> lc(close, 1);

    setName(QString("ASI%1").arg(QString::number(period)));

    // 计算式
    ReusableOperationWrapper<double> aa (ABS<double>(high - lc));
    ReusableOperationWrapper<double> bb (ABS<double>(low - lc));
    ReusableOperationWrapper<double> cc (ABS<double>(lc - REF<double>(open, 1)));
    ReusableOperationWrapper<double> dd (ABS<double>(lc - REF<double>(open, 1)));
    ReusableOperationWrapper<double> r  (IF<double>(aa>bb&&aa>cc, aa+bb/2.0+dd/4.0, IF<double>(bb>cc&&bb>aa,bb+aa/2.0+dd/4.0,cc+dd/4.0)));
    ReusableOperationWrapper<double> x  (close + (close - open)/2.0 - REF<double>(open, 1));
    ReusableOperationWrapper<double> si (16.0*x/r*MAX<double>(aa, bb));
    ReusableOperationWrapper<double> asi(SUM<double>(si, period));

    // 曲线
    cAsi.setInput(asi);
}

void ASI::onDataLoaded()
{

}

void ASI::onDataUpdated()
{

}

void ASI::onDataAppended()
{

}
