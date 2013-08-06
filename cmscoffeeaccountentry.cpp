#include "cmscoffeeaccountentry.h"

CMSCoffeeAccountEntry::CMSCoffeeAccountEntry(const QString uuid,
                                             const QDate& date, float amount,
                                             CMSCoffeeUser* user) :
    uuid_(uuid),
    date_(date),
    amount_(amount),
    user_(user)
{

}
