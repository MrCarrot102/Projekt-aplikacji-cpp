// makrofunctions.h
#ifndef MAKROFUNCTIONS_H
#define MAKROFUNCTIONS_H

#include <QObject>
#include <QTabWidget>

class MakroFunctions : public QObject
{
    Q_OBJECT

public:
    explicit MakroFunctions(QObject *parent = nullptr);

public slots:
    void setVolume(int volume) const; // Dodaj const tutaj
};

void createMakrosTab (QTabWidget *TabWidget);

#endif // MAKROFUNCTIONS_H
