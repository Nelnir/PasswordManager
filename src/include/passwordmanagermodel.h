#ifndef PASSWORDMANAGERMODEL_H
#define PASSWORDMANAGERMODEL_H

#include "shared_global.h"
#include <QString>
#include "usermanager.h"
#include "accountmanager.h"


#define MINIMUM_PASSWORD_LENGTH 8
#define MINIMUM_LOGIN_LENGTH 4

enum class State{Normal, Good, Wrong};
enum class AddPageState{Add, Edit};

class SHAREDSHARED_EXPORT PasswordManagerModel{
    friend class PasswordManagerController;
    friend class PasswordManager;
    friend class CopyData;
public:
    PasswordManagerModel();
    const State& registerLoginState() const { return m_registerLogin; }
    const State& registerPasswordState() const { return m_registerPassword; }
    const bool& isRegisterPageEnabled() const { return m_isRegisterPageEnabled; }
    const bool& isResizingEnabled() const { return m_isResizingEnabled; }
    const State& loginLoginState() const { return m_loginLogin; }
    const State& loginPasswordState() const { return m_loginPassword; }
    const State& addForLineState() const { return m_addForLineState; }
    const State& addLoginLineState() const { return m_addLoginLineState; }
    const State& addPasswordLineState() const { return m_addPasswordLineState; }
    const AddPageState& addPageState() const { return m_addPageState; }
    void setLoginState(const State& l_state) { m_registerLogin = l_state; }

    bool updateMorePage();

    bool updateLoginPage();
    bool updateAddPage();
    bool addUserAccount(const UserAccount& l_acc);

    void resetRegisterPage();
    bool createAccount(const QString &l_login, const QString &l_password);
    bool login(const Account& l_acc);

    void setLoginPage();
    void setAddPage();
    void setUserPage();
    void setAboutPage();
    void setMorePage();
    void setExtrasPage();

    ///
    AccountManager m_accountManager;
    UserManager m_userManager;
private:
    State verifyRegisterLoginText(const QString& l_text);
    State verifyRegisterPassText(const QString& l_text);
    State verifyBasicPasswordRequirments(const QString&l_text);

    ///
    bool m_isRegisterPageEnabled;
    State m_registerLogin;
    State m_registerPassword;

    ///
    bool m_loginPageNeedReset;
    State m_loginLogin;
    State m_loginPassword;

    ///
    bool m_isResizingEnabled;

    ///
    bool m_addPageNeedReset;
    State m_addForLineState;
    State m_addLoginLineState;
    State m_addPasswordLineState;
    AddPageState m_addPageState;

    ///
    const QString m_version;
    const QString m_releaseDate;
    const QString m_author;
};

#endif // PASSWORDMANAGERMODEL_H
