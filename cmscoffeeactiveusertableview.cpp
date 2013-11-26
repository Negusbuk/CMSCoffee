#include <iostream>

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
    setMinimumWidth(600);
    setMinimumHeight(500);

    setColumnWidth(0, 300);
    setColumnWidth(1, 90);
    setColumnWidth(2, 90);

    horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    //setItemDelegate(new CMSCoffeeIntSpinBoxDelegate);

    setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::AnyKeyPressed);

    connect(userModel_, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(dataChanged(QModelIndex,QModelIndex)));

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void CMSCoffeeActiveUserTableView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    setCurrentIndex(userModel_->index(topLeft.row()+1, topLeft.column()));
}
