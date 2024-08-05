#ifndef SESBUTTON_H
#define SESBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QMouseEvent>
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
