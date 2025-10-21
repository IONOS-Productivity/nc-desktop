// linkbutton.cpp
#include "linkbutton.h"
#include "whitelabeltheme.h"
#include "theme.h"

namespace OCC {
    LinkButton::LinkButton(QWidget* parent)
        : QLabel(parent)
    {
        setStyleSheet(QStringLiteral("QLabel { color: %1; text-decoration: underline; font-size: %2; font-weight: %3; }")
            .arg(WhitelabelTheme::instance().settingsLinkColor()
                , WhitelabelTheme::instance().settingsTextSize()
                , WhitelabelTheme::instance().settingsTitleWeight600()
            ));

        setCursor(Qt::PointingHandCursor);
    }

    void LinkButton::mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        }
        QLabel::mousePressEvent(event);
    }
}