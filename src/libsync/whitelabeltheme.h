#ifndef _WHITELABELTHEME_H
#define _WHITELABELTHEME_H

#include <QFont>
#include <QString>
#include "theme.h"

namespace OCC {

class WhitelabelTheme {
public:

static Theme* getInstance() {
    
    #if WHITELABEL_BRAND == "IONOS"
        return IonosTheme::instance();
    #elif WHITELABEL_BRAND == "STRATO"
        return StratoTheme::instance();
    #else
        return Theme::instance();
    #endif
}

private:
    WhitelabelTheme() {}
};
}
#endif // _WHITELABELTHEME_H