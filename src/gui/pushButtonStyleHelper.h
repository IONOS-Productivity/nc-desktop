/****************************************************************************
**
** This file is part of the Oxygen2 project.
**
** SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef PUSHBUTTONSTYLEHELPER_H
#define PUSHBUTTONSTYLEHELPER_H

#include <QSize>

class QPainter;
class QPushButton;
class QStyleOptionButton;
class QWidget;
class QPixmap;
class QColor;

class PushButtonStyleHelper
{
public:
    void setupPainterForToolButtonShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget);
    void setupPainterForShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget);
    void drawButtonShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget);
    void drawToolButtonShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget);
    QSize sizeFromContents(const QStyleOptionButton *option, QSize contentsSize, const QWidget *widget, int margin) const;
    void adjustTextPalette(QStyleOptionButton *option, const QWidget *widget) const;
    bool isPrimary(const QWidget *widget, const QStyleOptionButton *option) const;
    void adjustIconColor(QStyleOptionButton *option, const QWidget *widget);

private:
    void recalculateContentSize(QSize &contentsSize, const QWidget *widget) const;
    QPixmap tintPixmap(const QPixmap &src, const QColor &color) const;

};
#endif // PUSHBUTTONSTYLEHELPER_H