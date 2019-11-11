#include <iostream>

#include <QApplication>
#include <QTextCodec>

#include "cmscoffeefilenames.h"
#include <cmscoffeeselector.h>
#include <cmscoffeemainwindow.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("macintosh");

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#else
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif

    QCoreApplication::setOrganizationName("Negusbuk");
    QCoreApplication::setOrganizationDomain("mussgiller.de");
    QCoreApplication::setApplicationName("CMSCoffee");

    CMSCoffeeFileNames::instance();

    CMSCoffeeSelector s;
    s.exec();

    CMSCoffeeMainWindow w;
    w.show();
    
    std::cout << s.currentSelection() << std::endl;

    return a.exec();
}
