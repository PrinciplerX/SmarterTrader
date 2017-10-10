#ifndef DATAPANEL_H
#define DATAPANEL_H

#include <QWidget>
#include <QTabWidget>
#include "mainframe_global.h"
#include "tradedatapanel.h"

class DataPanel : public QTabWidget
{
    Q_OBJECT
public:
    explicit DataPanel(QWidget *parent = 0);

signals:

public slots:
};

#endif // DATAPANEL_H
