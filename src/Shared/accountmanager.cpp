#include "accountmanager.h"
#include <QFileInfo>
#include <QDir>
#include <utility>
#include <QCryptographicHash>

// STRUCT

QDataStream& operator<<(QDataStream& l_stream, const Account& l_acc)
{
    l_stream << l_acc.m_login << l_acc.m_password;
    return l_stream;
}

QDataStream& operator>>(QDataStream& l_stream, Account& l_acc)
{
    l_stream >> l_acc.m_login >> l_acc.m_password;
    return l_stream;
}

bool Account::operator==(const Account& l_acc) const
{
    return m_login.toLower() == l_acc.m_login.toLower() && m_password == l_acc.m_password;
}

QDataStream& operator<<(QDataStream& l_stream, const AccountHashed& l_acc)
{
    l_stream << l_acc.m_login << l_acc.m_password << l_acc.m_salt;
    return l_stream;
}

QDataStream& operator>>(QDataStream& l_stream, AccountHashed& l_acc)
{
    l_stream >> l_acc.m_login >> l_acc.m_password >> l_acc.m_salt;
    return l_stream;
}


QDataStream& operator<<(QDataStream& l_stream, const AccountData& l_acc)
{
    l_stream << l_acc.m_account;
    return l_stream;
}

QDataStream& operator>>(QDataStream& l_stream, AccountData& l_acc)
{
    l_stream >> l_acc.m_account;
    return l_stream;
}

// CLASS

AccountManager::AccountManager() : m_pepper("")
{
    qsrand(QTime::currentTime().msec());
}

AccountManager::~AccountManager()
{
    if(m_needsSave){
        save();
    }
}


bool AccountManager::registerAccount(const Account &l_acc)
{
    if(exists(l_acc)){
        return false;
    }
    auto acc = std::make_unique<AccountData>();
    acc->m_account.m_login = l_acc.m_login;
    acc->m_account.m_salt = generateSalt();
    acc->m_account.m_password = hash(l_acc.m_password.toUtf8(), acc->m_account.m_salt.toUtf8());
    addData(std::move(acc));
    return true;
}

bool AccountManager::removeAccount(const Account &l_acc)
{
    return removeAccount(l_acc.m_login);
}

bool AccountManager::removeAccount(const QString &login)
{
    for(auto& itr : m_data){
        if(itr.second->m_account.m_login.toLower() == login.toLower()){
            m_data.erase(m_data.find(itr.first));
            m_needsSave = true;
            return true;
        }
    }
    return false;
}

bool AccountManager::verifyData(const Account &l_acc) const
{
    if(!exists(l_acc)){
        return false;
    }
    for(auto& itr : m_data){
        QByteArray pass(hash(l_acc.m_password.toUtf8(), itr.second->m_account.m_salt.toUtf8()));
        if(itr.second->m_account.m_password == pass && itr.second->m_account.m_login.toLower() == l_acc.m_login.toLower()) return true;
    }
    return false;
}

bool AccountManager::exists(const Account &l_acc) const
{
    return exists(l_acc.m_login);
}

bool AccountManager::exists(const QString &login) const
{
    for(auto& itr : m_data){
        if(itr.second->m_account.m_login.toLower() == login.toLower()) return true;
    }
    return false;
}

AccountData AccountManager::getAccountWithLogin(const QString &login)
{
    for(auto& itr : m_data){
        if(itr.second->m_account.m_login.toLower() == login.toLower()){
            return *itr.second;
        }
    }
    return AccountData();
}

QByteArray AccountManager::hash(const QByteArray &l_data, const QByteArray& l_salt) const
{
    auto alg = QCryptographicHash::Sha512;
    auto h = QCryptographicHash::hash;
    return
}

QString AccountManager::generateSalt()
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_!@#$%^*");
    const int randomStringLength = 25; // assuming you want random strings of 12 characters

    QString randomString;
    for(int i = 0 ; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}

int AccountManager::loadFromStreamV1(QDataStream &stream)
{
    QString login, salt;
    QByteArray password;
    stream >> login >> password >> salt;
    AccountData data;
    data.m_account.m_login = login;
    data.m_account.m_password = password;
    data.m_account.m_salt = salt;
    if(exists(data.m_account.m_login)){
        return -1;
    }
    return addData(data);
}

void AccountManager::saveToStream(QDataStream &l_stream, const QString &l_login)
{
    l_stream << getAccountWithLogin(l_login);
}
