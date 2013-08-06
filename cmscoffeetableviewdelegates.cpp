#include <iostream>

#include <QSpinBox>
#include <QDateEdit>
#include <QLineEdit>
#include <QCompleter>

#include "cmscoffeetableviewdelegates.h"

CMSCoffeeIntSpinBoxDelegate::CMSCoffeeIntSpinBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}

QWidget *CMSCoffeeIntSpinBoxDelegate::createEditor(QWidget *parent,
                                                   const QStyleOptionViewItem &/* option */,
                                                   const QModelIndex &/* index */) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setMinimum(0);
    editor->setMaximum(500);
    editor->setAlignment(Qt::AlignRight);

    return editor;
}

void CMSCoffeeIntSpinBoxDelegate::setEditorData(QWidget *editor,
                                                const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}

void CMSCoffeeIntSpinBoxDelegate::setModelData(QWidget *editor,
                                               QAbstractItemModel *model,
                                               const QModelIndex &index) const
{
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

void CMSCoffeeIntSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
                                                       const QStyleOptionViewItem &option,
                                                       const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

CMSCoffeeDateEditDelegate::CMSCoffeeDateEditDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}

QWidget *CMSCoffeeDateEditDelegate::createEditor(QWidget *parent,
                                                   const QStyleOptionViewItem &/* option */,
                                                   const QModelIndex &/* index */) const
{
    QDateEdit *editor = new QDateEdit(parent);
    editor->setDisplayFormat("dd/MM/yy");
    return editor;
}

void CMSCoffeeDateEditDelegate::setEditorData(QWidget *editor,
                                              const QModelIndex &index) const
{
    QDate value = index.model()->data(index, Qt::EditRole).toDate();

    QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);
    dateEdit->setDate(value);
}

void CMSCoffeeDateEditDelegate::setModelData(QWidget *editor,
                                               QAbstractItemModel *model,
                                               const QModelIndex &index) const
{
    QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);
    QDate value = dateEdit->date();

    model->setData(index, value, Qt::EditRole);
}

void CMSCoffeeDateEditDelegate::updateEditorGeometry(QWidget *editor,
                                                       const QStyleOptionViewItem &option,
                                                       const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

CMSCoffeeEuroSpinBoxDelegate::CMSCoffeeEuroSpinBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}

QWidget *CMSCoffeeEuroSpinBoxDelegate::createEditor(QWidget *parent,
                                                   const QStyleOptionViewItem &/* option */,
                                                   const QModelIndex &/* index */) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setMinimum(-1000);
    editor->setMaximum(1000);
    editor->setAlignment(Qt::AlignRight);
    editor->setSuffix(QString::fromUtf8(" \u20AC"));
    editor->setDecimals(2);

    return editor;
}

void CMSCoffeeEuroSpinBoxDelegate::setEditorData(QWidget *editor,
                                                 const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void CMSCoffeeEuroSpinBoxDelegate::setModelData(QWidget *editor,
                                               QAbstractItemModel *model,
                                               const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

void CMSCoffeeEuroSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
                                                       const QStyleOptionViewItem &option,
                                                       const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

CMSCoffeeLineEditDelegate::CMSCoffeeLineEditDelegate(CMSCoffeeAccountEntryCompletionModel* completionModel,
                                                     QObject *parent) :
    QItemDelegate(parent),
    completionModel_(completionModel)
{

}

QWidget *CMSCoffeeLineEditDelegate::createEditor(QWidget *parent,
                                                 const QStyleOptionViewItem &/* option */,
                                                 const QModelIndex &/* index */) const
{
    QLineEdit *editor = new QLineEdit(parent);
    if (completionModel_) editor->setCompleter(new QCompleter(completionModel_));
    return editor;
}

void CMSCoffeeLineEditDelegate::setEditorData(QWidget *editor,
                                              const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void CMSCoffeeLineEditDelegate::setModelData(QWidget *editor,
                                               QAbstractItemModel *model,
                                               const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString value = lineEdit->text();

    model->setData(index, value, Qt::EditRole);
}

void CMSCoffeeLineEditDelegate::updateEditorGeometry(QWidget *editor,
                                                       const QStyleOptionViewItem &option,
                                                       const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
