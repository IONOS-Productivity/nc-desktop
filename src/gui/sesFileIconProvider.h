#ifndef SESFILEICONPROVIDER_H
#define SESFILEICONPROVIDER_H

#include <QFileIconProvider>
#include <QIcon>
#include <QFileInfo>

class SesFileIconProvider : public QFileIconProvider
{
public:
    QIcon icon(QFileInfo fileInfo);
};

#endif // SESFILEICONPROVIDER_H