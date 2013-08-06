#ifndef CMSCOFFEEACCOUNTENTRYWIDGET_H
#define CMSCOFFEEACCOUNTENTRYWIDGET_H

#include <QWidget>
#include <QDateEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>

#include <cmscoffeeaccountmodel.h>
#include <cmscoffeeaccountentrycompletionmodel.h>

class CMSCoffeeAccountEntryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CMSCoffeeAccountEntryWidget(CMSCoffeeAccountModel* accountModel,
                                         CMSCoffeeAccountEntryCompletionModel* completionModel,
                                         QWidget *parent = 0);
    
signals:
    
    void newAccountEntry(const QDate&, const QString&, float);

public slots:
    
    void addEntryClicked();

protected:

    CMSCoffeeAccountModel* accountModel_;
    CMSCoffeeAccountEntryCompletionModel* completionModel_;

    QDateEdit* dateEdit_;
    QLineEdit* descriptionEdit_;
    QDoubleSpinBox* amountSpinBox_;
    QPushButton* addEntryButton_;
};

#endif // CMSCOFFEEACCOUNTENTRYWIDGET_H
