import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15
import Qt.labs.platform 1.1 as NativeDialogs

import "../"
import "../filedetails/"

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

        Button {
            id: currentAccountButton

            Layout.preferredWidth:  Style.currentAccountButtonWidth
            Layout.preferredHeight: Style.trayWindowHeaderHeight
            display:                AbstractButton.IconOnly
            flat:                   true

            Accessible.role: Accessible.ButtonMenu
            Accessible.name: qsTr("Current account")
            Accessible.onPressAction: currentAccountButton.clicked()

            // We call open() instead of popup() because we want to position it
            // exactly below the dropdown button, not the mouse
            onClicked: {
                syncPauseButton.text = Systray.syncIsPaused ? qsTr("Resume sync for all") : qsTr("Pause sync for all")
                if (accountMenu.visible) {
                    accountMenu.close()
                } else {
                    accountMenu.open()
                }
            }

            Menu {
                id: accountMenu

                // x coordinate grows towards the right
                // y coordinate grows towards the bottom
                x: (currentAccountButton.x + 2)
                y: (currentAccountButton.y + Style.trayWindowHeaderHeight + 2)

                width: (Style.currentAccountButtonWidth - 2)
                height: Math.min(implicitHeight, maxMenuHeight)
                closePolicy: Menu.CloseOnPressOutsideParent | Menu.CloseOnEscape
                
                clip: true

                background: Rectangle {
                    border.color: Style.sesBorderColor
                    color: Style.sesWhite
                    radius: 8 //Style.currentAccountButtonRadius
                }

                contentItem: ScrollView {
                    id: accMenuScrollView
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

                    data: WheelHandler {
                        target: accMenuScrollView.contentItem
                    }
                    ListView {
                        implicitHeight: contentHeight
                        model: accountMenu.contentModel
                        interactive: true
                        clip: true
                        currentIndex: accountMenu.currentIndex
                    }
                }

                onClosed: {
                    // HACK: reload account Instantiator immediately by restting it - could be done better I guess
                    // see also onVisibleChanged above
                    userLineInstantiator.active = false;
                    userLineInstantiator.active = true;
                }

                Instantiator {
                    id: userLineInstantiator
                    model: UserModel
                    delegate: UserLine {
                        onShowUserStatusSelector: {
                            userStatusDrawer.openUserStatusDrawer(model.index);
                            accountMenu.close();
                        }
                        onClicked: UserModel.currentUserId = model.index;
                    }
                    onObjectAdded: accountMenu.insertItem(index, object)
                    onObjectRemoved: accountMenu.removeItem(object)
                }

                Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    implicitHeight: 1
                    color: Style.sesBorderColor
                }

                MenuItem {
                    id: addAccountButton
                    hoverEnabled: true
                    visible: Systray.enableAddAccount

                    icon.source: Style.sesAddAccountIcon 
                    icon.height: Style.smallIconSize
                    icon.width: Style.smallIconSize
                    icon.color: Style.sesIconColor 

                    text: qsTr("Add account")
                    font.pixelSize: Style.topLinePixelSize

                    background: Item {
                        height: parent.height
                        width: parent.menu.width
                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: 1
                            color: parent.parent.hovered || parent.parent.visualFocus ? Style.sesHover : "transparent"
                        }
                    }

                    onClicked: UserModel.addAccount()

                    Accessible.role: Accessible.MenuItem
                    Accessible.name: qsTr("Add new account")
                    Accessible.onPressAction: addAccountButton.clicked()
                }

                Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    implicitHeight: 1
                    color: Style.sesBorderColor
                }

                MenuItem {
                    id: syncPauseButton
                    font.pixelSize: Style.topLinePixelSize
                    hoverEnabled: true
                    onClicked: Systray.syncIsPaused = !Systray.syncIsPaused

                    icon.source: Style.sesPauseSyncIcon 
                    icon.height: Style.smallIconSize 
                    icon.width: Style.smallIconSize  
                    icon.color: Style.sesIconColor 

                    background: Item {
                        height: parent.height
                        width: parent.menu.width
                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: 1
                            color: parent.parent.hovered || parent.parent.visualFocus ? Style.sesHover : "transparent"
                        }
                    }

                    Accessible.role: Accessible.MenuItem
                    Accessible.name: Systray.syncIsPaused ? qsTr("Resume sync for all") : qsTr("Pause sync for all")
                    Accessible.onPressAction: syncPauseButton.clicked()
                }

                MenuItem {
                    id: settingsButton
                    text: qsTr("Settings")
                    font.pixelSize: Style.topLinePixelSize
                    hoverEnabled: true
                    onClicked: Systray.openSettings()

                    icon.source: Style.sesSettingsIcon 
                    icon.height: Style.smallIconSize
                    icon.width: Style.smallIconSize 
                    icon.color: Style.sesIconColor 

                    background: Item {
                        height: parent.height
                        width: parent.menu.width
                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: 1
                            color: parent.parent.hovered || parent.parent.visualFocus ? Style.sesHover : "transparent"
                        }
                    }

                    Accessible.role: Accessible.MenuItem
                    Accessible.name: text
                    Accessible.onPressAction: settingsButton.clicked()
                }

                MenuItem {
                    id: exitButton
                    text: qsTr("Exit");
                    font.pixelSize: Style.topLinePixelSize
                    hoverEnabled: true
                    onClicked: Systray.shutdown()

                    icon.source: Style.sesQuitIcon 
                    icon.height: Style.smallIconSize 
                    icon.width: Style.smallIconSize 
                    icon.color: Style.sesIconColor 

                    background: Item {
                        height: parent.height
                        width: parent.menu.width
                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: 1
                            color: parent.parent.hovered || parent.parent.visualFocus ? Style.sesHover : "transparent"
                        }
                    }

                    Accessible.role: Accessible.MenuItem
                    Accessible.name: text
                    Accessible.onPressAction: exitButton.clicked()
                }
            }

            background: Rectangle {
                color: parent.hovered || parent.visualFocus ? Style.currentUserHeaderTextColor : "transparent"
                opacity: 0.2
            }

            RowLayout {
                id: accountControlRowLayout

                height: Style.trayWindowHeaderHeight
                width:  Style.currentAccountButtonWidth
                spacing: 0

                Image {
                    id: currentAccountAvatar

                    Layout.leftMargin: Style.trayHorizontalMargin
                    verticalAlignment: Qt.AlignCenter
                    cache: false
                    source: UserModel.currentUser.avatar != "" ? UserModel.currentUser.avatar : "image://avatars/fallbackWhite"
                    Layout.preferredHeight: Style.accountAvatarSize
                    Layout.preferredWidth: Style.accountAvatarSize

                    Accessible.role: Accessible.Graphic
                    Accessible.name: qsTr("Current account avatar")

                    Rectangle {
                        id: currentAccountStatusIndicatorBackground
                        visible: UserModel.currentUser.isConnected
                                    && UserModel.currentUser.serverHasUserStatus
                        width: Style.accountAvatarStateIndicatorSize +  + Style.trayFolderStatusIndicatorSizeOffset
                        height: width
                        anchors.bottom: currentAccountAvatar.bottom
                        anchors.right: currentAccountAvatar.right
                        color: Style.currentUserHeaderColor
                        radius: width * Style.trayFolderStatusIndicatorRadiusFactor
                    }

                    Rectangle {
                        id: currentAccountStatusIndicatorMouseHover
                        visible: UserModel.currentUser.isConnected
                                    && UserModel.currentUser.serverHasUserStatus
                        width: Style.accountAvatarStateIndicatorSize +  + Style.trayFolderStatusIndicatorSizeOffset
                        height: width
                        anchors.bottom: currentAccountAvatar.bottom
                        anchors.right: currentAccountAvatar.right
                        color: currentAccountButton.hovered ? Style.sesHover : "transparent"
                        opacity: Style.trayFolderStatusIndicatorMouseHoverOpacityFactor
                        radius: width * Style.trayFolderStatusIndicatorRadiusFactor
                    }

                    Image {
                        id: currentAccountStatusIndicator
                        visible: UserModel.currentUser.isConnected
                                    && UserModel.currentUser.serverHasUserStatus
                        source: UserModel.currentUser.statusIcon
                        cache: false
                        x: currentAccountStatusIndicatorBackground.x + 1
                        y: currentAccountStatusIndicatorBackground.y + 1
                        sourceSize.width: Style.accountAvatarStateIndicatorSize
                        sourceSize.height: Style.accountAvatarStateIndicatorSize

                        Accessible.role: Accessible.Indicator
                        Accessible.name: UserModel.desktopNotificationsAllowed ? qsTr("Current account status is online") : qsTr("Current account status is do not disturb")
                    }
                }

                Column {
                    id: accountLabels
                    spacing: 0
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    Layout.leftMargin: Style.userStatusSpacing
                    Layout.fillWidth: true
                    Layout.maximumWidth: parent.width

                    EnforcedPlainTextLabel {
                        id: currentAccountUser
                        Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                        width: Style.currentAccountLabelWidth
                        text: UserModel.currentUser.name
                        elide: Text.ElideRight
                        color: Style.currentUserHeaderTextColor

                        font.pixelSize: Style.topLinePixelSize
                        font.bold: true
                    }

                    EnforcedPlainTextLabel {
                        id: currentAccountServer
                        Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                        width: Style.currentAccountLabelWidth
                        text: UserModel.currentUser.server
                        elide: Text.ElideRight
                        color: Style.currentUserHeaderTextColor
                        visible: UserModel.numUsers() > 1
                    }

                    RowLayout {
                        id: currentUserStatus
                        visible: UserModel.currentUser.isConnected &&
                                    UserModel.currentUser.serverHasUserStatus
                        spacing: Style.accountLabelsSpacing
                        width: parent.width

                        EnforcedPlainTextLabel {
                            id: emoji
                            visible: UserModel.currentUser.statusEmoji !== ""
                            width: Style.userStatusEmojiSize
                            text: UserModel.currentUser.statusEmoji
                        }
                        EnforcedPlainTextLabel {
                            id: message
                            Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                            Layout.fillWidth: true
                            visible: UserModel.currentUser.statusMessage !== ""
                            width: Style.currentAccountLabelWidth
                            text: UserModel.currentUser.statusMessage !== ""
                                    ? UserModel.currentUser.statusMessage
                                    : UserModel.currentUser.server
                            elide: Text.ElideRight
                            color: Style.currentUserHeaderTextColor
                            font.pixelSize: Style.subLinePixelSize
                        }
                    }
                }

                ColorOverlay {
                    cached: true
                    color: Style.currentUserHeaderTextColor
                    width: source.width
                    height: source.height
                    source: Image {
                        Layout.alignment: Qt.AlignRight
                        verticalAlignment: Qt.AlignCenter
                        Layout.margins: Style.accountDropDownCaretMargin
                        source: "qrc:///client/theme/white/caret-down.svg"
                        sourceSize.width: Style.accountDropDownCaretSize
                        sourceSize.height: Style.accountDropDownCaretSize
                        Accessible.role: Accessible.PopupMenu
                        Accessible.name: qsTr("Account switcher and settings menu")
                    }
                }
            }
        }

        HeaderButton {
            id: trayWindowWebsiteButton

            icon.source: Style.sesWebsiteIcon
            icon.color: Style.sesIconColor 
            onClicked: UserModel.openCurrentAccountServer()

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

        HeaderButton {
            id: trayWindowTalkButton

            visible: false //SES-4 removed  
            icon.source: "qrc:///client/theme/white/talk-app.svg"
            icon.color: Style.currentUserHeaderTextColor
            onClicked: UserModel.openCurrentAccountTalk()

            Accessible.role: Accessible.Button
            Accessible.name: qsTr("Open Nextcloud Talk in browser")
            Accessible.onPressAction: trayWindowTalkButton.clicked()

            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth:  Style.trayWindowHeaderHeight
            Layout.preferredHeight: Style.trayWindowHeaderHeight

        }

        HeaderButton {
            id: trayWindowAppsButton
            icon.source: "qrc:///client/theme/white/more-apps.svg"
            icon.color: Style.currentUserHeaderTextColor
            
            visible: false //SES-4 removed

            onClicked: {
                if(appsMenuListView.count <= 0) {
                    UserModel.openCurrentAccountServer()
                } else if (appsMenu.visible) {
                    appsMenu.close()
                } else {
                    appsMenu.open()
                }
            }

            Accessible.role: Accessible.ButtonMenu
            Accessible.name: qsTr("More apps")
            Accessible.onPressAction: trayWindowAppsButton.clicked()

            Menu {
                id: appsMenu
                x: Style.trayWindowMenuOffsetX
                y: (trayWindowAppsButton.y + trayWindowAppsButton.height + Style.trayWindowMenuOffsetY)
                width: Style.trayWindowWidth * Style.trayWindowMenuWidthFactor
                height: implicitHeight + y > Style.trayWindowHeight ? Style.trayWindowHeight - y : implicitHeight
                closePolicy: Menu.CloseOnPressOutsideParent | Menu.CloseOnEscape

                background: Rectangle {
                    border.color: Style.sesBorderColor
                    color: Style.sesWhite
                    radius: 2
                }

                contentItem: ScrollView {
                    id: appsMenuScrollView
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

                    data: WheelHandler {
                        target: appsMenuScrollView.contentItem
                    }
                    ListView {
                        id: appsMenuListView
                        implicitHeight: contentHeight
                        model: UserAppsModel
                        interactive: true
                        clip: true
                        currentIndex: appsMenu.currentIndex
                        delegate: MenuItem {
                            id: appEntry
                            anchors.left: parent.left
                            anchors.right: parent.right

                            text: model.appName
                            font.pixelSize: Style.topLinePixelSize
                            icon.source: model.appIconUrl
                            icon.color: Style.ncTextColor
                            onTriggered: UserAppsModel.openAppUrl(appUrl)
                            hoverEnabled: true

                            background: Item {
                                height: parent.height
                                width: parent.width
                                Rectangle {
                                    anchors.fill: parent
                                    anchors.margins: 1
                                    color: parent.parent.hovered || parent.parent.visualFocus ? Style.sesHover : "transparent"
                                }
                            }

                            Accessible.role: Accessible.MenuItem
                            Accessible.name: qsTr("Open %1 in browser").arg(model.appName)
                            Accessible.onPressAction: appEntry.triggered()
                        }
                    }
                }
            }
        }
    }
}   // Rectangle trayWindowHeaderBackground
