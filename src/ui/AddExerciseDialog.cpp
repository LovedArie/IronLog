// AddExerciseDialog.cpp

#include "ui/AddExerciseDialog.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

#include "services/QtConvert.h"

namespace ironlog {

AddExerciseDialog::AddExerciseDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle(tr("Add exercise"));

    m_name = new QLineEdit(this);
    m_name->setPlaceholderText(tr("e.g. Cable Fly"));
    m_name->setMaxLength(60);

    m_muscle = new QComboBox(this);
    for (MuscleGroup m : allMuscleGroups())
        // Store the enum's int as item data, so muscle() never parses text.
        m_muscle->addItem(toQt(toString(m)), static_cast<int>(m));

    m_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    m_buttons->button(QDialogButtonBox::Ok)->setEnabled(false);

    auto* form = new QFormLayout(this);
    form->addRow(tr("Name"), m_name);
    form->addRow(tr("Primary muscle"), m_muscle);
    form->addRow(m_buttons);

    // OK only makes sense once there's a name. Disabling the button is
    // friendlier than accepting and then showing an error.
    connect(m_name, &QLineEdit::textChanged, this, [this](const QString& text) {
        m_buttons->button(QDialogButtonBox::Ok)->setEnabled(!text.trimmed().isEmpty());
    });
    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString AddExerciseDialog::name() const { return m_name->text().trimmed(); }

MuscleGroup AddExerciseDialog::muscle() const {
    return static_cast<MuscleGroup>(m_muscle->currentData().toInt());
}

} // namespace ironlog
