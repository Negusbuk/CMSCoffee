#ifndef CMSCOFFEETABLEVIEWDELEGATES_H
#define CMSCOFFEETABLEVIEWDELEGATES_H

#include <QItemDelegate>

#include <cmscoffeeaccountentrycompletionmodel.h>

class CMSCoffeeIntSpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CMSCoffeeIntSpinBoxDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
};

class CMSCoffeeDateEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CMSCoffeeDateEditDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
};

class CMSCoffeeEuroSpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CMSCoffeeEuroSpinBoxDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
};

class CMSCoffeeLineEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CMSCoffeeLineEditDelegate(CMSCoffeeAccountEntryCompletionModel* completionModel,
                                       QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
protected:

    CMSCoffeeAccountEntryCompletionModel* completionModel_;
};

#endif // CMSCOFFEETABLEVIEWDELEGATES_H
