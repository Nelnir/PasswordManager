#ifndef ACCOUNTWIDGET_H
#define ACCOUNTWIDGET_H

#include "accountwidgettitle.h"
#include "accountwidgetlabel.h"
#include <QParallelAnimationGroup>

class AccountWidget : public QWidget
{
public:
    AccountWidget(QWidget* l_parent, const UserAccount& l_acc, AnimationManager<AccountWidgetDesign>* l_manager);
    ~AccountWidget();
    QString title() { return m_title.title(); }
    QString login() { return m_login.data(); }
    QString password() { return m_password.data(); }
    AccountWidgetTitle* titleWidget() { return &m_title; }
    void initialize(const UserAccount& l_acc);
private slots:
    void expand();
private:
    void setUp();
    void toggleWidgets(const bool& l_immediately);
    AccountWidgetTitle m_title;
    AccountWidgetLabel m_login;
    AccountWidgetLabel m_password;
    QParallelAnimationGroup m_anim;

    AnimationManager<AccountWidgetDesign>* m_manager;
    bool m_expanded;
};

#endif // ACCOUNTWIDGET_H
