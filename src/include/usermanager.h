#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "abstractfilemanager.h"
#include "simplecrypt.h"

struct UserAccount{
    QString m_password;
    QString m_login;
    QString m_for;
    friend QDataStream& operator>>(QDataStream& l_stream, UserAccount& l_data);
    friend QDataStream& operator<<(QDataStream& l_stream, const UserAccount& l_data);
};

class SHAREDSHARED_EXPORT UserManager : public BaseFileManager<UserAccount>
{
public:
    UserManager();
    ~UserManager();
    void setUser(const QString& l_user) { m_loggedUser = l_user; }
    QString user() { return m_loggedUser; }
    bool removeUser();
    void decrypt();
    void encrypt();

    void saveToStream(QDataStream& l_stream);
    void loadFromStreamV1(QDataStream& l_stream, const QString& l_file);
private:
    void createFile(const QString& l_file);
    QString m_loggedUser;
    SimpleCrypt m_crypt;
};

#endif // USERMANAGER_H
