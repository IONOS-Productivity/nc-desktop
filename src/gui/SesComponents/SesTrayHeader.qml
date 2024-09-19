import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15
import Qt.labs.platform 1.1 as NativeDialogs

import "../"
import "../filedetails/"
import "../tray/"

// Custom qml modules are in /theme (and included by resources.qrc)
import Style 1.0

import com.nextcloud.desktopclient 1.0


Rectangle {

    height:         Style.trayWindowHeaderHeight
    color:          Style.sesWhite
    radius: 0.0

    RowLayout {
        id: trayWindowHeaderLayout

        anchors.fill:   parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        TrayWindowAccountMenu{
            Layout.preferredWidth:  Style.currentAccountButtonWidth
            Layout.preferredHeight: Style.trayWindowHeaderHeight
        }

        HeaderButton {
            id: trayWindowWebsiteButton

            icon.source: Style.sesWebsiteIcon
            icon.color: Style.sesIconColor
            onClicked: UserModel.openCurrentAccountServer()

            text: qsTr("Website")

            Accessible.role: Accessible.Button
            Accessible.name: qsTr("Open Nextcloud in browser")
            Accessible.onPressAction: trayWindowWebsiteButton.clicked()

            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth:  Style.trayWindowHeaderHeight
            Layout.preferredHeight: Style.trayWindowHeaderHeight
        }

        TrayFoldersMenuButton {
            id: openLocalFolderButton

            visible: currentUser.hasLocalFolder
            currentUser: UserModel.currentUser


            onClicked: openLocalFolderButton.userHasGroupFolders ? openLocalFolderButton.toggleMenuOpen() : UserModel.openCurrentAccountLocalFolder()

            onFolderEntryTriggered: isGroupFolder ? UserModel.openCurrentAccountFolderFromTrayInfo(fullFolderPath) : UserModel.openCurrentAccountLocalFolder()

            Accessible.role: Accessible.Graphic
            Accessible.name: qsTr("Open local or group folders")
            Accessible.onPressAction: openLocalFolderButton.userHasGroupFolders ? openLocalFolderButton.toggleMenuOpen() : UserModel.openCurrentAccountLocalFolder()

            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth:  Style.trayWindowHeaderHeight
            Layout.preferredHeight: Style.trayWindowHeaderHeight
        }
    }
}   // Rectangle trayWindowHeaderBackground
