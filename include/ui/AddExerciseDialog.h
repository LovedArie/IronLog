// AddExerciseDialog.h — small modal form: exercise name + muscle group.
#pragma once

#include <QDialog>

#include "core/Exercise.h"

class QComboBox;
class QDialogButtonBox;
class QLineEdit;

namespace ironlog {

class AddExerciseDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddExerciseDialog(QWidget* parent = nullptr);

    QString name() const;
    MuscleGroup muscle() const;

private:
    // Forward-declared pointers above (class QLineEdit;) instead of
    // #includes: the header compiles faster and leaks less. Qt's parent
    // system deletes these child widgets, so raw pointers are fine here.
    QLineEdit* m_name = nullptr;
    QComboBox* m_muscle = nullptr;
    QDialogButtonBox* m_buttons = nullptr;
};

} // namespace ironlog
