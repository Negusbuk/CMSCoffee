#include "cmscoffeetickentry.h"

CMSCoffeeTickEntry::CMSCoffeeTickEntry(const QString uuid,
                                       const QDate& date, float price, int count) :
    uuid_(uuid),
    date_(date),
    price_(price),
    count_(count)
{

}
