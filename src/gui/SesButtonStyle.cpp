#include "IonosTheme.h"
#include "SesButtonStyle.h"
#include "Theme.h"

namespace OCC {

    SesButtonStyle::SesButtonStyle(QWidget* parent)
        : QPushButton(parent)
    {
    }

    ButtonStyleName SesButtonStyle::buttonStyle() const { return m_buttonStyle; }

    void SesButtonStyle::setButtonStyle(ButtonStyleName style) {
        if (m_buttonStyle != style) {
            m_buttonStyle = style;
            emit buttonStyleChanged(m_buttonStyle);
        }
    }

    QString SesButtonStyle::rawPrimaryStyle(){
        return QStringLiteral(
                        "{"
                        "background-color: %1; color: %2;"
                        "font-family: %3; font-size: %4; font-weight: %5;"
                        "border-radius: %6; padding: %7; }")
                    .arg(
                        IonosTheme::buttonPrimaryColor()
                        , IonosTheme::white()
                        , IonosTheme::settingsFont()
                        , IonosTheme::buttonFontSize()
                        , IonosTheme::buttonFontWeight()
                        , IonosTheme::buttonRadius()
                        , IonosTheme::buttonPadding()
                    );
    }

    QString SesButtonStyle::rawSecondaryStyle(){
        return QStringLiteral(
                        "{"
                        "background-color: %1; color: %2;"
                        "font-family: %3; font-size: %4; font-weight: %5;"
                        "border-radius: %6; border: 1px solid %7;"
                        "padding: %8; }")
                    .arg(
                        IonosTheme::white()
                        , IonosTheme::black()
                        , IonosTheme::settingsFont()
                        , IonosTheme::buttonFontSize()
                        , IonosTheme::buttonFontWeight()
                        , IonosTheme::buttonRadius()
                        , IonosTheme::buttonSecondaryBorderColor()
                        , IonosTheme::buttonPadding()
                    );
    }

    QString SesButtonStyle::rawDisabledStyle(){
        return QStringLiteral(
                        "{"
                        "background-color: %1; color: %2;"
                        "font-family: %3; font-size: %4; font-weight: %5;"
                        "border-radius: %6;"
                        "padding: %7; }")
                    .arg(
                        IonosTheme::buttonDisabledColor()
                        , IonosTheme::buttonDisabledFontColor()
                        , IonosTheme::settingsFont()
                        , IonosTheme::buttonFontSize()
                        , IonosTheme::buttonFontWeight()
                        , IonosTheme::buttonRadius()
                        , IonosTheme::buttonPadding()
                    );
    }

    void SesButtonStyle::updateStyleSheet() {
        QString styleSheet;
        switch (m_buttonStyle) {
            case ButtonStyleName::Primary:
                styleSheet = QStringLiteral("QPushButton") + rawPrimaryStyle();
            break;
            case ButtonStyleName::Secondary:
                styleSheet = QStringLiteral("QPushButton") + rawSecondaryStyle();
            break;
        }
        setStyleSheet(styleSheet);
    }
}