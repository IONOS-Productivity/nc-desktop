#ifndef _IONOSTHEME_H
#define _IONOSTHEME_H

#include <QString>
#include "theme.h"

namespace OCC {

class IonosTheme {
public:

    static QString avatarIcon() {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-setupAvatar.svg");
    }

    static QString folderIcon() {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-folder32.svg");
    }

    static QString syncArrows() {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-syncArrows.svg");
    }

    static QString toolbarActionBorderRadius() {
        return "8px";
    }

    static QString toolbarSideMargin() {
        return "10px";
    }

    static int toolbarIconSize(){
        return 24;
    }

    static QString settingsFont() {
        return "'Segoe UI'";
    }

    static QString settingsTitleSize() {
        return "20px";
    }

    static QString settingsTitleWeigth() {
        return "600";
    }

    static QString settingsTextSize() {
        return "16px"; 
    }

    static QString settingsTextWeigth() {
        return "normal"; 
    }

    static QString settingsLinkColor() {
        return "#02306A"; 
    }

    static QString buttonFontSize() {
        return "16px"; 
    }

    static QString buttonFontWeight() {
        return "550";
    }

    static QString buttonColor() {
        return "#0F6CBD"; 
    }

    static QString buttonRadius() {
        return "4px";
    }

    static QString buttonPadding() {
        return "10px";
    }
    
    static QString buttonBorderColor() {
        return "#D1D1D1";
    }

    static QString buttonDisabledColor() {
        return "#F0F0F0";
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

    static QString welcomeBackgroundColor() {
        return "#FAFAFA"; 
    }

private:
    IonosTheme() {} 
};
}
#endif // _IONOSTHEME_H