#include "copydata.h"
#include <QFile>
#include "passwordmanagermodel.h"
#include <QDebug>

CopyData::CopyData(PasswordManagerModel* l_mgr) : m_model(l_mgr)
{

}

CopyData::~CopyData()
{

}

bool CopyData::Export(const QString &l_file)
{
    m_model->m_userManager.encrypt();

    QFile file(l_file);
    file.open(QIODevice::WriteOnly);
    file.resize(0);

    QDataStream stream(&file);

    stream << QString("psm") << m_model->m_version;
    m_model->m_accountManager.saveToStream(stream, m_model->m_userManager.user());
    m_model->m_userManager.saveToStream(stream);

    file.close();

    m_model->m_userManager.decrypt();
    return true;
}

ImportError CopyData::Import(const QString &l_file)
{
    QFile file(l_file);
    file.open(QIODevice::ReadOnly);

    QDataStream stream(&file);

    QString type;
    stream >> type;
    if(type != "psm"){
        file.close();
        return ImportError::IncorrectFileType;
    }

    QString version;
    stream >> version;

    if(version == "1.0"){
        int id = m_model->m_accountManager.loadFromStreamV1(stream);
        if(id == -1){
            file.close();
            return ImportError::AccountExist;
        }
        m_model->m_userManager.loadFromStreamV1(stream, QString("/data/%1/data.pm").arg((*m_model->m_accountManager.getItemWithId(id))->m_account.m_login));
    }

    file.close();
    return ImportError::Success;
}
