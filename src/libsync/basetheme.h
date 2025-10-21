#ifndef _BASETHEME_H
#define _BASETHEME_H

#include <QFont>
#include <QString>
#include "theme.h"

namespace OCC {

class BaseTheme {
public:
    virtual ~BaseTheme() = default;

    // Base theme prefix (can be overridden)
    virtual QString themePrefix() const { return QStringLiteral(":/theme/"); }

    // Common interface for icons, fonts, colors, etc.
    virtual QString avatarIcon() const = 0;

    virtual QString folderIcon() const = 0;

    virtual QString syncArrows() const = 0;

    virtual QString questionCircleIcon() const {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-questionMark.svg");
    }

    virtual QString liveBackupPlusIcon() const {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-addlivebackup.svg");
    }
    

    virtual QString plusIcon() const {
        return QStringLiteral("qrc:///client/theme/ses/ses-darkPlus.svg");
    }

    virtual QString deleteIcon() const {
        return QStringLiteral("qrc:///client/theme/ses/ses-accountDelete.svg");
    }

    virtual QString refreshIcon() const {
        return QStringLiteral("qrc:///client/theme/ses/ses-refresh.svg");
    }

    virtual QString syncSuccessIcon() const {
        return QStringLiteral("qrc:///client/theme/ses/ses-syncstatus-success.svg");
    }

    virtual QString syncWarnIcon() const {
        return QStringLiteral("qrc:///client/theme/ses/ses-syncstatus-warning.svg");
    }

    virtual QString syncErrorIcon() const {
        return QStringLiteral("qrc:///client/theme/ses/ses-syncstatus-error.svg");
    }

    virtual QString syncPausedIcon() const {
        return QStringLiteral("qrc:///client/theme/ses/ses-syncstatus-paused.svg");
    }

    virtual QString syncingIcon() const {
        return QStringLiteral("qrc:///client/theme/ses/ses-syncstatus-syncing.svg");
    }

    virtual QString syncOfflineIcon() const {
        return QStringLiteral("qrc:///client/theme/ses/ses-state-offline.svg");
    }

    virtual int treeViewIconSize() const {
        return 32;
    }

    //Control Configuration: Sizes
    virtual QString toolbarActionBorderRadius() const {
        return "8px";
    }

    virtual QString toolbarSideMargin() const {
        return "10px";
    }

    virtual int toolbarIconSize() const {
        return 24;
    }

    virtual QString buttonRadius() const {
        return "4px";
    }

    virtual int buttonRadiusInt() const {
        return 4;
    }

    virtual QString buttonPadding() const {
        return "10px";
    }

    virtual QString smallMargin() const {
        return "8";
    }

    virtual int minimalSettingsDialogWidth() const {
        return 740;
    }

    virtual int wizardFixedWidth() const {
        return 576;
    }

    virtual int wizardFixedHeight() const {
        return 704;
    }

    virtual int LoginPageSpacer() const {
        return 45;
    }

    //Font Configuration
    virtual QString settingsFont() const {
        return "Segoe UI";
    }

    virtual QString contextMenuFont() const {
        return ":/client/fonts/OpenSans-Regular.ttf";
    }

    virtual QString settingsSmallTextSize() const {
        return "14px";
    }

    virtual int settingsTextPixel() const {
        return 16;
    }

    virtual QString settingsTextSize() const {
        return QString::number(settingsTextPixel()) + "px";
    }

    virtual int settingsTitlePixel() const {
        return 20;
    }

    virtual QString settingsTitleSize() const {
        return QString::number(settingsTitlePixel()) + "px";
    }

    virtual int settingsBigTitlePixel() const {
        return 24;
    }

    virtual QString settingsBigTitleSize() const {
        return QString::number(settingsBigTitlePixel()) + "px";
    }

    virtual QString onboardingTitle() const {
        return "28px";
    }

    virtual QString settingsTextWeight() const {
        return "400";
    }

    virtual QString settingsTitleWeight400() const {
        return "400";
    }

    virtual QString settingsTitleWeight500() const {
        return "500";
    }

    virtual QString settingsTitleWeight600() const {
        return "600";
    }

    virtual QFont::Weight settingsTitleWeightDemiBold() const {
        return QFont::DemiBold;
    }

    virtual QFont::Weight settingsTitleWeightNormal() const {
        return QFont::Normal;
    }

    virtual QFont settingsFontDefault() const {
        QFont defaultFont(settingsFont());
        defaultFont.setPixelSize(settingsTextPixel());
        defaultFont.setWeight(settingsTitleWeightNormal());
        return defaultFont;
    }

    virtual QString titleColor() const {
        return "#000000";
    }

    virtual QString folderWizardSubtitleColor() const {
        return "#104996";
    }

    virtual QString folderWizardPathColor() const {
        return "#97A3B4";
    }

    virtual QString loginWizardFontGrey() const {
        return "#616161";
    }

    virtual QString loginWizardFontLightGrey() const {
        return "#BDBDBD";
    }

    virtual QString fontConfigurationCss(QString font, QString size, QString weight, QString color) const {
        return QString("font-family: %1; font-size: %2; font-weight: %3; color: %4; ").arg(
            font,
            size,
            weight,
            color);
    }

    //Colors
    virtual QString settingsLinkColor() const {
        return "#02306A";
    }

    virtual QString buttonPrimaryColor() const {
        return "#0F6CBD";
    }

    virtual QString buttonSecondaryBorderColor() const {
        return "#D1D1D1";
    }

    virtual QString buttonDisabledColor() const {
        return "#F0F0F0";
    }

    virtual QString buttonPrimaryHoverColor() const {
        return "#115EA3";
    }

    virtual QString buttonSecondaryHoverColor() const {
        return "#F5F5F5";
    }

    virtual QString buttonPrimaryPressedColor() const {
        return "#0C3B5E";
    }

    virtual QString buttonSecondaryPressedColor() const {
        return "#E0E0E0";
    }

    virtual QString buttonDisabledFontColor() const {
        return "#BDBDBD";
    }

    virtual QString white() const {
        return "#FFFFFF";
    }

    virtual QString black() const {
        return "#000000";
    }

    virtual QString dialogBackgroundColor() const {
        return "#FAFAFA";
    }

    virtual QString menuBorderColor() const {
        return "#2E4360";
    }

    virtual QString menuTextColor() const {
        return "#001B41";
    }

    virtual QString menuSelectedItemColor() const {
        return "#F4F7FA";
    }

    virtual QString menuBorderRadius() const {
        return "16px";
    }

    virtual QString buttonPressedColor() const {
        return "#0B2A63";
    }

    virtual QString buttonHoveredColor() const {
        return "#1474C4";
    }

    virtual QString toolButtonHoveredColor() const {
        return "#DBEDF8";
    }

    virtual QString toolButtonPressedColor() const {
        return "#95CAEB";
    }

    virtual QString errorBorderColor() const {
        return "#EEACB2";
    }

    virtual QString errorColor() const {
        return "#FDF3F4";
    }

    virtual QString warningBorderColor() const {
        return "#F4BFAB";
    }

    virtual QString warningColor() const {
        return "#FDF6F3";
    }

    virtual QString successBorderColor() const {
        return "#9FD89F";
    }

    virtual QString successColor() const {
        return "#F1FAF1";
    }

    virtual QString infoBorderColor() const {
        return "#11C7E6";
    }

    virtual QString infoColor() const {
        return "#E6F9FC";
    }
};
}


#endif // _BASETHEME_H