#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include <QMainWindow>
#include "passwordmanagermodel.h"
#include "passwordmanagercontroller.h"
#include "animationmanager.h"
#include "accountwidget.h"
#include <QParallelAnimationGroup>
#include <functional>
#include <unordered_map>
#include <QMenu>

namespace Ui {
class PasswordManager;
}
class QPropertyAnimation;

class PasswordManager : public QMainWindow
{
    Q_OBJECT
public:
    explicit PasswordManager(QWidget *parent = 0);
    ~PasswordManager();
private slots:
    void mousePressEvent(QMouseEvent *event);
    void on_moreButton_clicked();

    void on_backButton_clicked();

    void on_registerButton_clicked();

    void on_createAccountButton_clicked();

    void on_loginButton_clicked();

    void on_aboutButton_clicked();

    void on_backToMorePageButton_clicked();

    void on_logoutButton_clicked();

    void on_addButton_clicked();

    void hideAnimationFinished();

    void on_backToUserPageButton_clicked();

    void on_addAccountButton_clicked();

    void on_searchEdit_textChanged(const QString &arg1);

    void showContextMenu(const QPoint& l_point);

    void removeWidgetActionTriggered();

    void editWidgetActionTriggered();

    void on_extrasButton_clicked();

    void on_backFromExtrasButton_clicked();

    void on_exportButton_clicked();

    void on_importButton_clicked();

    void on_deleteAccountButton_clicked();

private:
    int idOfWidget(AccountWidget* l_widget);
    void removeWidget(AccountWidget* l_widget);
    void setMorePage();
    void setLoginPage();
    void setAboutPage();
    void setUserPage();
    void setAddPage();
    void setExtrasPage();
    void createDesigns();
    void resetRegisterPage(const bool& b);
    void enableRegisterPage(const bool& b);
    void loginWithUser(const Account& l_acc);
    void animateToLoginPage();
    void animateToUserPage();
    void animateToAboutPage();
    void animateToMorePage();
    void animateToAddPage();
    void animateToExtrasPage();
    void toggleRegisterWidgets();
    void addNewAccount(const UserAccount& l_acc, const bool& l_onlyVisually = false, int l_id = -1);
    QWidget* createScrollAreaWidget();
    void createCustomContextMenu();
    void hideAnimation();
    void extendAnimation(const QSize& l_size);
    Ui::PasswordManager *ui;
    std::unordered_map<int, AccountWidget*> m_widgets;
    AnimationManager<PushButtonDesign> m_buttonManager;
    AnimationManager<LineEditDesign> m_lineEditManager;
    AnimationManager<AccountWidgetDesign> m_accWidgetManager;
    PasswordManagerModel m_model;
    PasswordManagerController m_controller;
    QParallelAnimationGroup m_hideAnimation;

    std::function<void()> m_hideAnimationFinish;

    AccountWidget* m_editing;
    QMenu m_customContextMenu;

    const signed int m_hideAnimationDuration;
};

#endif // PASSWORDMANAGER_H
