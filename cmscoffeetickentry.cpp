#include "cmscoffeetickentry.h"

CMSCoffeeTickEntry::CMSCoffeeTickEntry(const QString uuid,
                                       const QDate& date, float price, int count) :
    uuid_(uuid),
    date_(date),
    price_(price),
    count_(count)
{

}

float CMSCoffeeTickEntry::tickPrice(const QDate& date)
{
    static const QDate periode1end(2015, 9, 30);
    static const float periode1price = 0.25;

    static const QDate periode2end(2025, 9, 30);
    static const float periode2price = 0.25;

    if (date<=periode1end) {
        return periode1price;

    } else if (date<=periode2end) {
        return periode2price;

    } else {
        return 0.25;
    }
}
