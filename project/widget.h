#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <paintboard.h>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
signals:

private slots:
    void on_pushButton_1_clicked();//退出程序

    void on_pushButton_2_clicked();//其他模式

    void on_pushButton_3_clicked();//快速对局

private:
    Ui::Widget *ui;
    Paintboard *paintboard;//下棋界面

};
#endif // WIDGET_H
