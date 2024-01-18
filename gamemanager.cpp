#include "gamemanager.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QScreen>
#include <QDebug>
#include "piece.h"
#include <ctype.h>


using namespace std;

const QColor GameManager::light_square_color = QColor(237, 228, 221);
const QColor GameManager::dark_square_color = QColor(112, 63, 20);
const QColor GameManager::gray = QColor(181, 181, 181);

const int GameManager::square_width = 70;
int GameManager::window_width = square_width * 8;

MainWindow* GameManager::window;
QGraphicsScene* GameManager::scene;
QGraphicsRectItem* GameManager::squares[8][8];
Piece* GameManager::pieceOnSquare[8][8];
QPixmap* GameManager::images[2][6];
string GameManager::fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
vector<Piece*> GameManager::pieces[2];

void GameManager::generateChessBoard(){
    //set window
    window->setGeometry(0, 0, square_width*8, square_width*8);
    QRect screenSize = window->screen()->geometry();
    window->move(screenSize.width()/2 - window_width/2, screenSize.height()/2 - window_width/2);

    //set view
    window->getui()->graphicsView->setFixedSize(window_width, window_width);
    window->getui()->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    window->getui()->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //set scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, window_width, window_width);
    window->getui()->graphicsView->setScene(scene);

    //set square
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if((i+j) % 2 == 0){
                squares[i][j] = scene->addRect(QRect(0, 0, square_width, square_width), QPen(light_square_color), QBrush(light_square_color));
            }else{
                squares[i][j] = scene->addRect(QRect(0, 0, square_width, square_width), QPen(dark_square_color), QBrush(dark_square_color));
            }
            squares[i][j]->setPos(square_width*j, square_width*i);
        }
    }

    //init pieceOnSquare
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            pieceOnSquare[i][j] = NULL;
        }
    }

    //create piece by fen
    int index = 0;
    for(int i = 0; i < fen.length(); i++){
        char c = fen[i];
        int color;
        int type;
        if(isdigit(c)){
            index += (int)c - (int)'0';
        }else{
            if(c == '/') continue;
            switch(c){
                case 'p':
                    color = Piece::black;
                    type = Piece::pawn;
                    break;
                case 'b':
                    color = Piece::black;
                    type = Piece::bishop;
                    break;
                case 'n':
                    color = Piece::black;
                    type = Piece::knight;
                    break;
                case 'r':
                    color = Piece::black;
                    type = Piece::rook;
                    break;
                case 'q':
                    color = Piece::black;
                    type = Piece::queen;
                    break;
                case 'k':
                    color = Piece::black;
                    type = Piece::king;
                    break;
                case 'P':
                    color = Piece::white;
                    type = Piece::pawn;
                    break;
                case 'B':
                    color = Piece::white;
                    type = Piece::bishop;
                    break;
                case 'N':
                    color = Piece::white;
                    type = Piece::knight;
                    break;
                case 'R':
                    color = Piece::white;
                    type = Piece::rook;
                    break;
                case 'Q':
                    color = Piece::white;
                    type = Piece::queen;
                    break;
                case 'K':
                    color = Piece::white;
                    type = Piece::king;
                    break;
            }
            Piece *newPiece = new Piece(type, color, index%8, index/8);
            pieces[color].push_back(newPiece);
            pieceOnSquare[index/8][index%8] = newPiece;
            index++;
        }
    }
}
