#ifndef PASSWORDMANAGERCONTROLLER_H
#define PASSWORDMANAGERCONTROLLER_H

#include "shared_global.h"
#include "passwordmanagermodel.h"
#include "accountmanager.h"
#include "usermanager.h"
#include <QString>


class SHAREDSHARED_EXPORT PasswordManagerController{
public:
    PasswordManagerController(PasswordManagerModel* l_model);

    bool toggleRegisterPage();

    void editWidget();

    void login();

    void on_addButton_clicked();
private:
    PasswordManagerModel* m_model;
};

#endif // PASSWORDMANAGERCONTROLLER_H
