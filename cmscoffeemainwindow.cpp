#include <QHeaderView>
#include <QToolButton>
#include <QBoxLayout>

#include "cmscoffeeusertableview.h"
#include "cmscoffeeactiveusertableview.h"
#include "cmscoffeeaccounttableview.h"
#include "cmscoffeeaccountentrywidget.h"

#include "cmscoffeemainwindow.h"

CMSCoffeeMainWindow::CMSCoffeeMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    userModel_ = new CMSCoffeeUserModel(this);
    tickModel_ = new CMSCoffeeTickModel(userModel_, this);
    accountModel_ = new CMSCoffeeAccountModel(userModel_, this);
    completionModel_ = new CMSCoffeeAccountEntryCompletionModel(userModel_, accountModel_, this);

    //userModel_->import("/Users/mussgill/Documents/Physik/CMS/CMS Coffee/Balance-Balance.csv");
    //tickModel_->import("/Users/mussgill/Documents/Physik/CMS/CMS Coffee/Balance-Balance.csv");
    //accountModel_->import("/Users/mussgill/Documents/Physik/CMS/CMS Coffee/Account-Account.csv");

    userModel_->read();
    tickModel_->read();
    accountModel_->read();

    userModel_->sortByName();
    tickModel_->sortByDate();
    accountModel_->sortByDate();

    completionModel_->refresh();

    userModel_->write();
    tickModel_->write();
    accountModel_->write();

    activeUserModel_ = new CMSCoffeeActiveUserModel(userModel_, tickModel_, this);

    connect(userModel_, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            userModel_, SLOT(write()));

    toolBar_ = new QToolBar(this);
    toolBar_->setMovable(false);

    QToolButton *buttonSort = new QToolButton(toolBar_);
    buttonSort->setText("Sort");
    buttonSort->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(buttonSort, SIGNAL(clicked()),
            userModel_, SLOT(sortByName()));
    connect(buttonSort, SIGNAL(clicked()),
            accountModel_, SLOT(sortByDate()));
    toolBar_->addWidget(buttonSort);

    QToolButton *buttonNewClearing = new QToolButton(toolBar_);
    buttonNewClearing->setText("New Clearing");
    buttonNewClearing->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(buttonNewClearing, SIGNAL(clicked()),
            tickModel_, SLOT(addDate()));
    toolBar_->addWidget(buttonNewClearing);

    QToolButton *buttonWrite = new QToolButton(toolBar_);
    buttonWrite->setText("Write");
    buttonWrite->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(buttonWrite, SIGNAL(clicked()),
            userModel_, SLOT(write()));
    connect(buttonWrite, SIGNAL(clicked()),
            tickModel_, SLOT(write()));
    connect(buttonWrite, SIGNAL(clicked()),
            accountModel_, SLOT(write()));
    toolBar_->addWidget(buttonWrite);

    QToolButton *buttonPrint = new QToolButton(toolBar_);
    buttonPrint->setText("Print");
    buttonPrint->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(buttonPrint, SIGNAL(clicked()),
            userModel_, SLOT(print()));
    toolBar_->addWidget(buttonPrint);

    this->addToolBar(toolBar_);

    tabWidget_ = new QTabWidget(this);
    tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    CMSCoffeeUserTableView * userTable = new CMSCoffeeUserTableView(userModel_,
                                                                    tabWidget_);
    tabWidget_->addTab(userTable, "Users");

    CMSCoffeeActiveUserTableView * activeUserTable = new CMSCoffeeActiveUserTableView(activeUserModel_,
                                                                                      tabWidget_);
    tabWidget_->addTab(activeUserTable, "Ticks");

    QWidget * accountWidget = new QWidget(tabWidget_);
    //accountWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    QLayout * vlayout = new QVBoxLayout();
    vlayout->setContentsMargins(0,0,0,0);
    accountWidget->setLayout(vlayout);

    CMSCoffeeAccountEntryWidget * accountEntryWidget = new CMSCoffeeAccountEntryWidget(accountModel_,
                                                                                       completionModel_,
                                                                                       accountWidget);
    vlayout->addWidget(accountEntryWidget);

    CMSCoffeeAccountTableView * accountTable = new CMSCoffeeAccountTableView(accountModel_,
                                                                             completionModel_,
                                                                             accountWidget);
    //accountTable->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    vlayout->addWidget(accountTable);
    tabWidget_->addTab(accountWidget, "Account");

    setCentralWidget(tabWidget_);
    updateGeometry();
}

CMSCoffeeMainWindow::~CMSCoffeeMainWindow()
{
    
}
