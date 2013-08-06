#ifndef CMSCOFFEEUSERMODEL_H
#define CMSCOFFEEUSERMODEL_H

#include <vector>
#include <map>

#include <QObject>
#include <QAbstractTableModel>

#include <cmscoffeeuser.h>

class CMSCoffeeTickModel;

class CMSCoffeeUserModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CMSCoffeeUserModel(QObject *parent = 0);
    
    const std::vector<CMSCoffeeUser*>& getUsers() const { return users_; }
    CMSCoffeeUser* userByUUID(const QString& uuid);
    CMSCoffeeUser* userByName(const QString& name);

    void read();
    void import(const std::string& filename);

    int rowCount(const QModelIndex &index = QModelIndex()) const ;
    int columnCount(const QModelIndex &index = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const;

signals:
    
public slots:

    void write();
    void print();
    void sortByName();

protected:

    std::vector<CMSCoffeeUser*> users_;
    std::map<QString,CMSCoffeeUser*> usersByUUID_;
    std::map<QString,CMSCoffeeUser*> usersByName_;
};

class CMSCoffeeActiveUserModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CMSCoffeeActiveUserModel(CMSCoffeeUserModel* userModel,
                                      CMSCoffeeTickModel* tickModel,
                                      QObject *parent = 0);

    int rowCount(const QModelIndex &index = QModelIndex()) const ;
    int columnCount(const QModelIndex &index = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const;

protected slots:

    void updateActiveUsers();
    void sortByName();

protected:

    CMSCoffeeUserModel* userModel_;
    CMSCoffeeTickModel* tickModel_;
    std::vector<CMSCoffeeUser*> users_;
};

#endif // CMSCOFFEEUSERMODEL_H
