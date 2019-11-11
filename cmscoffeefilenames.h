#ifndef CMSCOFFEEFILENAMES_H
#define CMSCOFFEEFILENAMES_H

#include <QString>

class CMSCoffeeFileNames
{
public:

    static CMSCoffeeFileNames* instance();

    const QString& usersFilename() { return usersFilename_; }
    const QString& ticksFilename() { return ticksFilename_; }
    const QString& accountFilename() { return accountFilename_; }
    const QString& printFilename() { return printFilename_; }

    void setCMSFilenames();
    void setDAFFilenames();

private:

    CMSCoffeeFileNames();
    static CMSCoffeeFileNames *instance_;

    QString usersFilename_;
    QString ticksFilename_;
    QString accountFilename_;
    QString printFilename_;
};

#endif // CMSCOFFEEFILENAMES_H
