#include "usermanager.h"
#include <QFileInfo>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

QDataStream& operator>>(QDataStream& stream, UserAccount& data)
{
    stream >> data.m_for >> data.m_login >> data.m_password;
    return stream;
}

QDataStream& operator<<(QDataStream& stream, const UserAccount& data)
{
    stream << data.m_for << data.m_login << data.m_password;
    return stream;
}

UserManager::UserManager() : BaseFileManager(), m_crypt()
{

}

UserManager::~UserManager() { }

void UserManager::encrypt()
{
    for(auto& itr : m_data){
        itr.second->m_for = m_crypt.encryptToString(itr.second->m_for);
        itr.second->m_login = m_crypt.encryptToString(itr.second->m_login);
        itr.second->m_password = m_crypt.encryptToString(itr.second->m_password);
    }
}

void UserManager::decrypt()
{
    for(auto& itr : m_data){
        itr.second->m_for = m_crypt.decryptToString(itr.second->m_for);
        itr.second->m_login = m_crypt.decryptToString(itr.second->m_login);
        itr.second->m_password = m_crypt.decryptToString(itr.second->m_password);
    }
}

void UserManager::loadFromStreamV1(QDataStream &stream, const QString& saveTo)
{
    createFile(saveTo);
    QFile file(QDir::currentPath() + saveTo);
    file.open(QIODevice::WriteOnly);
    QDataStream save(&file);

    size_t size = 0;
    stream >> size;
    save << size;
    for(size_t i = 0;i < size; ++i){
        QString forr, login, pass;
        stream >> forr >> login >> pass;
        save << forr << login << pass;
    }
    file.close();
}

void UserManager::saveToStream(QDataStream &l_stream)
{
   l_stream << m_data.size();
   for(auto& itr : m_data){
       l_stream << *itr.second;
   }
}

void UserManager::createFile(const QString &l_file)
{
    const QString directory = QFileInfo(QDir::currentPath() + l_file).absolutePath();
    if(!QDir(directory).exists()){
        QDir().mkpath(directory);
    }
}

bool UserManager::removeUser()
{
    unload();
    bool ret = QDir(QFileInfo(m_cfgFile.fileName()).absoluteDir()).removeRecursively();
    m_loggedUser.clear();
    return ret;
}
