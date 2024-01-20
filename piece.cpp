#include "piece.h"
#include "gamemanager.h"
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

const int Piece::promotionTurn = 3;
const int Piece::endTurn = 4;

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

bool Piece::getHasFirstMove(){
    return hasFirstMove;
}

void Piece::captured(){
    GameManager::pieceOnSquare[y][x] = NULL;
    GameManager::pieces_erase(this);
    delete(this);
}

int Piece::oppositeColor(){
    if(color == white){
        return black;
    }else{
        return white;
    }
}

bool Piece::isAttackPosition(int tx, int ty){
    bool flag = false;
    findLegalMove();
    for(int i = 0; i < legalMove.size(); i++){
        if(legalMove[i].x() == tx && legalMove[i].y() == ty){
            flag = true;
            break;
        }
    }
    return flag;
}

bool Piece::hasLegalMove(){
    findLegalMove();
    findLegalSpecialMove();
    removeIllegalMove();
    if(legalMove.empty()){
        return false;
    }else{
        return true;
    }
}

void Piece::addEnPassant(int c){
    enPassant.push_back(c);
}

void Piece::clearEnPassant(){
    enPassant.clear();
}

void Piece::findLegalMove(){
    legalMove.clear();
    if(type == pawn || type == knight || type == king){
        oneBlockMove();
    }else{
        slideMove();
    }
}

void Piece::removeIllegalMove(){
    for(int i = 0; i < legalMove.size(); i++){
        //save origin board
        Piece *target = NULL;
        int originX = x;
        int originY = y;
        if(GameManager::pieceOnSquare[legalMove[i].y()][legalMove[i].x()] != NULL){
            target = GameManager::pieceOnSquare[legalMove[i].y()][legalMove[i].x()];
            GameManager::pieces_erase(target);
        }

        //assume piece already move
        GameManager::pieceOnSquare[y][x] = NULL;
        GameManager::pieceOnSquare[legalMove[i].y()][legalMove[i].x()] = this;
        x = legalMove[i].x();
        y = legalMove[i].y();

        //check if be checked
        QPoint kingPosition = GameManager::kingPosition(color);
        bool flag = GameManager::isSquareBeAttacked(oppositeColor(), kingPosition.x(), kingPosition.y());

        //return origin board
        if(target != NULL){
            GameManager::pieceOnSquare[legalMove[i].y()][legalMove[i].x()] = target;
            GameManager::pieces[target->getColor()].push_back(target);
        }else{
            GameManager::pieceOnSquare[legalMove[i].y()][legalMove[i].x()] = NULL;
        }
        GameManager::pieceOnSquare[originY][originX] = this;
        x = originX;
        y = originY;

        //remove illegal move
        if(flag){
            legalMove.erase(legalMove.begin() + i);
            i = -1; //restart loop
        }
    }
}

void Piece::move(int tx, int ty){
    bool flag = false;
    for(int i = 0; i < legalMove.size(); i++){
        if(tx == legalMove[i].x() && ty == legalMove[i].y()){
            flag = true;
            break;
        }
    }

    //illegal move
    if(!flag){
        this->setPos(GameManager::square_width*x, GameManager::square_width*y);
        return;
    }

    //pawn
    if(type == pawn){
        //en passant
        if(ty - y == 2 || ty - y == -2){ //move 2 square
            if(edgeCheck(x-1, ty) &&
               GameManager::pieceOnSquare[ty][x-1] != NULL &&
               GameManager::pieceOnSquare[ty][x-1]->type == pawn &&
               GameManager::pieceOnSquare[ty][x-1]->color != color){
                GameManager::pieceOnSquare[ty][x-1]->addEnPassant(x);
            }
            if(edgeCheck(x+1, ty) &&
               GameManager::pieceOnSquare[ty][x+1] != NULL &&
               GameManager::pieceOnSquare[ty][x+1]->type == pawn &&
               GameManager::pieceOnSquare[ty][x+1]->color != color){
                GameManager::pieceOnSquare[ty][x+1]->addEnPassant(x);
            }
        }
        for(int i = 0; i < enPassant.size(); i++){
            if(tx == enPassant[i] && edgeCheck(tx, y) &&
               GameManager::pieceOnSquare[y][tx] != NULL &&
               GameManager::pieceOnSquare[y][tx]->type == pawn &&
               GameManager::pieceOnSquare[y][tx]->color != color){
                GameManager::pieceOnSquare[y][tx]->captured();
                break;
            }
        }
    }

    //king
    int castlingFlag = 0;
    if(type == king){
        //castling
        if(tx - x == 2){
            castlingFlag = 1; //king side
        }else if(tx - x == -2){
            castlingFlag = 2; //queen side
        }
    }

    if(GameManager::pieceOnSquare[ty][tx] != NULL && GameManager::pieceOnSquare[ty][tx]->getColor() != color){
        GameManager::pieceOnSquare[ty][tx]->captured();
    }

    moveToSquare(tx, ty);

    //pawn promotion
    if(type == pawn && (y == 0 || y == 7)){
        Visualize::showPromotionList(this);
        GameManager::turn = promotionTurn;
        return;
    }

    //castling
    if(type == king){
        if(castlingFlag == 1 && edgeCheck(x+1, y) && GameManager::pieceOnSquare[y][x+1] != NULL && GameManager::pieceOnSquare[y][x-1] == NULL){ //king side
            Piece *rook = GameManager::pieceOnSquare[y][x+1];
            rook->moveToSquare(x-1, y);
        }
        if(castlingFlag == 2 && edgeCheck(x-2, y) && GameManager::pieceOnSquare[y][x-2] != NULL && GameManager::pieceOnSquare[y][x+1] == NULL){
            Piece *rook = GameManager::pieceOnSquare[y][x-2];
            rook->moveToSquare(x+1, y);
        }
    }

    GameManager::changTurn(oppositeColor());
    hasFirstMove = true;
    for(int i = 0; i < GameManager::pieces[color].size(); i++){
        GameManager::pieces[color][i]->clearEnPassant();
    }

    //check if end
    if(!GameManager::hasLegalMove(oppositeColor())){
        QPoint enemyKingPosition = GameManager::kingPosition(oppositeColor());
        if(GameManager::isSquareBeAttacked(color, enemyKingPosition.x(), enemyKingPosition.y())){
            qDebug() << color << " win";
            GameManager::endGame(color);
        }else{
            qDebug() << "draw";
            GameManager::endGame(endTurn);
        }
        GameManager::turn = endTurn;
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

    //pawn move
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

void Piece::findLegalSpecialMove(){
    //en passant
    if(type == pawn){
        QPoint offset = moveOffset[type][0];
        if(color == black) offset = -offset;
        for(int i = 0; i < enPassant.size(); i++){
            int ty = y + offset.y();
            if(edgeCheck(enPassant[i], ty) && GameManager::pieceOnSquare[ty][enPassant[i]] == NULL){
                legalMove.push_back(QPoint(enPassant[i], ty));
            }
        }
    }

    //king castling
    if(type == king && hasFirstMove == false && !GameManager::isSquareBeAttacked(oppositeColor(), x, y)){
        //king side
        if(edgeCheck(x+1, y) && GameManager::pieceOnSquare[y][x+1] == NULL && !GameManager::isSquareBeAttacked(oppositeColor(), x+1, y) &&
           edgeCheck(x+2, y) && GameManager::pieceOnSquare[y][x+2] == NULL && !GameManager::isSquareBeAttacked(oppositeColor(), x+2, y) &&
           edgeCheck(x+3, y) && GameManager::pieceOnSquare[y][x+3] != NULL && GameManager::pieceOnSquare[y][x+3]->getType() == rook &&
           GameManager::pieceOnSquare[y][x+3]->getColor() == color && GameManager::pieceOnSquare[y][x+3]->getHasFirstMove() == false){
            legalMove.push_back(QPoint(x+2, y));
        }
        //queen side
        if(edgeCheck(x-1, y) && GameManager::pieceOnSquare[y][x-1] == NULL && !GameManager::isSquareBeAttacked(oppositeColor(), x-1, y) &&
           edgeCheck(x-2, y) && GameManager::pieceOnSquare[y][x-2] == NULL && !GameManager::isSquareBeAttacked(oppositeColor(), x-2, y) &&
           edgeCheck(x-3, y) && GameManager::pieceOnSquare[y][x-3] == NULL &&
           edgeCheck(x-4, y) && GameManager::pieceOnSquare[y][x-4] != NULL && GameManager::pieceOnSquare[y][x-4]->getType() == rook &&
           GameManager::pieceOnSquare[y][x-4]->getColor() == color && GameManager::pieceOnSquare[y][x+4]->getHasFirstMove() == false){
            legalMove.push_back(QPoint(x-2, y));
        }
    }
}

void Piece::moveToSquare(int tx, int ty){
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
    findLegalSpecialMove();
    removeIllegalMove();
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
}

void Piece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(GameManager::turn != color) return;

    this->setZValue(0);

    move(event->scenePos().x() / GameManager::square_width, event->scenePos().y() / GameManager::square_width);

    //visualize
    Visualize::hideLegalMove();
    Visualize::hideHighlightSquare();
}

