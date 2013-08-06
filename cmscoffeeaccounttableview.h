#ifndef CMSCOFFEEACCOUNTTABLEVIEW_H
#define CMSCOFFEEACCOUNTTABLEVIEW_H

#include <QTableView>

#include <cmscoffeeaccountmodel.h>
#include <cmscoffeeaccountentrycompletionmodel.h>

class CMSCoffeeAccountTableView : public QTableView
{
    Q_OBJECT
public:
    explicit CMSCoffeeAccountTableView(CMSCoffeeAccountModel* accountModel,
                                       CMSCoffeeAccountEntryCompletionModel* completionModel,
                                       QWidget *parent = 0);
    
signals:
    
public slots:
    
protected:

    CMSCoffeeAccountModel* accountModel_;
};

#endif // CMSCOFFEEACCOUNTTABLEVIEW_H
