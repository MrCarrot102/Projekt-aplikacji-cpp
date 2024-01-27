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
#include <QCalendarWidget>
#include <QUrl>


class TimerInfo {
public:
    QString name;
    int duration;

    TimerInfo(const QString &name, int duration) : name(name), duration(duration) {}
};

class TimerDialog : public QDialog {
public:
    TimerDialog(const TimerInfo &timer, QWidget *parent = nullptr) : QDialog(parent), timer(timer) {
        setWindowTitle(timer.name);
        setWindowFlags(windowFlags() | Qt::Window | Qt::WindowStaysOnTopHint);
        setAttribute(Qt::WA_TranslucentBackground, true);

        QVBoxLayout *layout = new QVBoxLayout(this);
        timerLabel = new QLabel("Minutnik: " + timer.name, this);
        timeLabel = new QLabel("Czas: " + QString::number(timer.duration) + " minut", this);

        startButton = new QPushButton("Uruchom", this);
        stopButton = new QPushButton("Stop", this);
        resumeButton = new QPushButton("Wznów", this);

        remainingTime = timer.duration * 60;

        countdownTimer = new QTimer(this);

        connect(startButton, &QPushButton::clicked, this, &TimerDialog::startTimer);
        connect(stopButton, &QPushButton::clicked, this, &TimerDialog::stopTimer);
        connect(resumeButton, &QPushButton::clicked, this, &TimerDialog::resumeTimer);

        layout->addWidget(timerLabel);
        layout->addWidget(timeLabel);
        layout->addWidget(startButton);
        layout->addWidget(stopButton);
        layout->addWidget(resumeButton);

        updateLabels();
    }

private slots:
    void startTimer() {
        countdownTimer->setInterval(1000);

        if (remainingTime > 0) {
            countdownTimer->start();

            connect(countdownTimer, &QTimer::timeout, this, &TimerDialog::updateTimer);
        }

        startButton->setEnabled(false);
        stopButton->setEnabled(true);
        resumeButton->setEnabled(false);
    }

    void stopTimer() {
        countdownTimer->stop();
        startButton->setEnabled(false);
        stopButton->setEnabled(false);
        resumeButton->setEnabled(true);
    }

    void resumeTimer() {
        if (remainingTime > 0) {
            countdownTimer->start();
            startButton->setEnabled(false);
            stopButton->setEnabled(true);
            resumeButton->setEnabled(false);
        }
    }

    void updateTimer() {
        --remainingTime;
        updateLabels();
    }

    void updateLabels() {
        int remainingMinutes = remainingTime / 60;
        int remainingSeconds = remainingTime % 60;

        timeLabel->setText("Pozostało: " + QString::number(remainingMinutes) + " minut " +
                           QString::number(remainingSeconds) + " sekund");
    }

private:
    TimerInfo timer;
    QLabel *timerLabel;
    QLabel *timeLabel;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *resumeButton;
    qint64 remainingTime;
    QTimer *countdownTimer;
};

class WidgetsTab : public QWidget {
public:
    QList<TimerInfo> timers;  // Lista przechowująca informacje o minutnikach

    WidgetsTab() {
        // Wczytaj zapisane minutniki
        loadTimers();
        setAttribute(Qt::WA_TranslucentBackground, true);

        // Dodaj przycisk "Dodaj kalendarz"
        QPushButton *addCalendarButton = new QPushButton("Dodaj kalendarz", this);

        // Połącz sygnał z przycisku z odpowiednim slotem
        QObject::connect(addCalendarButton, &QPushButton::clicked, this, &WidgetsTab::showAddCalendarWidget);

        // Dodaj przycisk "Dodaj minutnik"
        QPushButton *addTimerButton = new QPushButton("Dodaj minutnik", this);

        // Połącz sygnał z przycisku z odpowiednim slotem
        QObject::connect(addTimerButton, &QPushButton::clicked, this, &WidgetsTab::showAddTimerDialog);

        // Dodaj listę zapisanych minutników
        timersListWidget = new QListWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(addCalendarButton);
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
        // Utwórz niezależne okno dla minutnika
        TimerDialog *runTimerDialog = new TimerDialog(timer);
        activeTimerDialogs.append(runTimerDialog);

        // Pokaż okno dialogowe bez blokowania wykonania
        runTimerDialog->show();
    }

    // Dodaj funkcję do obsługi przycisku "Dodaj kalendarz"
    void showAddCalendarWidget() {
        // Utwórz okno dialogowe z kalendarzem
        QDialog *calendarDialog = new QDialog(this);
        calendarDialog->setWindowTitle("Dodaj kalendarz");

        // Ustaw właściwości okna dialogowego kalendarza
        calendarDialog->setWindowFlags(calendarDialog->windowFlags() | Qt::Window | Qt::WindowStaysOnTopHint);
        calendarDialog->setAttribute(Qt::WA_TranslucentBackground, true);

        QVBoxLayout *layout = new QVBoxLayout(calendarDialog);

        // Dodaj widżet kalendarza
        QCalendarWidget *calendarWidget = new QCalendarWidget(calendarDialog);
        layout->addWidget(calendarWidget);

        // Pokaż okno dialogowe kalendarza
        calendarDialog->exec();
    }

    // dodawanie stopera
    void showAddStoperDialog() {
        // Utwórz okno dialogowe dla dodawania stopera
        QDialog *stoperDialog = new QDialog(this);
        stoperDialog->setWindowTitle("Dodaj stoper");

        // Ustaw właściwości okna dialogowego stopera
        stoperDialog->setWindowFlags(stoperDialog->windowFlags() | Qt::Window | Qt::WindowStaysOnTopHint);
        stoperDialog->setAttribute(Qt::WA_TranslucentBackground, true);

        QVBoxLayout *layout = new QVBoxLayout(stoperDialog);

        // Dodaj elementy interfejsu użytkownika do okna dialogowego stopera
        QLabel *nameLabel = new QLabel("Nazwa stopera:", stoperDialog);
        QLineEdit *nameLineEdit = new QLineEdit(stoperDialog);

        QLabel *durationLabel = new QLabel("Czas trwania (minuty):", stoperDialog);
        QSpinBox *durationSpinBox = new QSpinBox(stoperDialog);
        durationSpinBox->setMinimum(1);  // Ustaw minimalną wartość

        QPushButton *saveButton = new QPushButton("Zapisz", stoperDialog);

        // Połącz sygnał zapisywania z odpowiednim slotem
        QObject::connect(saveButton, &QPushButton::clicked, [=]() {
            // Zapisz informacje o stoperze
            QString stoperName = nameLineEdit->text();
            int stoperDuration = durationSpinBox->value();
            timers.append(TimerInfo(stoperName, stoperDuration));

            // Zaktualizuj listę zapisanych stoperów
            updateTimersList();

            // Zamknij okno dialogowe po zapisaniu
            stoperDialog->accept();

            // Zapisz stopery po dodaniu nowego
            saveTimers();
        });

        // Dodaj elementy do układu
        layout->addWidget(nameLabel);
        layout->addWidget(nameLineEdit);
        layout->addWidget(durationLabel);
        layout->addWidget(durationSpinBox);
        layout->addWidget(saveButton);

        // Pokaż okno dialogowe stopera
        stoperDialog->exec();
    }
private:
    QListWidget *timersListWidget;
    QList<TimerDialog *> activeTimerDialogs;
};

void createWidgetsTab(QTabWidget *tabWidget) {
    WidgetsTab *widgetsTab = new WidgetsTab;

    // Dodaj zakładkę do głównego widżetu zakładek
    tabWidget->addTab(widgetsTab, "Widgety");
}
