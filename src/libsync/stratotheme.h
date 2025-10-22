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

    QString dialogBackgroundColor() const override {
        return "#F7F7F9";
    }

    QString settingsLinkColor() const override {
        return "#272CB2";
    }

    QString quotaProgressColor() const override {
        return "#272CB2";
    }

    QString buttonPrimaryColor() const override { 
        return "#272CB2";
    }

    QString buttonPrimaryHoverColor() const override {
        return "#2944CC";
    }

    QString buttonPrimaryPressedColor() const override {
        return "#272CB2";
    }

    QString buttonPrimaryFocusedBorderColor() const override {
        return "#CDD5E3";
    }

    QString buttonSecondaryColor() const override { 
        return "#F7F7F9";
    }

    QString buttonSecondaryBorderColor() const override { 
        return "#CDD5E3";
    }

    QString buttonSecondaryHoverColor() const override {
        return "#EDEEF3";
    }

    QString buttonSecondaryPressedColor() const override {
        return "#D6D6E4";
    }

    QString buttonSecondaryFocusedBorderColor() const override {
        return "#8493B3";
    }

    QString buttonDisabledColor() const override {
        return "#EDEEF3";
    }

    QString buttonIconColor() const override {
        return "#2f2f70";
    }

    QString buttonIconHoverColor() const override {
        return "#2f2f70";
    }

    QString buttonHoveredColor() const override {
        return "#eeeff9";
    }

    QString buttonPressedColor() const override {
        return "#D6D6E4";
    }

    QString toolButtonHoveredColor() const override {
        return "#EDEEF3";
    }

    QString toolButtonPressedColor() const override {
        return "#D6D6E4";
    }
};
}
#endif // _STRATOTHEME_H