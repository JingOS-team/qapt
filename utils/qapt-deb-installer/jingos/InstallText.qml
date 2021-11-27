/*
 * Copyright (C) 2021 Beijing Jingling Information System Technology Co., Ltd. All rights reserved.
 * 
 * Authors: 
 * Jiashu Yu <yujiashu@jingos.com>
 *
 */

import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12
import QtQml 2.12
import jingos.display 1.0

ScrollView {
    id: textView
    property alias text: textContent.text

    anchors.centerIn: parent
    ScrollBar.vertical.policy: ScrollBar.AlwaysOn
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    clip: true
    Flickable {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: textContent.contentHeight
        TextArea {
            id: textContent
            anchors.topMargin: tabHeight * 0.118
            readOnly : true
            width: parent.width
            wrapMode: TextEdit.NoWrap
            font.pixelSize: JDisplay.sp(11)
            selectByMouse: false
            background:Rectangle {
                color: "transparent"
            }
        }
    }

    background: Rectangle {
        color: "transparent"
    }
}
