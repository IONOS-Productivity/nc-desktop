/****************************************************************************
**
** This file is part of the Oxygen2 project.
**
** SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "pushButtonStyleHelper.h"

 
#include "buttonStyle.h"
#include "ionostheme.h"
#include <QMap>
#include <QPainter>
#include <QStyleOptionButton>

class ButtonStyleStrategy
{
public:
    virtual ~ButtonStyleStrategy() = default;

    static OCC::ButtonStyle& getButtonStyle(const QWidget *widget, const QStyleOptionButton *option)
    {
        OCC::ButtonStyleName buttonStyleName;
        buttonStyleName = determineButtonStyleName(widget, option);

        switch (buttonStyleName)
        {
            case OCC::ButtonStyleName::Primary:
                return OCC::PrimaryButtonStyle::GetInstance();
            case OCC::ButtonStyleName::Secondary:
            default:
                return OCC::SecondaryButtonStyle::GetInstance();
        }
    }

    static OCC::ButtonStyleName determineButtonStyleName(const QWidget *widget, const QStyleOptionButton *option)
    {
        QVariant propertyValue = widget->property("buttonStyle");        
        if(propertyValue.isValid()){             

            return propertyValue.value<OCC::ButtonStyleName>();
        }

        return getButtonStyleNameByObjectName(widget);
    }

    static OCC::ButtonStyleName getButtonStyleNameByObjectName(const QWidget *widget)
    {
        static const QMap<QString, OCC::ButtonStyleName> buttonStyleMap = {
            {"qt_wizard_finish", OCC::ButtonStyleName::Primary}
        };

        QString buttonName = widget->objectName();
        return buttonStyleMap.value(buttonName, OCC::ButtonStyleName::Secondary);
    }
};

void PushButtonStyleHelper::setupPainterForShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget)
{
    Q_UNUSED(widget)

    OCC::ButtonStyle& style = ButtonStyleStrategy::getButtonStyle(widget, option);

    // Disabled
    if (!(option->state & QStyle::State_Enabled)) {
        painter->setPen(QColor(style.buttonDisabledBorderColor()));
        painter->setBrush(QColor(style.buttonDisabledColor()));
    }
    //Pressed 
    else if (option->state & QStyle::State_Sunken) {

        painter->setPen(QColor(style.buttonPressedBorderColor()));
        painter->setBrush(QColor(style.buttonPressedColor()));
    } 
    // Hover
    else if(option->state & QStyle::State_MouseOver)
    {
        painter->setPen(QColor(style.buttonHoverBorderColor()));
        painter->setBrush(QColor(style.buttonHoverColor()));
    }
    // Focused
    else if (option->state & QStyle::State_HasFocus) {
        painter->setPen(QColor(style.buttonFocusedBorderColor()));
        painter->setBrush(QColor(style.buttonFocusedColor()));
    } 
    // Else - Just beeing there
    else {
        painter->setPen(QColor(style.buttonDefaultBorderColor()));
        painter->setBrush(QColor(style.buttonDefaultColor()));
    }
}

void PushButtonStyleHelper::drawButtonShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    setupPainterForShape(option, painter, widget);
    const int radius = OCC::IonosTheme::buttonRadiusInt();
    painter->drawRoundedRect(option->rect, radius, radius);
    painter->restore();
}

QSize PushButtonStyleHelper::sizeFromContents(const QStyleOptionButton *option, QSize contentsSize, const QWidget *widget, int margin) const
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    const int frameWidth = 2; // due to pen width 1 in drawButtonBevel, on each side
    return QSize(qMax(80, contentsSize.width() + 2 * margin + frameWidth), qMin(40, contentsSize.height() + 2 * margin + frameWidth));
}

void PushButtonStyleHelper::adjustTextPalette(QStyleOptionButton *option, const QWidget *widget) const
{
    QColor textColor;
    OCC::ButtonStyle& style = ButtonStyleStrategy::getButtonStyle(widget, option);

    // Disabled
    if (!(option->state & QStyle::State_Enabled)) {
        textColor = style.buttonDisabledFontColor();
    }    
    else 
    {
        textColor = style.buttonFontColor();
    }
    option->palette.setColor(QPalette::ButtonText, textColor);
}