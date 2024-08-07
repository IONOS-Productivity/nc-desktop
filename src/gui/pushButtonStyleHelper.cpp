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

#include <QPainter>
#include <QStyleOptionButton>
#include "ionostheme.h"
#include "sesButton.h"
#include "buttonStyle.h"

static const int s_radius = 6;


class ButtonStyleStrategy
{
public:
    virtual ~ButtonStyleStrategy() = default;

    static OCC::ButtonStyle& getButtonStyle(const QWidget *widget, const QStyleOptionButton *option)
    {
        if(isPrimary(widget, option))
            return OCC::PrimaryButtonStyle::GetInstance();
        return OCC::SecondaryButtonStyle::GetInstance();
    }

    static bool isPrimary(const QWidget *widget, const QStyleOptionButton *option)
    {
        QVariant propertyValue = widget->property("buttonStyle");        
        if(propertyValue.isValid()){             
            QString buttonType = propertyValue.toString(); 
                      
            if(buttonType =="primary") {
                return true;
            }
        }

        if (const auto *customOption = qstyleoption_cast<const CustomStyleOption *>(option)) {

            QString buttonName = widget->objectName();
            return buttonName == "qt_wizard_finish" || customOption->customData == OCC::ButtonStyleName::Primary;
        }
        return false;
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

QSize PushButtonStyleHelper::sizeFromContents(const QStyleOptionButton *option, QSize contentsSize, const QWidget *widget) const
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    const int margin = 11; // usually this comes from PM_ButtonMargin
    const int frameWidth = 2; // due to pen width 1 in drawButtonBevel, on each side
    return QSize(qMax(80, contentsSize.width() + 2 * margin + frameWidth), contentsSize.height() + 2 * margin + frameWidth);
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