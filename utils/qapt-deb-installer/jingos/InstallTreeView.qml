/*
 * Copyright (C) 2021 Beijing Jingling Information System Technology Co., Ltd. All rights reserved.
 * 
 * Authors: 
 * Jiashu Yu <yujiashu@jingos.com>
 *
 */

import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQml 2.12
import QtQuick.Controls 2.5
import jingos.display 1.0
//it items just tow level,if more it will look bad

TreeView {
    id:treeView
    property int iconWidth: JDisplay.dp(20)

    anchors.centerIn: parent
    model: debInstaller.treeModel

    Rectangle {
        anchors.fill: parent
        color: "transparent"
    }

    style:TreeViewStyle {
        frame:Rectangle {
            color: "transparent"
        }

        backgroundColor: "transparent"

        headerDelegate:Rectangle {
            visible: false
        }

        rowDelegate : Rectangle {
            height:  JDisplay.dp(30)
            color: "transparent"
        }

        itemDelegate: Rectangle {
            id:itemArea
            anchors.left: parent.left

            Rectangle {
                height: styleData.hasSibling ? parent.height : parent.height / 2
                visible: !styleData.hasChildren
                width: 1
                color: "#767680"
                anchors.left: parent.left
                anchors.leftMargin: iconWidth/2
            }

            Rectangle {
                id:line1
                height: 1
                width: JDisplay.dp(20)
                visible: !styleData.hasChildren
                color: "#767680"
                anchors.left: parent.left
                anchors.leftMargin: iconWidth/2
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id:textArea
                anchors.left: parent.left
                anchors.leftMargin: styleData.hasChildren ? 0 : iconWidth+line1.width + iconWidth/2 + 2
                anchors.verticalCenter: parent.verticalCenter
                text: styleData.value === undefined ? "" : styleData.value
                font.pixelSize: JDisplay.dp(11)
                font.family :"Gilroy"
                font.weight :Font.Normal
                horizontalAlignment:Text.AlignLeft
            }

            Rectangle {
                visible: styleData.isExpanded ? styleData.hasChildren : false
                anchors.top: textArea.bottom
                anchors.bottom: parent.bottom
                width: 1
                color: "#767680"
                anchors.left: parent.left
                anchors.leftMargin: iconWidth/2
            }

            Image {
                id: nodeTextImage
                anchors.left:styleData.hasChildren ? parent.left : line1.right
                anchors.verticalCenter: textArea.verticalCenter
                width: iconWidth
                height: iconWidth
                visible: !styleData.hasChildren
                source: "file:///usr/share/icons/jing/SwiMachine/broom.svg"
            }
        }
    }
    Component.onCompleted: {
        for(var i = 0 ;i < debInstaller.treeModel.rowCount(); i++){
            treeView.expand(debInstaller.treeModel.index(i, 0));
        }
    }
    TableViewColumn {
        role: "dataRole"
        width: treeView.width
    }
}
