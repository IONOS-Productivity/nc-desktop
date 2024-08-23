#include "sesSnackBar.h"
#include "IonosTheme.h"
#include "Theme.h"
#include <QLayout>
#include <QHBoxLayout>
#include <QLabel>

namespace OCC {

    sesSnackBar::sesSnackBar(QWidget* parent)
        : QFrame(parent)
    {
        setObjectName("sesSnackBar");
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        setContentsMargins(0, 0, 0, 0);

        auto policy = sizePolicy();
        policy.setRetainSizeWhenHidden(false);
        setSizePolicy(policy);
        
        const auto layout = new QHBoxLayout();
        layout->setObjectName("sesSnackBarLayout");
        layout->setContentsMargins(16, 15, 16, 15);
        layout->setSpacing(0);

        m_captionLabel.setObjectName("sesSnackBarCaption");
        m_captionLabel.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        m_captionLabel.setText(m_caption);

        m_messageLabel.setObjectName("sesSnackBarMessage");
        m_messageLabel.setText(m_message);
        m_messageLabel.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        m_messageLabel.setWordWrap(true);

        const auto iconLabel = new QLabel();
        iconLabel->setObjectName("sesSnackBarIcon");
        iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        iconLabel->setFixedSize(16, 16);
        const auto logoIconFileName = Theme::hidpiFileName(":/client/theme/ses/ses-snackBarErrorIcon.svg");
        iconLabel->setPixmap(logoIconFileName);

        layout->addWidget(&m_captionLabel);
        layout->addSpacerItem(new QSpacerItem(8, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));
        layout->addWidget(&m_messageLabel);
        layout->addSpacerItem(new QSpacerItem(8, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));
        layout->addWidget(iconLabel);
        setLayout(layout);

        QString style = QLatin1String("QFrame {border: 1px solid #EEACB2; border-radius: 4px;"
                                      "background-color: #FDF3F4; color: #000000;}"
                                      "QLabel {border: 0px none; padding 0px; background-color: transparent; color: #000000;}"
                                      "QLabel#sesSnackBarCaption {font-weight: bold;}"
                                      );



        setStyleSheet(style);
    }

    QString sesSnackBar::caption() const { return m_caption; }
    QString sesSnackBar::message() const { return m_message; }

    
    void sesSnackBar::setCaption(QString captionText) {
        if (m_caption != captionText) {
            m_caption = captionText;
            m_captionLabel.setText(m_caption);
            emit captionChanged(m_caption);
        }
    }

    void sesSnackBar::setMessage(QString messageText) {
        if (m_message != messageText) {
            m_message = messageText;
            m_messageLabel.setText(m_message);
            emit messageChanged(m_message);
        }
    }

    void sesSnackBar::setWordWrap(bool on)
    {
        m_messageLabel.setWordWrap(on);
    }

    bool sesSnackBar::wordWrap() const
    {
        return m_messageLabel.wordWrap();
    }

    void sesSnackBar::updateStyleSheet()
    {
    }
}