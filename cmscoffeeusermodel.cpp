#include <iostream>
#include <algorithm>
#include <iterator>
#include <locale>

#include <QDebug>
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

#include <QFileDialog>
#include <QPrinter>
#include <QPainter>

#include "cmscoffeeusermodel.h"
#include "cmscoffeetickmodel.h"

CMSCoffeeUserModel::CMSCoffeeUserModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

CMSCoffeeUser* CMSCoffeeUserModel::userByUUID(const QString& uuid)
{
    std::map<QString,CMSCoffeeUser*>::const_iterator it = usersByUUID_.find(uuid);
    if (it==usersByUUID_.end()) return 0;
    return it->second;
}

CMSCoffeeUser* CMSCoffeeUserModel::userByName(const QString& name)
{
    std::map<QString,CMSCoffeeUser*>::const_iterator it = usersByName_.find(name);
    if (it==usersByName_.end()) return 0;
    return it->second;
}

void CMSCoffeeUserModel::sortByName()
{
    std::sort(users_.begin(), users_.end(),
              [](const CMSCoffeeUser* a, const CMSCoffeeUser* b)
    {
        int ret = a->getName().localeAwareCompare(b->getName());
        return ret<0;
    });

    emit layoutChanged();
}

void CMSCoffeeUserModel::write()
{
    QDir path;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#else
    path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#endif

    QFile file(path.absoluteFilePath("users.xml"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QXmlStreamWriter stream(&file);
    stream.setCodec("UTF-8");
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("users");

    for (std::vector<CMSCoffeeUser*>::const_iterator it = users_.begin();
         it != users_.end();
         ++it) {
        stream.writeStartElement("user");

        stream.writeAttribute("uuid", (*it)->getUUID());
        stream.writeAttribute("name", (*it)->getName());
        stream.writeAttribute("active", (*it)->getActive()==true ? "1" : "0");

        stream.writeEndElement();
    }

    stream.writeEndElement();
    stream.writeEndDocument();
}

void CMSCoffeeUserModel::print()
{
    QString filename = QFileDialog::getSaveFileName(0,
                                                    "Print to PDF",
                                                    QDir::homePath() + "/CMSCoffee.pdf",
                                                    "Documents (*.pdf)");
    if (filename.isEmpty()) return;

    std::vector<CMSCoffeeUser*> au;
    for (std::vector<CMSCoffeeUser*>::iterator it = users_.begin();
         it!=users_.end();
         ++it) {
        if ((*it)->getActive()==true) au.push_back(*it);
    }

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setOutputFileName(filename);

    int width = printer.width();
    int height = printer.height();

    QPainter painter;
    painter.begin(&printer);

    QPen pen(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);

    QColor grey(220, 220, 220);
    QBrush brush(grey);

    int headerHeight = 20;
    int rowHeight = 20;
    int linesPerPage = (height - 20 - headerHeight)/rowHeight;
    headerHeight = height - 20 - linesPerPage * rowHeight;

    int lineNumber = 0;

    QString balanceFormat = QString::fromUtf8("%1 \u20AC");

    for (std::vector<CMSCoffeeUser*>::iterator it = au.begin();
         it!=au.end();
         ++it) {
        if (lineNumber==0) {
            painter.fillRect(10, 10, width-20, headerHeight, brush);

            painter.drawRect(10, 10, width-20, height-20);

            painter.drawLine(10, 10 + headerHeight, width-10, 10 + headerHeight);

            painter.drawText(15, 10, 220, headerHeight,
                             Qt::AlignHCenter + Qt::AlignVCenter, "NAME");

            painter.drawText(245, 10, 60, headerHeight,
                             Qt::AlignHCenter + Qt::AlignVCenter, "BALANCE");

            for (int i=1;i<linesPerPage;++i) {
                painter.drawLine(10, 10 + headerHeight + i*rowHeight,
                                 width-10, 10 + headerHeight + i*rowHeight);
            }

            painter.drawLine(240, 10, 240, height-10);
            painter.drawLine(310, 10, 310, height-10);
        }

        painter.drawText(15, 10 + headerHeight + lineNumber*rowHeight, 220, rowHeight,
                         Qt::AlignLeft + Qt::AlignVCenter, (*it)->getName());

        double balance = (*it)->getBalance();
        QString balanceString = balanceFormat.arg(balance, 0, 'f', 2);
        painter.drawText(245, 10 + headerHeight + lineNumber*rowHeight, 60, rowHeight,
                         Qt::AlignRight + Qt::AlignVCenter,balanceString);

        lineNumber++;
        if (lineNumber==linesPerPage) {
            lineNumber = 0;
            printer.newPage();
        }
    }

    painter.end();
}

void CMSCoffeeUserModel::read()
{
    users_.clear();

    QDir path;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#else
    path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#endif

    QFile file(path.absoluteFilePath("users.xml"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QXmlStreamReader stream(&file);

    QString uuid, name;
    bool active;

    while (!stream.atEnd()) {
        stream.readNextStartElement();

        if (stream.isStartElement() && stream.name()=="user") {
            uuid = stream.attributes().value("uuid").toString();
            name = stream.attributes().value("name").toString();
            active = stream.attributes().value("active").toString().toInt();

            CMSCoffeeUser * user = new CMSCoffeeUser(uuid, name, active);
            users_.push_back(user);
            usersByUUID_[user->getUUID()] = user;
            usersByName_[user->getName()] = user;
        }
    }
}

void CMSCoffeeUserModel::import(const std::string& filename)
{
    users_.clear();

    QFile file(filename.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line = in.readLine();
    while (!in.atEnd()) {
        line = in.readLine();
        QStringList list = line.split(",");
        QString name = list.at(0);

        if (name.length()==0) break;

        CMSCoffeeUser * user = new CMSCoffeeUser(name, true);
        users_.push_back(user);
        usersByUUID_[user->getUUID()] = user;
        usersByName_[user->getName()] = user;
    }

    sortByName();
}

int CMSCoffeeUserModel::rowCount(const QModelIndex & /*parent*/) const
{
    return users_.size() + 1;
}

int CMSCoffeeUserModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 4;
}

QVariant CMSCoffeeUserModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation==Qt::Horizontal) {
            if (section==0) return QString("Name");
            if (section==1) return QString("Active");
            if (section==2) return QString("Balance");
            if (section==3) return QString("Deposits");
        } else if (orientation==Qt::Vertical) {
            if (section<users_.size()) return QString::number(section+1);
            return QString("*");
        }
    }
    if (role == Qt::TextAlignmentRole) {
        if (orientation==Qt::Horizontal) {
            return Qt::AlignHCenter;
        } else if (orientation==Qt::Vertical) {
            if (section<users_.size()) return Qt::AlignRight;
            return Qt::AlignHCenter;
        }
    }
    if (role == Qt::SizeHintRole) {
        if (orientation==Qt::Horizontal) {
            //return QSize
        }
    }

    return QVariant();
}

QVariant CMSCoffeeUserModel::data(const QModelIndex &index, int role) const
{
    static const QString balanceFormat =  QString::fromUtf8("%1 \u20AC");

    if (role == Qt::DisplayRole) {
        if (index.row()<users_.size()) {
            if (index.column()==0) {
                return QString::fromUtf8(users_.at(index.row())->getName().toStdString().c_str());
            }
            if (index.column()==1) {
                //return QVariant((bool)users_.at(index.row())->getActive());
            }
            if (index.column()==2) {
                return QVariant(balanceFormat.arg(users_.at(index.row())->getBalance(), 0, 'f', 2));
            }
            if (index.column()==3) {
                return QVariant(balanceFormat.arg(users_.at(index.row())->getAccountBalance(), 0, 'f', 2));
            }
        } else {
            if (index.column()==0) {
                return QString("");
            }
        }
    }
    if (role == Qt::EditRole) {
        if (index.row()<users_.size() && index.column()==0) {
            return QString::fromUtf8(users_.at(index.row())->getName().toStdString().c_str());
        }
    }
    if (role == Qt::CheckStateRole) {
        if (index.row()<users_.size() && index.column()==1) {
            return (users_.at(index.row())->getActive()==true ? Qt::Checked : Qt::Unchecked);
        }
    }
    if (role == Qt::TextAlignmentRole) {
        if (index.column()==0) return Qt::AlignLeft + Qt::AlignVCenter;
        if (index.column()==1) return Qt::AlignHCenter + Qt::AlignVCenter;
        if (index.column()==2) return Qt::AlignRight + Qt::AlignVCenter;
        if (index.column()==3) return Qt::AlignRight + Qt::AlignVCenter;
    }

    return QVariant();
}

bool CMSCoffeeUserModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::CheckStateRole) {
        if (index.row()<users_.size() && index.column()==1) {
            users_.at(index.row())->setActive(value.toBool());
            emit dataChanged(index, index);
        }
    }
    if (role == Qt::EditRole) {
        if (index.column()==0) {
            if (index.row()<users_.size()) {
                QString name = value.toString();
                users_.at(index.row())->setName(name.toUtf8().data());
                emit dataChanged(index, index);
            } else {
                QString uname = value.toString();
                CMSCoffeeUser * user = userByName(uname.toUtf8().data());
                int c = 2;
                while (user) {
                    uname = value.toString();
                    uname += " ";
                    uname += QString::number(c++);
                    user = userByName(uname);
                }

                user = new CMSCoffeeUser(uname, true);
                users_.push_back(user);
                usersByUUID_[user->getUUID()] = user;
                usersByName_[user->getName()] = user;

                emit dataChanged(index, index);
                emit layoutChanged();
            }
        }
    }
    return true;
}

Qt::ItemFlags CMSCoffeeUserModel::flags(const QModelIndex & index) const
{
    if (index.column()==0)
        return Qt::ItemIsEditable | Qt::ItemIsEnabled;

    if (index.row()<users_.size() && index.column()==1)
        return Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled;
}

CMSCoffeeActiveUserModel::CMSCoffeeActiveUserModel(CMSCoffeeUserModel* userModel,
                                                   CMSCoffeeTickModel* tickModel,
                                                   QObject *parent) :
    QAbstractTableModel(parent),
    userModel_(userModel),
    tickModel_(tickModel)
{
    connect(userModel_, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(updateActiveUsers()));

    connect(tickModel_, SIGNAL(datesChanged()),
            this, SLOT(updateActiveUsers()));

    updateActiveUsers();
}

void CMSCoffeeActiveUserModel::updateActiveUsers()
{
    std::cout << "CMSCoffeeActiveUserModel::updateActiveUsers()" << std::endl;

    unsigned long oldSize = users_.size();

    users_.clear();

    for (std::vector<CMSCoffeeUser*>::const_iterator it = userModel_->getUsers().begin();
         it!=userModel_->getUsers().end();
         ++it) {
        CMSCoffeeUser* user = *it;
        if (user->getActive()==true) {
            users_.push_back(user);
        }
    }

    //emit dataChanged(index(0,0), index(std::max(users_.size(), oldSize), 1));
    emit layoutChanged();
}

void CMSCoffeeActiveUserModel::sortByName()
{
    std::sort(users_.begin(), users_.end(),
              [](const CMSCoffeeUser* a, const CMSCoffeeUser* b)
    {
        int ret = a->getName().localeAwareCompare(b->getName());
        return ret<0;
    });

    emit layoutChanged();
}

int CMSCoffeeActiveUserModel::rowCount(const QModelIndex &index) const
{
    return users_.size();
}

int CMSCoffeeActiveUserModel::columnCount(const QModelIndex &index) const
{
    return tickModel_->getDates().size() + 3;
}

QVariant CMSCoffeeActiveUserModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation==Qt::Horizontal) {
            if (section==0) return QString("Name");
            if (section==1) return QString("Balance");
            if (section==2) return QString("Deposits");
            if (section>2) {
                std::set<QDate>& dates = tickModel_->getDates();
                std::set<QDate>::reverse_iterator it = dates.rbegin();
                std::advance(it, section-3);
                return (*it).toString("dd/MM/yy");
            }
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

QVariant CMSCoffeeActiveUserModel::data(const QModelIndex &index, int role) const
{
    static const QString balanceFormat =  QString::fromUtf8("%1 \u20AC");

    if (role == Qt::DisplayRole) {
        if (index.column()==0) {
            return QString::fromUtf8(users_.at(index.row())->getName().toStdString().c_str());
        }
        if (index.column()==1) {
            return QVariant(balanceFormat.arg(users_.at(index.row())->getBalance(), 0, 'f', 2));
        }
        if (index.column()==2) {
            return QVariant(balanceFormat.arg(users_.at(index.row())->getAccountBalance(), 0, 'f', 2));
        }
        if (index.column()>2) {
            CMSCoffeeUser * user = users_.at(index.row());
            std::set<QDate>& dates = tickModel_->getDates();
            std::set<QDate>::reverse_iterator it = dates.rbegin();
            std::advance(it, index.column()-3);
            QDate date = *it;
            CMSCoffeeTickEntry * entry = user->getTickEntry(date);
            if (!entry) return QString::number(0);
            return QString::number(entry->getCount());
        }
    }

    if (role == Qt::EditRole) {
        if (index.column()>2) {
            CMSCoffeeUser * user = users_.at(index.row());
            std::set<QDate>& dates = tickModel_->getDates();
            std::set<QDate>::reverse_iterator it = dates.rbegin();
            std::advance(it, index.column()-3);
            QDate date = *it;
            CMSCoffeeTickEntry * entry = user->getTickEntry(date);
            if (!entry) return QVariant(0);
            return QVariant(entry->getCount());
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (index.column()==0) return Qt::AlignLeft + Qt::AlignVCenter;
        if (index.column()>0) return Qt::AlignRight + Qt::AlignVCenter;
    }

    return QVariant();
}

bool CMSCoffeeActiveUserModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole) {
        if (index.column()>2) {
            if (!value.canConvert<int>()) return false;
            CMSCoffeeUser * user = users_.at(index.row());
            std::set<QDate>& dates = tickModel_->getDates();
            std::set<QDate>::reverse_iterator it = dates.rbegin();
            std::advance(it, index.column()-3);
            QDate date = *it;
            CMSCoffeeTickEntry * entry = user->getTickEntry(date);
            bool valueChanged = false;
            if (!entry) {
                entry = new CMSCoffeeTickEntry(user->getUUID(), date,
                                               CMSCoffeeTickEntry::tickPrice(),
                                               value.toInt());
                tickModel_->addEntry(entry);
                user->addTickEntry(entry);
                valueChanged = true;
            } else {
                valueChanged = (value.toInt() != entry->getCount());
                entry->setCount(value.toInt());
            }
            emit dataChanged(index, index);
        }
    }
    return true;
}

Qt::ItemFlags CMSCoffeeActiveUserModel::flags(const QModelIndex & index) const
{
    if (index.column()==0)
        return Qt::ItemIsEnabled;

    if (index.column()==1)
        return Qt::ItemIsEnabled;

    if (index.column()==2)
        return Qt::ItemIsEnabled;

    if (index.column()>2)
        return Qt::ItemIsEditable | Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled;
}

QModelIndex CMSCoffeeActiveUserModel::index(int row, int column, const QModelIndex & parent)
{
    return createIndex(row, column);
}
