#ifndef CMSCOFFEEACTIVEUSERTABLEVIEW_H
#define CMSCOFFEEACTIVEUSERTABLEVIEW_H

#include <QTableView>

#include <cmscoffeeusermodel.h>

class CMSCoffeeActiveUserTableView : public QTableView
{
    Q_OBJECT
public:
    explicit CMSCoffeeActiveUserTableView(CMSCoffeeActiveUserModel* model,
                                          QWidget *parent = 0);
    
signals:
    
public slots:

protected:

    CMSCoffeeActiveUserModel* userModel_;

};

#endif // CMSCOFFEEACTIVEUSERTABLEVIEW_H
