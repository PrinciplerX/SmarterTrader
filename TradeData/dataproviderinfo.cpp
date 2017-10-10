#include "dataproviderinfo.h"

//静态实例会在程序启动时创建，程序关闭时销毁。
DataProviderInfo DataProviderInfo::sInstance;

const QString DataProviderInfo::UT_HISTORY_IFENG_SHORT = QString("http://api.finance.ifeng.com/akmin?scode=%1&type=%2");
const QString DataProviderInfo::UT_HISTORY_IFENG_LONG = QString("http://api.finance.ifeng.com/%1/?code=%2&type=last");
const QString DataProviderInfo::UT_HISTORY_YAHOO = QString("http://table.finance.yahoo.com/table.csv?s=%1");

const QString DataProviderInfo::UT_REALTIME_SINA = QString("http://hq.sinajs.cn/list=%1");
const QString DataProviderInfo::UT_REALTIME_TENCENT = QString("http://qt.gtimg.cn/q=%1");

const QString DataProviderInfo::UT_TICK_SINA = QString("http://vip.stock.finance.sina.com.cn/quotes_service/api/json_v2.php/CN_Transactions.getAllPageTime?date=%1&symbol=%2");
const QString DataProviderInfo::UT_HISTORY_TICK_SINA = QString("http://market.finance.sina.com.cn/downxls.php?date=%1&symbol=%2");

DataProviderInfo::DataProviderInfo()
{
	//凤凰财经（http://finance.ifeng.com/）历史数据周期参数
    ifengPeriodArgv.insert(DataPeriod::MONTH, QString("akmonthly"));
	ifengPeriodArgv.insert(DataPeriod::WEEK, QString("akweekly"));
    ifengPeriodArgv.insert(DataPeriod::DAY, QString("akdaily"));
	ifengPeriodArgv.insert(DataPeriod::MINUTES_60, QString("60"));
	ifengPeriodArgv.insert(DataPeriod::MINUTES_30, QString("30"));
	ifengPeriodArgv.insert(DataPeriod::MINUTES_15, QString("15"));
	ifengPeriodArgv.insert(DataPeriod::MINUTES_5, QString("5"));
}

bool DataProviderInfo::getPeriodArgv(DataProviderInfo::DataProvider provider,
	DataPeriod period, QString &argv) const
{
	switch (provider){
    case IFENG:
	{
        QMap<DataPeriod, QString>::const_iterator i = ifengPeriodArgv.find(period);
        if (i != ifengPeriodArgv.end()){
            argv = i.value();
            return true;
        }
	 break;
	}
	default:
		return false;
	}

	return false;
}

bool DataProviderInfo::getDataUrl(DataProviderInfo::DataProvider provider, DataPeriod period,
	const QString &symbol, QUrl &url) const
{
    QString periodArgv;

	if (!getPeriodArgv(provider, period, periodArgv))
		return false;

	switch (provider){
    case IFENG:	//凤凰财经数据接口
		switch (period)
		{
        case MONTH: case WEEK: case DAY:
            url = QUrl(UT_HISTORY_IFENG_LONG.arg(periodArgv, getStandardSymbol(provider, symbol)));
			break;
        case MINUTES_60: case MINUTES_30: case MINUTES_15: case MINUTES_5:
            url = QUrl(UT_HISTORY_IFENG_SHORT.arg(getStandardSymbol(provider, symbol), periodArgv));
			break;
		default:
			return false;
		}
        break;
    case YAHOO:
    case TENCENT:
    default:
        return false;
	}
	return true;
}

QString DataProviderInfo::getStandardSymbol(DataProviderInfo::DataProvider provider, const QString &symbol) const
{
    if(symbol.isEmpty()) return symbol;

	switch (provider){
    case IFENG:
    case TENCENT:
    case SINA:
		if (symbol.at(0).isNumber() && symbol.size() == 6){
			switch (symbol.left(1).toInt()){
			case 6:
				return QString("sh") + symbol;
            case 3: case 0:
                return QString("sz") + symbol;
			}
		}
		break;
    case YAHOO:
		if (symbol.at(0).isNumber() && symbol.size() == 6){
			switch (symbol.left(1).toInt()){
			case 6:
				return symbol + QString(".ss");
            case 3: case 0:
                return symbol + QString(".sz");
			}
		}
        break;
    default:
        qDebug() << "DataProviderInfo::getStandardSymbol: 出现了无法识别的数据提供商。";
	}
    return symbol;
}

QString DataProviderInfo::getStandardSymbolList(DataProviderInfo::DataProvider provider, const QStringList &symbols) const
{
    QString symbolList;
    for(int i = 0; i < symbols.size(); i++){
        symbolList += getStandardSymbol(provider, symbols.at(i));
        if(i != symbols.size() - 1)
            symbolList += ',';
    }
    return symbolList;
}

QString DataProviderInfo::getStandardSymbolList(DataProviderInfo::DataProvider provider, const QMap<QString, RealTimeDataItem *> &data)
{
    QString symbolList;
    int counter = 0;
    for(const RealTimeDataItem* i : data){
        symbolList += getStandardSymbol(provider, i->getQuoteData()->symbol);
        if(counter != data.size() - 1){
            symbolList += ',';
            counter++;
        }
    }
    return symbolList;
}

bool DataProviderInfo::getRealTimeDataUrl(DataProviderInfo::DataProvider provider, const QString &symbol, QUrl &url) const
{
    QString sSymbol = getStandardSymbol(provider, symbol);
    switch(provider){
    case TENCENT:   url = QUrl(UT_REALTIME_TENCENT.arg(sSymbol)); break;
    case SINA:      url = QUrl(UT_REALTIME_SINA.arg(sSymbol)); break;
    default:        return false;
    }
    return true;
}

bool DataProviderInfo::getRealTimeDataUrl(DataProviderInfo::DataProvider provider, const QStringList &symbols, QUrl &url) const
{
    QString sSymbol = getStandardSymbolList(provider, symbols);
    switch(provider){
    case TENCENT:   url = QUrl(UT_REALTIME_TENCENT.arg(sSymbol)); break;
    case SINA:      url = QUrl(UT_REALTIME_SINA.arg(sSymbol)); break;
    default:        return false;
    }
    return true;
}

// http://vip.stock.finance.sina.com.cn/quotes_service/view/vMS_tradedetail.php?symbol=%s&date=%s&page=%s (symbol, tdate, pageNo)

bool DataProviderInfo::getTickUrl(DataProvider provider, const QString &symbol, const QString& date, QUrl &url) const
{
    QString sSymbol = getStandardSymbol(provider, symbol);
    switch(provider){
    case SINA:      url = QUrl(UT_TICK_SINA.arg(date, sSymbol)); break;
    default:        return false;
    }
    return true;
}

bool DataProviderInfo::getHistoryTickUrl(DataProviderInfo::DataProvider provider, const QString &symbol, const QString &date, QUrl &url) const
{
    QString sSymbol = getStandardSymbol(provider, symbol);
    switch(provider){
    case SINA:      url = QUrl(UT_HISTORY_TICK_SINA.arg(date, sSymbol)); break;
    default:        return false;
    }
    return true;
}

DataProviderInfo &DataProviderInfo::getInstance()
{
	return sInstance;
}


bool DataProviderInfo::getDataUrl(DataProviderInfo::DataProvider provider, const HistoryDataIndex &index, QUrl &url) const {
    return getDataUrl(provider, index.period, index.symbol, url);
}
