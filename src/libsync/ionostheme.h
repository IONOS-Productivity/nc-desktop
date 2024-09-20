#ifndef _IONOSTHEME_H
#define _IONOSTHEME_H

#include <QString>
#include "theme.h"

namespace OCC {

class IonosTheme {
public:

    //Icons
    static QString avatarIcon() {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-setupAvatar.svg");
    }

    static QString folderIcon() {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-folder32.svg");
    }

    static QString syncArrows() {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-syncArrows.svg");
    }

    static QString questionCircleIcon() {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-questionCircle.svg");
    }

    static QString liveBackupPlusIcon() {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-addLiveBackupPlus.svg");
    }

    static QString plusIcon() {
        return QStringLiteral("qrc:///client/theme/ses/ses-darkPlus24.svg");
    }

    static QString deleteIcon() {
        return QStringLiteral("qrc:///client/theme/ses/ses-activityDelete.svg");
    }

    static QString refreshIcon() {
        return QStringLiteral("qrc:///client/theme/ses/ses-refresh.svg");
    }

    //Control Configuration: Sizes
    static QString toolbarActionBorderRadius() {
        return "8px";
    }

    static QString toolbarSideMargin() {
        return "10px";
    }

    static int toolbarIconSize(){
        return 24;
    }

    static QString buttonRadius() {
        return "4px";
    }

    static int buttonRadiusInt() {
        return 4;
    }

    static QString buttonPadding() {
        return "10px";
    }

    //Font Configuration
    static QString settingsFont() {
        return "Segoe UI";
    }

    static QString settingsTextSize() {
        return "16px";
    }

    static QString settingsTitleSize() {
        return "20px";
    }

    static QString settingsTitleWeight() {
        return "600";
    }

    static QString settingsTextWeight() {
        return "normal";
    }

    static QString buttonFontSize() {
        return "16px";
    }

    static QString buttonFontWeight() {
        return "500";
    }

    //Colors

    static QString settingsLinkColor() {
        return "#02306A";
    }

    static QString buttonPrimaryColor() {
        return "#0F6CBD";
    }

    static QString buttonSecondaryBorderColor() {
        return "#D1D1D1";
    }

    static QString buttonDisabledColor() {
        return "#F0F0F0";
    }

    static QString buttonPrimaryHoverColor() {
        return "#115EA3";
    }

    static QString buttonSecondaryHoverColor() {
        return "#F5F5F5";
    }

    static QString buttonPrimaryPressedColor() {
        return "#0C3B5E";
    }

    static QString buttonSecondaryPressedColor() {
        return "#E0E0E0";
    }

    static QString buttonDisabledFontColor() {
        return "#BDBDBD";
    }

    static QString white() {
        return "#FFFFFF";
    }

    static QString black() {
        return "#000000";
    }

    static QString dialogBackgroundColor() {
        return "#FAFAFA";
    }

    static QString menuBorderColor() {
        return "#2E4360";
    }

    static QString menuTextColor() {
        return "#001B41";
    }

    static QString menuSelectedItemColor() {
        return "#F4F7FA";
    }

    static QString menuBorderRadius() {
        return "16px";
    }

    static QString toolButtonPressedColor() {
        return "#0B2A63";
    }

    static QString toolButtonHoveredColor() {
        return "#1474C4";
    }

    static QString errorBorderColor() {
        return "#EEACB2";
    }

    static QString errorColor() {
        return "#FDF3F4";
    }

    static QString warningBorderColor() {
        return "#F4BFAB";
    }

    static QString warningColor() {
        return "#FDF6F3";
    }

    static QString successBorderColor() {
        return "#9FD89F";
    }

    static QString successColor() {
        return "#F1FAF1";
    }

    static QString infoBorderColor() {
        return "#11C7E6";
    }

    static QString infoColor() {
        return "#E6F9FC";
    }

private:
    IonosTheme() {}
};
}
#endif // _IONOSTHEME_H