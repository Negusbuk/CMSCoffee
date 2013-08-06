#ifndef CMSCOFFEEUSERTABLEVIEW_H
#define CMSCOFFEEUSERTABLEVIEW_H

#include <QTableView>

#include <cmscoffeeusermodel.h>

class CMSCoffeeUserTableView : public QTableView
{
    Q_OBJECT
public:
    explicit CMSCoffeeUserTableView(CMSCoffeeUserModel* model,
                                    QWidget *parent = 0);
    
signals:
    
public slots:
    
protected:

    CMSCoffeeUserModel* userModel_;
};

#endif // CMSCOFFEEUSERTABLEVIEW_H
