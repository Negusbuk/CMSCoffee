#include <QBoxLayout>
#include <QCompleter>

#include "cmscoffeeaccountentrywidget.h"

CMSCoffeeAccountEntryWidget::CMSCoffeeAccountEntryWidget(CMSCoffeeAccountModel* accountModel,
                                                         CMSCoffeeAccountEntryCompletionModel* completionModel,
                                                         QWidget *parent) :
    QWidget(parent),
    accountModel_(accountModel),
    completionModel_(completionModel)
{
    QHBoxLayout * layout = new QHBoxLayout();
    setLayout(layout);

    dateEdit_ = new QDateEdit(QDate::currentDate(), this);
    dateEdit_->setDisplayFormat("dd/MM/yy");
    layout->addWidget(dateEdit_);

    QCompleter* completer = new QCompleter(completionModel_, this);

    descriptionEdit_ = new QLineEdit(this);
    descriptionEdit_->setCompleter(completer);
    layout->addWidget(descriptionEdit_);

    amountSpinBox_ = new QDoubleSpinBox(this);
    amountSpinBox_->setDecimals(2);
    amountSpinBox_->setRange(-5000, 5000);
    amountSpinBox_->setSuffix(QString::fromUtf8(" \u20AC"));
    amountSpinBox_->setAlignment(Qt::AlignRight);
    layout->addWidget(amountSpinBox_);

    addEntryButton_ = new QPushButton("Add", this);
    layout->addWidget(addEntryButton_);

    connect(addEntryButton_, SIGNAL(clicked()),
            this, SLOT(addEntryClicked()));

    connect(this, SIGNAL(newAccountEntry(QDate,QString,float)),
            accountModel_, SLOT(addAccountEntry(QDate,QString,float)));
}

void CMSCoffeeAccountEntryWidget::addEntryClicked()
{
    if (amountSpinBox_->value()==0) return;
    if (descriptionEdit_->text().isEmpty()) return;

    emit newAccountEntry(dateEdit_->date(),
                         descriptionEdit_->text(),
                         amountSpinBox_->value());

    descriptionEdit_->setText("");
    amountSpinBox_->setValue(0);
}
