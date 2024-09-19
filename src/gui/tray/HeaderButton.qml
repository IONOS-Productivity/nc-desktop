/*
 * Copyright (C) 2020 by Nicolas Fella <nicolas.fella@gmx.de>
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

import QtQml 2.15
import QtQml.Models 2.15
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// Custom qml modules are in /theme (and included by resources.qrc)
import Style 1.0
import com.nextcloud.desktopclient 1.0

Button {
    id: root

    display: AbstractButton.TextUnderIcon
    flat: true
    hoverEnabled: Style.hoverEffectsEnabled

    icon.width: Style.headerButtonIconSize
    icon.height: Style.headerButtonIconSize

    Layout.alignment: Qt.AlignRight
    Layout.preferredWidth:  Style.trayWindowHeaderHeight
    Layout.preferredHeight: Style.trayWindowHeaderHeight

    background: Rectangle {
        color: root.hovered || root.visualFocus ? Style.sesHover : "transparent"
        radius: Style.sesCornerRadius
    }

    contentItem: Item {
        id: rootContent

        Image {
            id: buttonIcon
            anchors.horizontalCenter: rootContent.horizontalCenter
            anchors.top: rootContent.top
            anchors.topMargin: 10

            property int imageWidth: root.icon.width
            property int imageHeight: root.icon.height
            cache: true

            source: root.icon.source
            sourceSize {
                width: imageWidth
                height: imageHeight
            }

            width: imageWidth
            height: imageHeight

            anchors.verticalCenter: parent
        }

        Text {
            anchors.horizontalCenter: buttonIcon.horizontalCenter
            anchors.top: buttonIcon.bottom
            anchors.topMargin: 2
            font.family: root.font.family
            font.pointSize: Style.sesFontPointSize
            text: root.text
        }
    }
}
