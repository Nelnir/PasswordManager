#ifndef ACCOUNTWIDGETTITLE_H
#define ACCOUNTWIDGETTITLE_H

#include "accountwidgetabstractlabel.h"

class AccountWidgetTitle : public AccountWidgetAbstractLabel
{
public:
    AccountWidgetTitle(QWidget* parent, AnimationManager<AccountWidgetDesign>* l_manager);
    ~AccountWidgetTitle();
    void setUp(const QString &l_acc);
    void updateVisually();
    QString title(){ return m_title.text(); }
private:
    QLabel m_title;
};

#endif // ACCOUNTWIDGETTITLE_H
