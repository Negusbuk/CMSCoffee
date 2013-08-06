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

    setColumnWidth(0, 300);
    setColumnWidth(1, 50);
    setColumnWidth(2, 90);
    setColumnWidth(3, 90);

    horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}
