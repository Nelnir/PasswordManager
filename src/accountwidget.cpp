#include "accountwidget.h"
#include <QVBoxLayout>
#include <QParallelAnimationGroup>

AccountWidget::AccountWidget(QWidget* l_parent, const UserAccount& l_acc, AnimationManager<AccountWidgetDesign>* l_manager) : QWidget(l_parent),
    m_title(this, l_manager),
    m_login("Login:", this, l_manager),
    m_password("Password:", this, l_manager, false),
    m_manager(l_manager),
    m_expanded(false)
{
    setObjectName("AccountWidget");

    initialize(l_acc);
    m_title.updateVisually();
    m_login.updateVisually();
    m_password.updateVisually();

    m_anim.addAnimation(new QPropertyAnimation(&m_login, "minimumHeight"));
    m_anim.addAnimation(new QPropertyAnimation(&m_login, "maximumHeight"));
    m_anim.addAnimation(new QPropertyAnimation(&m_password, "minimumHeight"));
    m_anim.addAnimation(new QPropertyAnimation(&m_password, "maximumHeight"));
    setUp();
}

AccountWidget::~AccountWidget()
{

}

void AccountWidget::initialize(const UserAccount &l_acc)
{
    m_title.setUp(l_acc.m_for);
    m_login.setUp(l_acc.m_login);
    m_password.setUp(l_acc.m_password);
}

void AccountWidget::setUp()
{
    m_expanded = !m_expanded;
    toggleWidgets(true);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(&m_title);
    layout->addWidget(&m_login);
    layout->addWidget(&m_password);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);
    connect(&m_title, &AccountWidgetAbstractLabel::clicked, this, &AccountWidget::expand);
}

void AccountWidget::expand()
{
    toggleWidgets(false);
}

void AccountWidget::toggleWidgets(const bool &l_immediately)
{
    m_expanded = !m_expanded;

    for(int i = 0;i<m_anim.animationCount(); ++i){
        QPropertyAnimation * an = static_cast<QPropertyAnimation*>(m_anim.animationAt(i));
        an->setStartValue(m_login.height());
        an->setEndValue(m_expanded ? 19 : 0);
        an->setDuration(l_immediately ? 0 : 150);
    }

    m_anim.start();
}
