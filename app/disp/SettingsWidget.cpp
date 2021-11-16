//
// Created by pc on 11-11-21.
//

#include "SettingsWidget.h"

#include <QCheckBox>
#include <QFormLayout>
#include <QPushButton>
#include <QSpinBox>
#include <iostream>

namespace app::disp {

    SettingsWidget::SettingsWidget() {
        m_form_layout = new QFormLayout(this);
        setLayout(m_form_layout);
    }

    void SettingsWidget::add(const QString& name, size_t& value, int min, int max) {
        auto* spin_box = new QSpinBox(this);
        spin_box->setValue(value);
        spin_box->setMinimum(min);
        spin_box->setMaximum(max);
        connect(spin_box, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&](int v) { value = v; });
        m_form_layout->addRow(name, spin_box);
    }

    void SettingsWidget::add(const QString& name, bool& value) {
        auto* check_box = new QCheckBox(this);
        check_box->setChecked(value);
        connect(check_box, &QCheckBox::toggled, [&](bool b) { value = b; });
        m_form_layout->addRow(name, check_box);
    }

    void SettingsWidget::add(const QString& name, float& value, float min, float max) {
        auto* double_spin_box = new QDoubleSpinBox(this);
        double_spin_box->setDecimals(6);
        double_spin_box->setMinimum(min);
        double_spin_box->setMaximum(max);
        double_spin_box->setValue(value);
        connect(double_spin_box, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double d) { value = d; });
        m_form_layout->addRow(name, double_spin_box);
    }

    void SettingsWidget::add(const QString& name, const std::function<void()>& call_back) {
        auto* push_button = new QPushButton(this);
        connect(push_button, &QPushButton::pressed, call_back);
        m_form_layout->addRow(name, push_button);
    }

} // namespace app::disp