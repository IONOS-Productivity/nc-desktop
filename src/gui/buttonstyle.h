
#ifndef _BUTTONSTYLE_H
#define _BUTTONSTYLE_H
 
#include "whitelabeltheme.h"
#include <QMetaType>
#include <QString>

namespace OCC{

enum class ButtonStyleName {
    Primary,
    Secondary,
    MoreOptions,
};
OCSYNC_EXPORT Q_NAMESPACE;
Q_ENUM_NS(ButtonStyleName); 
}
Q_DECLARE_METATYPE(OCC::ButtonStyleName);

namespace OCC{
class ButtonStyle 
{
protected:
    ButtonStyle()
    {
        qRegisterMetaType<OCC::ButtonStyleName>("OCC::ButtonStyleName");
    }
    ~ButtonStyle() {}

public:

    // Default
    virtual QString buttonDefaultColor() const = 0;
    virtual QString buttonDefaultBorderColor() const = 0;
    // Hover
    virtual QString buttonHoverColor() const = 0;
    virtual QString buttonHoverBorderColor() const = 0;
    // Pressed
    virtual QString buttonPressedColor() const = 0;
    virtual QString buttonPressedBorderColor() const = 0;
    // Disabled
    virtual QString buttonDisabledColor() const = 0;
    virtual QString buttonDisabledBorderColor() const = 0;
    // Focused
    virtual QString buttonFocusedColor() const = 0;
    virtual QString buttonFocusedBorderColor() const = 0;
    // Font
    virtual QString buttonDisabledFontColor() const = 0;
    virtual QString buttonFontColor() const = 0;
};

class PrimaryButtonStyle : public ButtonStyle {
private: 
    PrimaryButtonStyle()
    {
    }
    ~PrimaryButtonStyle() {}
public:

    PrimaryButtonStyle(PrimaryButtonStyle &other) = delete;
    void operator=(const PrimaryButtonStyle &) = delete;

    static PrimaryButtonStyle& GetInstance() {
        static PrimaryButtonStyle instance;
        return instance;
    }

    // Default
    QString buttonDefaultColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonPrimaryColor();
    }

    QString buttonDefaultBorderColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonPrimaryColor();
    }

    //Hover
    QString buttonHoverColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonPrimaryHoverColor();
    }

    QString buttonHoverBorderColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonPrimaryHoverColor();
    }

    // Pressed
    QString buttonPressedColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonPrimaryPressedColor();
    }

    QString buttonPressedBorderColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonPrimaryPressedColor();
    }

    // Disabled
    QString buttonDisabledColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonDisabledColor();
    }

    QString buttonDisabledBorderColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonDisabledColor();
    }

    // Focused
    QString buttonFocusedColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonPrimaryColor();
    }

    QString buttonFocusedBorderColor() const override 
    {
        return OCC::WhitelabelTheme::instance().black();
    }

    // Font
    QString buttonDisabledFontColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonDisabledFontColor();
    }

    QString buttonFontColor() const override 
    {
        return OCC::WhitelabelTheme::instance().white();
    }
};

class SecondaryButtonStyle : public ButtonStyle {
protected: 
    SecondaryButtonStyle()
    {
    }
    ~SecondaryButtonStyle() {}
public:

    SecondaryButtonStyle(SecondaryButtonStyle &other) = delete;
    void operator=(const SecondaryButtonStyle &) = delete;

    static SecondaryButtonStyle& GetInstance() {
        static SecondaryButtonStyle instance; 
        return instance;
    }

    // Default
    QString buttonDefaultColor() const override 
    {
        return OCC::WhitelabelTheme::instance().white();
    }

    QString buttonDefaultBorderColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonSecondaryBorderColor();
    }

    // Hover
    QString buttonHoverColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonSecondaryHoverColor();
    }

    QString buttonHoverBorderColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonSecondaryBorderColor();
    }

    // Pressed
    QString buttonPressedColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonSecondaryPressedColor();
    }

    QString buttonPressedBorderColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonSecondaryBorderColor();
    }

    // Disabled
    QString buttonDisabledColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonDisabledColor();
    }

    QString buttonDisabledBorderColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonDisabledColor();
    }

    // Focused 
    QString buttonFocusedColor() const override 
    {
        return OCC::WhitelabelTheme::instance().white();
    }

    QString buttonFocusedBorderColor() const override 
    { 
        return OCC::WhitelabelTheme::instance().black();
    }

    // Font
    QString buttonDisabledFontColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonDisabledFontColor();
    }

    QString buttonFontColor() const override 
    {
        return OCC::WhitelabelTheme::instance().black();
    }
};

class MoreOptionsButtonStyle : public ButtonStyle {
protected: 
    MoreOptionsButtonStyle()
    {
    }
    ~MoreOptionsButtonStyle() {}
public:

    MoreOptionsButtonStyle(MoreOptionsButtonStyle &other) = delete;
    void operator=(const MoreOptionsButtonStyle &) = delete;

    static MoreOptionsButtonStyle& GetInstance() {
        static MoreOptionsButtonStyle instance; 
        return instance;
    }

    // Default
    QString buttonDefaultColor() const override 
    {
        return OCC::WhitelabelTheme::instance().white();
    }

    QString buttonDefaultBorderColor() const override 
    {
        return OCC::WhitelabelTheme::instance().white();
    }

    // Hover
    QString buttonHoverColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonHoveredColor();
    }

    QString buttonHoverBorderColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonHoveredColor();
    }

    // Pressed
    QString buttonPressedColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonPressedColor();
    }

    QString buttonPressedBorderColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonPressedColor();
    }

    // Disabled
    QString buttonDisabledColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonDisabledColor();
    }

    QString buttonDisabledBorderColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonDisabledColor();
    }

    // Focused 
    QString buttonFocusedColor() const override 
    {
        return OCC::WhitelabelTheme::instance().white();
    }

    QString buttonFocusedBorderColor() const override 
    { 
        return OCC::WhitelabelTheme::instance().black();
    }

    // Font
    QString buttonDisabledFontColor() const override 
    {
        return OCC::WhitelabelTheme::instance().buttonDisabledFontColor();
    }

    QString buttonFontColor() const override 
    {
        return OCC::WhitelabelTheme::instance().black();
    }
};
}

#endif // _BUTTONSTYLE_H
