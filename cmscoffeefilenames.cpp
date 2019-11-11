#include "cmscoffeefilenames.h"

CMSCoffeeFileNames* CMSCoffeeFileNames::instance_ = nullptr;

CMSCoffeeFileNames::CMSCoffeeFileNames()
{
    setCMSFilenames();
}

CMSCoffeeFileNames* CMSCoffeeFileNames::instance()
{
    if (instance_==nullptr) {
        instance_ = new CMSCoffeeFileNames();
    }
    return instance_;
}

void CMSCoffeeFileNames::setCMSFilenames()
{
    usersFilename_ = "users.xml";
    ticksFilename_ = "ticks.xml";
    accountFilename_ = "account.xml";
    printFilename_ = "CMSCoffee.pdf";
}

void CMSCoffeeFileNames::setDAFFilenames()
{
    usersFilename_ = "usersDAF.xml";
    ticksFilename_ = "ticksDAF.xml";
    accountFilename_ = "accountDAF.xml";
    printFilename_ = "DAFCoffee.pdf";
}
