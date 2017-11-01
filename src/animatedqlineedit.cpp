#include "animatedqlineedit.h"
#include <QMouseEvent>

AnimatedQLineEdit::AnimatedQLineEdit(QWidget *l_parent) : QLineEdit(l_parent), m_animation(this, "color"), m_manager(nullptr), m_enabled(isEnabled()),
    m_state(State::Normal), m_isFocused(false), m_stateJustChanged(false)
{
    m_currentColor = QColor(255, 255, 255, 255);
    connect(&m_animation, &QPropertyAnimation::finished, this, &AnimatedQLineEdit::animFinished);
}

void AnimatedQLineEdit::setColor(const QColor& color)
{
    if(true){ /// black flashilghts
        setStyleSheet(m_styleSheet.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
        m_currentColor = color;
    }
}

void AnimatedQLineEdit::setActive(const bool &l_bool)
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
void AnimatedQLineEdit::setState(const State &l_state, const bool& immediately)
{
    if(m_state == l_state){
        return;
    }
    m_state = l_state;
    m_animation.stop();
    if(!m_isFocused){
        m_animation.setStartValue(m_currentColor);
    } else{
        m_animation.setStartValue(m_manager->design()->m_pressedColor);
    }
    QColor end;
    if(m_state == State::Normal) end = m_manager->design()->m_defaultColor;
    else if(m_state == State::Good) end = m_manager->design()->m_goodColor;
    else if(m_state == State::Wrong) end = m_manager->design()->m_wrongColor;
    m_animation.setEndValue(end);
    if(!immediately){
        m_animation.setDuration(m_manager->design()->m_delay);
    } else m_animation.setDuration(0);

    if(m_state != State::Normal) end = end.lighter(130);

    if(m_manager->design()->m_enablePressedColor){
        if(!m_isFocused){
            updateFocusStylesheet();
        } else {
            m_animation.setEndValue(end);
                if(m_styleSheet.count(":focus") == 2){
                    m_stateJustChanged = true;
                    int index = m_styleSheet.lastIndexOf("QLineEdit:focus");
                    m_styleSheet.remove(index, m_styleSheet.length() - index);
            }
        }
    }
    m_animation.start();
}

void AnimatedQLineEdit::updateFocusStylesheet()
{
    QColor end;
    if(m_state == State::Good) end = m_manager->design()->m_goodColor.lighter(130);
    else if(m_state == State::Wrong) end = m_manager->design()->m_wrongColor.lighter(130);
    else end = m_manager->design()->m_pressedColor;
    QString sheet("QLineEdit:focus { background-color: rgba(%1, %2, %3, %4); }");
    sheet = sheet.arg(end.red()).arg(end.green()).arg(end.blue()).arg(end.alpha());

    if(m_styleSheet.count(":focus") == 2){
        int index = m_styleSheet.lastIndexOf("QLineEdit:focus");
        m_styleSheet.replace(index, m_styleSheet.length() - index, sheet);
    } else m_styleSheet.append(sheet);
}

void AnimatedQLineEdit::updateVisually()
{
    auto& design = m_manager->design();

    QColor color;
    m_styleSheet = QString("QLineEdit { border-style: solid;border-color: transparent;border-width: 1px;background-color: rgba(%1, %2, %3,%4);"
        "padding-left: 2px;}");

    color = design->m_pressedBorderColor;
    QString focus("QLineEdit:focus { border: 1px solid rgba(%1,%2,%3,%4); } ");
    focus = focus.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());

    color = design->m_pressedColor;
    QString pressed("QLineEdit:focus { background-color: rgba(%1, %2, %3, %4); }");
    pressed = pressed.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());

    color = design->m_textColor;
    QString text("QLineEdit {color: rgba(%1, %2, %3, %4);}");
    text = text.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());

    color = design->m_disabledColor;
    QString disabled("QLineEdit:disabled { color: grey; background: rgba(%1, %2, %3, %4); }");
    disabled = disabled.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());

    color = design->m_selectionColor;
    QString selection("QLineEdit { selection-background-color: rgb(%1, %2, %3, %4);}");
    selection = selection.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());

    m_styleSheet.append(selection);
    m_styleSheet.append(disabled);
    m_styleSheet.append(text);
    m_styleSheet.append(focus);
    if(design->m_enablePressedColor){
        m_styleSheet.append(pressed);
    }

    if(m_enabled){
        color = design->m_defaultColor;
    } else{
        color = design->m_disabledColor;
    }
    m_currentColor = color;
    setStyleSheet(m_styleSheet.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
}

void AnimatedQLineEdit::animFinished()
{
    if(m_stateJustChanged){
        updateFocusStylesheet();
        m_stateJustChanged = false;
        return;
    }
    if(m_enabled){
        return;
    }
    setEnabled(false);
}

void AnimatedQLineEdit::enterEvent(QEvent *e)
{
    Q_UNUSED(e)
    if(!m_enabled){
        return;
    }
    QColor end;
    if(m_state == State::Normal) end = m_manager->design()->m_hoverColor;
    else if(m_state == State::Good) end = m_manager->design()->m_goodColor.lighter(125);
    else if(m_state == State::Wrong) end = m_manager->design()->m_wrongColor.lighter(125);
    if(m_isFocused) end = end.lighter(125);
    m_animation.stop();
    m_animation.setStartValue(m_currentColor);
    m_animation.setEndValue(end);
    m_animation.setDuration(m_manager->design()->m_delay);
    m_animation.start();
}

void AnimatedQLineEdit::leaveEvent(QEvent *e)
{
    Q_UNUSED(e)
    if(!m_enabled){
        return;
    }
    QColor end;
    if(m_state == State::Normal) end = m_manager->design()->m_defaultColor;
    else if(m_state == State::Good) end = m_manager->design()->m_goodColor;
    else if(m_state == State::Wrong) end = m_manager->design()->m_wrongColor;
    m_animation.stop();
    m_animation.setStartValue(m_currentColor);
    m_animation.setEndValue(end);
    m_animation.setDuration(m_manager->design()->m_delay);
    m_animation.start();
}

void AnimatedQLineEdit::mousePressEvent(QMouseEvent *e)
{
    if(!m_enabled){
        e->ignore();
        return;
    }
    QLineEdit::mousePressEvent(e);
}

void AnimatedQLineEdit::mouseReleaseEvent(QMouseEvent *e)
{

    QLineEdit::mouseReleaseEvent(e);
}
