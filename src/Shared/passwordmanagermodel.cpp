#include "passwordmanagermodel.h"

PasswordManagerModel::PasswordManagerModel() :
    m_isRegisterPageEnabled(false),
    m_registerLogin(State::Normal),
    m_registerPassword(State::Normal),
    m_loginPageNeedReset(false),
    m_loginLogin(State::Normal),
    m_loginPassword(State::Normal),
    m_isResizingEnabled(false),
    m_addPageNeedReset(false),
    m_addForLineState(State::Normal),
    m_addLoginLineState(State::Normal),
    m_addPasswordLineState(State::Normal),
    m_addPageState(AddPageState::Add),
    m_version("1.0"),
    m_releaseDate("26.10.2017"),
    m_author("Marcin Dzieciątkowski")
{

}

bool PasswordManagerModel::createAccount(const QString &l_login, const QString &l_password)
{
    m_registerLogin = verifyRegisterLoginText(l_login);
    m_registerPassword = verifyRegisterPassText(l_password);
    return m_registerLogin == State::Good && m_registerPassword == State::Good;
}

bool PasswordManagerModel::updateLoginPage()
{
    if(m_loginPageNeedReset){
        m_loginPageNeedReset = false;
        return true;
    }
    return false;
}

bool PasswordManagerModel::addUserAccount(const UserAccount &l_acc)
{
    bool success = false;
    if(l_acc.m_for.isEmpty()){
        m_addForLineState = State::Wrong;
    } else{
        success = true;
        m_addForLineState = State::Good;
    }

    if(l_acc.m_login.isEmpty()){
        success = false;
        m_addLoginLineState = State::Wrong;
    } else{
        m_addLoginLineState = State::Good;
    }

    if(l_acc.m_password.isEmpty()){
        success = false;
        m_addPasswordLineState = State::Wrong;
    } else{
        m_addPasswordLineState = State::Good;
    }

    return success;
}

void PasswordManagerModel::resetRegisterPage()
{
    m_registerLogin = m_registerPassword = State::Normal;
}

bool PasswordManagerModel::updateAddPage()
{
    if(m_addPageNeedReset){
        m_addPageNeedReset = false;
        return true;
    }
    return false;
}

State PasswordManagerModel::verifyRegisterLoginText(const QString &l_text)
{
    State state;
    if(l_text.isEmpty()) state = State::Normal;
    else if(l_text.length() < MINIMUM_LOGIN_LENGTH) state = State::Wrong;
    else state = State::Good;

    for(auto& itr : l_text){
        if(itr.isSpace()) return State::Wrong;
    }
    return state;
}
State PasswordManagerModel::verifyRegisterPassText(const QString &l_text)
{
    State state = verifyBasicPasswordRequirments(l_text);
    if(state == State::Wrong) return state;
    else if(state == State::Good){
        if(l_text.length() < MINIMUM_PASSWORD_LENGTH){
            state = State::Wrong;
        }
    }


    return state;
}

State PasswordManagerModel::verifyBasicPasswordRequirments(const QString &l_text)
{
    if(l_text.isEmpty()) return State::Normal;
    bool upper = false, digit = false, letter = false, lower = false;
    for(auto& itr : l_text){
        if(itr.isUpper()) upper = true;
        if(itr.isDigit()) digit = true;
        if(itr.isLower()) lower = true;
        if(itr.isLetter()) letter = true;
    }
    if(upper && digit && letter && lower) return State::Good;
    return State::Wrong;
}

bool PasswordManagerModel::login(const Account &l_acc)
{
    bool tmp = false;
    if(l_acc.m_login.isEmpty()){
       m_loginLogin = State::Normal;
    } else if(m_accountManager.exists(l_acc)){
        m_loginLogin = State::Good;
        tmp = true;
    } else{
        m_loginLogin = State::Wrong;
    }

    if(l_acc.m_password.isEmpty()){
        m_loginPassword = State::Normal;
    } else if(m_accountManager.verifyData(l_acc)){
        m_loginPassword = State::Good;
        if(tmp) return true;
    } else {
        m_loginPassword = State::Wrong;
    }

    return false;
}

bool PasswordManagerModel::updateMorePage()
{
    if(m_isRegisterPageEnabled){
         m_isRegisterPageEnabled = false;
        return true;
    }
    return false;
}

void PasswordManagerModel::setLoginPage()
{
    m_isResizingEnabled = false;
}

void PasswordManagerModel::setAddPage()
{
    m_isResizingEnabled = false;
}

void PasswordManagerModel::setUserPage()
{
    m_isResizingEnabled = true;
}

void PasswordManagerModel::setAboutPage()
{
    m_isResizingEnabled = false;
}

void PasswordManagerModel::setMorePage()
{
    m_isResizingEnabled = false;
}

void PasswordManagerModel::setExtrasPage()
{
    m_isResizingEnabled = false;
}
