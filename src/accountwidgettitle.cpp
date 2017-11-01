#include "accountwidgettitle.h"
#include <QHBoxLayout>

AccountWidgetTitle::AccountWidgetTitle(QWidget *parent, AnimationManager<AccountWidgetDesign> *l_manager) : AccountWidgetAbstractLabel(parent, l_manager)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setObjectName("TitleWidget");
    setMaximumHeight(35);
    setMinimumHeight(35);

    m_title.setStyleSheet(QString(" QLabel { font: 17px; } "));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(&m_title, 0, Qt::AlignLeft);
    layout->setContentsMargins(5, 0, 0, 0);
    setLayout(layout);
}

AccountWidgetTitle::~AccountWidgetTitle()
{

}

void AccountWidgetTitle::setUp(const QString &l_acc)
{
    m_title.setText(l_acc);
}

void AccountWidgetTitle::updateVisually()
{
    auto& design = m_manager->design();

    QColor color;
    m_styleSheet = QString("QWidget#TitleWidget { background: rgba(%1, %2, %3, %4); border: 1px solid transparent; outline: none;}");

    color = design->m_textColor;
    QString text("QWidget#TitleWidget { color: rgba(%1,%2,%3,%4); }");
    text = text.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());

    m_styleSheet.append(text);

    color = design->m_defaultColor;
    m_currentColor = color;
    setStyleSheet(m_styleSheet.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
}
