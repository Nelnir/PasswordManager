#include "animatedqpushbutton.h"
#include <QMouseEvent>
AnimatedQPushButton::AnimatedQPushButton(QWidget *l_parent) : QPushButton(l_parent), m_animation(this, "color"), m_manager(nullptr), m_enabled(isEnabled())
{
    m_currentColor = QColor(255, 255, 255, 255);
    connect(&m_animation, &QPropertyAnimation::finished, this, &AnimatedQPushButton::animFinished);
}

void AnimatedQPushButton::setColor(const QColor& color)
{
    if(color.alpha() > 1 || color.alpha() == 0){ /// black flashilghts
        setStyleSheet(m_styleSheet.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
        m_currentColor = color;
    }
}

void AnimatedQPushButton::setActive(const bool &l_bool)
{
    if(m_enabled == l_bool){
        return;
    }
    m_enabled = l_bool;
    auto& design = m_manager->design();

   m_animation.setStartValue(m_currentColor);
   m_animation.setDuration(design->m_delay);
   m_animation.stop();
    if(m_enabled){
        setEnabled(true);
        m_animation.setEndValue(design->m_defaultColor);
    } else {
        m_animation.setEndValue(design->m_disabledColor);
    }
    m_animation.start();
}

void AnimatedQPushButton::updateVisually()
{
    auto& design = m_manager->design();

    QColor color;
    m_styleSheet = QString("QPushButton { background: rgba(%1, %2, %3, %4); border: 1px solid transparent; outline: none;}");

    color = design->m_textColor;
    QString text("QPushButton { color: rgba(%1,%2,%3,%4); }");
    text = text.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());

    color = design->m_disabledColor;
    QString disabled("QPushButton:disabled { background: rgba(%1,%2,%3,%4);}");
    disabled = disabled.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());

    color = design->m_pressedColor;
    QString pressed("QPushButton:pressed { background: rgba(%1, %2, %3, %4); border: 1px hidden transparent; } ");
    pressed = pressed.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());

    color = design->m_checkedColor;
    QString checked("QPushButton:checked { background: rgba(%1,%2,%3,%4); } QPushButton:hover:checked { background: rgba(%5,%6,%7,%8); }");
    checked = checked.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha())
              .arg(color.lighter(110).red()).arg(color.lighter(110).green()).arg(color.lighter(110).blue()).arg(color.alpha());

    color = design->m_focusedColor;
    QString focused("QPushButton:focus { background: rgba(%1,%2,%3,%4); }");
    focused = focused.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    m_styleSheet.append(focused);

    color = design->m_focusedText;
    if(color.isValid()){
        focused = QString("QPushButton:focus { color: rgba(%1 ,%2, %3, %4); }").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
        m_styleSheet.append(focused);
    }

    m_styleSheet.append(checked);
    m_styleSheet.append(disabled);
    m_styleSheet.append(text);
    m_styleSheet.append(pressed);

    if(m_enabled){
        color = design->m_defaultColor;
    } else{
        color = design->m_disabledColor;
    }
    m_currentColor = color;
    setStyleSheet(m_styleSheet.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
}

void AnimatedQPushButton::animFinished()
{
    if(m_enabled){
        return;
    }
    setEnabled(false);
}

void AnimatedQPushButton::enterEvent(QEvent *e)
{
    Q_UNUSED(e)
    if(!m_enabled){
        return;
    }
    m_animation.stop();
    m_animation.setStartValue(m_currentColor);
    m_animation.setEndValue(m_manager->design()->m_hoverColor);
    m_animation.setDuration(m_manager->design()->m_delay);
    m_animation.start();
}

void AnimatedQPushButton::leaveEvent(QEvent *e)
{
    Q_UNUSED(e)
    if(!m_enabled){
        return;
    }
    m_animation.stop();
    m_animation.setStartValue(m_currentColor);
    m_animation.setEndValue(m_manager->design()->m_defaultColor);
    m_animation.setDuration(m_manager->design()->m_delay);
    m_animation.start();
}

void AnimatedQPushButton::mousePressEvent(QMouseEvent *e)
{
    if(!m_enabled){
        e->ignore();
        return;
    }
    QPushButton::mousePressEvent(e);
}

void AnimatedQPushButton::mouseReleaseEvent(QMouseEvent *e)
{

    QPushButton::mouseReleaseEvent(e);
}
