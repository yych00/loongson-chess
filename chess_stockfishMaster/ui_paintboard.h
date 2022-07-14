/********************************************************************************
** Form generated from reading UI file 'paintboard.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAINTBOARD_H
#define UI_PAINTBOARD_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Paintboard
{
public:
    QWidget *chessboard;
    QLabel *chessbd;
    QLabel *lastmove_0;
    QLabel *lastmove_1;
    QPushButton *pushButton_1;
    QPushButton *pushButton_2;
    QLabel *row;
    QLabel *line;
    QTextBrowser *fishText;

    void setupUi(QWidget *Paintboard)
    {
        if (Paintboard->objectName().isEmpty())
            Paintboard->setObjectName(QStringLiteral("Paintboard"));
        Paintboard->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Paintboard->sizePolicy().hasHeightForWidth());
        Paintboard->setSizePolicy(sizePolicy);
        Paintboard->setMinimumSize(QSize(800, 600));
        Paintboard->setMaximumSize(QSize(800, 600));
        QIcon icon;
        icon.addFile(QStringLiteral(":/chess_pieces/wB.png"), QSize(), QIcon::Normal, QIcon::Off);
        Paintboard->setWindowIcon(icon);
        Paintboard->setWindowOpacity(1);
        Paintboard->setLayoutDirection(Qt::LeftToRight);
        Paintboard->setAutoFillBackground(false);
        Paintboard->setStyleSheet(QStringLiteral("background-color: rgb(244, 215, 255);"));
        chessboard = new QWidget(Paintboard);
        chessboard->setObjectName(QStringLiteral("chessboard"));
        chessboard->setGeometry(QRect(60, 60, 480, 480));
        chessboard->setStyleSheet(QStringLiteral("background-color: rgb(85, 255, 255);"));
        chessbd = new QLabel(chessboard);
        chessbd->setObjectName(QStringLiteral("chessbd"));
        chessbd->setEnabled(true);
        chessbd->setGeometry(QRect(0, 0, 480, 480));
        QFont font;
        font.setPointSize(20);
        chessbd->setFont(font);
        chessbd->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255,0);"));
        lastmove_0 = new QLabel(chessboard);
        lastmove_0->setObjectName(QStringLiteral("lastmove_0"));
        lastmove_0->setEnabled(false);
        lastmove_0->setGeometry(QRect(300, 120, 60, 60));
        lastmove_0->setContextMenuPolicy(Qt::NoContextMenu);
        lastmove_0->setStyleSheet(QLatin1String("background-color: transparent;\n"
"border-image: url(:/chess_pieces/lastmove.gif);"));
        lastmove_1 = new QLabel(chessboard);
        lastmove_1->setObjectName(QStringLiteral("lastmove_1"));
        lastmove_1->setEnabled(false);
        lastmove_1->setGeometry(QRect(120, 120, 60, 60));
        lastmove_1->setContextMenuPolicy(Qt::NoContextMenu);
        lastmove_1->setAutoFillBackground(false);
        lastmove_1->setStyleSheet(QLatin1String("background-color: transparent;\n"
"border-image: url(:/chess_pieces/lastmove.gif);"));
        pushButton_1 = new QPushButton(Paintboard);
        pushButton_1->setObjectName(QStringLiteral("pushButton_1"));
        pushButton_1->setGeometry(QRect(610, 510, 140, 30));
        pushButton_2 = new QPushButton(Paintboard);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(610, 450, 140, 30));
        row = new QLabel(Paintboard);
        row->setObjectName(QStringLiteral("row"));
        row->setEnabled(false);
        row->setGeometry(QRect(9, 59, 41, 481));
        QFont font1;
        font1.setPointSize(36);
        row->setFont(font1);
        row->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        row->setWordWrap(true);
        line = new QLabel(Paintboard);
        line->setObjectName(QStringLiteral("line"));
        line->setEnabled(false);
        line->setGeometry(QRect(50, 540, 501, 61));
        line->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        line->setWordWrap(true);
        fishText = new QTextBrowser(Paintboard);
        fishText->setObjectName(QStringLiteral("fishText"));
        fishText->setEnabled(false);
        fishText->setGeometry(QRect(550, 60, 241, 81));
        pushButton_1->raise();
        pushButton_2->raise();
        chessboard->raise();
        row->raise();
        line->raise();
        fishText->raise();

        retranslateUi(Paintboard);

        QMetaObject::connectSlotsByName(Paintboard);
    } // setupUi

    void retranslateUi(QWidget *Paintboard)
    {
        Paintboard->setWindowTitle(QApplication::translate("Paintboard", "chess @_@", nullptr));
        chessbd->setText(QString());
        lastmove_0->setText(QString());
        lastmove_1->setText(QString());
        pushButton_1->setText(QApplication::translate("Paintboard", "\351\200\200\345\207\272", nullptr));
        pushButton_2->setText(QApplication::translate("Paintboard", "\345\233\236\345\210\260\344\270\273\347\225\214\351\235\242", nullptr));
        row->setText(QApplication::translate("Paintboard", "<html><head/><body><p><span style=\" font-size:28pt; font-weight:600;\">8</span></p><p><span style=\" font-size:28pt; font-weight:600;\">7</span></p><p><span style=\" font-size:28pt; font-weight:600;\">6</span></p><p><span style=\" font-size:28pt; font-weight:600;\">5</span></p><p><span style=\" font-size:28pt; font-weight:600;\">4</span></p><p><span style=\" font-size:28pt; font-weight:600;\">3</span></p><p><span style=\" font-size:28pt; font-weight:600;\">2</span></p><p><span style=\" font-size:28pt; font-weight:600;\">1</span></p></body></html>", nullptr));
        line->setText(QApplication::translate("Paintboard", "<html><head/><body><p><span style=\" font-size:36pt; font-weight:600; vertical-align:sub;\">a__ b __ c __ d __ e __ f __ g __ h</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Paintboard: public Ui_Paintboard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAINTBOARD_H
