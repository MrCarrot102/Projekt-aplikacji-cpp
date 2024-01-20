// folderfunctions.h
#ifndef FOLDERFUNCTIONS_H
#define FOLDERFUNCTIONS_H

#include <QString>
#include <QListWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>

void showFolderContent(const QString &folderPath);

bool setFolderIcon(const QString &folderPath, const QString &iconPath);

void saveCreatedFolders(const QListWidget *folderList);

void clearCreatedFolders(QListWidget *folderList);

void deleteFilesInFolder(const QString &folderPath);

void renameFilesInFolder(const QString &folderPath);

void createSubfolder(const QString &folderPath);

void showFolderContentInList(QListWidget *folderList, const QString &folderPath);

void showFolderInLocation ( const QString &location );

#endif // FOLDERFUNCTIONS_H
