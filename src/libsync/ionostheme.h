#include <QString>

class IonosTheme {
public:
    static QString getBackgroundColor() {
        return "#FFFFFF"; // Example color for background
    }

    static QString getTextColor() {
        return "#000000"; // Example color for text
    }

    static QString getButtonColor() {
        return "#FF0000"; // Example color for buttons
    }

    static QString toolbarActionBorderRadius() {
        return "8px";
    }

    static QString toolbarSideMargin() {
        return "10px";
    }


    // Add more methods for different elements and their colors

private:
    IonosTheme() {} // Private constructor to prevent instantiation
};