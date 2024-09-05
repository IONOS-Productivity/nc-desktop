#include "sesfileiconprovider.h"

#include <QFileIconProvider>
#include <QIcon>

QIcon SesFileIconProvider::icon(QFileInfo fileInfo)
{
    QFileIconProvider provider;

    if (fileInfo.isDir())
    {
        return QIcon(":/client/theme/ses/ses-filesIcon.svg");
    }

    if (fileInfo.suffix().isEmpty())
    {
        return QIcon(":/client/theme/ses/ses-file.svg");
    }
    
    
    return provider.icon(fileInfo);
};