/*
 * Copyright (C) by Klaas Freitag <freitag@owncloud.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#ifndef CONNECTIONVALIDATOR_H
#define CONNECTIONVALIDATOR_H

#include "owncloudlib.h"
#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include <QNetworkReply>
#include "accountfwd.h"
#include "clientsideencryption.h"

namespace OCC {

/**
 * This is a job-like class to check that the server is up and that we are connected.
 * There are two entry points: checkServerAndAuth and checkAuthentication
 * checkAuthentication is the quick version that only does the propfind
 * while checkServerAndAuth is doing the 4 calls.
 *
 * We cannot use the capabilities call to test the login and the password because of
 * https://github.com/owncloud/core/issues/12930
 *
 * Here follows the state machine

\code{.unparsed}
*---> checkServerAndAuth  (check status.php)
        Will asynchronously check for system proxy (if using system proxy)
        And then invoke slotCheckServerAndAuth
        CheckServerJob
        |
        +-> slotNoStatusFound --> X
        |
        +-> slotJobTimeout --> X
        |
        +-> slotStatusFound --+--> X (if credentials are still missing)
                              |
  +---------------------------+
  |
*-+-> checkAuthentication (PROPFIND on root)
        PropfindJob
        |
        +-> slotAuthFailed --> X
        |
        +-> slotAuthSuccess --+--> X (depending if coming from checkServerAndAuth or not)
                              |
  +---------------------------+
  |
  +-> checkServerCapabilities --------------v (in parallel)
        JsonApiJob (cloud/capabilities)
        +-> slotCapabilitiesRecieved -+
                                      |
    +---------------------------------+
    |
  fetchUser
        Utilizes the UserInfo class to fetch the user and avatar image
  +-----------------------------------+
  |
  +-> Client Side Encryption Checks --+ --reportResult()
    \endcode
 */

class UserInfo;

class TermsOfServiceChecker : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool needToSign READ needToSign NOTIFY needToSignChanged FINAL)
public:
    explicit TermsOfServiceChecker(AccountPtr account,
                                   QObject *parent = nullptr);

    explicit TermsOfServiceChecker(QObject *parent = nullptr);

    [[nodiscard]] bool needToSign() const;

public slots:
    void start();

signals:
    void needToSignChanged();

    void done();

private slots:
    void slotServerTermsOfServiceRecieved(const QJsonDocument &reply);

private:
    void checkServerTermsOfService();

    AccountPtr _account;
    bool _needToSign = false;
};

class ConnectionValidator : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionValidator(AccountStatePtr accountState,
                                 const QStringList &previousErrors,
                                 QObject *parent = nullptr);

    enum Status {
        Undefined,
        Connected,
        NotConfigured,
        ServerVersionMismatch, // The server version is too old
        CredentialsNotReady, // Credentials aren't ready
        CredentialsWrong, // AuthenticationRequiredError
        SslError, // SSL handshake error, certificate rejected by user?
        StatusNotFound, // Error retrieving status.php
        StatusRedirect, // 204 URL received one of redirect HTTP codes (301-307), possibly a captive portal
        ServiceUnavailable, // 503 on authed request
        MaintenanceMode, // maintenance enabled in status.php
        Timeout, // actually also used for other errors on the authed request
        NeedToSignTermsOfService,
    };
    Q_ENUM(Status);

    // How often should the Application ask this object to check for the connection?
    enum { DefaultCallingIntervalMsec = 62 * 1000 };

public slots:
    /// Checks the server and the authentication.
    void checkServerAndAuth();
    void systemProxyLookupDone(const QNetworkProxy &proxy);

    /// Checks authentication only.
    void checkAuthentication();

signals:
    void connectionResult(OCC::ConnectionValidator::Status status, const QStringList &errors);

protected slots:
    void slotCheckRedirectCostFreeUrl();

    void slotCheckServerAndAuth();

    void slotCheckRedirectCostFreeUrlFinished(int statusCode);

    void slotStatusFound(const QUrl &url, const QJsonObject &info);
    void slotNoStatusFound(QNetworkReply *reply);
    void slotJobTimeout(const QUrl &url);

    void slotAuthFailed(QNetworkReply *reply);
    void slotAuthSuccess();

    void slotCapabilitiesRecieved(const QJsonDocument &);
    void slotUserFetched(OCC::UserInfo *userInfo);

    void termsOfServiceCheckDone();

private:
#ifndef TOKEN_AUTH_ONLY
    void reportConnected();
#endif
    void reportResult(Status status);
    void checkServerCapabilities();
    void fetchUser();

    /** Sets the account's server version
     *
     * Returns false and reports ServerVersionMismatch for very old servers.
     */
    bool setAndCheckServerVersion(const QString &version);

    void checkServerTermsOfService();

    const QStringList _previousErrors;
    QStringList _errors;
    AccountStatePtr _accountState;
    AccountPtr _account;
    TermsOfServiceChecker _termsOfServiceChecker;
    bool _isCheckingServerAndAuth = false;
};
}

#endif // CONNECTIONVALIDATOR_H
