#include "visualize.h"
#include "gamemanager.h"

vector<QGraphicsEllipseItem*> Visualize::legalMoveCircle;
QGraphicsRectItem* Visualize::highlightSquare = NULL;

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
