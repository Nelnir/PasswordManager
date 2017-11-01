#include "accountwidgetabstractlabel.h"
#include <QStyleOption>
#include <QPainter>

AccountWidgetAbstractLabel::AccountWidgetAbstractLabel(QWidget* l_parent, AnimationManager<AccountWidgetDesign>* l_manager) : QWidget(l_parent),
    m_animation(this, "color"),
    m_manager(l_manager)
{

}

AccountWidgetAbstractLabel::~AccountWidgetAbstractLabel()
{

}

void AccountWidgetAbstractLabel::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void AccountWidgetAbstractLabel::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)

    m_animation.stop();
    m_animation.setStartValue(m_currentColor);
    m_animation.setEndValue(m_manager->design()->m_defaultColor);
    m_animation.setDuration(m_manager->design()->m_delay);
    m_animation.start();
}

void AccountWidgetAbstractLabel::enterEvent(QEvent *event)
{
    Q_UNUSED(event)

    m_animation.stop();
    m_animation.setStartValue(m_currentColor);
    m_animation.setEndValue(m_manager->design()->m_hoverColor);
    m_animation.setDuration(m_manager->design()->m_delay);
    m_animation.start();
}

void AccountWidgetAbstractLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit clicked();
}

void AccountWidgetAbstractLabel::setColor(const QColor &color)
{
    setStyleSheet(m_styleSheet.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
    m_currentColor = color;
}
