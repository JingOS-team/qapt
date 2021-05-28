/***************************************************************************
 *   Copyright © 2021 yujiashu <yujiashu@jingos.com>                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation; either version 2 of        *
 *   the License or (at your option) version 3 or any later version        *
 *   accepted by the membership of KDE e.V. (or its successor approved     *
 *   by the membership of KDE e.V.), which shall act as a proxy            *
 *   defined in Section 14 of version 3 of the license.                    *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
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
