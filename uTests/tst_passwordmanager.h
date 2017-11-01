#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "passwordmanagercontroller.h"
#include "passwordmanagermodel.h"
#include <memory>

class ModelAndControllerTest : public testing::Test{
public:
    virtual void SetUp(){
        model = std::make_unique<PasswordManagerModel>();
        controller = std::make_unique<PasswordManagerController>(model.get());
    }
    virtual void TearDown(){

    }
protected:
    std::unique_ptr<PasswordManagerController> controller;
    std::unique_ptr<PasswordManagerModel> model;
};

TEST_F(ModelAndControllerTest, GUI_Initialization)
{
    EXPECT_EQ(false, model->isRegisterPageEnabled());
    EXPECT_EQ(State::Normal, model->registerLoginState());
    EXPECT_EQ(State::Normal, model->registerPasswordState());
}

TEST_F(ModelAndControllerTest, GUI_SwitchingToMorePage)
{
    controller->toggleRegisterPage();
    EXPECT_TRUE(model->isRegisterPageEnabled());
    EXPECT_FALSE(model->updateLoginPage());
    EXPECT_TRUE(model->isRegisterPageEnabled());
    EXPECT_TRUE(model->updateMorePage());
    EXPECT_FALSE(model->isRegisterPageEnabled());
}

TEST_F(ModelAndControllerTest, GUI_CreatingAccountAndResetting)
{
    QString login("login");
    QString password("Password");
    EXPECT_FALSE(model->createAccount(login, password));
    EXPECT_EQ(State::Good, model->registerLoginState());
    EXPECT_EQ(State::Wrong, model->registerPasswordState());
    password += "1";
    EXPECT_TRUE(model->createAccount(login, password));
    EXPECT_EQ(State::Good, model->registerPasswordState());

    model->resetRegisterPage();

    EXPECT_EQ(State::Normal, model->registerPasswordState());
    EXPECT_EQ(State::Normal, model->registerLoginState());
}

TEST_F(ModelAndControllerTest, GUI_Logout)
{
    EXPECT_FALSE(model->updateLoginPage());
    model->setUserPage();
    EXPECT_FALSE(model->updateLoginPage());
    controller->login();
    EXPECT_TRUE(model->updateLoginPage());
}

TEST_F(ModelAndControllerTest, GUI_LoginPageResetting)
{
    Account acc;
    acc.m_login = "login";
    acc.m_password = "password";
    EXPECT_TRUE(model->m_accountManager.registerAccount(acc));
    EXPECT_EQ(State::Normal, model->loginLoginState());
    EXPECT_EQ(State::Normal, model->loginPasswordState());
    acc.m_login = "LOGIN";
    acc.m_password = "passwOrd";
    EXPECT_FALSE(model->login(acc));
    EXPECT_EQ(State::Good, model->loginLoginState());
    EXPECT_EQ(State::Wrong, model->loginPasswordState());

    EXPECT_FALSE(model->updateLoginPage());
    controller->login();
    EXPECT_TRUE(model->updateLoginPage());
    EXPECT_EQ(State::Normal, model->loginLoginState());
    EXPECT_EQ(State::Normal, model->loginPasswordState());
}

TEST_F(ModelAndControllerTest, GUI_AddPageResetting)
{
    EXPECT_FALSE(model->updateAddPage());
    EXPECT_EQ(State::Normal, model->addPasswordLineState());
    EXPECT_EQ(State::Normal, model->addLoginLineState());
    EXPECT_EQ(State::Normal, model->addForLineState());

    UserAccount acc;
    acc.m_login = "login";
    acc.m_password = "password";
    acc.m_for = "";
    EXPECT_FALSE(model->addUserAccount(acc));
    EXPECT_EQ(State::Good, model->addPasswordLineState());
    EXPECT_EQ(State::Good, model->addLoginLineState());
    EXPECT_EQ(State::Wrong, model->addForLineState());

    acc.m_for = "fejsbuk";
    EXPECT_TRUE(model->addUserAccount(acc));
    EXPECT_EQ(State::Good, model->addPasswordLineState());
    EXPECT_EQ(State::Good, model->addLoginLineState());
    EXPECT_EQ(State::Good, model->addForLineState());

    EXPECT_FALSE(model->updateAddPage());
    controller->on_addButton_clicked();
    EXPECT_TRUE(model->updateAddPage());
    EXPECT_EQ(State::Normal, model->addPasswordLineState());
    EXPECT_EQ(State::Normal, model->addLoginLineState());
    EXPECT_EQ(State::Normal, model->addForLineState());
}
