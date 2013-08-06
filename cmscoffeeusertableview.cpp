#include <QHeaderView>

#include "cmscoffeeusertableview.h"

CMSCoffeeUserTableView::CMSCoffeeUserTableView(CMSCoffeeUserModel* model,
                                               QWidget *parent) :
    QTableView(parent),
    userModel_(model)
{
    setModel(userModel_);
    setAlternatingRowColors(true);
    setMinimumWidth(450);
    setMinimumHeight(400);

    setColumnWidth(0, 225);
    setColumnWidth(1, 50);
    setColumnWidth(2, 75);

    horizontalHeader()->setStretchLastSection(true);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}
