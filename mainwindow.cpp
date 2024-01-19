#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gamemanager.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GameManager::window = this;
    GameManager::images[0][0] = new QPixmap(":/white_pawn");
    GameManager::images[0][1] = new QPixmap(":/white_bishop");
    GameManager::images[0][2] = new QPixmap(":/white_knight");
    GameManager::images[0][3] = new QPixmap(":/white_rook");
    GameManager::images[0][4] = new QPixmap(":/white_queen");
    GameManager::images[0][5] = new QPixmap(":/white_king");
    GameManager::images[1][0] = new QPixmap(":/black_pawn");
    GameManager::images[1][1] = new QPixmap(":/black_bishop");
    GameManager::images[1][2] = new QPixmap(":/black_knight");
    GameManager::images[1][3] = new QPixmap(":/black_rook");
    GameManager::images[1][4] = new QPixmap(":/black_queen");
    GameManager::images[1][5] = new QPixmap(":/black_king");

    GameManager::generateChessBoard();

}

Ui::MainWindow* MainWindow::getui(){
    return ui;
}


MainWindow::~MainWindow()
{
    delete ui;
}

