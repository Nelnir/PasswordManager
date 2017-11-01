#ifndef ACCOUNTWIDGETLABEL_H
#define ACCOUNTWIDGETLABEL_H

#include "accountwidgetabstractlabel.h"

class AccountWidgetLabel : public AccountWidgetAbstractLabel
{
public:
    AccountWidgetLabel(const QString& l_title, QWidget* parent, AnimationManager<AccountWidgetDesign>* l_manager, const bool& l_visible = true);
    ~AccountWidgetLabel();
    void setUp(const QString &l_acc);
    void updateVisually();
    const QString& data() { return m_dataText; }
private slots:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    void updateDataText();
    QLabel m_title;
    QLabel m_data;
    QString m_dataText;
    bool m_visible;
};

#endif // ACCOUNTWIDGETLABEL_H
