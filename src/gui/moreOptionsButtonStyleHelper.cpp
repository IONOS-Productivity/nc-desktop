#include "moreOptionsButtonStyleHelper.h"

#include "buttonStyleStrategy.h"
#include "buttonstyle.h"
#include "ionostheme.h"
#include <QPainter>
#include <QStyleOptionButton>
#include <QWidget> 


void MoreOptionsButtonStyleHelper::setupPainterForToolButtonShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget)
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

void MoreOptionsButtonStyleHelper::drawToolButtonShape(const QStyleOptionButton *option, QPainter *painter, const QWidget *widget)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    setupPainterForToolButtonShape(option, painter, widget);
    const int radius =  option->rect.height() / 2;
    painter->drawRoundedRect(option->rect, radius,radius);
    painter->restore();
}

QPixmap MoreOptionsButtonStyleHelper::tintPixmap(const QPixmap &src, const QColor &color) const{
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

void MoreOptionsButtonStyleHelper::adjustIconColor(QStyleOptionButton *option, const QWidget *widget)
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