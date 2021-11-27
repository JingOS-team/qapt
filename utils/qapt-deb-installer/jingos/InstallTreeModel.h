/*
 * Copyright (C) 2021 Beijing Jingling Information System Technology Co., Ltd. All rights reserved.
 * 
 * Authors: 
 * Jiashu Yu <yujiashu@jingos.com>
 *
 */

#ifndef INSTALLTREEMODEL_H
#define INSTALLTREEMODEL_H

#include <QApt/Package>
#include <QStandardItemModel>

class InstallTreeModel : public QStandardItemModel
{
    Q_OBJECT
public:

    enum MyTreeModel_Roles
    {
        TreeModelDataRow = Qt::DisplayRole,
    };
    explicit InstallTreeModel(QObject *parent = nullptr);
    virtual ~InstallTreeModel() = default;
    QHash<int, QByteArray> roleNames() const override;

    void addPackages(const QApt::StateChanges &changes);
    int countChanges(const QApt::StateChanges &changes);

signals:


private:
    QHash<int, QByteArray> m_mapRoleName;

};

#endif // INSTALLTREEMODEL_H
