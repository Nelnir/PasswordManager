#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "accountmanager.h"
#include <memory>
#include <QDir>

class AccountManagerTest : public testing::Test{
public:
    virtual void SetUp(){
        m_1 = std::make_unique<AccountManager>();
    }
    virtual void TearDown(){

    }
protected:
    std::unique_ptr<AccountManager> m_1;
};

TEST_F(AccountManagerTest, RegisteringAccount)
{
    Account acc;
    acc.m_login = "login";
    acc.m_password = "password";
    size_t accounts = m_1->size();
    EXPECT_FALSE(m_1->exists(acc));
    EXPECT_TRUE(m_1->registerAccount(acc));
    EXPECT_EQ(accounts + 1, m_1->size());
    EXPECT_TRUE(m_1->exists(acc));
    EXPECT_FALSE(m_1->registerAccount(acc));
}

TEST_F(AccountManagerTest, RemovingAccount)
{
    Account acc;
    acc.m_login = "login";
    acc.m_password = "password";
    size_t accounts = m_1->size();
    EXPECT_FALSE(m_1->removeAccount(acc));
    m_1->registerAccount(acc);
    EXPECT_TRUE(m_1->removeAccount(acc));
    EXPECT_EQ(accounts, m_1->size());
}

TEST_F(AccountManagerTest, VeryfingData)
{
    Account acc;
    acc.m_login = "login";
    acc.m_password = "password";
    m_1->registerAccount(acc);
    EXPECT_TRUE(m_1->verifyData(acc));
    acc.m_login = "LOGIN";
    EXPECT_TRUE(m_1->verifyData(acc));
    acc.m_password = "Password";
    EXPECT_FALSE(m_1->verifyData(acc));
}
