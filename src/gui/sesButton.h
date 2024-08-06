#ifndef SESBUTTON_H
#define SESBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QStylePainter>
#include <QStyleOptionButton>

class CustomStyleOption : public QStyleOptionButton {
public:
    enum StyleOptionType {
        Primary 
        , Secondary
        , Disabled
    };

    CustomStyleOption(CustomStyleOption::StyleOptionType type = StyleOptionType::Primary)
        : QStyleOptionButton(Version)
    {
        customData = type;
    }

    CustomStyleOption(const CustomStyleOption &other)
        : QStyleOptionButton(other)
        , customData(other.customData)
    {
    }

    StyleOptionType customData;

    enum { Version = 1 }; // Custom version value
};
namespace OCC {

    enum class ButtonStyle {
        Primary,
        Secondary,
        Disabled
    };

    class SesButton : public QPushButton
    {
        Q_OBJECT
        Q_PROPERTY(ButtonStyle buttonStyle READ buttonStyle WRITE setButtonStyle NOTIFY buttonStyleChanged)

    public:
        explicit SesButton(QWidget* parent = nullptr);
        static QString rawPrimaryStyle();
        static QString rawSecondaryStyle();
        static QString rawDisabledStyle();
        ButtonStyle buttonStyle() const;
    protected:
        void paintEvent(QPaintEvent *event) override {
            QStylePainter  painter(this);

            CustomStyleOption::StyleOptionType type = m_buttonStyle == OCC::ButtonStyle::Primary ? CustomStyleOption::Primary : CustomStyleOption::Secondary;
            CustomStyleOption option(type);
            option.initFrom(this);
            option.state |= isDown() ? QStyle::State_Sunken : QStyle::State_Raised;
            option.rect = rect();
            option.text = text();
            option.icon = icon();

            style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);
        }

    public slots:
        void setButtonStyle(ButtonStyle style);
    
    signals:
        void buttonStyleChanged(ButtonStyle newStyle);

    private:
        ButtonStyle m_buttonStyle;

        void updateStyleSheet();
    };
}
#endif // SESBUTTON_H
