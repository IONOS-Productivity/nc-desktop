#include "dataprotectionpage.h"
#include "buttonstyle.h"
#include "guiutility.h"
#include "theme.h"
#include "ui_dataprotectionpage.h"
#include "wizard/owncloudwizard.h"
#include "wizard/slideshow.h"

namespace OCC{

    DataProtectionPage::DataProtectionPage(OwncloudWizard *ocWizard)
        : QWizardPage()
        , _ui(new Ui::DataProtectionPage)
        , _ocWizard(ocWizard)
    {
        setupUi();
    }

    DataProtectionPage::~DataProtectionPage() = default;

    void DataProtectionPage::setupUi()
    {
        _ui->setupUi(this);
        setupSlideShow();
    }

    void DataProtectionPage::initializePage()
    {
        customizeStyle();
    }

    void DataProtectionPage::styleSlideShow()
    {
        const auto dataProtectionLogoFileName = Theme::hidpiFileName(":/client/theme/colored/data-protection-logo.png");

        _ui->slideShow->addSlide(dataProtectionLogoFileName, tr("This application uses tracking technologies. By clicking on Agree, you accept the processing of your anonymized data. You can adjust your choices at any time via the settings.Information on data processing and more can be found in our privacy policy")); 
    }

    void DataProtectionPage::setupSlideShow()
    {
        connect(_ui->agreeButton, &QPushButton::clicked, this, [this]() {
            _nextPage = WizardCommon::Page_AdvancedSetup; 
            _ocWizard->next();
        });

        connect(_ui->settingsButton, &QPushButton::clicked, this, [this](){
            _nextPage = WizardCommon::Page_DataProtectionSettings; 
            _ocWizard->next();
        });
    }

    int DataProtectionPage::nextId() const
    {
        return _nextPage;
    }

    void DataProtectionPage::customizeStyle()
    {
        _ui->slideShow->setStyleSheet(
            QStringLiteral("QLabel { %1; }").arg(
                IonosTheme::fontConfigurationCss(
                    IonosTheme::settingsFont(),
                    IonosTheme::settingsTextSize(),
                    "0",
                    IonosTheme::titleColor()
                )
            )
        );
        _ocWizard->setFixedSize(626, 460);
        _ui->mainVBox->setContentsMargins(24, 0, 24, 24);   
        _ui->agreeButton->setMinimumWidth(80);
        _ui->settingsButton->setMinimumWidth(80);

        styleSlideShow();

        _ui->buttonLayout->setContentsMargins(160, 0, 160, 0);
        _ui->agreeButton->setProperty("buttonStyle", QVariant::fromValue(OCC::ButtonStyleName::Primary));
    }
}