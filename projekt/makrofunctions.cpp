#include "makrofunctions.h"
#include <QTabWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <QProcess>
#include <QMessageBox>


void createMacrosTab(QTabWidget *tabWidget) {
    QWidget *macrosTab = new QWidget;
    QVBoxLayout *macrosLayout = new QVBoxLayout(macrosTab);

    // Dodajemy komponenty UI dla tworzenia makr
    QLineEdit *macroNameInput = new QLineEdit;
    macroNameInput->setPlaceholderText("Nazwa makra");
    macrosLayout->addWidget(macroNameInput);

    QLineEdit *shortcutInput = new QLineEdit;
    shortcutInput->setPlaceholderText("Skrót klawiszowy");
        macrosLayout->addWidget(shortcutInput);

    QPushButton *createMacroButton = new QPushButton("Utwórz makro");
                                     macrosLayout->addWidget(createMacroButton);

    QSlider *volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    macrosLayout->addWidget(volumeSlider);

    QCheckBox *muteCheckbox = new QCheckBox("Wycisz");
    macrosLayout->addWidget(muteCheckbox);

    // Obsługa przycisku utworzenia makra
    QObject::connect(createMacroButton, &QPushButton::clicked, [=]() {
        QString macroName = macroNameInput->text();
        QString shortcut = shortcutInput->text();
        int volume = volumeSlider->value();
        bool mute = muteCheckbox->isChecked();

        if (!macroName.isEmpty() && !shortcut.isEmpty()) {
            // Prosta logika makra zmieniająca głośność i włączająca/wyłączająca dźwięk systemowy
            QString command = "amixer -D pulse sset Master " + QString::number(volume) + "%";
            if (mute) {
                command += " && amixer -D pulse sset Master mute";
            } else {
                command += " && amixer -D pulse sset Master unmute";
            }

            // Wykonaj polecenie systemowe
            QProcess::startDetached(command);

            // Przykładowa informacja, że makro zostało utworzone:
            QMessageBox::information(nullptr, "Sukces", "Makro utworzone: " + macroName);
        } else {
            QMessageBox::warning(nullptr, "Błąd", "Wprowadź nazwę makra i skrót klawiszowy.");
        }
    });

    // Dodaj zakładkę do głównego widżetu zakładek
    tabWidget->addTab(macrosTab, "Makra");
}

#define MARKTOFUNCTIONS_H
