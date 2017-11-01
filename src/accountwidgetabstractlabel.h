#ifndef ACCOUNTWIDGETABSTRACTLABEL_H
#define ACCOUNTWIDGETABSTRACTLABEL_H

#include <QWidget>
#include "usermanager.h"
#include "animationmanager.h"
#include <QPropertyAnimation>
#include <QColor>
#include <QLabel>

class AccountWidgetAbstractLabel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
public:
    AccountWidgetAbstractLabel(QWidget* l_parent, AnimationManager<AccountWidgetDesign>* l_manager);
    ~AccountWidgetAbstractLabel();
    virtual void updateVisually() = 0;
    virtual void setUp(const QString& l_acc) = 0;
signals:
    void clicked();
protected slots:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void setColor (const QColor& color);
    QColor color(){ return Qt::black; } // getter is not really needed for now
protected:
    QColor m_currentColor;
    QPropertyAnimation m_animation;
    AnimationManager<AccountWidgetDesign>* m_manager;
    QString m_styleSheet;
};

#endif // ACCOUNTWIDGETABSTRACTLABEL_H
