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

int Piece::oppositeColor(){
    if(color == white){
        return black;
    }else{
        return white;
    }
}

void Piece::findLegalMove(){
    legalMove.clear();
    if(type == pawn || type == knight || type == king){
        oneBlockMove();
    }else{
        slideMove();
    }
}

void Piece::slideMove(){ //for bishop rook queen
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

void Piece::oneBlockMove(){ //for pawn knight king
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
                if(type != pawn){ //pawn can't eat forward piece
                    legalMove.push_back(now);
                }
                continue;
            }
        }
    }

    //pawn other move
    if(type == pawn){
        QPoint offset = moveOffset[type][0];
        if(color == black) offset = -offset;

        //first move 2 square
        if(hasFirstMove == false){

            QPoint now(x, y);
            now += offset;
            if(edgeCheck(now.x(), now.y()) && GameManager::pieceOnSquare[now.y()][now.x()] == NULL){ //check first square
                now += offset;
                if(edgeCheck(now.x(), now.y()) && GameManager::pieceOnSquare[now.y()][now.x()] == NULL){ //check second square
                    legalMove.push_back(now);
                }
            }
        }

        //diagonally capture
        QPoint diagonal1 = QPoint(x, y) + offset + QPoint(1, 0);
        QPoint diagonal2 = QPoint(x, y) + offset + QPoint(-1, 0);
        if(edgeCheck(diagonal1.x(), diagonal1.y()) &&
           GameManager::pieceOnSquare[diagonal1.y()][diagonal1.x()] != NULL &&
           GameManager::pieceOnSquare[diagonal1.y()][diagonal1.x()]->getColor() != color){
            legalMove.push_back(diagonal1);
        }
        if(edgeCheck(diagonal2.x(), diagonal2.y()) &&
           GameManager::pieceOnSquare[diagonal2.y()][diagonal2.x()] != NULL &&
           GameManager::pieceOnSquare[diagonal2.y()][diagonal2.x()]->getColor() != color){
            legalMove.push_back(diagonal2);
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


//mouseEvent

void Piece::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(GameManager::turn != color) return;

    findLegalMove();
    this->setPos(event->scenePos().x()-GameManager::square_width/2, event->scenePos().y()-GameManager::square_width/2);
    this->setZValue(1);

    //visualize
    Visualize::showLegalMove(legalMove);
    Visualize::showHighlightSquare((int)event->scenePos().x() / GameManager::square_width, (int)event->scenePos().y() / GameManager::square_width);
}

void Piece::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(GameManager::turn != color) return;

    this->setPos(event->scenePos().x()-GameManager::square_width/2, event->scenePos().y()-GameManager::square_width/2);

    //visualize
    Visualize::showHighlightSquare((int)event->scenePos().x() / GameManager::square_width, (int)event->scenePos().y() / GameManager::square_width);
    //qDebug() << (int)event->scenePos().x() / GameManager::square_width << " " << (int)event->scenePos().y() / GameManager::square_width;
}

void Piece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(GameManager::turn != color) return;

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
        if(GameManager::pieceOnSquare[ty][tx] != NULL && GameManager::pieceOnSquare[ty][tx]->getColor() != color){
            GameManager::pieceOnSquare[ty][tx]->captured();
        }
        movetoSquare(tx, ty);
        GameManager::turn = oppositeColor();
        hasFirstMove = true;
    }else{
        this->setPos(GameManager::square_width*x, GameManager::square_width*y);
    }

    //visualize
    Visualize::hideLegalMove();
    Visualize::hideHighlightSquare();
    //qDebug() << x << " " << y;
}

