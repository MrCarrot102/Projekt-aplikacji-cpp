// camerawidget.h
#ifndef kamera_H
#define kamera_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QVBoxLayout>

class CameraWidget : public QWidget {
    Q_OBJECT

public:
    explicit CameraWidget(QWidget *parent = nullptr);

private:
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QVBoxLayout *layout;
};

#endif // CAMERAWIDGET_H
