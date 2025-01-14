/*
 * Copyright (C) by Claudio Cambra <claudio.cambra@nextcloud.com>
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
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import Style 1.0

RowLayout {
    id: root

    property bool hovered: false
    property string imageSourceHover: ""
    property string imageSource: ""
    property int imageSourceWidth: undefined
    property int imageSourceHeight: undefined
    property string text: ""
    property var display

    property color textColor: palette.buttonText
    property color textColorHovered: textColor
    property alias font: buttonLabel.font

    height: parent.height

    EnforcedPlainTextLabel {
        id: buttonLabel

        Layout.alignment: Qt.AlignVCenter

        text: root.text

        visible: root.text !== ""

        color: root.hovered ? root.textColorHovered : root.textColor

        elide: Text.ElideRight
    }

    Image {
        id: icon

        Layout.alignment: Qt.AlignVCenter

        source: root.hovered ? root.imageSourceHover : root.imageSource

        sourceSize {
            width: root.imageSourceWidth
            height: root.imageSourceHeight
        }

        fillMode: Image.PreserveAspectFit
        visible: root.display === Button.TextOnly ? false : root.hovered ? root.imageSourceHover !== "" : root.imageSource !== ""

        layer.enabled: true
        layer.effect: ColorOverlay {
            color: Style.sesWhite
        }
    }
}