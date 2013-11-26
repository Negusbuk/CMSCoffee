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

#include "cmscoffeeaccountmodel.h"

CMSCoffeeAccountModel::CMSCoffeeAccountModel(CMSCoffeeUserModel* userModel,
                                             QObject *parent) :
    QAbstractTableModel(parent),
    userModel_(userModel)
{

}

void CMSCoffeeAccountModel::sortByDate()
{
    std::sort(entries_.begin(), entries_.end(),
              [&](const CMSCoffeeAccountEntry* a, const CMSCoffeeAccountEntry* b)
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

    emit layoutChanged();
}

void CMSCoffeeAccountModel::addAccountEntry(const QDate& date, const QString& name, float amount)
{
    CMSCoffeeUser * user = userModel_->userByName(name);
    CMSCoffeeAccountEntry * entry;
    if (user) {
        entry = new CMSCoffeeAccountEntry(user->getUUID(), date, amount, user);
    } else {
        entry = new CMSCoffeeAccountEntry(name, date, amount);
    }

    entries_.push_back(entry);
    if (user) user->addAccountEntry(entry);

    sortByDate();
}

void CMSCoffeeAccountModel::write()
{
    QDir path;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#else
    path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#endif

    QFile file(path.absoluteFilePath("account.xml"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("entries");

    for (std::vector<CMSCoffeeAccountEntry*>::const_iterator it = entries_.begin();
         it != entries_.end();
         ++it) {

        if ((*it)->getUser()) {
            stream.writeStartElement("userentry");

            stream.writeAttribute("uuid", (*it)->getUUID());
            stream.writeAttribute("date", (*it)->getDate().toString(Qt::ISODate));
            stream.writeAttribute("amount", QString::number((*it)->getAmount(), 'f', 2));

            stream.writeEndElement();
        } else {
            stream.writeStartElement("entry");

            stream.writeAttribute("date", (*it)->getDate().toString(Qt::ISODate));
            stream.writeAttribute("amount", QString::number((*it)->getAmount(), 'f', 2));
            stream.writeCharacters((*it)->getUUID());

            stream.writeEndElement();
        }
    }

    stream.writeEndElement();
    stream.writeEndDocument();
}

void CMSCoffeeAccountModel::read()
{
    entries_.clear();

    QDir path;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#else
    path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#endif

    QFile file(path.absoluteFilePath("account.xml"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QXmlStreamReader stream(&file);

    QDate date;
    QString uuid;
    float amount;

    while (!stream.atEnd()) {
        stream.readNextStartElement();

        if (stream.isStartElement() && stream.name()=="entry") {
            date = QDate::fromString(stream.attributes().value("date").toString(), Qt::ISODate);
            amount = stream.attributes().value("amount").toString().toFloat();
            uuid = stream.readElementText();
        } else if (stream.isStartElement() && stream.name()=="userentry") {
            date = QDate::fromString(stream.attributes().value("date").toString(), Qt::ISODate);
            amount = stream.attributes().value("amount").toString().toFloat();
            uuid = stream.attributes().value("uuid").toString();
        } else
            continue;

        CMSCoffeeUser* user = userModel_->userByUUID(uuid);

        CMSCoffeeAccountEntry* entry;
        if (user!=0) {
            entry = new CMSCoffeeAccountEntry(user->getUUID(), date, amount, user);
        } else {
            entry = new CMSCoffeeAccountEntry(uuid, date, amount);
        }
        entries_.push_back(entry);
        if (user) user->addAccountEntry(entry);
    }
}

void CMSCoffeeAccountModel::import(const std::string& filename)
{
    entries_.clear();

    QFile file(filename.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line = in.readLine();

    while (!in.atEnd()) {
        line = in.readLine();

        if (!line.contains(',') || line.length()<5) break;

        QRegExp rx("(?:^|,)(?:\\s*)\"(.*)\"(?:\\s*)(?:,|$)|(?:^|,)(?:\\s*)([^\"]*)(?:\\s*)(?:,|$)",
                   Qt::CaseSensitive, QRegExp::RegExp2);
        rx.setMinimal(true);
        QStringList list;
        int pos = 0;
        while ((pos = rx.indexIn(line, pos)) != -1) {
            list << (rx.cap(1).isEmpty() ? rx.cap(2) : rx.cap(1));
            pos += rx.matchedLength()-1;
        }

        if (list.at(0).length()==0) break;

        QDate date = dateFromString(list.at(0));
        QString description = list.at(1);
        QString s = list.at(2);
        s.remove(s.length()-1, 1);
        s.replace(",", ".");

        float amount = s.toFloat();

        CMSCoffeeUser* user = userModel_->userByName(description.toUtf8().data());

        CMSCoffeeAccountEntry* entry;
        if (user!=0) {
            entry = new CMSCoffeeAccountEntry(user->getUUID(), date, amount, user);
        } else {
            entry = new CMSCoffeeAccountEntry(description, date, amount);
        }
        entries_.push_back(entry);
        if (user) user->addAccountEntry(entry);
    }

    sortByDate();
}

int CMSCoffeeAccountModel::rowCount(const QModelIndex &index) const
{
    return entries_.size();
}

int CMSCoffeeAccountModel::columnCount(const QModelIndex &index) const
{
    return 3;
}

QVariant CMSCoffeeAccountModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation==Qt::Horizontal) {
            if (section==0) return QString("Date");
            if (section==1) return QString("Description");
            if (section==2) return QString("Amount");
        } else if (orientation==Qt::Vertical) {
            return QString::number(section+1);
        }
    }
    if (role == Qt::TextAlignmentRole) {
        if (orientation==Qt::Horizontal) {
            return Qt::AlignHCenter;
        } else if (orientation==Qt::Vertical) {
            return Qt::AlignRight;
        }
    }
    if (role == Qt::SizeHintRole) {
        if (orientation==Qt::Horizontal) {
            //return QSize
        }
    }

    return QVariant();
}

QVariant CMSCoffeeAccountModel::data(const QModelIndex &index, int role) const
{
    static const QString amountFormat =  QString::fromUtf8("%1 \u20AC");

    if (role == Qt::DisplayRole) {
        CMSCoffeeAccountEntry * entry = entries_.at(index.row());
        CMSCoffeeUser * user = userModel_->userByUUID(entry->getUUID());

        if (index.column()==0) return entry->getDate().toString("dd/MM/yy");
        if (index.column()==1) {
            if (user)
                return user->getName();
            else
                return entry->getUUID();
        }
        if (index.column()==2) return QVariant(amountFormat.arg(entry->getAmount(), 0, 'f', 2));
    }

    if (role == Qt::EditRole) {
        CMSCoffeeAccountEntry * entry = entries_.at(index.row());
        if (index.column()==0) {
            return QVariant(entry->getDate());
        }
        if (index.column()==1) {
            if (entry->getUser())
                return QVariant(entry->getUser()->getName());
            else
                return QVariant(entry->getUUID());
        }
        if (index.column()==2) {
            return QVariant(entry->getAmount());
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (index.column()==0) return Qt::AlignRight + Qt::AlignVCenter;
        if (index.column()==1) return Qt::AlignLeft + Qt::AlignVCenter;
        if (index.column()==2) return Qt::AlignRight + Qt::AlignVCenter;
    }

    return QVariant();
}

bool CMSCoffeeAccountModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole) {
        CMSCoffeeAccountEntry * entry = entries_.at(index.row());
        if (index.column()==0) {
            if (!value.canConvert<QDate>()) return false;
            entry->setDate(value.toDate());
            emit dataChanged(index, index);
        }
        if (index.column()==1) {
            if (!value.canConvert<QString>()) return false;
            QString description = value.toString();
            CMSCoffeeUser * user = userModel_->userByName(description);
            if (!user) {
                if (entry->getUser()) {
                    user = entry->getUser();
                    user->removeAccountEntry(entry);
                }
            } else {
                if (user->getUUID()==entry->getUUID()) return false;
                if (entry->getUser()) {
                    user = entry->getUser();
                    user->removeAccountEntry(entry);
                }
                entry->setUUID(user->getUUID());
                user->addAccountEntry(entry);
            }
            emit dataChanged(index, index);
        }
        if (index.column()==2) {
            if (!value.canConvert<float>()) return false;
            entry->setAmount(value.toFloat());
            emit dataChanged(index, index);
        }

    }
    return true;
}

Qt::ItemFlags CMSCoffeeAccountModel::flags(const QModelIndex & index) const
{
    if (index.column()==0)
        return Qt::ItemIsEditable | Qt::ItemIsEnabled;

    if (index.column()>0)
        return Qt::ItemIsEditable | Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled;
}

QDate CMSCoffeeAccountModel::dateFromString(const QString& date)
{
    QStringList tokens = date.split(".");
    if (tokens.size()!=3) return QDate();
    return QDate(tokens.at(2).toInt(), tokens.at(1).toInt(), tokens.at(0).toInt());
}
