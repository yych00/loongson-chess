#ifndef DRAGPUSHBUTTON_H
#define DRAGPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>

class DragPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DragPushButton(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QPoint pressPoint;
};

#endif // DRAGPUSHBUTTON_H
