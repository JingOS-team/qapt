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
import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQml 2.12
//it items just tow level,if more it will look bad
TreeView {
    id:treeView
    anchors.centerIn: parent
    model: debInstaller.treeModel
    property int iconWidth: 20

    style:TreeViewStyle{
        frame:Item {
        }
        headerDelegate:Rectangle{
            visible: false
        }
        rowDelegate : Rectangle{
            height:  30
        }
        itemDelegate: Rectangle {
            id:itemArea
            anchors.left: parent.left
            Rectangle{
                height: styleData.hasSibling ? parent.height : parent.height / 2
                visible: !styleData.hasChildren
                width: 1
                color: "#767680"
                anchors.left: parent.left
                anchors.leftMargin: iconWidth/2
            }
            Rectangle{
                id:line1
                height: 1
                width: 20
                visible: !styleData.hasChildren
                color: "#767680"
                anchors.left: parent.left
                anchors.leftMargin: iconWidth/2
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                id:textArea
                anchors.left: parent.left
                anchors.leftMargin: styleData.hasChildren ? 0:iconWidth+line1.width+iconWidth/2+2
                anchors.verticalCenter: parent.verticalCenter
                text: styleData.value === undefined ? "" : styleData.value
                font.pixelSize: parseInt(viewHeight*0.02)
                font.family :"Gilroy"
                font.weight :Font.Normal
                horizontalAlignment:Text.AlignLeft
            }
            Rectangle{
                visible: styleData.isExpanded ? styleData.hasChildren:false
                anchors.top: textArea.bottom
                anchors.bottom: parent.bottom
                width: 1
                color: "#767680"
                anchors.left: parent.left
                anchors.leftMargin: iconWidth/2
            }
            Image {
                id: nodeTextImage
                anchors.left:styleData.hasChildren ? parent.left: line1.right
                anchors.verticalCenter: textArea.verticalCenter
                width: iconWidth
                height: iconWidth
                visible: !styleData.hasChildren
                source: "file:///usr/share/icons/jing/SwiMachine/broom.svg"
            }
        }
    }
    Component.onCompleted: {
        for(var i=0;i<debInstaller.treeModel.rowCount();i++){
            treeView.expand(debInstaller.treeModel.index(i,0));
        }
    }
    TableViewColumn {
        role: "dataRole"
        width: treeView.width
    }
}
