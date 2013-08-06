#ifndef CMSCOFFEEACCOUNTMODEL_H
#define CMSCOFFEEACCOUNTMODEL_H

#include <vector>

#include <QAbstractTableModel>
#include <QObject>
#include <QDate>

#include <cmscoffeeaccountentry.h>
#include <cmscoffeeusermodel.h>

class CMSCoffeeAccountModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CMSCoffeeAccountModel(CMSCoffeeUserModel* userModel,
                                   QObject *parent = 0);

    void read();
    void import(const std::string& filename);
    const std::vector<CMSCoffeeAccountEntry*> getEntries() const { return entries_; }

    int rowCount(const QModelIndex &index = QModelIndex()) const ;
    int columnCount(const QModelIndex &index = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const;

public slots:

    void write();
    void sortByDate();
    void addAccountEntry(const QDate&date, const QString&name, float amount);

protected:

    QDate dateFromString(const QString& date);

    CMSCoffeeUserModel* userModel_;

    std::vector<CMSCoffeeAccountEntry*> entries_;
 };

#endif // CMSCOFFEEACCOUNTMODEL_H
