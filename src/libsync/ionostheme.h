#ifndef _IONOSTHEME_H
#define _IONOSTHEME_H

#include <QFont>
#include <QString>
#include "theme.h"
#include "basetheme.h"

namespace OCC {

class IonosTheme : public BaseTheme {
public:
    IonosTheme() = default;

     QString additionalThemePrefix() const override { return QStringLiteral(""); }
};
}
#endif // _IONOSTHEME_H