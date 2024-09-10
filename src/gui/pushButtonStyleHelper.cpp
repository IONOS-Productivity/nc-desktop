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

 
#include "buttonstyle.h"
#include "buttonStyleStrategy.h"
#include "ionostheme.h"
#include <QMap>
#include <QPainter>
#include <QStyleOptionButton>
#include <QPushButton>
#include <QWidget> 


void PushButtonStyleHelper::setupPainterForToolButtonShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget)
{
    Q_UNUSED(widget)

    OCC::ButtonStyle& style = ButtonStyleStrategy::getButtonStyle(widget, option);

    // Disabled
    if (!(option->state & QStyle::State_Enabled)) {
        painter->setPen(QColor(style.buttonDisabledBorderColor()));
        painter->setBrush(QColor(style.buttonDisabledColor()));
    }
    //Pressed 
    else if (option->state & QStyle::State_Sunken) 
    {
        painter->setPen(QColor("#0B2A63"));
        painter->setBrush(QColor("#0B2A63"));
    } 
    // Hover
    else if(option->state & QStyle::State_MouseOver)
    {
        painter->setPen(QColor("#1474C4"));
        painter->setBrush(QColor("#1474C4"));
    }
    // Focused
    else if (option->state & QStyle::State_HasFocus) {
        painter->setPen(QColor(style.buttonFocusedBorderColor()));
        painter->setBrush(QColor(style.buttonFocusedColor()));
    } 
    // Else - Just beeing there
    else {
        painter->setPen(QColor("#FFFFFF"));
        painter->setBrush(QColor("#FFFFFF"));
    }
}

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

void PushButtonStyleHelper::drawToolButtonShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    setupPainterForToolButtonShape(option, painter, widget);
    const int radius =  option->rect.height() / 2;
    painter->drawRoundedRect(option->rect, radius,radius);
    painter->restore();
}

QPixmap PushButtonStyleHelper::tintPixmap(const QPixmap &src, const QColor &color) const{
    QPixmap result(src.size());
    result.fill(Qt::transparent);  

    QPainter painter(&result);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0, 0, src);  // Draw the original pixmap

    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(result.rect(), color);

    painter.end();
    return result;
}

void PushButtonStyleHelper::adjustIconColor(QStyleOptionButton *option, const QWidget *widget)
{
    QColor iconColor;
    OCC::ButtonStyle& style = ButtonStyleStrategy::getButtonStyle(widget, option);
    
    if (!(option->state & QStyle::State_Enabled)) {
        iconColor = style.buttonDisabledFontColor();
    }    
    else if(option->state & QStyle::State_MouseOver)
    {
        iconColor = QColor("#FFFFFF");
    }
    else 
    {
        iconColor = QColor("#1474C4");
    }

    QIcon icon = option->icon;
    QSize iconSize = option->iconSize.isValid() ? option->iconSize : QSize(16, 16);
    QPixmap pixmap = icon.pixmap(iconSize);

    QPixmap coloredPixmap = tintPixmap(pixmap, iconColor);

    option->icon = (QIcon(coloredPixmap) );
}

void PushButtonStyleHelper::recalculateContentSize(QSize &contentsSize, const QWidget *widget) const
{
    QFont font = widget->font();
    font.setWeight(QFont::DemiBold);
    QFontMetrics fm(font);

    //Code aus qpushbutton.cpp - sizeHint
    const QPushButton* pushButton = qobject_cast<const QPushButton*>(widget);
    int w = 0, h = 0;
    QString s(pushButton->text());
    bool empty = s.isEmpty();
    if (empty)
        s = QStringLiteral("XXXX");
    QSize sz = fm.size(Qt::TextShowMnemonic, s);
    if (!empty || !w)
        w += sz.width();
    if (!empty || !h)
        h = qMax(h, sz.height());
    // -- end code

    contentsSize.setHeight(h);
    contentsSize.setWidth(w);
}

QSize PushButtonStyleHelper::sizeFromContents(const QStyleOptionButton *option, QSize contentsSize, const QWidget *widget, int margin) const
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(widget != nullptr)
    {
        recalculateContentSize(contentsSize, widget);
    }
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