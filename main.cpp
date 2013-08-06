#include <iostream>

#include <QApplication>
#include <QTextCodec>

#include <cmscoffeemainwindow.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("macintosh");

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    CMSCoffeeMainWindow w;
    w.show();
    
    return a.exec();
}
