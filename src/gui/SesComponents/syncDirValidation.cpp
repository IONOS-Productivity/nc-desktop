#include "syncDirValidation.h"
#include <QDir>

// Implementation of the validate method
bool SyncDirValidator::isValidDir(const QString &path) {
    // Add actual validation logic here
    return !QDir::homePath().startsWith(path); // Return true if validation is successful, false otherwise
}