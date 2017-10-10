#include "tradedatahelper.h"
#include <QFile>

TradeDataHelper::TradeDataHelper()
{

}

DateSeries TradeDataHelper::getDateSeries(const QVector<TradeData> &data)
{
    DateSeries series;
    series.resize(data.size());
    for(int i = 0; i < data.size(); i++){
        series[i] = data[i].date;
    }
    return series;
}

bool TradeDataHelper::saveToCsv(const QString &fileName, const QVector<TradeData> &data)
{
    QFile* file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        delete file;
        qWarning() << "文件无法打开或创建，存储失败！";
        return false;
    }
    QTextStream fout(file);
    QString item("date, open, high, low, close, volume");
    fout << item;
    for (int i = 0; i < data.size(); i++) {
        item = QString("\n%1, %2, %3, %4, %5, %6").arg(data[i].date,
                                                     QString::number(data[i].open),
                                                     QString::number(data[i].high),
                                                     QString::number(data[i].low),
                                                     QString::number(data[i].close),
                                                     QString::number(data[i].volume));
        fout << item;
    }
    file->close();
    return true;
}

bool TradeDataHelper::loadFromCsv(const QString &fileName, QVector<TradeData> &data)
{
    QFile* file = new QFile(fileName);
    if (!file->open(QIODevice::ReadOnly)) {
        delete file;
        qWarning() << "文件无法打开，读取失败！";
        return false;
    }

    QTextStream fin(file);
    fin.readLine();
    while (!fin.atEnd()) {
        QString line = fin.readLine();
        QStringList dataList = line.split(',');

        if(dataList.size() < 6) return false;

        data.push_back({dataList.at(1).toDouble(),
                        dataList.at(2).toDouble(),
                        dataList.at(3).toDouble(),
                        dataList.at(4).toDouble(),
                        dataList.at(5).toLong(),
                        dataList.at(0)});
    }

    file->close();
    return true;
}

TradeDataSeries TradeDataHelper::fromTickData(const QVector<TickData> &ticks, DailyTradeHour tradeHour)
{
    QVector<TradeData> result;
    if(ticks.size() == 0) return result;
    if(tradeHour.intervalCount() == 0) return result;

    QString timeFormat("hh:mm");

    //计算每一时间段所需要的存储空间。
    QVector<int> intervalSize(tradeHour.intervalCount());
    int totalSize = 0;
    for(int i = 0; i<tradeHour.intervalCount(); i++){
        intervalSize[i] = tradeHour.interval(i).startTime.secsTo(tradeHour.interval(i).endTime)/60 + 1;
        totalSize += intervalSize[i];
    }
    result.resize(totalSize);

    int resultIndex = 0;
    int tickIndex = 0;
    double lastPrice = ticks.at(0).price;

    for(int intervalIndex = 0; intervalIndex<intervalSize.size(); intervalIndex++){
        QTime currentTime = tradeHour.interval(intervalIndex).startTime;
        for(int i = 0; i < intervalSize[intervalIndex]; i++){
            TradeData& currentTradeData = result[resultIndex];

            currentTradeData.date = currentTime.toString(timeFormat);

            if(currentTradeData.date == ticks.at(tickIndex).time.left(5)){
                //接下来的tick数据刚好是当前需要的数据，直接开始统计
                //初始化数据
                currentTradeData.open = ticks.at(tickIndex).price;
                currentTradeData.high = ticks.at(tickIndex).price;
                currentTradeData.low = ticks.at(tickIndex).price;
                currentTradeData.close = ticks.at(tickIndex).price;
                currentTradeData.volume = ticks.at(tickIndex).volume;

                while (true){
                    if (tickIndex >= ticks.size())
                        break;
                    if (currentTradeData.date != ticks.at(tickIndex).time.left(5))
                        break;

                    currentTradeData.high = qMax(currentTradeData.high, ticks.at(tickIndex).price);
                    currentTradeData.low = qMin(currentTradeData.low, ticks.at(tickIndex).price);
                    currentTradeData.volume += ticks.at(tickIndex).volume;
                    currentTradeData.close = ticks.at(tickIndex).price;
                    tickIndex++;
                }
                lastPrice = currentTradeData.close;
            }else{
                //接下来的tick数据和当前所需数据不同，插入生成数据
                currentTradeData.open = lastPrice;
                currentTradeData.close = lastPrice;
                currentTradeData.high = lastPrice;
                currentTradeData.low = lastPrice;
                currentTradeData.volume = 0L;
            }

            currentTime = currentTime.addSecs(60);
            resultIndex++;
        }
    }

    return result;
}

void TradeDataHelper::appendFromTickData(const QVector<TickData> &ticks, int parsedSize, QVector<TradeData> &output, DailyTradeHour tradeHour)
{
    if(ticks.size() == 0) return;
    if(tradeHour.size() == 0) return;

    QString timeFormat("hh:mm");
    QTime currentTime = QTime::fromString(output.last().date, timeFormat);
    QTime tickEndTime = QTime::fromString(ticks.at(ticks.size() - 1).time.left(5), timeFormat);

    int intervalIndex = tradeHour.intervalOf(currentTime);

    int resultIndex = output.size() - 1;
    output.resize(tradeHour.indexOf(tickEndTime));

    int tickIndex = parsedSize;
    for(; tickIndex < output.size(); tickIndex++){
        if(output[resultIndex].date == ticks[tickIndex].time.left(5)){
            // 新的分笔数据属于本周期，直接更新。
            output[resultIndex].high   = qMax(ticks.at(tickIndex).price, output[resultIndex].high);
            output[resultIndex].low    = qMin(ticks.at(tickIndex).price, output[resultIndex].low);
            output[resultIndex].volume += ticks.at(tickIndex).volume;
            output[resultIndex].close = ticks.at(tickIndex).price;
        }else{
            // 出现了新周期内的数据。
            int lastIndex = resultIndex;
            do{
                resultIndex++;
                if(resultIndex >= output.size()) return;

                // 更新时间。
                currentTime.addSecs(60);
                if(currentTime > tradeHour.interval(intervalIndex).endTime){
                    intervalIndex++;
                    if(intervalIndex >= tradeHour.intervalCount()) return;
                    currentTime = tradeHour.interval(intervalIndex).startTime;
                }

                // 使用上一周期的价格数据初始化。
                output[resultIndex].date = currentTime.toString(timeFormat);
                output[resultIndex].open = output[lastIndex].close;
                output[resultIndex].high = output[lastIndex].close;
                output[resultIndex].low = output[lastIndex].close;
                output[resultIndex].volume = 0L;
            }
            while(output[resultIndex].date != ticks[tickIndex].time.left(5));
        }
    }
}
