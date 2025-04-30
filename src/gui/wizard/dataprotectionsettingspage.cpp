#include "dataprotectionsettingspage.h"
#include "configfile.h"
#include "buttonstyle.h"
#include "guiutility.h"
#include "theme.h"
#include "ui_dataprotectionsettingspage.h"
#include "wizard/owncloudwizard.h"
#include "wizard/slideshow.h"

namespace OCC{

    DataProtectionSettingsPage::DataProtectionSettingsPage(OwncloudWizard *ocWizard)
        : QWizardPage()
        , _ui(new Ui::DataProtectionSettingsPage)
        , _ocWizard(ocWizard)
        , _anonymousUseState(false)
    {
        setupUi();
    }

    DataProtectionSettingsPage::~DataProtectionSettingsPage() = default;

    void DataProtectionSettingsPage::setupUi()
    {
        _ui->setupUi(this);
        setupSlideShow();
    }

    void DataProtectionSettingsPage::initializePage()
    {
        _anonymousUseState = _ui->anonymousDataCheckBox->isChecked();
        customizeStyle();
    }

    void DataProtectionSettingsPage::setupSlideShow()
    {
        ConfigFile cfgFile;

        connect(_ui->backButton, &QPushButton::clicked, this, [this, &cfgFile]() {
            _ui->anonymousDataCheckBox->setChecked(_anonymousUseState);
            cfgFile.setSendData(_anonymousUseState);
            _ocWizard->back();
        });

        connect(_ui->saveButton, &QPushButton::clicked, this, [this, &cfgFile](){
            _anonymousUseState = _ui->anonymousDataCheckBox->isChecked();
            cfgFile.setSendData(_anonymousUseState);
            _ocWizard->back();
        });

        connect(_ui->necessaryDataCheckBox, &QCheckBox::clicked, this, [this](){
            _ui->necessaryDataCheckBox->setChecked(true);
        });
    }

    void DataProtectionSettingsPage::customizeStyle()
    {
        _ui->slideShow->setStyleSheet(
            QStringLiteral("QLabel { %1; }").arg(
                IonosTheme::fontConfigurationCss(
                    IonosTheme::settingsFont(),
                    IonosTheme::settingsSmallTextSize(),
                    IonosTheme::settingsTextWeight(),
                    IonosTheme::titleColor()
                )
            )
        );
        _ocWizard->setFixedSize(626, 460);
        _ui->mainVBox->setContentsMargins(32, 0, 32, 32);
        _ui->necessaryDataCheckBox->setChecked(true);
        _ui->backButton->setMinimumWidth(80);
        _ui->saveButton->setMinimumWidth(160);
        _ui->saveButton->setProperty("buttonStyle", QVariant::fromValue(OCC::ButtonStyleName::Primary));
    }
}