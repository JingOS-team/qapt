/*
 * Copyright (C) 2021 Beijing Jingling Information System Technology Co., Ltd. All rights reserved.
 * 
 * Authors: 
 * Jiashu Yu <yujiashu@jingos.com>
 *
 */

import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import org.kde.kirigami 2.15
import jingos.display 1.0

TabView {
    id: frame

    implicitWidth: tabWidth
    implicitHeight: tabHeight

    Tab {
        title: i18nd("qapt-deb-installer", "Description")
        InstallText {
            text:debInstaller.descriPtionText
        }
    }

    Tab {
        title: i18nd("qapt-deb-installer", "Details")
        InstallText {
            text:debInstaller.detailsText
        }
    }

    Tab {
        title: i18nd("qapt-deb-installer", "Included Files")
        Item {
            Rectangle {
                width: viewHeight/5
                height: viewHeight/5
                visible: debInstaller.showInclude
                anchors.centerIn: parent

                AnimatedImage {
                    source: "file:///usr/share/icons/jing/SwiMachine/load.gif"
                }
                color: "transparent"
            }

            InstallText {
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
                anchors.topMargin: 10
                anchors.left: parent.left
                width: parent.width
                height: tabHeight * 0.116
                text: debInstaller.treeModel.rowCount? i18nd("qapt-deb-installer", "This action no requires changes to other packages.") :
                                                       i18nd("qapt-deb-installer","This action requires changes to other packages: ")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.weight: Font.Normal
                font.family: "Gilroy"
                font.pixelSize: JDisplay.sp(8)
            }

            Rectangle {
                anchors.top: lables.bottom
                anchors.left: parent.left
                width: parent.width
                anchors.bottom: parent.bottom
                color: "transparent"
                InstallTreeView {
                    anchors.fill: parent
                }
            }
        }
    }

    Tab {

        title: i18nd("qapt-deb-installer", "Progress")
        InstallText {
            text:debInstaller.progressText
        }
    }
    style: TabViewStyle {
        id:style
        frameOverlap: -5
        tab: Rectangle {
            id:tabBarBtn
            color: JTheme.colorScheme==="jingosLight" ? (styleData.selected ? "#F2FFFFFF" :"#00767680") :
                                                        (styleData.selected ? "#338E8E93" :"#00767680")
            implicitWidth: text.implicitWidth + (tabWidth - textlen.width)/5
            implicitHeight: tabHeight * 0.136 - tabHeight * 0.136/7
            radius: tabHeight * 0.0388
            Text {
                id: text
                anchors.centerIn: parent
                text: styleData.title
                color: JTheme.colorScheme === "jingosLight" ? (styleData.selected ? "#000000" : "#848488") :
                                                            (styleData.selected ? "#FFF7F7F7" : "#8CF7F7F7")
                font.pixelSize: JDisplay.sp(9)
                font.weight :Font.Medium
                font.family :"Gilroy"
            }
        }

        tabBar: Item {
            id: barItem
            Rectangle {
                anchors.centerIn: barItem
                implicitHeight: tabHeight*0.136
                implicitWidth: tabWidth
                color: JTheme.colorScheme === "jingosLight" ? "#1f767680" : "#4D303030"
                radius: tabHeight * 0.0388
            }
        }
        tabsAlignment:Qt.AlignHCenter
        frame: Rectangle {
            id:framearea
            color: "transparent"
        }
    }

    Text {
        id: textlen
        color: "#00000000"
        font.pixelSize: JDisplay.sp(9)
        font.weight: Font.Medium
        font.family: "Gilroy"
        text: i18nd("qapt-deb-installer", "DescriptionDetailsIncluded FilesAddtional ChangesProgress")
    }
}
