#ifndef WHITELABELTHEME_H
#define WHITELABELTHEME_H
// #define WHITELABEL_IONOS
#define WHITELABEL_STRATO

#include "basetheme.h"
#include "stratotheme.h"
#include "ionostheme.h"

namespace OCC {


#if defined(WHITELABEL_IONOS)
    static IonosTheme WLTheme;    
#elif defined(WHITELABEL_STRATO)
    static StratoTheme WLTheme;
#else
    static BaseTheme WLTheme;
#endif

} // namespace OCC

#endif // WHITELABELTHEME_H
