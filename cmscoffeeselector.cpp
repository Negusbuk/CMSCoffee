#include <iostream>

#include <QButtonGroup>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>

#include "cmscoffeefilenames.h"
#include "cmscoffeeselector.h"

CMSCoffeeSelector::CMSCoffeeSelector()
    :QDialog()
{
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    QRadioButton *cms = new QRadioButton("CMS", this);
    cms->setChecked(true);
    QRadioButton *daf = new QRadioButton("DAF", this);

    buttonGroup->addButton(cms, 1);
    buttonGroup->addButton(daf, 2);

    currentSelection_ = 1;
    connect(buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
        [=](int id){
        currentSelection_ = id;

        // std::cout << "CMSCoffeeSelector::selectionChanged(int selection) " << currentSelection_ << std::endl;

        if (currentSelection_==1) CMSCoffeeFileNames::instance()->setCMSFilenames();
        if (currentSelection_==2) CMSCoffeeFileNames::instance()->setDAFFilenames();
    });

    QPushButton *button = new QPushButton("Ok");
    connect(button, SIGNAL(clicked()), this, SLOT(accept()));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(cms);
    layout->addWidget(daf);
    layout->addWidget(button);
}
