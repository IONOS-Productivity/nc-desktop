#include "sesButton.h"
#include "IonosTheme.h"
#include "Theme.h"

namespace OCC {
    SesButton::SesButton(QWidget* parent)
        : QPushButton(parent)
    {
        updateStyleSheet();
        setCursor(Qt::PointingHandCursor);
    }

    ButtonStyle SesButton::buttonStyle() const { return m_buttonStyle; }

    void SesButton::setButtonStyle(ButtonStyle style) {
        if (m_buttonStyle != style) {
            m_buttonStyle = style;
            updateStyleSheet();
            emit buttonStyleChanged(m_buttonStyle);
        }
    }

    void SesButton::updateStyleSheet() {
        QString styleSheet;
        switch (m_buttonStyle) {
            case ButtonStyle::Primary:
                styleSheet = QStringLiteral(
                        "QPushButton {"
                        "background-color: %1; color: %2;"
                        "font-family: %3; font-size: %4; font-weight: %5;"
                        "border-radius: %6; padding: %7; }")
                    .arg(
                        IonosTheme::buttonColor()
                        , IonosTheme::white()
                        , IonosTheme::settingsFont()
                        , IonosTheme::buttonFontSize()
                        , IonosTheme::buttonFontWeight()
                        , IonosTheme::buttonRadius()
                        , IonosTheme::buttonPadding()
                    );
            break;
            case ButtonStyle::Secondary:
                styleSheet = QStringLiteral(
                        "QPushButton {"
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
                        , IonosTheme::buttonBorderColor()
                        , IonosTheme::buttonPadding()
                    );
            break;
            case ButtonStyle::Disabled:
                styleSheet = QStringLiteral(
                        "QPushButton {"
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
                setEnabled(false);
                break;
        }
        setStyleSheet(styleSheet);
    }
}