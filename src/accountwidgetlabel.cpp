#include "accountwidgetlabel.h"
#include <QHBoxLayout>

AccountWidgetLabel::AccountWidgetLabel(const QString& l_title, QWidget *parent, AnimationManager<AccountWidgetDesign> *l_manager,
                                       const bool& l_visible) :
    AccountWidgetAbstractLabel(parent, l_manager),
    m_visible(l_visible)
{
    setObjectName("LabelWidget");

    m_title.setText(l_title);
    m_title.setStyleSheet( QString(" QLabel { font: 16px; } "));

    m_data.setStyleSheet(QString(" QLabel { font: 16px; } "));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(&m_title, 0, Qt::AlignLeft);
    layout->addWidget(&m_data, 0, Qt::AlignRight);
    layout->setContentsMargins(5, 0, 5, 0);
    setLayout(layout);
}

AccountWidgetLabel::~AccountWidgetLabel()
{

}

void AccountWidgetLabel::setUp(const QString &l_acc)
{
    m_dataText = l_acc;
    updateDataText();
}

void AccountWidgetLabel::updateVisually()
{
    auto& design = m_manager->design();

    QColor color;
    m_styleSheet = QString("QWidget#LabelWidget { background: rgba(%1, %2, %3, %4); border: 1px solid transparent; outline: none;}");

    color = design->m_textColor;
    QString text("QWidget#LabelWidget { color: rgba(%1,%2,%3,%4); }");
    text = text.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());

    m_styleSheet.append(text);

    color = design->m_childDefaultColor;
    m_currentColor = color;
    setStyleSheet(m_styleSheet.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
}

void AccountWidgetLabel::updateDataText()
{
    m_data.setText(m_visible ? m_dataText : QString(m_dataText.length(), '*'));
}

void AccountWidgetLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_visible = !m_visible;
    updateDataText();
}

void AccountWidgetLabel::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    m_animation.stop();
    m_animation.setStartValue(m_currentColor);
    m_animation.setEndValue(m_manager->design()->m_childHoverColor);
    m_animation.setDuration(m_manager->design()->m_delay);
    m_animation.start();
}

void AccountWidgetLabel::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    m_animation.stop();
    m_animation.setStartValue(m_currentColor);
    m_animation.setEndValue(m_manager->design()->m_childDefaultColor);
    m_animation.setDuration(m_manager->design()->m_delay);
    m_animation.start();
}
