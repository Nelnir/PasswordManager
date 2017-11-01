#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <vector>
#include <memory>
#include <QColor>

struct BaseDesign{
    QColor m_defaultColor;
    QColor m_hoverColor;
    QColor m_pressedColor;
    QColor m_textColor;
    QColor m_disabledColor;

    signed int m_delay;
};

struct PushButtonDesign : public BaseDesign{
    QColor m_checkedColor;
    QColor m_focusedColor;
    QColor m_focusedText;
};

struct LineEditDesign : public BaseDesign{
    QColor m_goodColor;
    QColor m_wrongColor;
    QColor m_selectionColor;
    QColor m_pressedBorderColor;
    bool m_enablePressedColor;
};

struct AccountWidgetDesign{
    QColor m_defaultColor;
    QColor m_hoverColor;
    QColor m_childDefaultColor;
    QColor m_childHoverColor;
    QColor m_textColor;

    signed int m_delay;
};

template<class T>
class AnimationManager
{
    std::vector<std::unique_ptr<T>> m_designs;
    signed int m_current;
public:
    AnimationManager() {}
    void addDesign(const T& l_design){
        m_designs.emplace_back(std::make_unique<T>(l_design));
    }
    void setCurrent(const signed int& l_index) { m_current = l_index; }
    const std::unique_ptr<T>& design() { return m_designs[m_current]; }
};

#endif // ANIMATIONMANAGER_H
