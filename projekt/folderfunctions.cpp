// folderfunctions.cpp
#include "folderfunctions.h"
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QListWidget>
#include <QInputDialog>
#include <QRandomGenerator>

void showFolderContent(const QString &folderPath)
{
    QDir dir(folderPath);
    QStringList content = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    QString contentText = "Zawartość folderu:\n";
    for (const QString &item : content)
    {
        contentText += item + "\n";
    }
    QMessageBox::information(nullptr, "Zawartość folderu", contentText);
}

bool setFolderIcon(const QString &folderPath, const QString &iconPath)
{
    QProcess process;
    QStringList args;
    args << "set";
    args << folderPath;
    args << "metadata::custom-icon";
    args << "file://" + iconPath;
    process.start("gio", args);
    process.waitForFinished();
    return (process.exitCode() == 0);
}

void saveCreatedFolders(const QListWidget *folderList)
{
    QSettings settings("MyApp", "FolderApp");
    QStringList folders;
    for (int i = 0; i < folderList->count(); ++i)
    {
        QListWidgetItem *item = folderList->item(i);
        folders.append(item->text());
    }
    settings.setValue("createdFolders", folders);
}

void clearCreatedFolders(QListWidget *folderList)
{
    folderList->clear();
    QSettings settings("MyApp", "FolderApp");
    settings.remove("createdFolders");
}

// usuwanie plikow z folderu
void deleteFilesInFolder(const QString &folderPath)
{
    QDir dir(folderPath);

    // Usuń wszystkie pliki w folderze
    QStringList files = dir.entryList(QDir::Files);
    for (const QString &file : files)
    {
        QFile::remove(dir.filePath(file));
    }

    // Usuń wszystkie foldery w folderze
    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folder : folders)
    {
        dir.rmdir(folder);
    }

    QMessageBox::information(nullptr, "Usunięcie plików i folderów", "Wszytkie pliki i foldery zostały usunięte.");
}


// zmiana nazwy plikow w folderze
void renameFilesInFolder(const QString &folderPath)
{
    QDir dir(folderPath);

    // Pokaż listę plików/folderów przed naciśnięciem "Zmień nazwę"
    QStringList content = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    QString contentText = "Zawartość folderu:\n";
    /*for (const QString &item : content)
    {
        contentText += item + "\n";
    }
    QMessageBox::information(nullptr, "Zawartość folderu", contentText);
*/
    // Użytkownik wybiera plik z listy
    bool ok;
    QString selectedFile = QInputDialog::getItem(nullptr, "Wybierz plik", "Wybierz plik do zmiany nazwy:", content, 0, false, &ok);

    if (!ok || selectedFile.isEmpty())
    {
        // Użytkownik anulował wybór lub nie wybrał pliku
        return;
    }

    // Użytkownik wprowadza nową nazwę
    QString currentFilePath = dir.filePath(selectedFile);
    QString newFileName = QInputDialog::getText(nullptr, "Zmiana nazwy pliku", "Nowa nazwa dla: " + selectedFile, QLineEdit::Normal, selectedFile);

    if (!newFileName.isEmpty()) // Sprawdź, czy użytkownik wprowadził nową nazwę
    {
        QString newFilePath = dir.filePath(newFileName);
        if (QFile::rename(currentFilePath, newFilePath))
        {
            QMessageBox::information(nullptr, "Zmiana nazwy pliku", "Nazwa pliku została zmieniona na: " + newFileName);
        }
        else
        {
            QMessageBox::warning(nullptr, "Błąd", "Nie udało się zmienić nazwy pliku.");
        }
    }

    // Ponownie pokaż listę plików/folderów po zmianie nazwy
    content = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    contentText = "Zawartość folderu:\n";
    for (const QString &item : content)
    {
        contentText += item + "\n";
    }
    QMessageBox::information(nullptr, "Zawartość folderu po zmianie nazwy", contentText);
}




// tworzenie podfolderow
void createSubfolder(const QString& folderPath) {

    QDir dir(folderPath);

    // Znajdź ostatni utworzony podkatalog
    QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time);

    // Utwórz nowy podkatalog z unikalną liczbą w nazwie
    QString subfolderName = QString("NowyPodFolder") + QString::number(QRandomGenerator:: global()->bounded(1 , 100 ) );

    if (dir.mkdir(subfolderName)) {
        QMessageBox::information(nullptr, "Sukces", "Utworzono podfolder pomyślnie.");
    }
    else {
        QMessageBox::warning(nullptr, "Błąd", "Nie udało się utworzyć podfolderu.");
    }

}

// przegladanie folderu w formie listy
void showFolderContentInList(QListWidget *folderList, const QString &folderPath)
{
    folderList->clear();
    QDir dir(folderPath);
    QStringList content = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &item : content)
    {
        folderList->addItem(item);
    }
}

void showFoldersInLocation(const QString &location)
{
    QDir dir(location);

    if (!dir.exists())
    {
        QMessageBox::warning(nullptr, "Błąd", "Podana lokalizacja nie istnieje.");
        return;
    }

    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    if (folders.isEmpty())
    {
        QMessageBox::information(nullptr, "Brak folderów", "W wybranej lokalizacji nie ma żadnych folderów.");
        return;
    }

    // Wyświetl listę folderów
    QString folderListText = "Foldery w lokalizacji:\n";
    for (const QString &folder : folders)
    {
        folderListText += folder + "\n";
    }

    QMessageBox::information(nullptr, "Lista folderów", folderListText);
}

