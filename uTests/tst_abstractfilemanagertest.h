#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "abstractfilemanager.h"
#include <memory>
#include <QDir>

struct TestData{
    QString m_login;
    QString m_password;
    friend QDataStream& operator>>(QDataStream& stream, TestData& data){
        stream >> data.m_login >> data.m_password;
        return stream;
    }
    friend QDataStream& operator<<(QDataStream& stream, const TestData& data){
        stream << data.m_login << data.m_password;
        return stream;
    }
    bool operator==(const TestData& data) const {
        return m_login.toLower() == data.m_login.toLower() && m_password == data.m_password;
    }
};

class BaseFileManagerTest : public testing::Test{
public:
    virtual void SetUp(){
        m_1 = std::make_unique<BaseFileManager<TestData>>();
    }
    virtual void TearDown(){

    }
protected:
    std::unique_ptr<BaseFileManager<TestData>> m_1;
};

TEST_F(BaseFileManagerTest, CreatingFile)
{
    EXPECT_FALSE(QFile("data/data.cfg").exists());
    EXPECT_TRUE(m_1->load(QString("data/data.cfg")));
    EXPECT_TRUE(QFile("data/data.cfg").exists());
    m_1->unload();
    QDir("data").removeRecursively();
    EXPECT_FALSE(QFile("data/data.cfg").exists());
}

TEST_F(BaseFileManagerTest, SavingToFileAndReading)
{
    TestData acc;
    acc.m_login = "login";
    acc.m_password = "password";
    m_1->load("data/data.cfg");
    EXPECT_EQ(0, m_1->size());
    m_1->addData(acc);
    EXPECT_EQ(1, m_1->size());
    m_1->save();
    m_1->unload();
    EXPECT_EQ(0, m_1->size());
    m_1->load("data/data.cfg");
    ASSERT_EQ(1, m_1->size());
    EXPECT_EQ(acc, *m_1->container().begin()->second);
    m_1->unload();

    QDir("data").removeRecursively();
}

TEST_F(BaseFileManagerTest, ID_Test)
{
    TestData acc;
    acc.m_login = "login";
    acc.m_password = "pass";
    int id = m_1->addData(acc);
    EXPECT_EQ(acc, **m_1->getItemWithId(id));
    int id2 = m_1->addData(acc);
    EXPECT_NE(id, id2);

    m_1->unload();
}

TEST_F(BaseFileManagerTest, ReplaceTest)
{
    TestData acc1;
    acc1.m_login = "login";
    acc1.m_password = "pass";
    TestData acc2;
    acc2.m_login = "pass";
    acc2.m_password = "login";
    int id = m_1->addData(acc1);
    int id2 = m_1->addData(acc2);
    EXPECT_TRUE(m_1->replace(id, acc2));
    EXPECT_EQ(**m_1->getItemWithId(id), acc2);
    EXPECT_FALSE(m_1->replace(id2 + 1, acc1));
}
