#ifndef SESBUTTON_H
#define SESBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QStylePainter>
#include <QStyleOptionButton>

#include "buttonStyle.h"

class CustomStyleOption : public QStyleOptionButton {
public:

    CustomStyleOption(OCC::ButtonStyleName name = OCC::ButtonStyleName::Primary)
        : QStyleOptionButton(Version), customData(name)
    {
        this->init(Type, Version);
    }

    CustomStyleOption(const CustomStyleOption &other)
        : QStyleOptionButton(other)
        , customData(other.customData)
    {
        this->init(Type, Version);
    }

    OCC::ButtonStyleName customData;

    enum { Type = SO_CustomBase + 1 }; // Unique type
    enum { Version = 1 }; // Custom version value

    private:
    void init(int type, int version) {
        this->type = type;
        this->version = version;
    }
};
namespace OCC {

    class SesButtonStyle : public QPushButton
    {
        Q_OBJECT
        Q_PROPERTY(ButtonStyleName buttonStyle READ buttonStyle WRITE setButtonStyle NOTIFY buttonStyleChanged)

    public:
        explicit SesButtonStyle(QWidget* parent = nullptr);
        static QString rawPrimaryStyle();
        static QString rawSecondaryStyle();
        static QString rawDisabledStyle();
        ButtonStyleName buttonStyle() const;
    protected:
        void paintEvent(QPaintEvent *event) override {
            QStylePainter  painter(this);

            CustomStyleOption option(m_buttonStyle);
            option.initFrom(this);
            option.state |= isDown() ? QStyle::State_Sunken : QStyle::State_Raised;
            option.rect = rect();
            option.text = text();
            option.icon = icon();

            style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);
        }

    public slots:
        void setButtonStyle(ButtonStyleName style);
    
    signals:
        void buttonStyleChanged(ButtonStyleName newStyle);

    private:
        ButtonStyleName m_buttonStyle;

        void updateStyleSheet();
    };
}
#endif // SESBUTTON_H
