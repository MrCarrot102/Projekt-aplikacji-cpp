// folderfunctions.cpp
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QListWidget>

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

void clearCreatedFolders ( QListWidget *folderList )
{
    folderList  -> clear();
    QSettings settings("MyApp" , "FolderApp");
    settings.remove ("createdFolders");
}


#define FOLDERFUNCTIONS_H
