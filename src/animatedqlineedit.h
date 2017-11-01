#ifndef ANIMATEDQLINEEDIT_H
#define ANIMATEDQLINEEDIT_H


#include <QLineEdit>
#include <QPropertyAnimation>
#include "animationmanager.h"
#include "passwordmanagermodel.h"

class AnimatedQLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
    QColor m_currentColor;
    QPropertyAnimation m_animation;
    AnimationManager<LineEditDesign>* m_manager;
    QString m_styleSheet;
    bool m_enabled;
    State m_state;
    bool m_stateJustChanged;
    bool m_isFocused;

    void updateFocusStylesheet();
private slots:
    void enterEvent(QEvent* e);
    void leaveEvent(QEvent* e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void animFinished();
    void focusInEvent(QFocusEvent * e) { m_isFocused = true; QLineEdit::focusInEvent(e); }
    void focusOutEvent(QFocusEvent * e) { m_isFocused = false; QLineEdit::focusOutEvent(e); }


    void setColor (const QColor& color);
    QColor color(){ return Qt::black; } // getter is not really needed for now
public:
    AnimatedQLineEdit(QWidget* l_parent = nullptr);
    void setManager(AnimationManager<LineEditDesign>* l_manager) { m_manager = l_manager; }
    void updateVisually();
    void setActive(const bool& l_bool);
    void setState(const State& l_state, const bool& immediately = false);
};
#endif // ANIMATEDQLINEEDIT_H
