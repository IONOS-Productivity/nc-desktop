import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15
import Qt.labs.platform 1.1 as NativeDialogs

import "../"

// Custom qml modules are in /theme (and included by resources.qrc)
import Style 1.0

import com.nextcloud.desktopclient 1.0        
        
Rectangle {
    Image{
            id: trayWindowLogo
            cache: false
            source: Style.sesIonosLogoIcon
            sourceSize: Qt.size(width, height)
            fillMode: Image.PreserveAspectFit
            anchors{
                top: parent.top
                left: parent.left
                topMargin: Style.sesHeaderLogoTopMargin
                leftMargin: Style.sesHeaderLogoLeftMargin
            }
    }

    color: Style.sesSelectedColor
}