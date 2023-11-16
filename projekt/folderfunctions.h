#ifndef FOLDERFUNTIONS_H
#define FOLDERFUNTIONS_H

#include <QString>
#include <QListWidget>

// funkcja do pokazywania zawartosci
void showFolderContent ( const QString &folderPath );
// ustawianie ikony
bool setFolderIcon ( const QString &folderPath , const QString &iconPath );
// lista utworzonych folderow
void saveCreatedFolders ( const QListWidget *folderList );
// czyszczenie listy utworzonych folderow
void clearCreatedFolders ( QListWidget *folderList );

#endif // FOLDERFUNTIONS_H
