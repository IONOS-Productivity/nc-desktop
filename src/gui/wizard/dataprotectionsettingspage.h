#pragma once

#include <QWizardPage>

#include "wizard/owncloudwizardcommon.h"

namespace OCC {

class OwncloudWizard;
    
namespace Ui {
    class DataProtectionSettingsPage;
}

class DataProtectionSettingsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit DataProtectionSettingsPage(OwncloudWizard *ocWizard);
    ~DataProtectionSettingsPage() override;
    void initializePage() override;

private:
    void setupUi();
    void customizeStyle();
    void setupSlideShow();

    QScopedPointer<Ui::DataProtectionSettingsPage> _ui;

    OwncloudWizard *_ocWizard;

    bool _anonymousUseState;
};

}