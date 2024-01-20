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
#include <QTabWidget>
#include "folderfunctions.h"
#include "widgetfunctions.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Twój zbędnik");
    window.resize(600, 400);

    QTabWidget tabWidget(&window);

    // Zakładka do tworzenia folderów
    QWidget folderTab;
    QVBoxLayout folderLayout(&folderTab);

    QLineEdit folderNameInput;
    folderNameInput.setPlaceholderText("Nazwa folderu");
    folderLayout.addWidget(&folderNameInput);

    QPushButton chooseLocationButton("Wybierz lokalizację");
    folderLayout.addWidget(&chooseLocationButton);


    QPushButton createButton("Utwórz folder");
    folderLayout.addWidget(&createButton);

    QListWidget folderList;
    folderLayout.addWidget(&folderList);

    QString selectedLocation = QDir::homePath() + "/Pulpit";

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

                    QListWidgetItem *folderItem = new QListWidgetItem(QIcon(":/folder.png"), folderName);
                    folderList.addItem(folderItem);

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
                                          QAction *deleteFilesAction = contextMenu.addAction("Usuń pliki w folderze");
                  QAction *renameFilesAction = contextMenu.addAction("Zmień nazwy plików");
                                        QAction *createSubfolderAction = contextMenu.addAction("Utwórz podfolder");

            QAction *selectedAction = contextMenu.exec(QCursor::pos());
            if (selectedAction == changeIconAction) {
                QString folderName = selectedItem->text();
                QString folderPath = QDir(selectedLocation).filePath(folderName);

                QString iconPath = QFileDialog::getOpenFileName(nullptr, "Wybierz nową ikonę", QDir::homePath(), "Ikony (*.png *.ico)");

                if (!iconPath.isEmpty()) {
                    QPixmap pixmap(iconPath);
                    QIcon folderIcon(pixmap);
                    selectedItem->setIcon(folderIcon);

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

            // Usuń folder z listy QListWidget
            int index = folderList.row(selectedItem);
            folderList.takeItem(index);

            // Usuń folder z listy zapisanej w pliku konfiguracyjnym
            QStringList folders = settings.value("createdFolders").toStringList();
            folders.removeAll(folderName);
            settings.setValue("createdFolders", folders);
        }
    }
            }else if (selectedAction == deleteFilesAction) {
                // Wywołanie nowej funkcji
                QString folderName = selectedItem->text();
                QString folderPath = QDir(selectedLocation).filePath(folderName);
                deleteFilesInFolder(folderPath);
            } else if (selectedAction == renameFilesAction) {
                // Wywołanie nowej funkcji
                QString folderName = selectedItem->text();
                QString folderPath = QDir(selectedLocation).filePath(folderName);
                renameFilesInFolder(folderPath);
            } else if (selectedAction == createSubfolderAction) {
                // Wywołanie nowej funkcji
                QString folderName = selectedItem->text();
                QString folderPath = QDir(selectedLocation).filePath(folderName);
                createSubfolder( folderPath);
            }
        } else {
            QAction *clearListAction = contextMenu.addAction("Wyczyść listę folderów");
            QObject::connect(clearListAction, &QAction::triggered, [&]() {
                clearCreatedFolders(&folderList);
            });
        }
    });


    // jesli jest potrzeba wyczyszczenia listy folderow odkomentowac i uruchomic program
   /* while (folderList.count() > 0) {
        delete folderList.takeItem(0);
    }*/

    // Zakładka do tworzenia makr
    QWidget makroTab;
    //createMakrosTab(&makroTab);

    // Zakładka do tworzenia widgetów
    QTabWidget widgetsTab;
    createWidgetsTab(&widgetsTab);

    // Dodaj zakładki do głównego widżetu zakładek
    tabWidget.addTab(&folderTab, "Foldery");
    tabWidget.addTab(&makroTab, "Makra");
    tabWidget.addTab(&widgetsTab, "Widgety");

    // Dodaj główną zakładkę
    QVBoxLayout mainLayout(&window);
    mainLayout.addWidget(&tabWidget);

    window.show();

    return app.exec();
}
