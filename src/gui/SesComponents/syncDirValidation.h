#ifndef SYNCDIRVALIDATION_H
#define SYNCDIRVALIDATION_H

#include <QDir>

class SyncDirValidator {
public:
    // Method to validate the path and users
    bool isValidDir(const QString &path);
};

#endif // SYNCDIRVALIDATION_H