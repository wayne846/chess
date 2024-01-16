#include "piece.h"
#include "gamemanager.h"
#include <QDebug>

const int Piece::pawn = 0;
const int Piece::bishop = 1;
const int Piece::knight = 2;
const int Piece::rook = 3;
const int Piece::queen = 4;
const int Piece::king = 5;

const int Piece::white = 0;
const int Piece::black = 1;

Piece::Piece(int type, int color, int x, int y){
    this->type = type;
    this->color = color;
    this->x = x;
    this->y = y;
    this->setPixmap(GameManager::images[color][type]->scaled(QSize(GameManager::square_width, GameManager::square_width)));
    this->setPos(GameManager::square_width*x, GameManager::square_width*y);
    GameManager::scene->addItem(this);


}

void Piece::mousePressEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << "test";
}

void Piece::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << (int)event->scenePos().x() / GameManager::square_width << " " << (int)event->scenePos().y() / GameManager::square_width;
}
void Piece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << "test end";
}

