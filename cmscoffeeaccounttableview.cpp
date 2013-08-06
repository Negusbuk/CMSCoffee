#include <QHeaderView>

#include "cmscoffeetableviewdelegates.h"
#include "cmscoffeeaccounttableview.h"

CMSCoffeeAccountTableView::CMSCoffeeAccountTableView(CMSCoffeeAccountModel* accountModel,
                                                     CMSCoffeeAccountEntryCompletionModel* completionModel,
                                                     QWidget *parent) :
    QTableView(parent),
    accountModel_(accountModel)
{
    setModel(accountModel_);
    setAlternatingRowColors(true);
    setMinimumWidth(450);
    setMinimumHeight(400);

    setColumnWidth(0, 75);
    setColumnWidth(1, 250);
    setColumnWidth(2, 75);

    //setItemDelegateForColumn(0, new CMSCoffeeDateEditDelegate);
    setItemDelegateForColumn(1, new CMSCoffeeLineEditDelegate(completionModel));
    //setItemDelegateForColumn(2, new CMSCoffeeEuroSpinBoxDelegate);

    setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);

    horizontalHeader()->setStretchLastSection(true);
    //setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}
