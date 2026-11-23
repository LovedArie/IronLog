// main.cpp — wires the three layers together and starts the event loop.
// Stroustrup §15.4: main() is a global function, outside any namespace.
//
// Command-line options (all optional; normal users just double-click):
//   --data <file>        use this JSON file instead of the default location
//   --demo               if the log is empty, fill it with sample data
//   --date <YYYY-MM-DD>  select this day on startup
//   --tab <session|week|progress>   open on this tab
//   --screenshot <png>   render the window to an image and quit
//                        (how docs/screenshots/*.png are made)

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTimer>

#include "ui/MainWindow.h"
#include "services/TrainingController.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    // The application name DECIDES the data folder (QStandardPaths), e.g.
    // Windows: C:/Users/<you>/AppData/Roaming/IronLog/
    // No organization name on purpose: AppDataLocation is <org>/<app>, so
    // org = app = "IronLog" would give a doubled IronLog/IronLog/ folder
    // (TickTimer lesson). Renaming the app later moves the data folder too,
    // so a rename needs a copy-the-old-file migration.
    QApplication::setApplicationName("IronLog");
    QApplication::setApplicationVersion(IRONLOG_VERSION); // from CMake's project(VERSION)

    QCommandLineParser parser;
    parser.setApplicationDescription("IronLog — bodybuilding training log");
    parser.addHelpOption();
    parser.addVersionOption();
    const QCommandLineOption dataOpt("data", "Use <file> for the training log.", "file");
    const QCommandLineOption demoOpt("demo", "Fill an empty log with sample data.");
    const QCommandLineOption dateOpt("date", "Select <YYYY-MM-DD> on startup.", "date");
    const QCommandLineOption tabOpt("tab", "Open on <session|week|progress>.", "tab");
    const QCommandLineOption shotOpt("screenshot", "Save the window to <png> and quit.", "png");
    parser.addOptions({dataOpt, demoOpt, dateOpt, tabOpt, shotOpt});
    parser.process(app);

    QString dataPath = parser.value(dataOpt);
    if (dataPath.isEmpty()) {
        const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(dir); // first run: the folder doesn't exist yet
        dataPath = dir + "/ironlog.json";
    }
    dataPath = QFileInfo(dataPath).absoluteFilePath();

    ironlog::TrainingController controller(dataPath);
    if (parser.isSet(demoOpt)) controller.loadSampleData(QDate::currentDate());

    ironlog::MainWindow window(controller);
    if (parser.isSet(dateOpt)) {
        const QDate d = QDate::fromString(parser.value(dateOpt), Qt::ISODate);
        if (d.isValid()) window.selectDate(d);
    }
    const QString tab = parser.value(tabOpt);
    if (tab == "week") window.showTab(ironlog::MainWindow::Tab::Week);
    if (tab == "progress") window.showTab(ironlog::MainWindow::Tab::Progress);
    window.show();

    if (parser.isSet(shotOpt)) {
        // Give the event loop one pass to lay out and paint, then capture.
        QTimer::singleShot(200, &window, [&] {
            const bool ok = window.grab().save(parser.value(shotOpt));
            QApplication::exit(ok ? 0 : 1);
        });
        return app.exec();
    }

    // Show load problems AFTER the window is up, so the dialog has a parent.
    if (!controller.startupWarning().isEmpty()) {
        QTimer::singleShot(0, &window, [&] {
            QMessageBox::warning(&window, QObject::tr("IronLog"), controller.startupWarning());
        });
    }
    return app.exec();
}
