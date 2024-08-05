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

static const int s_radius = 6;

void PushButtonStyleHelper::setupPainterForShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget)
{
    Q_UNUSED(widget)

        if(isPrimary(widget, option)){

            // Disabled
            if (!(option->state & QStyle::State_Enabled)) {
                painter->setPen(QColor(OCC::IonosTheme::buttonDisabledColor()));
                painter->setBrush(QColor(OCC::IonosTheme::buttonDisabledColor()));
            }
            //Pressed 
            else if (option->state & QStyle::State_Sunken) {
                painter->setPen(QColor(OCC::IonosTheme::buttonPrimaryPressedColor()));
                painter->setBrush(QColor(OCC::IonosTheme::buttonPrimaryPressedColor()));
            } 
            // Hover
            else if(option->state & QStyle::State_MouseOver)
            {
                painter->setPen(QColor(OCC::IonosTheme::buttonPrimaryHoverColor()));
                painter->setBrush(QColor(OCC::IonosTheme::buttonPrimaryHoverColor()));
            }
            // Focused
            else if (option->state & QStyle::State_HasFocus) {
                painter->setPen(QColor(OCC::IonosTheme::black()));
                painter->setBrush(QColor(OCC::IonosTheme::buttonPrimaryColor()));
            } 
            // Else - Just beeing there
            else {
                painter->setPen(QColor(OCC::IonosTheme::buttonPrimaryColor()));
                painter->setBrush(QColor(OCC::IonosTheme::buttonPrimaryColor()));
            }
        }
        else 
        {
            // Disabled
            if (!(option->state & QStyle::State_Enabled)) {
                painter->setPen(QColor(OCC::IonosTheme::buttonDisabledColor()));
                painter->setBrush(QColor(OCC::IonosTheme::buttonDisabledColor()));
            }
            //Pressed 
            else if (option->state & QStyle::State_Sunken) {
                painter->setPen(QColor(OCC::IonosTheme::buttonSecondaryBorderColor()));
                painter->setBrush(QColor(OCC::IonosTheme::buttonSecondaryPressedColor()));
            } 
            // Hover
            else if(option->state & QStyle::State_MouseOver)
            {
                painter->setPen(QColor(OCC::IonosTheme::buttonSecondaryBorderColor()));
                painter->setBrush(QColor(OCC::IonosTheme::buttonSecondaryHoverColor()));
            }
            // Focused
            else if (option->state & QStyle::State_HasFocus) {
                painter->setPen(QColor(OCC::IonosTheme::buttonSecondaryBorderColor()));
                painter->setBrush(QColor(OCC::IonosTheme::white()));
            } 
            // Else - Just beeing there
            else {
                painter->setPen(QColor(OCC::IonosTheme::buttonSecondaryBorderColor()));
                painter->setBrush(QColor(OCC::IonosTheme::white()));
            }
        }
}

bool PushButtonStyleHelper::isPrimary(const QWidget *widget, const QStyleOptionButton *option) const
{
    if (const auto *customOption = qstyleoption_cast<const CustomStyleOption *>(option)) {

        QString buttonName = widget->objectName();
        return buttonName == "qt_wizard_finish" || customOption->customData == CustomStyleOption::StyleOptionType::Primary;
    }
    return false;
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
    return QSize(qMax(60, contentsSize.width() + 2 * margin + frameWidth), contentsSize.height() + 2 * margin + frameWidth);
}

void PushButtonStyleHelper::adjustTextPalette(QStyleOptionButton *option, bool isPrimary) const
{
    QColor textColor;

    // Disabled
    if (!(option->state & QStyle::State_Enabled)) {
        textColor = QColor(OCC::IonosTheme::buttonDisabledFontColor());
    }    
    else if(isPrimary)
    {
        textColor = QColor(OCC::IonosTheme::white());
    }
    else 
    {
        textColor = QColor(OCC::IonosTheme::black());

    }
    option->palette.setColor(QPalette::ButtonText, textColor);
}