#include <iostream>

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include "cmscoffeetickmodel.h"
#include "cmscoffeeuser.h"

CMSCoffeeTickModel::CMSCoffeeTickModel(CMSCoffeeUserModel* userModel,
                                       QObject *parent) :
    QObject(parent),
    userModel_(userModel)
{

}

void CMSCoffeeTickModel::addDate()
{
    QDate date = QDate::currentDate();
    std::set<QDate>::iterator it = dates_.find(date);
    if (it!=dates_.end()) return;
    dates_.insert(date);
    emit datesChanged();
}

void CMSCoffeeTickModel::addEntry(CMSCoffeeTickEntry* entry)
{
    QDate date = entry->getDate();
    std::set<QDate>::iterator it = dates_.find(date);
    if (it==dates_.end()) {
        dates_.insert(date);
        emit datesChanged();
    }
    entries_.push_back(entry);
}

void CMSCoffeeTickModel::sortByDate()
{
    std::sort(entries_.begin(), entries_.end(),
              [&](const CMSCoffeeTickEntry* a, const CMSCoffeeTickEntry* b)
    {
        int days = a->getDate().daysTo(b->getDate());
        if (days!=0) return (days<0);

        CMSCoffeeUser * ua = userModel_->userByUUID(a->getUUID());
        QString sa = a->getUUID();
        if (ua) sa = ua->getName();

        CMSCoffeeUser * ub = userModel_->userByUUID(b->getUUID());
        QString sb = b->getUUID();
        if (ub) sb = ub->getName();

        //std::cout << "comp " << sa.toStdString() << " " << sb.toStdString() << std::endl;

        int ret = sa.localeAwareCompare(sb);
        return ret<0;
    });
}

void CMSCoffeeTickModel::write()
{
    QDir path;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#else
    path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#endif

    QFile file(path.absoluteFilePath("ticks.xml"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("ticks");

    for (std::vector<CMSCoffeeTickEntry*>::const_iterator it = entries_.begin();
         it != entries_.end();
         ++it) {
        stream.writeStartElement("tick");

        stream.writeAttribute("uuid", (*it)->getUUID());
        stream.writeAttribute("date", (*it)->getDate().toString(Qt::ISODate));
        stream.writeAttribute("price", QString::number((*it)->getPrice(), 'f', 2));
        stream.writeAttribute("count", QString::number((*it)->getCount()));

        stream.writeEndElement();
    }

    stream.writeEndElement();
    stream.writeEndDocument();
}

void CMSCoffeeTickModel::read()
{
    entries_.clear();
    dates_.clear();

    QDir path;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#else
    path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#endif

    QFile file(path.absoluteFilePath("ticks.xml"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QXmlStreamReader stream(&file);

    QDate date;
    QString uuid;
    float price;
    int count;

    while (!stream.atEnd()) {
        stream.readNextStartElement();

        if (stream.isStartElement() && stream.name()=="tick") {
            uuid = stream.attributes().value("uuid").toString();
            date = QDate::fromString(stream.attributes().value("date").toString(), Qt::ISODate);
            price = stream.attributes().value("price").toString().toFloat();
            count = stream.attributes().value("count").toString().toInt();

            CMSCoffeeUser* user = userModel_->userByUUID(uuid);
            if (!user) continue;

            CMSCoffeeTickEntry* entry = new CMSCoffeeTickEntry(user->getUUID(), date, price, count);
            entries_.push_back(entry);
            user->addTickEntry(entry);
            dates_.insert(date);
        }
    }
}

void CMSCoffeeTickModel::import(const std::string& filename)
{
    entries_.clear();

    QFile file(filename.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line = in.readLine();

    std::vector<QDate> dates;
    QStringList dateStrings = line.split(",");
    for (int i=0;i<4;++i) dateStrings.removeFirst();
    dateStrings.removeAll("");
    for (QStringList::Iterator it = dateStrings.begin();
         it!=dateStrings.end();
         ++it) {
        QDate date = dateFromString(*it);
        dates.push_back(date);
        dates_.insert(date);
    }

    while (!in.atEnd()) {
        line = in.readLine();
        //std::cout << line.toStdString() << std::endl;
        QStringList list = line.split(",");
        QString name = list.at(0);
        if (name.length()==0) break;

        CMSCoffeeUser* user = userModel_->userByName(name.toUtf8().data());
        if (!user) continue;

        for (int i=0;i<6;++i) list.removeFirst();
        list.removeLast();

        int d = 0;
        for (QStringList::Iterator it = list.begin();
             it!=list.end();
             ++it) {
            QDate date = dates.at(d++);
            int count = it->toInt();

            if (count==0) continue;

            CMSCoffeeTickEntry* entry = new CMSCoffeeTickEntry(user->getUUID(), date, 0.25, count);
            entries_.push_back(entry);
            user->addTickEntry(entry);
        }

        // std::cout << user->getTickCount() << std::endl;
        // std::cout << user->getTickBalance() << std::endl;
    }
}

QDate CMSCoffeeTickModel::dateFromString(const QString& date)
{
    QStringList tokens = date.split(".");
    if (tokens.size()!=3) return QDate();
    return QDate(tokens.at(2).toInt()+2000, tokens.at(1).toInt(), tokens.at(0).toInt());
}
