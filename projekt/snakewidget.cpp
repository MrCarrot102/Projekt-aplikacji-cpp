// snakewidget.cpp
#include "snakewidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>

SnakeWidget::SnakeWidget(QWidget *parent) : QWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);

    // Ustawienia gry
    gridSize = 20;
    maxSnakeLength = 5; // Początkowa długość węża
    snake.append(QPoint(5, 5));  // Początkowa pozycja węża
    direction = Right;

    // Ustawienia timera
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SnakeWidget::moveSnake);
    timer->start(100);  // Aktualizacja co 100 ms

    // Inicjalizacja generatora liczb losowych
    srand(QTime::currentTime().msec());

    // Wygeneruj pierwszy punkt jedzenia
    generateFood();
}

void SnakeWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // Rysuj węża
    painter.setBrush(Qt::green);
    for (const QPoint &point : snake) {
        painter.drawRect(point.x() * gridSize, point.y() * gridSize, gridSize, gridSize);
    }

    // Rysuj jedzenie
    painter.setBrush(Qt::red);
    painter.drawRect(food.x() * gridSize, food.y() * gridSize, gridSize, gridSize);
}

void SnakeWidget::keyPressEvent(QKeyEvent *event) {
    // Obsługa klawiszy strzałek
    switch (event->key()) {
    case Qt::Key_Left:
        if (direction != Right)
            direction = Left;
        break;
    case Qt::Key_Right:
        if (direction != Left)
            direction = Right;
        break;
    case Qt::Key_Up:
        if (direction != Down)
            direction = Up;
        break;
    case Qt::Key_Down:
        if (direction != Up)
            direction = Down;
        break;
    default:
        break;
    }
}

void SnakeWidget::moveSnake() {
    // Porusz węża w zależności od aktualnego kierunku
    QPoint head = snake.first();
    switch (direction) {
    case Left:
        head.setX(head.x() - 1);
        break;
    case Right:
        head.setX(head.x() + 1);
        break;
    case Up:
        head.setY(head.y() - 1);
        break;
    case Down:
        head.setY(head.y() + 1);
        break;
    default:
        break;
    }

    // Sprawdź kolizję z krawędziami okna
    if (head.x() < 0 || head.x() >= width() / gridSize || head.y() < 0 || head.y() >= height() / gridSize) {
        resetGame();
        return;
    }

    // Dodaj nową głowę węża
    snake.prepend(head);

    // Sprawdź kolizję z jedzeniem
    if (head == food) {
        // Zjedzono jedzenie, zwiększ długość węża
        maxSnakeLength += 1;
        // Wygeneruj nowe jedzenie
        generateFood();
    }

    // Usuń ogon węża, jeśli przekroczył maksymalną długość
    if (snake.length() > maxSnakeLength) {
        snake.removeLast();
    }

    // Sprawdź kolizję z samym sobą
    if (checkSelfCollision()) {
        // Przegrałeś grę - dodaj obsługę końca gry
        resetGame();
    }

    // Odśwież widok
    update();
}

bool SnakeWidget::checkSelfCollision() {
    QPoint head = snake.first();
    for (int i = 1; i < snake.length(); ++i) {
        if (snake.at(i) == head) {
            return true;  // Kolizja z samym sobą
        }
    }
    return false;
}

void SnakeWidget::resetGame() {
    // Resetuj ustawienia gry
    snake.clear();
    snake.append(QPoint(5, 5));  // Początkowa pozycja węża
    direction = Right;
    maxSnakeLength = 5;

    // Restartuj timer
    timer->start();

    // Wygeneruj nowe jedzenie
    generateFood();
}

void SnakeWidget::generateFood() {
    // Generuj losową pozycję dla jedzenia
    int x = rand() % (width() / gridSize);
    int y = rand() % (height() / gridSize);
    food = QPoint(x, y);

    // Sprawdź, czy jedzenie nie generuje się w miejscu, gdzie jest wąż
    while (snake.contains(food)) {
        x = rand() % (width() / gridSize);
        y = rand() % (height() / gridSize);
        food = QPoint(x, y);
    }
}
