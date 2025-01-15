import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Style

import com.ionos.hidrivenext.desktopclient as NC

RowLayout {
    id: root

    property alias model: syncStatus
    property color accentColor: Style.ncBlue

    spacing: Style.trayHorizontalMargin

    NC.SyncStatusSummary {
        id: syncStatus
    }

    NCBusyIndicator {
        id: syncIcon
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.topMargin: 16
        Layout.rightMargin: 0
        Layout.bottomMargin: 16
        Layout.leftMargin: Style.sesActivityItemDistanceToFrame
        padding: 0

        imageSource: syncStatus.syncIcon
        running: false // hotfix for download speed slowdown when tray is open
    }

    ColumnLayout {
        id: syncProgressLayout

        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
        Layout.topMargin: 8
        Layout.rightMargin: Style.sesActivityItemDistanceToFrame
        Layout.bottomMargin: 8
        Layout.leftMargin: 0
        Layout.fillWidth: true
        Layout.fillHeight: true

        EnforcedPlainTextLabel {
            id: syncProgressText

            Layout.fillWidth: true

            text: syncStatus.syncStatusString
            verticalAlignment: Text.AlignVCenter
            font: root.font
            wrapMode: Text.Wrap
        }

        Loader {
            Layout.fillWidth: true
            Layout.preferredHeight: Style.progressBarPreferredHeight

            active: syncStatus.syncing && syncStatus.totalFiles > 0
            visible: active

            sourceComponent: NCProgressBar {
                id: syncProgressBar
                value: syncStatus.syncProgress
                fillColor: root.accentColor
            }
        }

        EnforcedPlainTextLabel {
            id: syncProgressDetailText

            Layout.fillWidth: true

            text: syncStatus.syncStatusDetailString
            visible: syncStatus.syncStatusDetailString !== ""
            color: palette.midlight
            font: root.font
            wrapMode: Text.Wrap
        }
    }

    Button {
        id: syncNowButton

        Layout.rightMargin: Style.trayHorizontalMargin

        text: qsTr("Sync now")

        padding: Style.smallSpacing
        textColor: Style.adjustedCurrentUserHeaderColor
        textColorHovered: Style.currentUserHeaderTextColor
        bgColor: Style.currentUserHeaderColor

        visible: false // SES-4 removed
        enabled: visible
        onClicked: {
            if(!syncStatus.syncing) {
                NC.UserModel.currentUser.forceSyncNow();
            }
        }
    }

    Button {
        Layout.rightMargin: Style.trayHorizontalMargin

        text: qsTr("Resolve conflicts")
        textColor: Style.adjustedCurrentUserHeaderColor
        textColorHovered: Style.currentUserHeaderTextColor
        bgColor: Style.currentUserHeaderColor

        visible: activityModel.hasSyncConflicts &&
                 !syncStatus.syncing &&
                 NC.UserModel.currentUser.hasLocalFolder &&
                 NC.UserModel.currentUser.isConnected
        enabled: visible
        onClicked: NC.Systray.createResolveConflictsDialog(activityModel.allConflicts);
    }

    Button {
        Layout.rightMargin: Style.trayHorizontalMargin

        text: qsTr("Open browser")

        visible: NC.UserModel.currentUser.needsToSignTermsOfService
        enabled: visible

        onClicked: NC.UserModel.openCurrentAccountServer()
    }
}
