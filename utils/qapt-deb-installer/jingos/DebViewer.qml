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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12
import QtQml 2.12
import QtGraphicalEffects 1.12
import jingos.debInstaller 1.0
import org.kde.kirigami 2.15
Item{
    property int  viewWidth:  572//1143*0.75
    property int  viewHeight: 540//1100*0.75
    property int   tabWidth: 507//1013*0.75
    property int   tabHeight: 258//515*0.75
    property int   iconHeight: 85
    property int   iconWidth: 84
    property int   topSpace: 49

    Rectangle {
        id:debViewDialog
        property bool btnFlag:true

        height:  viewHeight
        width: viewWidth
        //        anchors.top: parent.top
        //        anchors.topMargin: topSpace
        //        anchors.horizontalCenter: parent.horizontalCenter
        anchors.centerIn: parent

        radius: viewHeight*0.0278

        Rectangle{
            anchors.fill: parent
            color: "#000000"
            opacity: 0.1
            radius: parent.radius
        }

        Rectangle{
            id:devname
            implicitHeight: icon.height+iconName.height+statusStr.height+viewHeight*0.025
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: viewHeight*0.052
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
                height: viewHeight*0.0315//34
                anchors.top: icon.bottom
                anchors.horizontalCenter: devname.horizontalCenter
                horizontalAlignment:  Text.AlignHCenter
                color: "#000000"
                font.family :"Gilroy"
                font.pixelSize : parseInt(viewHeight*0.02)
                font.weight :Font.Medium
                text: debInstaller.strDebName
            }
            Label {
                id: statusStr
                width: viewWidth*0.7
                height: viewHeight*0.0519
                anchors.top: iconName.bottom
                anchors.topMargin: viewHeight*0.01
                anchors.horizontalCenter: devname.horizontalCenter
                color: "#99000000"
                font.family :"Gilroy"
                font.pixelSize : parseInt(tabHeight*0.032)
                wrapMode :Text.WrapAnywhere
                font.weight :Font.Normal
                horizontalAlignment:  Text.AlignHCenter
                text: debInstaller.statusText
            }
        }
        Item{
            id:frame
            anchors.top: devname.bottom
            anchors.topMargin: viewHeight*0.039
            anchors.horizontalCenter: devname.horizontalCenter
            implicitWidth: tabWidth
            implicitHeight: tabHeight
            RowLayout{
                DebInstallTab{
                    id:deb
                    visible: true
                }
            }
        }
        Rectangle{
            id:btnGroup
            anchors.top: frame.bottom
            anchors.topMargin: viewHeight*0.059
            anchors.horizontalCenter: devname.horizontalCenter
            implicitWidth: tabWidth
            implicitHeight: viewHeight*0.06

            color: "#00000000"
            Row{
                id:lay
                spacing: tabWidth*0.04//0.0106
                anchors.centerIn: parent
                /*
                Rectangle{
                    id:cancelBtn
                    color:mouseArea.containsMouse ? (mouseArea.pressed ? "#0DEFEFEF" : "#EFEFEF"):"#EFEFEF"
                    implicitHeight: btnGroup.height
                    width:tabWidth*0.4906
                    radius: viewHeight*0.013
                    Label{
                        anchors.fill: parent

                        font.family :"Gilroy"
                        font.pixelSize : parseInt(viewHeight*0.02)
                        font.weight :Font.Medium
                        horizontalAlignment:Text.AlignHCenter
                        verticalAlignment:Text.AlignVCenter
                        text: i18nd("qapt-deb-installer", "Cancel")
                        MouseArea{
                            id:mouseArea
                            anchors.fill:parent
                            hoverEnabled: true
                            onClicked: {
                                window.close()
                            }
                        }
                    }
                }
                */
                JButton{
                    id:cancelBtn
                    implicitHeight: btnGroup.height
                    width:tabWidth*0.48
                    radius: viewHeight*0.013
                    backgroundColor:"#EFEFEF"
                    font.family :"Gilroy"
                    font.pixelSize : parseInt(viewHeight*0.02)
                    font.weight :Font.Medium
                    text: i18nd("qapt-deb-installer", "Cancel")
                    onClicked:{
                        window.close()
                    }
                }
                JButton{
                    id:installBtn
                    implicitHeight: btnGroup.height
                    width:tabWidth*0.48
                    radius: viewHeight*0.013
                    backgroundColor:"#EFEFEF"
                    font.family :"Gilroy"
                    font.pixelSize : parseInt(viewHeight*0.02)
                    font.weight :Font.Medium
                    text: i18nd("qapt-deb-installer", "Install")
                    onClicked:{
                        if(debInstaller.installProcess===JdebInstaller.InstallStart){
                            debInstaller.installDebFile()
                        }
                        else if(debInstaller.installProcess===JdebInstaller.InstallError||
                                debInstaller.installProcess===JdebInstaller.InstallEnd){
                            window.close()
                        }
                    }
                }
                /*
                Rectangle{
                    id:installBtn
                    color:enabled? (mouseArea1.pressed ? "#0DEFEFEF":"#EFEFEF"):"#EFEFEF"
                    implicitHeight:btnGroup.height
                    width:tabWidth*0.4906
                    radius: viewHeight*0.013
                    Label{
                        id:installBtnLabel
                        anchors.fill: parent

                        color: enabled ? "#3C4BE8":"#DF000000"
                        opacity : enabled ? 1:0.5
                        font.family :"Gilroy"
                        font.pixelSize : parseInt(viewHeight*0.02)
                        font.weight :Font.Medium
                        horizontalAlignment:Text.AlignHCenter
                        verticalAlignment:Text.AlignVCenter
                        text: i18nd("qapt-deb-installer", "Install")
                        MouseArea{
                            id:mouseArea1
                            anchors.fill:parent
                            onClicked: {
                                if(debInstaller.installProcess===JdebInstaller.InstallStart){
                                    debInstaller.installDebFile()
                                }
                                else if(debInstaller.installProcess===JdebInstaller.InstallError||
                                        debInstaller.installProcess===JdebInstaller.InstallEnd){
                                    window.close()
                                }
                            }
                        }
                    }
                }*///Rectangel
            }
        }

        Connections{
            target:debInstaller
            function onInstallChangedProcess(){
                if(debInstaller.installProcess===JdebInstaller.Installing){
                    cancelBtn.visible=false;
                    installBtn.enabled=false
                    installBtnLabel.text=i18nd("qapt-deb-installer", "installing")
                    installBtnLabel.enabled=false
                    deb.currentIndex=4

                }
                else if(debInstaller.installProcess===JdebInstaller.InstallStart){
                    cancelBtn.visible=true;
                    installBtn.enabled=true
                    installBtnLabel.text=i18nd("qapt-deb-installer", "Install")
                    installBtnLabel.enabled=true
                }
                else if(debInstaller.installProcess===JdebInstaller.InstallEnd){
                    cancelBtn.visible=false;
                    installBtn.enabled=true
                    installBtnLabel.text=i18nd("qapt-deb-installer", "Done")
                    installBtnLabel.enabled=true
                    deb.currentIndex=4
                }
                else if(debInstaller.installProcess===JdebInstaller.InstallError){
                    cancelBtn.visible=false;
                    installBtn.enabled=tue
                    installBtnLabel.text=i18nd("qapt-deb-installer", "OK")
                    installBtnLabel.enabled=true
                    deb.currentIndex=4
                }

            }
        }//end Connections
    }
    DropShadow {
        anchors.fill: debViewDialog
        horizontalOffset: 0
        verticalOffset: viewHeight*0.01667
        radius: 8.0
        samples: 24
        color: "#1A000000"
        source: debViewDialog
    }
}
