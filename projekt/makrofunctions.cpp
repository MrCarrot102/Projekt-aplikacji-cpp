/*#include <QTabWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <QMessageBox>
#include <QProcess>
#include "makrofunctions.h"  // Dodaj włączenie pliku nagłówkowego
#include "makrofunctions.h"

void createMacrosTab(QTabWidget *tabWidget) {
    QWidget *macrosTab = new QWidget;
    QVBoxLayout *macrosLayout = new QVBoxLayout(macrosTab);

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

    MakroFunctions makroFunctions;  // Poprawiono nazwę klasy

    QObject::connect(createMacroButton, &QPushButton::clicked, [=]() {
        QString macroName = macroNameInput->text();
        QString shortcut = shortcutInput->text();
        int volume = volumeSlider->value();
        bool mute = muteCheckbox->isChecked();

        if (!macroName.isEmpty() && !shortcut.isEmpty()) {
            makroFunctions.setVolume(volume);

            // Prosta logika makra wyciszająca/dźwięk systemowy
            QString command = "amixer -D pulse sset Master " + QString::number(volume) + "%";
            command += mute ? " && amixer -D pulse sset Master mute" : " && amixer -D pulse sset Master unmute";
            QProcess::startDetached(command);

            QMessageBox::information(nullptr, "Sukces", "Makro utworzone: " + macroName);
        } else {
            QMessageBox::warning(nullptr, "Błąd", "Wprowadź nazwę makra i skrót klawiszowy.");
        }
    });

    tabWidget->addTab(macrosTab, "Makra");
}
*/
