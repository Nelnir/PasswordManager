#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include "abstractfilemanager.h"
#include "shared_global.h"
#include <QString>
#include <QFile>
#include <QDate>
#include <QVector>
#include <QDataStream>
#include <memory>

struct Account{
    QString m_login;
    QString m_password;
    friend QDataStream& operator<<(QDataStream& l_stream, const Account& l_acc);
    friend QDataStream& operator>>(QDataStream& l_stream, Account& l_acc);
    bool operator==(const Account& l_acc) const;
};

struct AccountHashed{
    QString m_login;
    QString m_salt;
    QByteArray m_password;
    friend QDataStream& operator<<(QDataStream& l_stream, const AccountHashed& l_acc);
    friend QDataStream& operator>>(QDataStream& l_stream, AccountHashed& l_acc);
};

struct AccountData{
    AccountHashed m_account;
    friend QDataStream& operator<<(QDataStream& l_stream, const AccountData& l_acc);
    friend QDataStream& operator>>(QDataStream& l_stream, AccountData& l_acc);
};

class SHAREDSHARED_EXPORT AccountManager : public BaseFileManager<AccountData>
{
public:
    AccountManager();
    ~AccountManager();
    bool registerAccount(const Account& l_acc);
    bool removeAccount(const Account& l_acc);
    bool removeAccount(const QString& login);
    bool verifyData(const Account& l_acc) const;
    bool exists(const Account& l_acc) const;
    bool exists(const QString& l_login) const;
    QByteArray hash(const QByteArray& l_data, const QByteArray& l_salt) const;
    AccountData getAccountWithLogin(const QString& l_login);

    void saveToStream(QDataStream& l_stream, const QString& l_login);
    int loadFromStreamV1(QDataStream& l_stream);
private:
    QString generateSalt();

    const QString m_pepper;
};

#endif // ACCOUNTMANAGER_H
