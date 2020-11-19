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
import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4

TabView {
    id: frame
//    property bool loadingFileVisiable : true

    implicitWidth: tabWidth
    implicitHeight: tabHeight
    Tab {
        title: i18nd("qapt-deb-installer", "Description")
        InstallText{
            text:debInstaller.descriPtionText
        }
    }
    Tab {
        title: i18nd("qapt-deb-installer", "Details")
        InstallText{
            text:debInstaller.detailsText
        }
    }
    Tab {
        title: i18nd("qapt-deb-installer", "Included Files")
        Item{
            Rectangle{
                width: viewHeight/5
                height: viewHeight/5
                visible: debInstaller.showInclude
                anchors.centerIn: parent
                AnimatedImage {
                    source: "file:///usr/share/icons/jing/SwiMachine/load.gif"
                }
            }
            InstallText{
                anchors.fill: parent
                visible: !debInstaller.showInclude
                text:debInstaller.includeFilesText
            }
        }

    }
    Tab {
        title: i18nd("qapt-deb-installer", "Addtional Changes")
        Item {
            id: treeViwFrame
            Label {
                id:lables
                anchors.top: parent.top
                anchors.topMargin:10
                anchors.left: parent.left
                width: parent.width
                height: tabHeight*0.116
                text: debInstaller.treeModel.rowCount? i18nd("qapt-deb-installer", "This action no requires changes to other packages."):
                                                       i18nd("qapt-deb-installer","This action requires changes to other packages: ")
                verticalAlignment:Text.AlignVCenter
                horizontalAlignment:Text.AlignLeft
                font.weight :Font.Normal
                font.family :"Gilroy"
                font.pixelSize : parseInt(tabHeight*0.032)
            }
            Rectangle{
                anchors.top:lables.bottom
                anchors.left: parent.left
                width: parent.width
                anchors.bottom: parent.bottom
                InstallTreeView{
                    anchors.fill: parent
                }
            }
        }
    }
    Tab {

        title: i18nd("qapt-deb-installer", "Progress")
        InstallText{
            text:debInstaller.progressText
        }
    }
    style: TabViewStyle {
        id:style
        frameOverlap: -5
        tab: Rectangle {
            id:tabBarBtn
            color: styleData.selected ? "#F2FFFFFF" :"#00767680"
            implicitWidth:  text.implicitWidth+(tabWidth-textlen.width)/5
            implicitHeight:tabHeight*0.136-tabHeight*0.136/7
            radius: tabHeight*0.0388
            Text {
                id: text
                anchors.centerIn: parent
                text: styleData.title
                color: styleData.selected ? "#000000":"#848488"
                font.pixelSize : parseInt(tabHeight*0.035)
                font.weight :Font.Medium
                font.family :"Gilroy"
            }
        }
        tabBar: Item {
            id: barItem
            Rectangle{
                anchors.centerIn: barItem
                implicitHeight: tabHeight*0.136
                implicitWidth: tabWidth
                color: "#1f767680"
                radius: tabHeight*0.0388
            }
        }
        tabsAlignment:Qt.AlignHCenter
        frame: Rectangle {
            id:framearea
        }
    }
    Text{
        id:textlen
        color: "#00000000"
        font.pixelSize : parseInt(tabHeight*0.035)
        font.weight :Font.Medium
        font.family :"Gilroy"
        text: i18nd("qapt-deb-installer", "DescriptionDetailsIncluded FilesAddtional ChangesProgress")
    }
}
