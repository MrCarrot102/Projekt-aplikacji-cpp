#ifndef FOLDERFUNCTIONS_H
#define FOLDERFUNCTIONS_H

#include <QString>
#include <QListWidget>
#include <QTabWidget>

// funkcja do pokazywania zawartości
void showFolderContent(const QString &folderPath);
// ustawianie ikony
bool setFolderIcon(const QString &folderPath, const QString &iconPath);
// lista utworzonych folderów
void saveCreatedFolders(const QListWidget *folderList);
// czyszczenie listy utworzonych folderów
void clearCreatedFolders( QListWidget *folderList);

#endif // FOLDERFUNCTIONS_H
