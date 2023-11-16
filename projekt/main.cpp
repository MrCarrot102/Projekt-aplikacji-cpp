// main.cpp

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDir>
#include <QListWidget>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QMenu>
#include <QFileDialog>
#include <QPixmap>
#include <QIcon>
#include <QInputDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include "folderfunctions.h" // Dodaj dołączenie pliku nagłówkowego

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Utwórz folder");
    window.resize(400, 400);

    QVBoxLayout layout(&window);

    QLineEdit folderNameInput;
    folderNameInput.setPlaceholderText("Nazwa folderu");
    layout.addWidget(&folderNameInput);

    QPushButton chooseLocationButton("Wybierz lokalizację");
    layout.addWidget(&chooseLocationButton);

    QPushButton createButton("Utwórz folder");
    layout.addWidget(&createButton);

    QListWidget folderList;  // Dodajemy listę na stworzone foldery
    layout.addWidget(&folderList);

    QString selectedLocation = QDir::homePath() + "/Pulpit"; // Ustaw domyślną lokalizację na pulpicie

    // Wczytywanie zapamiętanych folderów
    QSettings settings("MyApp", "FolderApp");
    QStringList folders = settings.value("createdFolders").toStringList();
    folderList.addItems(folders);

    QObject::connect(&chooseLocationButton, &QPushButton::clicked, [&]() {
        QString newLocation = QFileDialog::getExistingDirectory(nullptr, "Wybierz lokalizację", selectedLocation);
        if (!newLocation.isEmpty()) {
            selectedLocation = newLocation;
        }
    });

    QObject::connect(&createButton, &QPushButton::clicked, [&]() {
        QString folderName = folderNameInput.text();
        if (!folderName.isEmpty()) {
            QDir dir(selectedLocation);
            if (!dir.exists(folderName)) {
                if (dir.mkdir(folderName)) {
                    QString folderPath = dir.filePath(folderName);
                    folderNameInput.clear();

                    // Aktualizacja ikony folderu na liście
                    QListWidgetItem *folderItem = new QListWidgetItem(QIcon(":/folder.png"), folderName);
                    folderList.addItem(folderItem);

                    // Zapisz listę folderów
                    saveCreatedFolders(&folderList);
                }
            } else {
                QMessageBox::critical(nullptr, "Błąd", "Folder o tej nazwie już istnieje.");
            }
        }
    });

    folderList.setContextMenuPolicy(Qt::CustomContextMenu);

    QObject::connect(&folderList, &QWidget::customContextMenuRequested, [&]() {
        QMenu contextMenu;
        QListWidgetItem *selectedItem = folderList.currentItem();
        if (selectedItem) {
            QAction *showContentAction = contextMenu.addAction("Pokaż zawartość");
            QAction *changeIconAction = contextMenu.addAction("Zmień ikonę");
            QAction *deleteFolderAction = contextMenu.addAction("Usuń folder");

            QAction *selectedAction = contextMenu.exec(QCursor::pos());
            if (selectedAction == changeIconAction) {
                QString folderName = selectedItem->text();
                QString folderPath = QDir(selectedLocation).filePath(folderName);

                // Wybieranie nowej ikony z plików
                QString iconPath = QFileDialog::getOpenFileName(nullptr, "Wybierz nową ikonę", QDir::homePath(), "Ikony (*.png *.ico)");

                if (!iconPath.isEmpty()) {
                    // Aktualizacja ikony folderu na liście
                    QPixmap pixmap(iconPath);
                    QIcon folderIcon(pixmap);
                    selectedItem->setIcon(folderIcon);

                    // Aktualizacja ikony folderu na pulpicie
                    setFolderIcon(folderPath, iconPath);
                }
            } else if (selectedAction == showContentAction) {
                QString folderName = selectedItem->text();
                QString folderPath = QDir(selectedLocation).filePath(folderName);
                showFolderContent(folderPath);
            } else if (selectedAction == deleteFolderAction) {
                QString folderName = selectedItem->text();
                QString folderPath = QDir(selectedLocation).filePath(folderName);
                QDir dir(folderPath);
                if (dir.exists()) {
                    if (QMessageBox::question(nullptr, "Usuń folder", "Czy na pewno chcesz usunąć ten folder?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                        dir.removeRecursively();
                        delete selectedItem;

                        // Zaktualizuj zapisane foldery
                        saveCreatedFolders(&folderList);
                    }
                }
            }
        } else {
            // Dodaj opcję czyszczenia listy przy kliknięciu prawym przyciskiem myszy na pustym polu w liście
            QAction *clearListAction = contextMenu.addAction("Wyczyść listę folderów");
            connect(clearListAction, &QAction::triggered, [&]() {
                clearCreatedFolders(&folderList);
            });
        }
    });

    window.show();

    return app.exec();
}
