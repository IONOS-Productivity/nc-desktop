#ifndef SYNCDIRVALIDATION_H
#define SYNCDIRVALIDATION_H

#include <QDir>

class SyncDirValidator {
public:
    // Constructor that takes a string path
    SyncDirValidator(const QString &path) : _path(path) {}

    // Method to validate the path and users
    bool isValidDir();
    QString message();

private:
    // Add any private methods or variables here
    QString appDataPath();
    QString _path; // Member variable to store the path
};;


#endif // SYNCDIRVALIDATION_H