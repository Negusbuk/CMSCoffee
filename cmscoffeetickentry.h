#ifndef CMSCOFFEETICKENTRY_H
#define CMSCOFFEETICKENTRY_H

#include <QString>
#include <QDate>

class CMSCoffeeTickEntry
{
public:
    explicit CMSCoffeeTickEntry(const QString uuid,
                                const QDate& date, float price, int count);

    const QString& getUUID() const { return uuid_; }
    const QDate& getDate() const { return date_; }
    float getPrice() const { return price_; }
    int getCount() const { return count_; }

    void setCount(int count) { count_ = count; }

    static float tickPrice(const QDate& date);

protected:

    QString uuid_;
    QDate date_;
    float price_;
    int count_;
};

#endif // CMSCOFFEETICKENTRY_H
