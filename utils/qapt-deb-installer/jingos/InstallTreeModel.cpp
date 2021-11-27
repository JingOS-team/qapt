/*
 * Copyright (C) 2021 Beijing Jingling Information System Technology Co., Ltd. All rights reserved.
 * 
 * Authors: 
 * Jiashu Yu <yujiashu@jingos.com>
 *
 */

#include "InstallTreeModel.h"

#include <QHash>
#include <QList>
#include <KLocalizedString>
#include <KStandardGuiItem>

InstallTreeModel::InstallTreeModel(QObject *parent) : QStandardItemModel(parent)
{
    m_mapRoleName[TreeModelDataRow] = "dataRole";
}

QHash<int, QByteArray> InstallTreeModel::roleNames() const
{
    return m_mapRoleName;
}

void InstallTreeModel::addPackages(const QApt::StateChanges &changes)
{
    QHash<int, QString> hash;
    hash[QApt::Package::ToInstall] = i18nc("@info:status Requested action", "Install");
    hash[QApt::Package::NewInstall] = i18nc("@info:status Requested action", "Install");
    hash[QApt::Package::ToRemove] = i18nc("@info:status Requested action", "Remove");

    for (auto i = changes.constBegin(); i != changes.constEnd(); ++i) {
        QStandardItem *root = new QStandardItem;
        root->setText(hash.value(i.key()));
        root->setEditable(false);

        QFont font = root->font();
        font.setBold(true);
        root->setFont(font);
        const QApt::PackageList packages = i.value();
        QStandardItem *item = 0;
        for (QApt::Package *package : packages) {
            item = new QStandardItem;
            item->setText(package->name());
            item->setEditable(false);
            item->setIcon(QIcon::fromTheme("muon"));
            root->appendRow(item);
        }

        this->appendRow(root);
    }
}
int InstallTreeModel::countChanges(const QApt::StateChanges &changes)
{
    int count = 0;
    auto iter = changes.constBegin();

    while (iter != changes.constEnd()) {
        const QApt::PackageList packages = iter.value();
        count += packages.size();
        ++iter;
    }

    return count;
}
