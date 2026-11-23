// MainWindow.h — the one window: calendar on the left, three tabs on the right.
//
//   ┌────────────┬──────────────────────────────────────────┐
//   │ Calendar   │ [Session] [Weekly volume] [Progress]      │
//   │ (bold =    │  Session: add-a-set form + table of sets  │
//   │  trained)  │  Weekly volume: sets per muscle vs target │
//   │ week stats │  Progress: e1RM line chart per exercise   │
//   └────────────┴──────────────────────────────────────────┘
//
// The window holds NO training logic. It turns clicks into controller calls
// and re-reads data when the controller signals a change (Larman Ch. 13:
// UI objects do UI work; the domain does domain work).
#pragma once

#include <QDate>
#include <QMainWindow>

class QAction;
class QCalendarWidget;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QPushButton;
class QSpinBox;
class QTabWidget;
class QTableView;

namespace ironlog {

class ProgressChart;
class TrainingController;
class WeeklyVolumeModel;
class WorkoutTableModel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(TrainingController& controller, QWidget* parent = nullptr);

    enum class Tab { Session = 0, Week = 1, Progress = 2 };
    void showTab(Tab tab);
    void selectDate(const QDate& date);

private slots:
    void onDateSelected();
    void onAddSet();
    void onDeleteSet();
    void onExerciseChosen();
    void onAddExercise();
    void onLoadSampleData();
    void onSaveFailed(const QString& message);
    void refreshAfterLogChange();
    void refreshExerciseLists();
    void refreshProgress();

private:
    QWidget* buildLeftPanel();
    QWidget* buildSessionTab();
    QWidget* buildWeekTab();
    QWidget* buildProgressTab();
    void buildMenus();

    void refreshCalendarMarks();
    void refreshSessionHeader();
    void refreshWeekHeader();
    void updateDeleteEnabled();

    TrainingController& m_controller;
    WorkoutTableModel* m_workoutModel = nullptr;
    WeeklyVolumeModel* m_weekModel = nullptr;

    // Left panel
    QCalendarWidget* m_calendar = nullptr;
    QLabel* m_weekSummary = nullptr;

    QTabWidget* m_tabs = nullptr;

    // Session tab
    QLabel* m_sessionHeader = nullptr;
    QComboBox* m_exerciseCombo = nullptr;
    QDoubleSpinBox* m_weightSpin = nullptr;
    QSpinBox* m_repsSpin = nullptr;
    QPushButton* m_addButton = nullptr;
    QTableView* m_setsView = nullptr;
    QPushButton* m_deleteButton = nullptr;

    // Weekly volume tab
    QLabel* m_weekHeader = nullptr;
    QTableView* m_weekView = nullptr;

    // Progress tab
    QComboBox* m_progressCombo = nullptr;
    QLabel* m_progressSummary = nullptr;
    ProgressChart* m_chart = nullptr;

    QAction* m_sampleAction = nullptr;
    bool m_warnedSaveFailure = false;
};

} // namespace ironlog
