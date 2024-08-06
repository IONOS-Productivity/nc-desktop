#include "sesButton.h"
#include "IonosTheme.h"
#include "Theme.h"


namespace OCC {

    SesButton::SesButton(QWidget* parent)
        : QPushButton(parent)
    {
    }



    ButtonStyle SesButton::buttonStyle() const { return m_buttonStyle; }

    void SesButton::setButtonStyle(ButtonStyle style) {
        if (m_buttonStyle != style) {
            m_buttonStyle = style;
            emit buttonStyleChanged(m_buttonStyle);
        }
    }

    QString SesButton::rawPrimaryStyle(){
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

    QString SesButton::rawSecondaryStyle(){
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

    QString SesButton::rawDisabledStyle(){
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

    void SesButton::updateStyleSheet() {
        QString styleSheet;
        switch (m_buttonStyle) {
            case ButtonStyle::Primary:
                styleSheet = QStringLiteral("QPushButton") + rawPrimaryStyle();
            break;
            case ButtonStyle::Secondary:
                styleSheet = QStringLiteral("QPushButton") + rawSecondaryStyle();
            break;
            case ButtonStyle::Disabled:
                styleSheet = QStringLiteral("QPushButton") + rawDisabledStyle();
                setEnabled(false);
                break;
        }
        setStyleSheet(styleSheet);
    }
}