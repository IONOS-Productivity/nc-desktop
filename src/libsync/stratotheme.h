#ifndef _STRATOTHEME_H
#define _STRATOTHEME_H

#include <QFont>
#include <QString>
#include "basetheme.h"
#include "theme.h"

namespace OCC {

class StratoTheme : public BaseTheme {
public:
    StratoTheme() = default;

    //Icons
    QString avatarIcon() const override {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-folderIcon.svg");
    }

    QString folderIcon() const override {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-folderIcon.svg");
    }

    QString syncArrows() const override {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-syncArrows.svg");
    }

    QString questionCircleIcon() const override {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-questionMark.svg");
    }

    QString liveBackupPlusIcon() const override {
        return QString(Theme::themePrefix) + QStringLiteral("ses/ses-addlivebackup.svg");
    }

    QString plusIcon() const override {
        return QStringLiteral("qrc:///client/theme/ses/ses-darkPlus.svg");
    }

    QString deleteIcon() const override {
        return QStringLiteral("qrc:///client/theme/ses/ses-accountDelete.svg");
    }

    QString refreshIcon() const override {
        return QStringLiteral("qrc:///client/theme/ses/ses-refresh.svg");
    }

    QString syncSuccessIcon() const override {
        return QStringLiteral("qrc:///client/theme/ses/ses-syncstatus-success.svg");
    }

    QString syncWarnIcon() const override {
        return QStringLiteral("qrc:///client/theme/ses/ses-syncstatus-warning.svg");
    }

    QString syncErrorIcon() const override {
        return QStringLiteral("qrc:///client/theme/ses/ses-syncstatus-error.svg");
    }

    QString syncPausedIcon() const override {
        return QStringLiteral("qrc:///client/theme/ses/ses-syncstatus-paused.svg");
    }

    QString syncingIcon() const override {
        return QStringLiteral("qrc:///client/theme/ses/ses-syncstatus-syncing.svg");
    }

    QString syncOfflineIcon() const override {
        return QStringLiteral("qrc:///client/theme/ses/ses-state-offline.svg");
    }

    int treeViewIconSize() const override {
        return 32;
    }

    //Control Configuration: Sizes
    QString toolbarActionBorderRadius() const override {
        return "8px";
    }

    QString toolbarSideMargin() const override {
        return "10px";
    }

    int toolbarIconSize() const override{
        return 24;
    }

    QString buttonRadius() const override {
        return "4px";
    }

    int buttonRadiusInt() const override {
        return 4;
    }

    QString buttonPadding() const override {
        return "10px";
    }

    QString smallMargin() const override {
        return "8";
    }

    int minimalSettingsDialogWidth() const override {
        return 740;
    }

    int wizardFixedWidth() const override {
        return 576;
    }

    int wizardFixedHeight() const override {
        return 704;
    }

    int LoginPageSpacer() const override {
        return 45;
    }

    //Font Configuration
    QString settingsFont() const override {
        return "Segoe UI";
    }

    QString contextMenuFont() const override {
        return ":/client/fonts/OpenSans-Regular.ttf";
    }

    QString settingsSmallTextSize() const override {
        return "14px";
    }

    int settingsTextPixel() const override {
        return 16;
    }

    QString settingsTextSize() const override {
        return QString::number(settingsTextPixel()) + "px";
    }

    int settingsTitlePixel() const override {
        return 20;
    }

    QString settingsTitleSize() const override {
        return QString::number(settingsTitlePixel()) + "px";
    }

    int settingsBigTitlePixel() const override {
        return 24;
    }

    QString settingsBigTitleSize() const override {
        return QString::number(settingsBigTitlePixel()) + "px";
    }

    QString onboardingTitle() const override {
        return "28px";
    }

    QString settingsTextWeight() const override {
        return "400";
    }

    QString settingsTitleWeight400() const override {
        return "400";
    }

    QString settingsTitleWeight500() const override {
        return "500";
    }

    QString settingsTitleWeight600() const override {
        return "600";
    }

    QFont::Weight settingsTitleWeightDemiBold() const override {
        return QFont::DemiBold;
    }

    QFont::Weight settingsTitleWeightNormal() const override {
        return QFont::Normal;
    }

    QFont settingsFontDefault() const override{
        QFont defaultFont(settingsFont());
        defaultFont.setPixelSize(settingsTextPixel());
        defaultFont.setWeight(settingsTitleWeightNormal());
        return defaultFont;
    }

    QString titleColor() const override {
        return "#000000";
    }

    QString folderWizardSubtitleColor() const override {
        return "#104996";
    }

    QString folderWizardPathColor() const override {
        return "#97A3B4";
    }

    QString loginWizardFontGrey() const override {
        return "#616161";
    }

    QString loginWizardFontLightGrey() const override {
        return "#BDBDBD";
    }

    QString fontConfigurationCss(QString font, QString size, QString weight, QString color) const override {
        return QString("font-family: %1; font-size: %2; font-weight: %3; color: %4; ").arg(
            font,
            size,
            weight,
            color);
    }

    //Colors
    QString settingsLinkColor() const override {
        return "#02306A";
    }

    QString buttonPrimaryColor() const override {
        return "#fa0000ff";
    }

    QString buttonSecondaryBorderColor() const override {
        return "#D1D1D1";
    }

    QString buttonDisabledColor() const override {
        return "#F0F0F0";
    }

    QString buttonPrimaryHoverColor() const override {
        return "#05ec8cff";
    }

    QString buttonSecondaryHoverColor() const override {
        return "#F5F5F5";
    }

    QString buttonPrimaryPressedColor() const override {
        return "#b8a850ff";
    }

    QString buttonSecondaryPressedColor() const override {
        return "#E0E0E0";
    }

    QString buttonDisabledFontColor() const override {
        return "#BDBDBD";
    }

    QString white() const override {
        return "#FFFFFF";
    }

    QString black() const override {
        return "#000000";
    }

    QString dialogBackgroundColor() const override {
        return "#FAFAFA";
    }

    QString menuBorderColor() const override {
        return "#2E4360";
    }

    QString menuTextColor() const override {
        return "#001B41";
    }

    QString menuSelectedItemColor() const override {
        return "#F4F7FA";
    }

    QString menuBorderRadius() const override {
        return "16px";
    }

    QString buttonPressedColor() const override {
        return "#0B2A63";
    }

    QString buttonHoveredColor() const override {
        return "#1474C4";
    }

    QString toolButtonHoveredColor() const override {
        return "#DBEDF8";
    }

    QString toolButtonPressedColor() const override {
        return "#95CAEB";
    }

    QString errorBorderColor() const override {
        return "#EEACB2";
    }

    QString errorColor() const override {
        return "#FDF3F4";
    }

    QString warningBorderColor() const override {
        return "#F4BFAB";
    }

    QString warningColor() const override {
        return "#FDF6F3";
    }

    QString successBorderColor() const override {
        return "#9FD89F";
    }

    QString successColor() const override {
        return "#F1FAF1";
    }

    QString infoBorderColor() const override {
        return "#11C7E6";
    }

    QString infoColor() const override {
        return "#E6F9FC";
    }

};
}
#endif // _STRATOTHEME_H