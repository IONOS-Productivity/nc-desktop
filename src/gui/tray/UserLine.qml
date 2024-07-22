/*
 * Copyright (C) 2019 by Dominique Fuchs <32204802+DominiqueFuchs@users.noreply.github.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// Custom qml modules are in /theme (and included by resources.qrc)
import Style 1.0
import com.nextcloud.desktopclient 1.0

AbstractButton {
    id: userLine

    property bool isHovered: userLine.hovered || userLine.visualFocus
    property bool isActive: userLine.pressed 

    signal showUserStatusSelector(int id)

    property variant dialog;
    property variant comp;

    Accessible.role: Accessible.MenuItem
    Accessible.name: qsTr("Switch to account") + " " + model.name

    height: Style.sesAccountMenuHeight

    background: Rectangle {
        radius: Style.sesCornerRadius
        anchors.fill: parent
        anchors.margins: 1
        color: userLine.isActive ? Style.sesButtonPressed : userLine.isHovered &&
               !userMoreButton.isHovered ?
                   Style.sesAccountMenuHover : "transparent"
    }

    contentItem: RowLayout {
        id: userLineLayout
        spacing: Style.userStatusSpacing

        Image {
            id: accountAvatar
            Layout.leftMargin: 7
            verticalAlignment: Qt.AlignCenter
            cache: false
            source: Style.sesAccountIconBig            
        }

        ColumnLayout {
            id: accountLabels
            Layout.leftMargin: Style.accountLabelsSpacing
            Layout.fillWidth: true
            Layout.fillHeight: true

            EnforcedPlainTextLabel {
                id: accountUser
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                verticalAlignment: Text.AlignBottom
                text: name
                elide: Text.ElideRight
                font.pixelSize: Style.topLinePixelSize
            }

            RowLayout {
                id: statusLayout
                Layout.fillWidth: true
                height: visible ? implicitHeight : 0
                visible: model.isConnected &&
                         model.serverHasUserStatus &&
                         (model.statusEmoji !== "" || model.statusMessage !== "")

                EnforcedPlainTextLabel {
                    id: emoji
                    visible: model.statusEmoji !== ""
                    text: statusEmoji
                    topPadding: -Style.accountLabelsSpacing
                }

                EnforcedPlainTextLabel {
                    id: message
                    Layout.fillWidth: true
                    visible: model.statusMessage !== ""
                    text: statusMessage
                    elide: Text.ElideRight
                    font.pixelSize: Style.subLinePixelSize
                    leftPadding: Style.accountLabelsSpacing
                }
            }
        }

        Button {
            id: userMoreButton
            Layout.preferredWidth: Style.headerButtonIconSize
            Layout.preferredHeight: Layout.preferredWidth
            flat: true

            property bool isHovered: userMoreButton.hovered || userMoreButton.visualFocus
            property bool isActive: userMoreButton.pressed || userMoreButtonMenu.visible

            icon.source: "qrc:///client/theme/more.svg"
            icon.color: userMoreButton.isActive || userMoreButton.isHovered ? Style.sesWhite : Style.sesIconColor

            Accessible.role: Accessible.ButtonMenu
            Accessible.name: qsTr("Account actions")
            Accessible.onPressAction: userMoreButtonMouseArea.clicked()

            onClicked: userMoreButtonMenu.visible ? userMoreButtonMenu.close() : userMoreButtonMenu.popup()
            background: Rectangle {
                anchors.fill: parent
                anchors.margins: 1
                color: userMoreButton.isActive ? Style.sesActionPressed : userMoreButton.isHovered ? Style.sesActionHover : "transparent"
                radius: width / 2
            }

            AutoSizingMenu {
                id: userMoreButtonMenu
                closePolicy: Menu.CloseOnPressOutsideParent | Menu.CloseOnEscape

                MenuItem {
                    visible: model.isConnected && model.serverHasUserStatus
                    height: visible ? implicitHeight : 0
                    text: qsTr("Set status")
                    font.pixelSize: Style.topLinePixelSize
                    palette.windowText: Style.ncTextColor
                    hoverEnabled: true
                    onClicked: showUserStatusSelector(index)
                }

                MenuItem {
                    text: model.isConnected ? qsTr("Log out") : qsTr("Log in")
                    font.pixelSize: Style.topLinePixelSize
                    palette.windowText: Style.ncTextColor
                    hoverEnabled: true
                    onClicked: {
                        model.isConnected ? UserModel.logout(index) : UserModel.login(index)
                        accountMenu.close()
                    }

                    background: Item {
                        height: parent.height
                        width: parent.menu.width
                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: 1
                            color: parent.parent.hovered ? Style.sesHover : "transparent"
                        }
                    }

                    Accessible.role: Accessible.Button
                    Accessible.name: model.isConnected ? qsTr("Log out") : qsTr("Log in")

                    onPressed: {
                        if (model.isConnected) {
                            UserModel.logout(index)
                        } else {
                            UserModel.login(index)
                        }
                        accountMenu.close()
                    }
                }

                MenuItem {
                    id: removeAccountButton
                    text: qsTr("Remove account")
                    font.pixelSize: Style.topLinePixelSize
                    palette.windowText: Style.ncTextColor
                    hoverEnabled: true
                    onClicked: {
                        UserModel.removeAccount(index)
                        accountMenu.close()
                    }

                    background: Item {
                        height: parent.height
                        width: parent.menu.width
                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: 1
                            color: parent.parent.hovered ? Style.sesHover : "transparent"
                        }
                    }

                    Accessible.role: Accessible.Button
                    Accessible.name: text
                    Accessible.onPressAction: removeAccountButton.clicked()
                }
            }
        }
    }
}   // MenuItem userLine