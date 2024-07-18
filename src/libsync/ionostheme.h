#include <QString>

class IonosTheme {
public:

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
        return "'Segoe UI'"; // Example color for button text
    }

    static QString settingsTitleSize() {
        return "20px"; // Example color for button text
    }

    static QString settingsTitleWeigth() {
        return "600";
    }

    static QString settingsTextSize() {
        return "16px"; // Example color for button text
    }

    static QString settingsTextWeigth() {
        return "normal"; // Example color for button text
    }

    static QString settingsLinkColor() {
        return "#02306A"; 
    }

private:
    IonosTheme() {} // Private constructor to prevent instantiation
};