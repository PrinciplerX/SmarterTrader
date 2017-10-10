#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <historydatamanager.h>
#include <realtimedatamanager.h>

/*!
 * \brief The DataManager class
 */
class DataManager : public QObject
{
    Q_OBJECT
public:
    static DataManager& instance();

    HistoryDataManager& historyDataManager();

signals:

public slots:

protected:
    explicit DataManager(QObject *parent = 0);

private:
    //HistoryDataManager mHistoryDataManager;
};

#endif // DATAMANAGER_H
