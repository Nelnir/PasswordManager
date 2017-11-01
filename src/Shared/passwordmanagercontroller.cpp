#include "passwordmanagercontroller.h"
PasswordManagerController::PasswordManagerController(PasswordManagerModel* l_model) : m_model(l_model)
{

}

void PasswordManagerController::login()
{
    m_model->m_loginPageNeedReset = true;
    m_model->m_loginLogin = m_model->m_loginPassword = State::Normal;
}

void PasswordManagerController::editWidget()
{
    m_model->m_addPageNeedReset = true;
    m_model->m_addPageState = AddPageState::Edit;
    m_model->m_addForLineState = m_model->m_addLoginLineState = m_model->m_addPasswordLineState = State::Normal;
}

bool PasswordManagerController::toggleRegisterPage()
{
    m_model->m_isRegisterPageEnabled = !m_model->m_isRegisterPageEnabled;
    return m_model->m_isRegisterPageEnabled;
}

void PasswordManagerController::on_addButton_clicked()
{
    m_model->m_addPageNeedReset = true;
    m_model->m_addPageState = AddPageState::Add;
    m_model->m_addForLineState = m_model->m_addLoginLineState = m_model->m_addPasswordLineState = State::Normal;
}
