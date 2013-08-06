#include <QHeaderView>

#include "cmscoffeetableviewdelegates.h"
#include "cmscoffeeactiveusertableview.h"

CMSCoffeeActiveUserTableView::CMSCoffeeActiveUserTableView(CMSCoffeeActiveUserModel* model,
                                                           QWidget *parent) :
    QTableView(parent),
    userModel_(model)
{
    setModel(userModel_);
    setAlternatingRowColors(true);
    setMinimumWidth(450);
    setMinimumHeight(400);

    setColumnWidth(0, 225);
    setColumnWidth(1, 75);

    //setItemDelegate(new CMSCoffeeIntSpinBoxDelegate);

    setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);

    horizontalHeader()->setStretchLastSection(true);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}
