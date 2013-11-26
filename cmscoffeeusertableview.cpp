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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
#else
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
#endif

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}
