#include "datamanager.h"

DataManager &DataManager::instance()
{
    static DataManager manager;
    return manager;
}

/**********
HistoryDataManager &DataManager::historyDataManager()
{
    return mHistoryDataManager;
}
**********/

DataManager::DataManager(QObject *parent) : QObject(parent)
{

}
