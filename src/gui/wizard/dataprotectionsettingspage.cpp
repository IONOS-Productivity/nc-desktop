#include "dataprotectionsettingspage.h"
#include "configfile.h"
#include "buttonstyle.h"
#include "guiutility.h"
#include "theme.h"
#include "ui_dataprotectionsettingspage.h"
#include "wizard/owncloudwizard.h"

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

        _ui->descriptionLabel->setText(tr("We collect anonymized data to optimize our app. We use software solutions from various partners for this purpose. We want to give you full transparency and freedom of choice regarding the collection and processing of your anonymized usage. You can change your settings at any time under the menu item Data Protection."));
    
        _ui->anonymousDataCheckBox->setChecked(true);
    }

    void DataProtectionSettingsPage::customizeStyle()
    {
        _ocWizard->setFixedSize(626, 460);

        _ui->mainVBox->setContentsMargins(32, 0, 32, 0);

        _ui->necessaryDataCheckBox->setStyleSheet(
            QStringLiteral("QCheckBox { %1; }").arg(
                IonosTheme::fontConfigurationCss(
                    IonosTheme::settingsFont(),
                    IonosTheme::settingsTextSize(),
                    IonosTheme::settingsTitleWeight600(),
                    IonosTheme::folderWizardSubtitleColor()
                )
            ));

            _ui->anonymousDataCheckBox->setStyleSheet(
                QStringLiteral("QCheckBox { %1; }").arg(
                    IonosTheme::fontConfigurationCss(
                        IonosTheme::settingsFont(),
                        IonosTheme::settingsTextSize(),
                        IonosTheme::settingsTitleWeight600(),
                        IonosTheme::folderWizardSubtitleColor()
                    )
                ));

        _ui->necessaryDataCheckBox->setChecked(true);

        _ui->descriptionLabel->setStyleSheet(
            QStringLiteral("QLabel { %1; margin-top: %2; margin-bottom: %2; }").arg(
                IonosTheme::fontConfigurationCss(
                    IonosTheme::settingsFont(),
                    IonosTheme::settingsTextSize(),
                    IonosTheme::settingsTextWeight(),
                    IonosTheme::black()
                ),
                "24"
            )
        );

        _ui->necessaryDataLabel->setStyleSheet(QStringLiteral("QLabel { margin-left: %1; margin-bottom: %2; }")
            .arg("16", IonosTheme::smallMargin()));
        _ui->anonymousDataLabel->setStyleSheet(QStringLiteral("QLabel { margin-left: %1; margin-bottom: %2; }")
            .arg("16", "48"));

        _ui->buttonLayout->setAlignment(Qt::AlignCenter);
        _ui->buttonLayout->setSpacing(16);
        _ui->buttonLayout->setContentsMargins(0, 16, 0, 16);

        _ui->backButton->setMinimumWidth(80);
        _ui->saveButton->setMinimumWidth(140);
        _ui->saveButton->setProperty("buttonStyle", QVariant::fromValue(OCC::ButtonStyleName::Primary));
    }
}