/*#ifndef PONGWIDGET_H
#define PONGWIDGET_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QTimer>
#include <QWidget>

class PongWidget : public QGraphicsView {
    Q_OBJECT

public:
    PongWidget(QWidget *parent = nullptr);

public slots:
    void movePaddle(int player, int direction);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QGraphicsScene *scene;
    QGraphicsRectItem *ball;
    QGraphicsRectItem *paddles[2];
    QPointF ballVelocity;
    int paddleSpeed;
    int score[2];
    QTimer *timer;

    void setupScene();
    void reset();
    void update();
};

#endif // PONGWIDGET_H
*/
