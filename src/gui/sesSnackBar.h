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
        Q_PROPERTY(QString message READ message)
        Q_PROPERTY(QString error WRITE setError NOTIFY errorChanged)
        Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap)

    public:
        explicit sesSnackBar(QWidget* parent = nullptr);
        QString caption() const;
        QString message() const;
        bool wordWrap() const;
        void clearMessage();
       
    public slots:
        void setCaption(QString captionText);
        void setError(QString errorMessage);

        void setWordWrap(bool on);
    
    signals:
        void captionChanged(QString captionText);
        void errorChanged(QString errorText);

    private:
        QString m_caption;
        QString m_message;

        QLabel m_messageLabel;
        QLabel m_captionLabel;
        
        QLabel m_iconLabel;

        void updateStyleSheet(QColor frameBorderColor, QColor frameBackgroundColor, QColor frameColor, QColor labelColor);
        void setMessage(QString messageText);

        void errorStyle();
    };
}
#endif // SESSNACKBAR_H
