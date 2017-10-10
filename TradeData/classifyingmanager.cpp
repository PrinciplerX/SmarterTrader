#include "classifyingmanager.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

const QString ClassifyingManager::URL_SINA_INDUSTRY_INDEX = QString("http://vip.stock.finance.sina.com.cn/q/view/newSinaHy.php");
const QString ClassifyingManager::URL_SW_INDUSTRY_INDEX = QString("http://vip.stock.finance.sina.com.cn/q/view/SwHy.php");
const QString ClassifyingManager::UT_SINA_INDUSTRY_DETAIL = QString("http://vip.stock.finance.sina.com.cn/quotes_service/api/json_v2.php"
                                                                     "/Market_Center.getHQNodeData?num=1000&sort=symbol&asc=1&node=%1&_s_r_a=page");

ClassifyingManager* ClassifyingManager::mInstance = nullptr;

ClassifyingManager *ClassifyingManager::instance()
{
    if(!mInstance){
        mInstance = new ClassifyingManager();
    }
    return mInstance;
}

ClassifyingManager::~ClassifyingManager()
{
    database.close();
}

QStringList ClassifyingManager::getIndustryList() const
{
    QSqlQuery query(database);
    query.exec("SELECT tag FROM IndustryTable");

    QStringList result;
    while (query.next()) {
        result.append(query.value(0).toString());
    }

    return result;
}

QStringList ClassifyingManager::getIndustryItems() const
{
    return QStringList();
}

void ClassifyingManager::updateIndustryDetail()
{
    QStringList industryList = getIndustryList();
    for(QString& item : industryList){
        updateIndustryDetail(item);
    }
}

void ClassifyingManager::updateIndustryInfo()
{
    industryInfoDownloadIndex = downloader.download(QUrl(URL_SINA_INDUSTRY_INDEX));
}

void ClassifyingManager::updateIndustryDetail(QString industry)
{
    int index = downloader.download(QUrl(UT_SINA_INDUSTRY_DETAIL.arg(industry)));
    industryDetailDownloadMap.insert(index, industry);
}

void ClassifyingManager::onDownloadFinished(bool isSucceed, int index, QByteArray data)
{
    if(isSucceed){
        if(index == industryInfoDownloadIndex){
            loadIndustryData(data);
        }else{
            QMap<int, QString>::iterator i = industryDetailDownloadMap.find(index);
            if(i != industryDetailDownloadMap.end()){
                loadIndustryDetailData(i.value(), data);
            }
        }
    }
}

ClassifyingManager::ClassifyingManager(QObject *parent):
    QObject(parent)
{
    connect(&downloader, SIGNAL(downloadComplete(bool,int,QByteArray)), this, SLOT(onDownloadFinished(bool,int,QByteArray)));

    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("Classification.db");
    if(!database.open()){
        qWarning() << "ClassifyingManager打开数据库时出现错误";
    }

    QStringList tables = database.tables();
    QSqlQuery query(database);

    if(!tables.contains(QString("IndustryTable"))){
        query.exec("CREATE TABLE IndustryTable (tag text primary key, name text)");
        updateIndustryInfo();
    }
    if(!tables.contains(QString("ShareTable"))){
        query.exec("CREATE TABLE ShareTable (symbol text primary key, code text, name text, industry text, foreign key (industry) references IndustryTable (tag))");
        updateIndustryDetail();
    }
}

void ClassifyingManager::loadIndustryData(const QByteArray rawData)
{
    QTextCodec* codec = QTextCodec::codecForName("GB18030");
    QString data = codec->toUnicode(rawData);

    QRegularExpression mainPattern(QString("^var S_Finance_bankuai_sinaindustry = {(.*)}$"));
    QRegularExpressionMatch match = mainPattern.match(data);

    if(!match.hasMatch()){
        return;
    }

    QStringRef content = match.capturedRef(1);

    QRegularExpression itemPattern(QString(":\"(.*?),(.*?),"));

    QRegularExpressionMatchIterator iterator = itemPattern.globalMatch(content);

    QSqlQuery query(database);

    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        query.exec(QString("INSERT INTO IndustryTable (tag, name) values('%1', '%2')").arg(match.captured(1), match.captured(2)));
    }
}

void ClassifyingManager::loadIndustryDetailData(const QString& industry, const QByteArray rawData)
{
    QTextCodec* codec = QTextCodec::codecForName("GB18030");
    QString data = codec->toUnicode(rawData);

    QRegularExpression itemPattern("{(.*?)}");
    QRegularExpression keyValuePattern(".*?:\\\"(.*?)\\\"");
    QRegularExpressionMatchIterator iterator = itemPattern.globalMatch(data);

    QVector<QuoteData> output;

    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        QSqlQuery query(database);

        QStringList list = match.captured(1).split(',');
        QuoteData item;

#define _value(string) keyValuePattern.match(string).captured(1)

        for(int index = 0; index < list.size(); index++){
            item.symbol        = _value(list[0]);
            item.code          = _value(list[1]);
            item.name          = _value(list[2]);
            item.current       = _value(list[3]);
            item.change        = _value(list[4]);
            item.changeRate    = _value(list[5]);

            // 省略6（buy），7（sell），8（settlement）

            item.open          = _value(list[9]);
            item.high          = _value(list[10]);
            item.low           = _value(list[11]);
            item.volume        = _value(list[12]);
            item.turnover      = _value(list[13]);
            item.time          = _value(list[14]);
            item.pe            = _value(list[15]);
            item.pb            = _value(list[16]);
            item.totalMktCap   = _value(list[17]);
            item.negotiableCap = _value(list[18]);

            item.industry      = industry;
        }

        output.append(item);
        query.exec(QString("INSERT INTO ShareTable (symbol, code, name, industry) values('%1', '%2', '%3', '%4')").
                   arg(item.symbol, item.code, item.name, item.industry));
    }
}
