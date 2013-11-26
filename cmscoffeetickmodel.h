#ifndef CMSCOFFEETICKMODEL_H
#define CMSCOFFEETICKMODEL_H

#include <vector>
#include <set>

#include <QObject>
#include <QDate>

#include <cmscoffeetickentry.h>
#include <cmscoffeeusermodel.h>

class CMSCoffeeTickModel : public QObject
{
    Q_OBJECT
public:
    explicit CMSCoffeeTickModel(CMSCoffeeUserModel* userModel,
                                QObject *parent = 0);

    const std::vector<CMSCoffeeTickEntry*>& getEntries() const { return entries_; }

    void read();
    void import(const std::string& filename);

    std::set<QDate>& getDates() { return dates_; }
    void addEntry(CMSCoffeeTickEntry* entry);

public slots:

    void write();
    void sortByDate();
    void addDate();

protected:

    QDate dateFromString(const QString& date);

    CMSCoffeeUserModel* userModel_;

    std::vector<CMSCoffeeTickEntry*> entries_;
    std::set<QDate> dates_;

signals:

    void datesChanged();
};

#endif // CMSCOFFEETICKMODEL_H
