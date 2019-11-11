#ifndef CMSCOFFEESELECTOR_H
#define CMSCOFFEESELECTOR_H

#include <QObject>
#include <QDialog>

class CMSCoffeeSelector : public QDialog
{
    Q_OBJECT
public:

    CMSCoffeeSelector();
    int currentSelection() { return currentSelection_; }

private:

    int currentSelection_;
};

#endif // CMSCOFFEESELECTOR_H
