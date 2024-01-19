#include "visualize.h"
#include "gamemanager.h"
#include "piece.h"
#include "promotionpiece.h"
#include <QDebug>

vector<QGraphicsEllipseItem*> Visualize::legalMoveCircle;
QGraphicsRectItem* Visualize::highlightSquare = NULL;
Piece* Visualize::promotionPawn = NULL;
vector<PromotionPiece*> Visualize::promotionPieces;
QGraphicsRectItem* Visualize::promotionBackground = NULL;

void Visualize::showLegalMove(vector<QPoint> v){
    hideLegalMove();
    for(int i = 0; i < v.size(); i++){
        int width = GameManager::square_width * 0.3;
        QGraphicsEllipseItem *newCircle = GameManager::scene->addEllipse(QRect(0, 0, width, width), QPen(GameManager::gray), QBrush(GameManager::gray));
        int originX = GameManager::square_width * v[i].x();
        int originY = GameManager::square_width * v[i].y();
        newCircle->setPos(originX + GameManager::square_width/2 - width/2, originY + GameManager::square_width/2 - width/2);
        legalMoveCircle.push_back(newCircle);
    }
}

void Visualize::hideLegalMove(){
    for(int i = 0; i < legalMoveCircle.size(); i++){
        if(legalMoveCircle[i] != NULL){
            delete(legalMoveCircle[i]);
        }
    }
    legalMoveCircle.clear();
}

void Visualize::showHighlightSquare(int x, int y){
    if(highlightSquare != NULL && highlightSquare->pos().x() == x && highlightSquare->pos().y() == y){
        return;
    }
    hideHighlightSquare();
    int width = GameManager::square_width;
    highlightSquare = GameManager::scene->addRect(QRect(0, 0, width, width), QPen(GameManager::gray, width/25));
    highlightSquare->setPos(width * x, width * y);
}

void Visualize::hideHighlightSquare(){
    if(highlightSquare != NULL){
        delete(highlightSquare);
        highlightSquare = NULL;
    }
}

void Visualize::showPromotionList(Piece *p){
    promotionPawn = p;

    //first init
    if(promotionPieces.empty()){
        promotionPieces.push_back(new PromotionPiece(Piece::queen, Piece::white));
        promotionPieces.push_back(new PromotionPiece(Piece::knight, Piece::white));
        promotionPieces.push_back(new PromotionPiece(Piece::rook, Piece::white));
        promotionPieces.push_back(new PromotionPiece(Piece::bishop, Piece::white));
    }
    if(promotionBackground == NULL){
        promotionBackground = GameManager::scene->addRect(QRect(0, 0, GameManager::square_width, GameManager::square_width * promotionPieces.size()), QPen(GameManager::white), QBrush(GameManager::white));
        promotionBackground->setZValue(1);
    }

    promotionBackground->show();
    if(p->getColor() == Piece::white){
        for(int i = 0; i < promotionPieces.size(); i++){
            promotionPieces[i]->setColor(Piece::white);
            promotionPieces[i]->setPos(GameManager::square_width * p->getX(), GameManager::square_width * (p->getY()+i));
            promotionPieces[i]->show();
        }
        promotionBackground->setPos(GameManager::square_width * p->getX(), GameManager::square_width * p->getY());
    }else{
        for(int i = 0; i < promotionPieces.size(); i++){
            promotionPieces[i]->setColor(Piece::black);
            promotionPieces[i]->setPos(GameManager::square_width * p->getX(), GameManager::square_width * (p->getY()-i));
            promotionPieces[i]->show();
        }
        promotionBackground->setPos(GameManager::square_width * p->getX(), GameManager::square_width * (p->getY()-promotionPieces.size()+1));
    }
}

void Visualize::promotionClicked(int type){
    promotionBackground->hide();
    for(int i = 0; i < promotionPieces.size(); i++){
        promotionPieces[i]->hide();
    }
    int x = promotionPawn->getX();
    int y = promotionPawn->getY();
    int color = promotionPawn->getColor();

    //delete pawn
    GameManager::pieceOnSquare[y][x] = NULL;
    GameManager::pieces_erase(promotionPawn);
    delete(promotionPawn);
    promotionPawn = NULL;

    //create new piece
    Piece *newPiece = new Piece(type, color, x, y);
    GameManager::pieceOnSquare[y][x] = newPiece;
    GameManager::pieces[color].push_back(newPiece);
    GameManager::turn = newPiece->oppositeColor();
}
