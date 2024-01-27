// snakewidget.h
#ifndef SNAKEWIDGET_H
#define SNAKEWIDGET_H

#include <QWidget>

class SnakeWidget : public QWidget {
    Q_OBJECT

public:
    explicit SnakeWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void moveSnake();
    void resetGame();
    bool checkSelfCollision();
    void generateFood();

private:
    enum Direction { Up, Down, Left, Right };

    int gridSize;
    int maxSnakeLength;
    QList<QPoint> snake;
    Direction direction;
    QTimer *timer;
    QPoint food;
};

#endif // SNAKEWIDGET_H
