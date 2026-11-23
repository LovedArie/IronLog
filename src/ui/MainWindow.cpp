// MainWindow.cpp

#include "ui/MainWindow.h"

#include <QAction>
#include <QCalendarWidget>
#include <QComboBox>
#include <QCoreApplication>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLocale>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QSplitter>
#include <QStatusBar>
#include <QTabWidget>
#include <QTableView>
#include <QTextCharFormat>
#include <QVBoxLayout>

#include <algorithm>

#include "ui/AddExerciseDialog.h"
#include "ui/ProgressChart.h"
#include "services/QtConvert.h"
#include "services/TrainingController.h"
#include "ui/WeeklyVolumeModel.h"
#include "ui/WorkoutTableModel.h"
#include "core/Stats.h"

namespace ironlog {

namespace {

QString formatVolume(double kg) {
    // QLocale adds the thousands separator the user expects: "12,450".
    return QLocale().toString(kg, 'f', 0);
}

// Shared look for both table views.
void styleTable(QTableView* view) {
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setAlternatingRowColors(true);
    view->verticalHeader()->setVisible(false);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

} // namespace

MainWindow::MainWindow(TrainingController& controller, QWidget* parent)
    : QMainWindow(parent), m_controller(controller) {
    setWindowTitle(tr("IronLog — Bodybuilding Training Log"));
    resize(1120, 680);

    // Models first: the tabs' views need them. Parent = this, so Qt
    // deletes them with the window (no manual delete, no leak).
    m_workoutModel = new WorkoutTableModel(m_controller, this);
    m_weekModel = new WeeklyVolumeModel(m_controller, this);

    m_tabs = new QTabWidget;
    m_tabs->addTab(buildSessionTab(), tr("Session"));
    m_tabs->addTab(buildWeekTab(), tr("Weekly volume"));
    m_tabs->addTab(buildProgressTab(), tr("Progress"));

    auto* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(buildLeftPanel());
    splitter->addWidget(m_tabs);
    splitter->setStretchFactor(1, 1); // extra width goes to the tabs
    setCentralWidget(splitter);

    buildMenus();

    auto* pathLabel = new QLabel(tr("Data: %1").arg(m_controller.dataPath()));
    pathLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    statusBar()->addPermanentWidget(pathLabel);

    // ---- Wiring: controller signals -> refresh the window parts ----------
    // (The two models connect themselves; see their constructors.)
    connect(&m_controller, &TrainingController::logChanged, this, &MainWindow::refreshAfterLogChange);
    connect(&m_controller, &TrainingController::libraryChanged, this, &MainWindow::refreshExerciseLists);
    connect(&m_controller, &TrainingController::saveFailed, this, &MainWindow::onSaveFailed);

    refreshExerciseLists();
    selectDate(QDate::currentDate());
    refreshAfterLogChange();

    if (m_controller.log().isEmpty())
        statusBar()->showMessage(
            tr("New log. Tip: File → Load sample data shows 8 weeks of demo training."));
}

// ---------------------------------------------------------------------------
// UI construction
// ---------------------------------------------------------------------------

QWidget* MainWindow::buildLeftPanel() {
    m_calendar = new QCalendarWidget;
    m_calendar->setFirstDayOfWeek(Qt::Monday); // training weeks start Monday
    m_calendar->setGridVisible(false);
    m_calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    m_calendar->setMaximumDate(QDate::currentDate()); // no logging the future
    connect(m_calendar, &QCalendarWidget::selectionChanged, this, &MainWindow::onDateSelected);

    auto* legend = new QLabel(tr("<b>Bold</b> days have a logged session."));
    legend->setWordWrap(true);

    m_weekSummary = new QLabel;
    m_weekSummary->setWordWrap(true);

    auto* panel = new QWidget;
    auto* layout = new QVBoxLayout(panel);
    layout->addWidget(m_calendar);
    layout->addWidget(legend);
    layout->addWidget(m_weekSummary);
    layout->addStretch();
    panel->setMaximumWidth(360);
    return panel;
}

QWidget* MainWindow::buildSessionTab() {
    m_sessionHeader = new QLabel;
    m_sessionHeader->setTextFormat(Qt::RichText);

    // ---- "Add a set" form ----
    m_exerciseCombo = new QComboBox;
    m_exerciseCombo->setMinimumContentsLength(18);
    connect(m_exerciseCombo, &QComboBox::currentIndexChanged, this, &MainWindow::onExerciseChosen);

    m_weightSpin = new QDoubleSpinBox;
    m_weightSpin->setRange(0.0, SetEntry::kMaxWeightKg); // same limits as the domain
    m_weightSpin->setDecimals(2);
    m_weightSpin->setSingleStep(2.5); // the smallest common plate jump
    m_weightSpin->setSuffix(tr(" kg"));
    m_weightSpin->setToolTip(tr("For bodyweight moves, enter the ADDED weight (0 = bodyweight)."));

    m_repsSpin = new QSpinBox;
    m_repsSpin->setRange(1, SetEntry::kMaxReps);
    m_repsSpin->setSuffix(tr(" reps"));

    m_addButton = new QPushButton(tr("Add set"));
    m_addButton->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return));
    m_addButton->setToolTip(tr("Add this set to the selected day (Ctrl+Enter)"));
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddSet);

    auto* addExercise = new QPushButton(tr("New exercise…"));
    connect(addExercise, &QPushButton::clicked, this, &MainWindow::onAddExercise);

    auto* formBox = new QGroupBox(tr("Log a set"));
    auto* form = new QHBoxLayout(formBox);
    form->addWidget(m_exerciseCombo, 1);
    form->addWidget(m_weightSpin);
    form->addWidget(m_repsSpin);
    form->addWidget(m_addButton);
    form->addWidget(addExercise);

    // ---- Table of the day's sets ----
    m_setsView = new QTableView;
    m_setsView->setModel(m_workoutModel);
    styleTable(m_setsView);
    m_setsView->horizontalHeader()->setSectionResizeMode(WorkoutTableModel::ExerciseCol,
                                                         QHeaderView::Stretch);

    m_deleteButton = new QPushButton(tr("Delete selected set"));
    m_deleteButton->setShortcut(QKeySequence::Delete);
    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteSet);
    // A model reset clears the selection WITHOUT emitting selectionChanged,
    // so we listen to both to keep the button's enabled state honest.
    connect(m_setsView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::updateDeleteEnabled);
    connect(m_workoutModel, &QAbstractItemModel::modelReset, this, &MainWindow::updateDeleteEnabled);

    auto* buttons = new QHBoxLayout;
    buttons->addStretch();
    buttons->addWidget(m_deleteButton);

    auto* page = new QWidget;
    auto* layout = new QVBoxLayout(page);
    layout->addWidget(m_sessionHeader);
    layout->addWidget(formBox);
    layout->addWidget(m_setsView, 1);
    layout->addLayout(buttons);
    return page;
}

QWidget* MainWindow::buildWeekTab() {
    m_weekHeader = new QLabel;
    m_weekHeader->setTextFormat(Qt::RichText);

    m_weekView = new QTableView;
    m_weekView->setModel(m_weekModel);
    styleTable(m_weekView);
    // Name column takes the spare width (same as the Session table), so the
    // coloured Status cells stay compact instead of filling the window.
    m_weekView->horizontalHeader()->setSectionResizeMode(WeeklyVolumeModel::Muscle,
                                                         QHeaderView::Stretch);

    const auto target = m_weekModel->target();
    auto* note = new QLabel(
        tr("Each logged set counts toward its exercise's <i>primary</i> muscle. "
           "%1–%2 sets per muscle per week is a common starting range for muscle "
           "growth — adjust it to how well you recover.")
            .arg(target.low)
            .arg(target.high));
    note->setWordWrap(true);

    auto* page = new QWidget;
    auto* layout = new QVBoxLayout(page);
    layout->addWidget(m_weekHeader);
    layout->addWidget(m_weekView, 1);
    layout->addWidget(note);
    return page;
}

QWidget* MainWindow::buildProgressTab() {
    m_progressCombo = new QComboBox;
    m_progressCombo->setMinimumContentsLength(18);
    connect(m_progressCombo, &QComboBox::currentIndexChanged, this, &MainWindow::refreshProgress);

    m_progressSummary = new QLabel;
    m_progressSummary->setTextFormat(Qt::RichText);

    m_chart = new ProgressChart;

    auto* top = new QHBoxLayout;
    top->addWidget(new QLabel(tr("Exercise:")));
    top->addWidget(m_progressCombo);
    top->addSpacing(12);
    top->addWidget(m_progressSummary, 1);

    auto* page = new QWidget;
    auto* layout = new QVBoxLayout(page);
    layout->addLayout(top);
    layout->addWidget(m_chart, 1);
    return page;
}

void MainWindow::buildMenus() {
    QMenu* file = menuBar()->addMenu(tr("&File"));

    QAction* addExercise = file->addAction(tr("Add &exercise…"));
    addExercise->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_E));
    connect(addExercise, &QAction::triggered, this, &MainWindow::onAddExercise);

    m_sampleAction = file->addAction(tr("Load &sample data"));
    connect(m_sampleAction, &QAction::triggered, this, &MainWindow::onLoadSampleData);

    file->addSeparator();
    QAction* quit = file->addAction(tr("&Quit"));
    quit->setShortcut(QKeySequence::Quit);
    connect(quit, &QAction::triggered, this, &QWidget::close);

    QMenu* help = menuBar()->addMenu(tr("&Help"));
    QAction* about = help->addAction(tr("&About IronLog"));
    connect(about, &QAction::triggered, this, [this] {
        QMessageBox::about(this, tr("About IronLog"),
                           tr("<h3>IronLog %1</h3>"
                              "<p>A bodybuilding training log: sets, personal records, "
                              "weekly volume per muscle, and estimated 1RM progress.</p>"
                              "<p>Your data: <code>%2</code></p>")
                               .arg(QCoreApplication::applicationVersion(),
                                    m_controller.dataPath().toHtmlEscaped()));
    });
}

// ---------------------------------------------------------------------------
// Public helpers
// ---------------------------------------------------------------------------

void MainWindow::showTab(Tab tab) { m_tabs->setCurrentIndex(static_cast<int>(tab)); }

void MainWindow::selectDate(const QDate& date) {
    m_calendar->setSelectedDate(date); // emits selectionChanged if it moved
    onDateSelected();                  // ...so also call directly for "same date"
}

// ---------------------------------------------------------------------------
// Slots: user actions
// ---------------------------------------------------------------------------

void MainWindow::onDateSelected() {
    const QDate date = m_calendar->selectedDate();
    m_workoutModel->setDate(date);
    m_weekModel->setWeekContaining(date);

    // Continue where that day left off: pick its last exercise. Changing
    // the combo fires onExerciseChosen(), which pre-fills weight and reps.
    if (const Workout* w = m_controller.log().find(toDomain(date)); w && !w->isEmpty())
        m_exerciseCombo->setCurrentText(toQt(w->sets().back().exercise()));

    refreshSessionHeader();
    refreshWeekHeader();
}

void MainWindow::onAddSet() {
    const QDate date = m_calendar->selectedDate();
    const QString exercise = m_exerciseCombo->currentText();

    switch (m_controller.addSet(date, exercise, m_weightSpin->value(), m_repsSpin->value())) {
    case TrainingController::AddResult::Ok: {
        // The model already reloaded (logChanged fired inside addSet), so
        // the new set is its last row. Was it a PR?
        const int row = m_workoutModel->rowCount() - 1;
        m_setsView->scrollToBottom();
        statusBar()->showMessage(m_workoutModel->isRecord(row)
                                     ? tr("New personal record on %1!").arg(exercise)
                                     : tr("Set added."),
                                 4000);
        break;
    }
    case TrainingController::AddResult::UnknownExercise:
        statusBar()->showMessage(tr("Pick an exercise first."), 4000);
        break;
    case TrainingController::AddResult::InvalidNumbers:
        statusBar()->showMessage(tr("Weight and reps are out of range."), 4000);
        break;
    }
}

void MainWindow::onDeleteSet() {
    const QModelIndexList rows = m_setsView->selectionModel()->selectedRows();
    if (rows.isEmpty()) return;
    if (m_controller.removeSet(m_calendar->selectedDate(), rows.first().row()))
        statusBar()->showMessage(tr("Set deleted."), 3000);
}

void MainWindow::onExerciseChosen() {
    // Pre-fill from last time: logging the same lift again should be one
    // click. Defaults when there's no history: empty barbell, 10 reps.
    const auto last = stats::lastSet(m_controller.log(), toStd(m_exerciseCombo->currentText()));
    m_weightSpin->setValue(last ? last->weightKg() : 20.0);
    m_repsSpin->setValue(last ? last->reps() : 10);
}

void MainWindow::onAddExercise() {
    AddExerciseDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) return;
    if (!m_controller.addExercise(dialog.name(), dialog.muscle())) {
        QMessageBox::information(this, tr("Add exercise"),
                                 tr("\"%1\" is already in your exercise list.").arg(dialog.name()));
        return;
    }
    m_exerciseCombo->setCurrentText(dialog.name()); // ready to log it now
    statusBar()->showMessage(tr("Added %1.").arg(dialog.name()), 3000);
}

void MainWindow::onLoadSampleData() {
    if (!m_controller.loadSampleData(QDate::currentDate())) {
        QMessageBox::information(this, tr("Sample data"),
                                 tr("Sample data can only be loaded into an empty log, "
                                    "so it can never overwrite your own training."));
        return;
    }
    selectDate(QDate::currentDate());
    statusBar()->showMessage(tr("Loaded 8 weeks of sample training."), 4000);
}

void MainWindow::onSaveFailed(const QString& message) {
    statusBar()->showMessage(tr("NOT SAVED: %1").arg(message));
    if (m_warnedSaveFailure) return; // one dialog per session, not one per set
    m_warnedSaveFailure = true;
    QMessageBox::warning(this, tr("Could not save"),
                         tr("IronLog could not save your log:\n%1\n\n"
                            "Your changes are still on screen, but will be lost when "
                            "you close the app until saving works again.")
                             .arg(message));
}

// ---------------------------------------------------------------------------
// Refreshers: re-read state and repaint the parts that show it
// ---------------------------------------------------------------------------

void MainWindow::refreshAfterLogChange() {
    refreshCalendarMarks();
    refreshSessionHeader();
    refreshWeekHeader();
    refreshProgress();
    m_sampleAction->setEnabled(m_controller.log().isEmpty());
}

void MainWindow::refreshExerciseLists() {
    // Remember selections, rebuild, restore — so adding an exercise doesn't
    // yank the user's current choice away.
    const QString logChoice = m_exerciseCombo->currentText();
    const QString progressChoice = m_progressCombo->currentText();

    QStringList names;
    for (const Exercise& e : m_controller.library().all()) names << toQt(e.name);
    names.sort(Qt::CaseInsensitive);

    // QSignalBlocker would also work; blocking here avoids N refreshes
    // while items are added one by one.
    const bool b1 = m_exerciseCombo->blockSignals(true);
    const bool b2 = m_progressCombo->blockSignals(true);
    m_exerciseCombo->clear();
    m_exerciseCombo->addItems(names);
    m_progressCombo->clear();
    m_progressCombo->addItems(names);

    if (!logChoice.isEmpty()) m_exerciseCombo->setCurrentText(logChoice);

    // Progress tab: keep the choice, else open on the most-logged lift.
    if (!progressChoice.isEmpty()) {
        m_progressCombo->setCurrentText(progressChoice);
    } else {
        int bestIndex = 0;
        std::size_t bestCount = 0;
        for (int i = 0; i < names.size(); ++i) {
            const auto n = stats::e1rmHistory(m_controller.log(), toStd(names[i])).size();
            if (n > bestCount) { bestCount = n; bestIndex = i; }
        }
        m_progressCombo->setCurrentIndex(bestIndex);
    }
    m_exerciseCombo->blockSignals(b1);
    m_progressCombo->blockSignals(b2);

    onExerciseChosen();
    refreshProgress();
}

void MainWindow::refreshCalendarMarks() {
    // A null QDate clears EVERY custom format in one call.
    m_calendar->setDateTextFormat(QDate(), QTextCharFormat());

    QTextCharFormat trained;
    trained.setFontWeight(QFont::Bold);
    trained.setForeground(palette().color(QPalette::Highlight));
    for (const Workout& w : m_controller.log().workouts())
        m_calendar->setDateTextFormat(toQt(w.date()), trained);
}

void MainWindow::refreshSessionHeader() {
    const QDate date = m_calendar->selectedDate();
    const int sets = m_workoutModel->rowCount();
    int records = 0;
    for (int r = 0; r < sets; ++r) records += m_workoutModel->isRecord(r) ? 1 : 0;

    QString summary;
    if (sets == 0) {
        summary = tr("Rest day — no sets logged.");
    } else {
        summary = countOf(sets, tr("set"), tr("sets")) + " · " +
                  tr("%1 kg volume").arg(formatVolume(m_workoutModel->totalVolume()));
        if (records > 0) summary += " · <b>" + countOf(records, tr("PR"), tr("PRs")) + "</b>";
    }
    m_sessionHeader->setText(QStringLiteral("<h2 style='margin:0'>%1</h2>%2")
                                 .arg(QLocale().toString(date, QLocale::LongFormat), summary));
}

void MainWindow::refreshWeekHeader() {
    const QDate monday = m_weekModel->monday();
    const QDate sunday = monday.addDays(6);
    m_weekHeader->setText(QStringLiteral("<h2 style='margin:0'>%1</h2>")
                              .arg(tr("Week of %1 – %2")
                                       .arg(monday.toString("MMM d"), sunday.toString("MMM d, yyyy"))));

    // Left-panel summary for the same week.
    int sessions = 0;
    int sets = 0;
    double volume = 0.0;
    for (const Workout& w : m_controller.log().workouts()) {
        const QDate d = toQt(w.date());
        if (d < monday || d > sunday) continue;
        ++sessions;
        sets += static_cast<int>(w.sets().size());
        volume += w.totalVolume();
    }
    m_weekSummary->setText(tr("<b>Selected week</b><br>%1 · %2 · %3 kg")
                               .arg(countOf(sessions, tr("session"), tr("sessions")),
                                    countOf(sets, tr("set"), tr("sets")), formatVolume(volume)));
}

void MainWindow::refreshProgress() {
    const QString exercise = m_progressCombo->currentText();
    auto points = stats::e1rmHistory(m_controller.log(), toStd(exercise));

    if (points.empty()) {
        m_progressSummary->setText(tr("No history yet."));
    } else {
        const double first = points.front().bestE1rm;
        const double best = stats::allTimeBestE1rm(m_controller.log(), toStd(exercise)).value_or(0);
        const double change = first > 0 ? (best - first) / first * 100.0 : 0.0;
        m_progressSummary->setText(
            tr("Best e1RM <b>%1 kg</b> · %2 · %3 since first")
                .arg(QString::number(best, 'f', 1),
                     countOf(static_cast<int>(points.size()), tr("session"), tr("sessions")),
                     (change >= 0 ? "+" : "") + QString::number(change, 'f', 1) + "%"));
    }
    m_chart->setSeries(tr("%1 — estimated 1RM (kg)").arg(exercise), std::move(points));
}

void MainWindow::updateDeleteEnabled() {
    m_deleteButton->setEnabled(m_setsView->selectionModel()->hasSelection());
}

} // namespace ironlog
