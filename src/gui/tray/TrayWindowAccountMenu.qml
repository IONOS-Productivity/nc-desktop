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

Button {
    id: currentAccountButton


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
            radius: Style.sesCornerRadius
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

        AccountMenuItem{
            id: addAccountButton
            icon.source: Style.sesAccountPlus 
            text: qsTr("Add account")
            onClicked: UserModel.addAccount()
            visible: Systray.enableAddAccount
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            implicitHeight: 1
            color: Style.sesBorderColor
        }

        AccountMenuItem{
            id: syncPauseButton
            onClicked: Systray.syncIsPaused = !Systray.syncIsPaused
            icon.source: Systray.syncIsPaused ? Style.sesAccountResume : Style.sesAccountPause 
        }

        AccountMenuItem{
            id: settingsButton
            text: qsTr("Settings")
            onClicked: Systray.openSettings()
            icon.source: Style.sesAccountSettings 
        }

        AccountMenuItem{
            id: exitButton
            text: qsTr("Exit")
            onClicked: Systray.shutdown()
            icon.source: Style.sesAccountQuit 
        } 
    }

    background: Rectangle {
        color: parent.hovered || parent.visualFocus ? Style.sesHover : "transparent"
        radius: Style.sesCornerRadius
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
            source: Style.sesAvatar
            Accessible.role: Accessible.Graphic
            Accessible.name: qsTr("Current account avatar")
            
            Rectangle {
                id: currentAccountStatusIndicatorBackground
                // SES-50 Remove Inidcator
                // visible: UserModel.currentUser.isConnected
                //             && UserModel.currentUser.serverHasUserStatus
                visible: false
                width: Style.accountAvatarStateIndicatorSize +  + Style.trayFolderStatusIndicatorSizeOffset
                height: width
                anchors.bottom: currentAccountAvatar.bottom
                anchors.right: currentAccountAvatar.right
                anchors.bottomMargin: -5
                anchors.rightMargin: -5
                color: Style.currentUserHeaderColor
                radius: width * Style.trayFolderStatusIndicatorRadiusFactor
            }

            Rectangle {
                id: currentAccountStatusIndicatorMouseHover
                // SES-50 Remove Inidcator
                // visible: UserModel.currentUser.isConnected
                //             && UserModel.currentUser.serverHasUserStatus
                visible: false
                width: Style.accountAvatarStateIndicatorSize +  + Style.trayFolderStatusIndicatorSizeOffset
                height: width
                anchors.bottom: currentAccountAvatar.bottom
                anchors.right: currentAccountAvatar.right
                anchors.bottomMargin: -5
                anchors.rightMargin: -5
                color: currentAccountButton.hovered ? Style.sesHover : "transparent"
                opacity: Style.trayFolderStatusIndicatorMouseHoverOpacityFactor
                radius: width * Style.trayFolderStatusIndicatorRadiusFactor
            }

            Image {
                id: currentAccountStatusIndicator
                // SES-50 Remove Inidcator
                // visible: UserModel.currentUser.isConnected
                //             && UserModel.currentUser.serverHasUserStatus
                visible: false
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