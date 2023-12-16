#include "widgetfunctions.h"
#include <QString>
#include <QPoint>
#include <QList>
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSettings>
#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QMenu>
#include <QInputDialog>
#include <QString>
#include <QTimer>
#include <QElapsedTimer>
#include <QTime>
#include <QMessageBox>

class TimerInfo {
public:
    QString name;
    int duration;

    TimerInfo(const QString &name, int duration) : name(name), duration(duration) {}
};

class WidgetsTab : public QWidget {
public:
    QList<TimerInfo> timers;  // Lista przechowująca informacje o minutnikach

    WidgetsTab() {
        // Wczytaj zapisane minutniki
        loadTimers();

        // Dodaj przycisk "Dodaj minutnik"
        QPushButton *addTimerButton = new QPushButton("Dodaj minutnik", this);

        // Połącz sygnał z przycisku z odpowiednim slotem
        QObject::connect(addTimerButton, &QPushButton::clicked, this, &WidgetsTab::showAddTimerDialog);

        // Dodaj listę zapisanych minutników
        timersListWidget = new QListWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(addTimerButton);
        mainLayout->addWidget(timersListWidget);

        // Aktualizuj listę zapisanych minutników przy starcie
        updateTimersList();

        // Dodaj menu kontekstowe do listy minutników
        timersListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(timersListWidget, &QListWidget::customContextMenuRequested, this, &WidgetsTab::showContextMenu);
    }

    void updateTimersList() {
        // Wyczyść i zaktualizuj listę zapisanych minutników
        timersListWidget->clear();
        for (const TimerInfo &timer : timers) {
            QString timerText;
            if (timer.duration != 1) {
                timerText = timer.name + "  -  " + QString::number(timer.duration) + "  minut";
            } else {
                timerText = timer.name + " - " + QString::number(timer.duration) + " minuta";
            }
            timersListWidget->addItem(timerText);
        }
    }

    void saveTimers() {
        QSettings settings("MyApp", "TimerApp");
        QStringList timerNames;
        QList<int> timerDurations;

        for (const TimerInfo &timer : timers) {
            timerNames << timer.name;
            timerDurations << timer.duration;
        }

        settings.setValue("timerNames", timerNames);
        settings.setValue("timerDurations", QVariant::fromValue(timerDurations));
    }

    void loadTimers() {
        QSettings settings("MyApp", "TimerApp");
        QStringList timerNames = settings.value("timerNames").toStringList();
        QList<int> timerDurations = settings.value("timerDurations").value<QList<int>>();

        timers.clear();
        for (int i = 0; i < timerNames.size() && i < timerDurations.size(); ++i) {
            timers.append(TimerInfo(timerNames.at(i), timerDurations.at(i)));
        }
    }

private slots:
    void showAddTimerDialog() {
        // Utwórz okno dialogowe
        QDialog *timerDialog = new QDialog(this);
        timerDialog->setWindowTitle("Dodaj minutnik");

        // Ustaw układ dla okna dialogowego
        QVBoxLayout *layout = new QVBoxLayout(timerDialog);

        // Dodaj elementy interfejsu użytkownika do okna dialogowego
        QLabel *nameLabel = new QLabel("Nazwa minutnika:", timerDialog);
        QLineEdit *nameLineEdit = new QLineEdit(timerDialog);

        QLabel *durationLabel = new QLabel("Czas trwania (minuty):", timerDialog);
        QSpinBox *durationSpinBox = new QSpinBox(timerDialog);
        durationSpinBox->setMinimum(1);  // Ustaw minimalną wartość

        QPushButton *saveButton = new QPushButton("Zapisz", timerDialog);

        // Połącz sygnał zapisywania z odpowiednim slotem
        QObject::connect(saveButton, &QPushButton::clicked, [=]() {
            // Zapisz informacje o minutniku
            QString timerName = nameLineEdit->text();
            int timerDuration = durationSpinBox->value();
            timers.append(TimerInfo(timerName, timerDuration));

            // Zaktualizuj listę zapisanych minutników
            updateTimersList();

            // Zamknij okno dialogowe po zapisaniu
            timerDialog->accept();

            // Zapisz minutniki po dodaniu nowego
            saveTimers();
        });

        // Dodaj elementy do układu
        layout->addWidget(nameLabel);
        layout->addWidget(nameLineEdit);
        layout->addWidget(durationLabel);
        layout->addWidget(durationSpinBox);
        layout->addWidget(saveButton);

        // Pokaż okno dialogowe
        timerDialog->exec();
    }

    void showContextMenu(const QPoint &pos) {
        QMenu contextMenu(tr("Kontekstowe menu"), this);

        // Sprawdź, czy wybrano element z listy
        QListWidgetItem *selectedItem = timersListWidget->itemAt(pos);
        if (selectedItem) {
            QAction *removeAction = contextMenu.addAction("Usuń minutnik");
            QAction *renameAction = contextMenu.addAction("Zmień nazwę minutnika");
            QAction *activeAction = contextMenu.addAction("Uruchom minutnik");

            // Wykonaj odpowiednią akcję na podstawie wyboru
            QAction *selectedAction = contextMenu.exec(timersListWidget->mapToGlobal(pos));
            if (selectedAction == removeAction) {
                // Usuń minutnik
                int index = timersListWidget->row(selectedItem);
                timers.removeAt(index);
                updateTimersList();
                saveTimers();
            } else if (selectedAction == renameAction) {
                // Zmień nazwę minutnika
                int index = timersListWidget->row(selectedItem);
                QString currentName = timers.at(index).name;

                // Utwórz okno dialogowe do zmiany nazwy
                bool ok;
                QString newName = QInputDialog::getText(this, "Zmień nazwę minutnika", "Nowa nazwa:", QLineEdit::Normal, currentName, &ok);
                if (ok && !newName.isEmpty()) {
                    timers[index].name = newName;
                    updateTimersList();
                    saveTimers();
                }
            } else if (selectedAction == activeAction) {
                // Uruchom minutnik
                int index = timersListWidget->row(selectedItem);
                runTimer(timers[index]);
            }
        }
    }


    void runTimer(const TimerInfo &timer) {
        QDialog *runTimerDialog = new QDialog(this);
        runTimerDialog->setWindowTitle(timer.name);

        QVBoxLayout *layout = new QVBoxLayout(runTimerDialog);

        QLabel *timerLabel = new QLabel("Minutnik: " + timer.name, runTimerDialog);
        QLabel *timeLabel = new QLabel("Czas: " + QString::number(timer.duration) + " minut", runTimerDialog);

        QPushButton *startButton = new QPushButton("Uruchom", runTimerDialog);
        QPushButton *stopButton = new QPushButton("Stop", runTimerDialog);
        QPushButton *resumeButton = new QPushButton("Wznów", runTimerDialog);

            qint64 remainingTime = timer.duration * 60;  // Initial remaining time in seconds

        QTimer *countdownTimer = new QTimer(runTimerDialog);  // Create QTimer on the heap

        QObject::connect(startButton, &QPushButton::clicked, [=]() mutable {
            countdownTimer->setInterval(1000);  // Every second

            if (remainingTime > 0) {
                countdownTimer->start();

                QObject::connect(countdownTimer, &QTimer::timeout, [=]() mutable {
                    --remainingTime;

                    int remainingMinutes = remainingTime / 60;
                    int remainingSeconds = remainingTime % 60;

                    timeLabel->setText("Pozostało: " + QString::number(remainingMinutes) + " minut " +
                                       QString::number(remainingSeconds) + " sekund");

                    if (remainingTime <= 0) {
                        countdownTimer->stop();

                        // Show message box when the timer reaches zero
                        QMessageBox msgBox;
                        msgBox.setText("Minutnik zakończony!");
                            msgBox.setInformativeText("Co chcesz zrobić?");
                            msgBox.addButton("Uruchom ponownie", QMessageBox::YesRole);
                        msgBox.addButton("Zakończ", QMessageBox::NoRole);

                            int result = msgBox.exec();
                        if (result == QMessageBox::YesRole) {
                            // User chose to start again
                            remainingTime = timer.duration * 60;
                            startButton->setEnabled(false);
                            stopButton->setEnabled(true);
                            resumeButton->setEnabled(false);
                            countdownTimer->start();
                        } else {
                            // User chose to finish
                            runTimerDialog->accept();
                        }
                    }
                });
            }

            startButton->setEnabled(false);
            stopButton->setEnabled(true);
            resumeButton->setEnabled(false);
        });

        QObject::connect(stopButton, &QPushButton::clicked, [=]() mutable {
            countdownTimer->stop();
            startButton->setEnabled(false);
            stopButton->setEnabled(false);
            resumeButton->setEnabled(true);
        });

        QObject::connect(resumeButton, &QPushButton::clicked, [=]() mutable {
            if (remainingTime > 0) {
                countdownTimer->start();
                startButton->setEnabled(false);
                stopButton->setEnabled(true);
                resumeButton->setEnabled(false);
            }
        });

        QObject::connect(startButton, &QPushButton::clicked, [=]() mutable {
            remainingTime = timer.duration * 60;
            startButton->setEnabled(false);
            stopButton->setEnabled(true);
            resumeButton->setEnabled(false);
            countdownTimer->start();
        });

        layout->addWidget(timerLabel);
        layout->addWidget(timeLabel);
        layout->addWidget(startButton);
        layout->addWidget(stopButton);
        layout->addWidget(resumeButton);

        runTimerDialog->exec();
    }


private:
    QListWidget *timersListWidget;
    QElapsedTimer elapsedTimer;
    QTimer updateTimer;
};

void createWidgetsTab(QTabWidget *tabWidget) {
    WidgetsTab *widgetsTab = new WidgetsTab;

    // Dodaj zakładkę do głównego widżetu zakładek
    tabWidget->addTab(widgetsTab, "Widgety");
}
