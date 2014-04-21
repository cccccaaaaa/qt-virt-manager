#ifndef VIRTNET_MODEL_H
#define VIRTNET_MODEL_H

#include <QAbstractTableModel>
#include <QIcon>
#include <QList>
#include <QDebug>
#include "virt_objects/index.h"

class VirtNetModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit VirtNetModel(QObject *parent = 0);
    ~VirtNetModel();
    QList<Index *>  virtNetDataList;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRow(int);
    bool removeRow(int);

signals:

private:
    QString  column0;
    QString  column1;
    QString  column2;
    QString  column3;
    QIcon    activeIcon;
    QIcon    no_activeIcon;
    QIcon    defined;
    QIcon    created;

public slots:
};

#endif // VIRTNET_MODEL_H
