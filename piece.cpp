#include "piece.h"
#include "gamemanager.h"
//#include "mainwindow.h"
#include "visualize.h"
#include <QDebug>

const int Piece::pawn = 0;
const int Piece::bishop = 1;
const int Piece::knight = 2;
const int Piece::rook = 3;
const int Piece::queen = 4;
const int Piece::king = 5;

const int Piece::white = 0;
const int Piece::black = 1;

const vector<vector<QPoint>> Piece::moveOffset = {{QPoint(0, -1)},
                                           {QPoint(1, 1), QPoint(1, -1), QPoint(-1, -1), QPoint(-1, 1)},
                                           {QPoint(-1, 2), QPoint(1, 2), QPoint(2, 1), QPoint(2, -1), QPoint(1, -2), QPoint(-1, -2), QPoint(-2, -1), QPoint(-2, 1)},
                                           {QPoint(0, 1), QPoint(1, 0), QPoint(0, -1), QPoint(-1, 0)},
                                           {QPoint(1, 1), QPoint(1, -1), QPoint(-1, -1), QPoint(-1, 1), QPoint(0, 1), QPoint(1, 0), QPoint(0, -1), QPoint(-1, 0)},
                                           {QPoint(1, 1), QPoint(1, -1), QPoint(-1, -1), QPoint(-1, 1), QPoint(0, 1), QPoint(1, 0), QPoint(0, -1), QPoint(-1, 0)}};

Piece::Piece(int type, int color, int x, int y){
    this->type = type;
    this->color = color;
    this->x = x;
    this->y = y;
    this->hasFirstMove = false;
    this->setPixmap(GameManager::images[color][type]->scaled(QSize(GameManager::square_width, GameManager::square_width)));
    this->setPos(GameManager::square_width*x, GameManager::square_width*y);
    GameManager::scene->addItem(this);


}

bool Piece::edgeCheck(int tx, int ty){
    if(tx < 0 || tx >= 8 || ty < 0 || ty >= 8){
        return false;
    }else{
        return true;
    }
}

int Piece::getColor(){
    return color;
}

int Piece::getType(){
    return type;
}

int Piece::getX(){
    return x;
}

int Piece::getY(){
    return y;
}

void Piece::captured(){
    GameManager::pieceOnSquare[y][x] = NULL;
    for(int i = 0; i < GameManager::pieces[color].size(); i++){
        if(GameManager::pieces[color][i] == this){
            GameManager::pieces[color].erase(GameManager::pieces[color].begin() + i);
        }
    }
    delete(this);
}

void Piece::findLegalMove(){
    legalMove.clear();
    if(type == 0 || type == 2 || type == 5){
        oneBlockMove();
    }else{
        slideMove();
    }
    for(int i = 0 ; i < legalMove.size(); i++){
        qDebug() << "(" << legalMove[i].x() << ", " << legalMove[i].y() << "), ";
    }
}

void Piece::slideMove(){
    for(int i = 0; i < moveOffset[type].size(); i++){
        QPoint offset = moveOffset[type][i];
        if(color == black) offset = -offset;
        QPoint now(x, y);
        while(edgeCheck(now.x(), now.y())){
            now += offset;
            if(!edgeCheck(now.x(), now.y())) break;
            if(GameManager::pieceOnSquare[now.y()][now.x()] == NULL){
                legalMove.push_back(now);
            }else{
                if(GameManager::pieceOnSquare[now.y()][now.x()]->getColor() == color){
                    break;
                }else{
                    legalMove.push_back(now);
                    break;
                }
            }
        }
    }
}

void Piece::oneBlockMove(){
    for(int i = 0; i < moveOffset[type].size(); i++){
        QPoint offset = moveOffset[type][i];
        if(color == black) offset = -offset;
        QPoint now(x, y);
        now += offset;
        if(!edgeCheck(now.x(), now.y())) continue;
        if(GameManager::pieceOnSquare[now.y()][now.x()] == NULL){
            legalMove.push_back(now);
        }else{
            if(GameManager::pieceOnSquare[now.y()][now.x()]->getColor() == color){
                continue;
            }else{
                legalMove.push_back(now);
                continue;
            }
        }
    }
}

void Piece::movetoSquare(int tx, int ty){
    GameManager::pieceOnSquare[y][x] = NULL;
    x = tx;
    y = ty;
    GameManager::pieceOnSquare[y][x] = this;
    this->setPos(GameManager::square_width*x, GameManager::square_width*y);
}

void Piece::mousePressEvent(QGraphicsSceneMouseEvent *event){
    findLegalMove();
    this->setPos(event->scenePos().x()-GameManager::square_width/2, event->scenePos().y()-GameManager::square_width/2);
    this->setZValue(1);
    Visualize::showLegalMove(legalMove);
}

void Piece::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    this->setPos(event->scenePos().x()-GameManager::square_width/2, event->scenePos().y()-GameManager::square_width/2);
    //qDebug() << (int)event->scenePos().x() / GameManager::square_width << " " << (int)event->scenePos().y() / GameManager::square_width;
}

void Piece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    Visualize::hideLegalMove();
    this->setZValue(0);
    bool flag = false;
    int tx = (int)event->scenePos().x() / GameManager::square_width;
    int ty = (int)event->scenePos().y() / GameManager::square_width;
    for(int i = 0; i < legalMove.size(); i++){
        if(tx == legalMove[i].x() && ty == legalMove[i].y()){
            flag = true;
            break;
        }
    }
    if(flag){
        movetoSquare(tx, ty);
    }else{
        this->setPos(GameManager::square_width*x, GameManager::square_width*y);
    }

    //qDebug() << x << " " << y;
}

