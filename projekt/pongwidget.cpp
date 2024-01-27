/*#include "pongwidget.h"
#include <QKeyEvent>
#include <QGraphicsTextItem>

PongWidget::PongWidget(QWidget *parent)
    : QGraphicsView(parent),
    scene(new QGraphicsScene(this)),
    ball(new QGraphicsRectItem),
    paddleSpeed(5),
    timer(new QTimer(this)) {
    setupScene();
    reset();

    connect(timer, &QTimer::timeout, this, &PongWidget::update);
    timer->start(16); // 60 FPS
}

void PongWidget::setupScene() {
    scene->setSceneRect(0, 0, 400, 300);

    ball->setRect(0, 0, 10, 10);
    scene->addItem(ball);

    paddles[0] = new QGraphicsRectItem(0, 0, 10, 50);
    paddles[0]->setPos(10, scene->height() / 2 - paddles[0]->rect().height() / 2);
    scene->addItem(paddles[0]);

    paddles[1] = new QGraphicsRectItem(0, 0, 10, 50);
    paddles[1]->setPos(scene->width() - 20, scene->height() / 2 - paddles[1]->rect().height() / 2);
    scene->addItem(paddles[1]);

    QGraphicsTextItem *scoreText = new QGraphicsTextItem();
    scene->addItem(scoreText);

    setScene(scene);
}

void PongWidget::reset() {
    ball->setPos(scene->width() / 2 - ball->rect().width() / 2, scene->height() / 2 - ball->rect().height() / 2);
    ballVelocity = QPointF(3, 2);
    score[0] = score[1] = 0;
    update();
}

void PongWidget::update() {
    ball->moveBy(ballVelocity.x(), ballVelocity.y());

    // Paddle collision with top and bottom
    for (int i = 0; i < 2; ++i) {
        if (paddles[i]->y() < 0)
            paddles[i]->setY(0);
        else if (paddles[i]->y() + paddles[i]->rect().height() > scene->height())
            paddles[i]->setY(scene->height() - paddles[i]->rect().height());
    }

    // Ball collision with paddles
    if (ball->collidesWithItem(paddles[0]) || ball->collidesWithItem(paddles[1]))
        ballVelocity.setX(-ballVelocity.x());

    // Ball collision with top and bottom
    if (ball->y() < 0 || ball->y() + ball->rect().height() > scene->height())
        ballVelocity.setY(-ballVelocity.y());

    // Ball out of bounds (score)
    if (ball->x() < 0) {
        score[1]++;
        reset();
    } else if (ball->x() + ball->rect().width() > scene->width()) {
        score[0]++;
        reset();
    }

    // Update paddles
    paddles[0]->moveBy(0, paddleSpeed * (keysPressed.contains(Qt::Key_W) - keysPressed.contains(Qt::Key_S)));
    paddles[1]->moveBy(0, paddleSpeed * (keysPressed.contains(Qt::Key_Up) - keysPressed.contains(Qt::Key_Down)));

    // Update the score text
    QGraphicsTextItem *scoreText = dynamic_cast<QGraphicsTextItem *>(scene->items().last());
    if (scoreText) {
        scoreText->setPlainText(QString("Score: %1 - %2").arg(score[0]).arg(score[1]));
        scoreText->setPos(scene->width() / 2 - scoreText->boundingRect().width() / 2, 10);
    }
}

void PongWidget::keyPressEvent(QKeyEvent *event) {
    keysPressed.insert(event->key());
}

void PongWidget::keyReleaseEvent(QKeyEvent *event) {
    keysPressed.remove(event->key());
}

void PongWidget::movePaddle(int player, int direction) {
    // Not used in this example, but you can implement if needed
}
*/
