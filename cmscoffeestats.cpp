#include <iostream>
#include <fstream>
#include <map>

#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include "cmscoffeestats.h"

CMSCoffeeStats::CMSCoffeeStats(CMSCoffeeUserModel* userModel,
                               CMSCoffeeTickModel* tickModel,
                               CMSCoffeeAccountModel* accountModel,
                               QObject *parent) :
    QObject(parent),
    userModel_(userModel),
    tickModel_(tickModel),
    accountModel_(accountModel)
{

}

void CMSCoffeeStats::makeStats()
{
    std::cout << "makeStats" << std::endl;

    QString	dir = QFileDialog::getExistingDirectory(0,
                                                    "Save Stats to Directory",
                                                    QDir::homePath() + "/");
    if (dir.isEmpty()) return;

    std::cout << dir.toStdString() << std::endl;

    makeAccountHistory(dir);
    makeWeeklyStats(dir);
    makeUserList(dir);
    makeTickList(dir);
    makeUserStats(dir);
}

void CMSCoffeeStats::makeAccountHistory(const QString& dir)
{
    std::map<QDateTime,float> historyMap;

    const std::vector<CMSCoffeeAccountEntry*>& entries = accountModel_->getEntries();
    for (std::vector<CMSCoffeeAccountEntry*>::const_iterator it = entries.begin();
         it!=entries.end();
         ++it) {
        const CMSCoffeeAccountEntry* entry = *it;
        QDateTime dt = entry->getDateTime();
        float amount = entry->getAmount();

        std::map<QDateTime,float>::iterator itFind = historyMap.find(dt);
        if (itFind==historyMap.end()) {
            historyMap.insert(std::pair<QDateTime,float>(dt, amount));
        } else {
            float temp = itFind->second;
            itFind->second = temp + amount;
        }
    }

    QString fn = dir + "/CMSCoffeeAccountHistory.txt";
    QFile data(fn);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);

        float balance = 0;
        for (std::map<QDateTime,float>::iterator it = historyMap.begin();
             it!=historyMap.end();
             ++it) {
            balance += it->second;
            out << it->first.toString("yyyy-MM-dd") << "\t" << it->first.toTime_t() << "\t" << balance << "\n";
        }
    }
}


void CMSCoffeeStats::makeWeeklyStats(const QString& dir)
{
    int dow;
    std::map<QDate,int> weeklyMap;

    const std::vector<CMSCoffeeTickEntry*>& entries = tickModel_->getEntries();
    for (std::vector<CMSCoffeeTickEntry*>::const_iterator it = entries.begin();
         it!=entries.end();
         ++it) {
        const CMSCoffeeTickEntry* entry = *it;
        QDate date = entry->getDate();
        dow = date.dayOfWeek();
        date = date.addDays(-(dow-1));

        std::map<QDate,int>::iterator itFind = weeklyMap.find(date);
        if (itFind==weeklyMap.end()) {
            weeklyMap.insert(std::pair<QDate,int>(date, entry->getCount()));
        } else {
            int temp = itFind->second;
            itFind->second = temp + entry->getCount();
        }
    }

    QString fn = dir + "/CMSCoffeeWeeklyStats.txt";
    QFile data(fn);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);

        QDate lastDate;
        for (std::map<QDate,int>::iterator it = weeklyMap.begin();
             it!=weeklyMap.end();
             ++it) {
            if (lastDate.isValid()) {
                float nDays = lastDate.daysTo(it->first);
                QDateTime dt(it->first);
                out << it->first.toString("yyyy-MM-dd") << "\t" << dt.toTime_t() << "\t" << it->second/nDays << "\n";
                //std::cout << dt.toTime_t() << "\t" << it->second/nDays << std::endl;
            }
            lastDate = it->first;
        }
    }
}

void CMSCoffeeStats::makeUserList(const QString& dir)
{
    QString fn = dir + "/CMSCoffeeUsers.txt";
    QFile data(fn);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);

        const std::vector<CMSCoffeeUser*>& users = userModel_->getUsers();
        for (std::vector<CMSCoffeeUser*>::const_iterator it = users.begin();
             it!=users.end();
             ++it) {
            const CMSCoffeeUser* user = *it;

            //if (user->getActive()==false) continue;

            out << user->getUUID() << "\t" << (int)user->getActive() << "\t" << user->getName() << "\n";
        }
    }
}

void CMSCoffeeStats::makeTickList(const QString& dir)
{
    QString fn = dir + "/CMSCoffeeTicks.txt";
    QFile data(fn);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);

        const std::vector<CMSCoffeeUser*>& users = userModel_->getUsers();
        for (std::vector<CMSCoffeeUser*>::const_iterator it = users.begin();
             it!=users.end();
             ++it) {
            const CMSCoffeeUser* user = *it;

            //if (user->getActive()==false) continue;

            const std::vector<CMSCoffeeTickEntry*>& entries = user->getTickEntries();
            for (std::vector<CMSCoffeeTickEntry*>::const_iterator it = entries.begin();
                 it!=entries.end();
                 ++it) {
                const CMSCoffeeTickEntry* entry = *it;
                QDateTime dt(entry->getDate());

                out << user->getUUID() << "\t" << dt.toString("yyyy-MM-dd") << "\t" << dt.toTime_t() << "\t" << entry->getCount() << "\t" << entry->getPrice() << "\n";
            }
        }
    }
}

void CMSCoffeeStats::makeUserStats(const QString& dir)
{
    QString fn = dir + "/CMSUserStats.txt";
    QFile data(fn);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);

        const std::vector<CMSCoffeeUser*>& users = userModel_->getUsers();
        for (std::vector<CMSCoffeeUser*>::const_iterator it = users.begin();
             it!=users.end();
             ++it) {
            const CMSCoffeeUser* user = *it;

            QDateTime maxDT = QDateTime::fromTime_t(1);
            QDateTime minDT = QDateTime::currentDateTime().addDays(10);
            int ticks = 0;

            const std::vector<CMSCoffeeTickEntry*>& entries = user->getTickEntries();
            for (std::vector<CMSCoffeeTickEntry*>::const_iterator it = entries.begin();
                 it!=entries.end();
                 ++it) {
                const CMSCoffeeTickEntry* entry = *it;
                QDateTime dt(entry->getDate());

                if (entry->getCount()>0) {
                    if (dt < minDT) minDT = dt;
                    if (dt > maxDT) maxDT = dt;

                    ticks += entry->getCount();
                }
            }

            if (ticks!=0) {
                out << user->getUUID() << "\t"
                    << (int)user->getActive() << "\t"
                    << minDT.toString("yyyy-MM-dd") << "\t"
                    << maxDT.toString("yyyy-MM-dd") << "\t"
                    << minDT.daysTo(maxDT) << "\t"
                    << ticks << "\n";
            }
        }
    }
}
