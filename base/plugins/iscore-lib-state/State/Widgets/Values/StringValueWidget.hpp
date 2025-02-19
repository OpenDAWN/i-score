#pragma once
#include "ValueWidget.hpp"

class QLineEdit;
class StringValueWidget : public ValueWidget
{
    public:
        StringValueWidget(const QString& value, QWidget* parent = nullptr);

        iscore::Value value() const override;

    private:
        QLineEdit* m_value;
};
