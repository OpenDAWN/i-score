#pragma once
#include <QWidget>
#include <State/Value.hpp>
/**
 * @brief The ValueWidget class
 *
 * Base class for the value widgets in the same folder.
 * They are used to edit a data type of the given type with the correct widgets.
 *
 * For instance :
 *  - Text : QLineEdit
 *  - Number : Q{Double}SpinBox
 * etc...
 */
class ValueWidget : public QWidget
{
    public:
        using QWidget::QWidget;
        virtual ~ValueWidget();
        virtual iscore::Value value() const = 0;
};
