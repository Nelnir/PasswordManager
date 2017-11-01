#ifndef ANIMATEDQPUSHBUTTON_H
#define ANIMATEDQPUSHBUTTON_H

#include <QPushButton>
#include <QPropertyAnimation>
#include "animationmanager.h"

class AnimatedQPushButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
    QColor m_currentColor;
    QPropertyAnimation m_animation;
    AnimationManager<PushButtonDesign>* m_manager;
    QString m_styleSheet;
    bool m_enabled;
private slots:
    void enterEvent(QEvent* e);
    void leaveEvent(QEvent* e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void animFinished();

    void setColor (const QColor& color);
    QColor color(){ return Qt::black; } // getter is not really needed for now
public:
    AnimatedQPushButton(QWidget* l_parent = nullptr);
    void setManager(AnimationManager<PushButtonDesign>* l_manager) { m_manager = l_manager; }
    void updateVisually();
    void setActive(const bool& l_bool);
};

#endif // ANIMATEDQPUSHBUTTON_H
