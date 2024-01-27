// camerawidget.cpp
#include "kamera.h"

kamera::CameraWidget(QWidget *parent) : QWidget(parent) {
    // Inicjalizacja kamery i widoku kamery
    camera = new QCamera(this);
    viewfinder = new QCameraViewfinder(this);
    layout = new QVBoxLayout(this);

    // Podłącz widok kamery do kamery
    camera->setViewfinder(viewfinder);

    // Dodaj widok kamery do układu
    layout->addWidget(viewfinder);

    // Start kamery
    camera->start();
}
