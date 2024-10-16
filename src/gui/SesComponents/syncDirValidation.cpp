#include "syncDirValidation.h"
#include <QDir>

// Implementation of the validate method
bool SyncDirValidator::isValidDir(const QString &path) {
    // Add actual validation logic here
    #ifdef Q_OS_WIN
      return !QDir::homePath().startsWith(path); // Return true if validation is successful, false otherwise
    #else
      return true; // For non-Windows systems, always return true
    #endif
}

QString SyncDirValidator::message() {
    return QObject::tr("The home directory cannot be part of your sync directory. Please choose another folder.");
}