/*
 * Copyright (C) 2021 Beijing Jingling Information System Technology Co., Ltd. All rights reserved.
 * 
 * Authors: 
 * Jiashu Yu <yujiashu@jingos.com>
 *
 */

import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12
import QtQml 2.12
import QtGraphicalEffects 1.12
import jingos.debInstaller 1.0
import org.kde.kirigami 2.15
import jingos.display 1.0

Item{
    property int  viewWidth:  JDisplay.dp(572)//1143*0.75
    property int  viewHeight: JDisplay.dp(540)//1100*0.75
    property int   tabWidth: JDisplay.dp(507)//1013*0.75
    property int   tabHeight: JDisplay.dp(258)//515*0.75
    property int   iconHeight: JDisplay.dp(85)
    property int   iconWidth: JDisplay.dp(84)
    property int   topSpace: JDisplay.dp(49)

    Rectangle {
        id:debViewDialog
        property bool btnFlag:true

        height: viewHeight
        width: viewWidth
        anchors.centerIn: parent

        radius: viewHeight * 0.0278

        color: JTheme.colorScheme === "jingosLight" ? "white" : "#E626262A"

        Rectangle {
            anchors.fill: parent
            color: "#000000"
            opacity: 0.1
            radius: parent.radius
        }

        Rectangle {
            id:devname
            implicitHeight: icon.height + iconName.height + statusStr.height + viewHeight * 0.025
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: viewHeight * 0.052
            Image {
                id: icon
                width: iconWidth
                height: iconHeight
                anchors.top: devname.top
                anchors.horizontalCenter: devname.horizontalCenter

                source: "file://"+debInstaller.strIcon
            }

            Label {
                id: iconName
                height: viewHeight * 0.0315//34
                anchors.top: icon.bottom
                anchors.horizontalCenter: devname.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                color: JTheme.majorForeground//"#000000"
                font.family: "Gilroy"
                font.pixelSize: JDisplay.sp(11)
                font.weight: Font.Medium
                text: debInstaller.strDebName
            }
            Label {
                id: statusStr
                width: viewWidth * 0.7
                height: viewHeight * 0.0519
                anchors.top: iconName.bottom
                anchors.topMargin: viewHeight * 0.01
                anchors.horizontalCenter: devname.horizontalCenter
                color: JTheme.colorScheme === "jingosLight" ? "#99000000" : JTheme.majorForeground//
                font.family :"Gilroy"
                font.pixelSize : JDisplay.sp(8)
                wrapMode :Text.WrapAnywhere
                font.weight :Font.Normal
                horizontalAlignment:  Text.AlignHCenter
                text: debInstaller.statusText
            }
        }

        Item {
            id:frame
            anchors.top: devname.bottom
            anchors.topMargin: viewHeight * 0.039
            anchors.horizontalCenter: devname.horizontalCenter
            implicitWidth: tabWidth
            implicitHeight: tabHeight
            RowLayout {
                DebInstallTab {
                    id:deb
                    visible: true
                }
            }
        }
        Rectangle {
            id:btnGroup
            anchors.top: frame.bottom
            anchors.topMargin: viewHeight * 0.059
            anchors.horizontalCenter: devname.horizontalCenter
            implicitWidth: tabWidth
            implicitHeight: viewHeight * 0.06
            color: "#00000000"

            Row {
                id:lay
                spacing: tabWidth * 0.04//0.0106
                anchors.centerIn: parent

                JButton {
                    id:cancelBtn
                    implicitHeight: btnGroup.height
                    width:tabWidth*0.48
                    radius: viewHeight * 0.013
                    backgroundColor: JTheme.colorScheme === "jingosLight" ? "#EFEFEF" : "E6444449"
                    fontColor: JTheme.buttonForeground
                    font.family :"Gilroy"
                    font.pixelSize : JDisplay.sp(11)
                    font.weight :Font.Medium
                    text: i18nd("qapt-deb-installer", "Cancel")
                    onClicked:{
                        window.close()
                    }
                }

                JButton {
                    id:installBtn
                    implicitHeight: btnGroup.height
                    width: tabWidth * 0.48
                    radius: viewHeight * 0.013
                    backgroundColor: JTheme.colorScheme === "jingosLight" ? "#EFEFEF" : "E6444449"
                    font.family: "Gilroy"
                    font.pixelSize : JDisplay.sp(11)
                    font.weight :Font.Medium
                    fontColor: enabled ? "#3C4BE8" : (JTheme.colorScheme === "jingosLight" ?"4DF7F7F7" : "#DF000000")
                    text: i18nd("qapt-deb-installer", "Install")
                    onClicked:{
                        if(debInstaller.installProcess === JdebInstaller.InstallStart) {
                            debInstaller.installDebFile()
                        } else if(debInstaller.installProcess === JdebInstaller.InstallError ||
                                debInstaller.installProcess === JdebInstaller.InstallEnd) {
                            window.close()
                        }
                    }
                }
            }
        }

        Connections {
            target:debInstaller
            function onInstallChangedProcess() {
                if(debInstaller.installProcess === JdebInstaller.Installing) {
                    cancelBtn.visible = false;
                    installBtn.enabled = false
                    installBtn.text = i18nd("qapt-deb-installer", "installing")
                    installBtn.enabled = false
                    deb.currentIndex = 4
                    console.log("debInstaller.installProcess===JdebInstaller.Installing")

                } else if(debInstaller.installProcess === JdebInstaller.InstallStart) {
                    cancelBtn.visible = true;
                    installBtn.enabled = true
                    installBtn.text = i18nd("qapt-deb-installer", "Install")
                    installBtn.enabled = true
                    console.log("debInstaller.installProcess===JdebInstaller.InstallStart")

                } else if(debInstaller.installProcess === JdebInstaller.InstallEnd) {
                    cancelBtn.visible = false;
                    installBtn.enabled = true
                    installBtn.text = i18nd("qapt-deb-installer", "Done")
                    installBtn.enabled = true
                    deb.currentIndex = 4
                    console.log("debInstaller.installProcess===JdebInstaller.InstallEnd")
                } else if(debInstaller.installProcess === JdebInstaller.InstallError) {
                    cancelBtn.visible = false;
                    installBtn.enabled = true
                    installBtn.text = i18nd("qapt-deb-installer", "OK")
                    installBtn.enabled = true
                    deb.currentIndex = 4
                    console.log("debInstaller.installProcess===JdebInstaller.InstallError")
                }

            }
        }//end Connections
    }
    DropShadow {
        anchors.fill: debViewDialog
        horizontalOffset: 0
        verticalOffset: JDisplay.dp(2)
        radius: 12.0
        samples: 16
        color: Qt.rgba(0, 0, 0, 0.1)
        source: debViewDialog
    }
}
