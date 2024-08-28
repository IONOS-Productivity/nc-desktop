#ifndef SESSNACKBAR_H
#define SESSNACKBAR_H

#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QStylePainter>
#include <QStyleOptionButton>

namespace OCC {

    class sesSnackBar : public QFrame
    {
        Q_OBJECT
        Q_PROPERTY(QString caption READ caption WRITE setCaption NOTIFY captionChanged)
        Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
        Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap)

    public:
        explicit sesSnackBar(QWidget* parent = nullptr);
        QString caption() const;
        QString message() const;
       
    public slots:
        void setCaption(QString captionText);
        void setMessage(QString messageText);

        void setWordWrap(bool on);
        bool wordWrap() const;
    
    signals:
        void captionChanged(QString captionText);
        void messageChanged(QString messageText);

    private:
        QString m_caption;
        QString m_message;

        QLabel m_messageLabel;
        QLabel m_captionLabel;
        
        QLabel m_iconLabel;

        void updateStyleSheet();

        void errorStyle();
    };
}
#endif // SESSNACKBAR_H
