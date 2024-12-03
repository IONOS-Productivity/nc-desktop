/*
 * Copyright (C) 2022 by Oleksandr Zolotov <alex@nextcloud.com>
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
import Style 1.0

Button {
    id: root

    property string imageSourceHover: root.icon.source
    property var iconItem: icon

    property string toolTipText: ""

    property color textColor: palette.buttonText
    property color textColorHovered: textColor

    property alias contentsFont: root.font

    property alias bgColor: bgRectangle.color
    property alias bgNormalColor: bgRectangle.normalColor
    property alias bgHoverColor: bgRectangle.hoverColor
    property alias bgNormalOpacity: bgRectangle.normalOpacity
    property alias bgHoverOpacity: bgRectangle.hoverOpacity

    property int bgBorderWidth
    property string bgBorderColor

    background: NCButtonBackground {
        id: bgRectangle
        hovered: root.hovered
        height: 36
        border.width: root.bgBorderWidth
        border.color: root.bgBorderColor
    }

    leftPadding: root.text === "" ? Style.smallSpacing : 20
    rightPadding: root.text === "" ? Style.smallSpacing : 20

    implicitHeight: 36
    implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding

    hoverEnabled: true

    NCToolTip {
        text: root.toolTipText
        visible: root.toolTipText !== "" && root.hovered
    }

    contentItem: SesButtonContents {
        id: contents
        display: root.display
        hovered: root.hovered
        imageSourceHover: root.imageSourceHover
        imageSource: root.icon.source
        imageSourceWidth: 12
        imageSourceHeight: 16
        text: root.text
        textColor: root.textColor
        textColorHovered: root.textColorHovered
    }
}