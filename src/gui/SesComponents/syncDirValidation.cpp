#include "syncDirValidation.h"
#include <QDir>
#include <QStandardPaths>
#include "logger.h"

// Implementation of the validate method
bool SyncDirValidator::isValidDir() {
    // Add actual validation logic here
    #ifdef Q_OS_WIN
      QString appDataPath = SyncDirValidator::appDataPath();
      return !_path.startsWith(appDataPath) && !appDataPath.startsWith(_path); // Return true if validation is successful, false otherwise
    #else
      return true; // For non-Windows systems, always return true
    #endif
}

QString SyncDirValidator::message() {
    return QObject::tr("The directory %1 cannot be part of your sync directory. Please choose another folder.").arg(_path);
}

// Implementation of the appDataPath method
QString SyncDirValidator::appDataPath() {
  //Path: AppData/Roaming/<ApplicationName>
  QString appDataRoamingApplicationNamePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir appDataRoamingApplicationNameDir(appDataRoamingApplicationNamePath);
  appDataRoamingApplicationNameDir.cdUp();
  appDataRoamingApplicationNameDir.cdUp();
  QString appDataPath = appDataRoamingApplicationNameDir.absolutePath();
  return appDataPath;

}