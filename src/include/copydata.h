#ifndef COPYDATA_H
#define COPYDATA_H

#include "shared_global.h"

enum class ImportError{IncorrectFileType, AccountExist, Success};

class PasswordManagerModel;
class SHAREDSHARED_EXPORT CopyData
{
public:
    CopyData(PasswordManagerModel* l_mgr = nullptr);
    ~CopyData();


    bool Export(const QString& l_file);
    ImportError Import(const QString& l_file);

private:
    PasswordManagerModel* m_model;
};

#endif // USERMANAGER_H
