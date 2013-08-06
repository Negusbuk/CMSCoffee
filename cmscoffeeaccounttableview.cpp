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

    setColumnWidth(0, 90);
    setColumnWidth(1, 350);
    setColumnWidth(2, 90);

    horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);

    //setItemDelegateForColumn(0, new CMSCoffeeDateEditDelegate);
    setItemDelegateForColumn(1, new CMSCoffeeLineEditDelegate(completionModel));
    //setItemDelegateForColumn(2, new CMSCoffeeEuroSpinBoxDelegate);

    setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);

}
