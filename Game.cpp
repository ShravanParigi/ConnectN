// Game.cpp

#include "provided.h"
#include <iostream>
using namespace std;
class GameImpl
{
  public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black); 
    bool completed(int& winner) const; 
    bool takeTurn(); 
    void play(); 
    int checkerAt(int c, int r) const;
private:
    Scaffold scaffold;
    Player* m_red;
    Player* m_black;
    int lastc = -1;
    int n;
    int current = 0;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black)
    : scaffold(nColumns, nLevels), m_red(red), m_black(black), n(N)
{}

bool GameImpl::completed(int& winner) const
{
    if (lastc == -1) {
        return false;
    }

    //finding level of most recently played coin
    int lastl = scaffold.levels();
    while (scaffold.checkerAt(lastc , lastl) == VACANT)
        lastl--;
    //cout << lastc << ", " << lastl << endl;
    int piece = scaffold.checkerAt(lastc, lastl);
    //cout << "checking for " << piece<<endl;

    //check row of last played element
    int c = 0;
    for (int i = 1; i <= scaffold.cols(); i++) {
        if (scaffold.checkerAt(i, lastl) == piece)
            c++;
        else
            c = 0;
        if (c == n) {
            winner = piece;
            return true;
        }
    }

    //check column of last played element
    c = 0;
    for (int i = 1; i <= scaffold.levels() && scaffold.checkerAt(lastc,i) != VACANT; i++) {
        if (scaffold.checkerAt(lastc, i) == piece)
            c++;
        else
            c = 0;
        if (c == n) {
            winner = piece;
            return true;
        }
    }

    //check backward diagonal of last played element
    c = 0;
    int t = lastc + lastl;
    for (int lev = t - 1; lev > 0 && t - lev <= scaffold.cols(); lev--) {
        if (scaffold.checkerAt(t-lev, lev) == piece)
            c++;
        else
            c = 0;
        if (c == n) {
            winner = piece;
            return true;
        }
    }

    //check forward diagonal of last played element
    c = 0;
    int d = lastl - lastc;
    for (int col = 1; col <= scaffold.cols() && col + d <= scaffold.levels(); col++) {
        if (scaffold.checkerAt(col, col+d) == piece)
            c++;
        else
            c = 0;
        if (c == n) {
            winner = piece;
            return true;
        }
    }

    if (scaffold.numberEmpty() == 0) {
        winner = TIE_GAME;
        return true;
    }
    
    return false; 
}

bool GameImpl::takeTurn()
{
    int w = -2;
    if (completed(w)) {
        return false;
    }
    if (current%2 == 0) { // player switcher
        int col = m_red->chooseMove(scaffold, n, current % 2);
        scaffold.makeMove(col, RED);
        lastc = col; // set last column
    }
    else {
        int col = m_black->chooseMove(scaffold, n, current % 2);
        scaffold.makeMove(col, BLACK);
        lastc = col; // set last column
    }
    current++;
    return true;
}

void GameImpl::play()
{
    while (takeTurn()) {
        scaffold.display();
        cout << endl << endl;
    }
    int winner = -2;
    completed(winner);
    if (winner == TIE_GAME) {
        cout << endl;
        cout << "TIE GAME!!" << endl;
    }
    if (winner == BLACK) {
        cout << endl;
        cout << "BLACK WINS!!" << endl;
    }
    if (winner == RED) {
        cout << endl;
        cout << "RED WINS!!" << endl;
    }
}

int GameImpl::checkerAt(int c, int r) const
{
    return scaffold.checkerAt(c, r);
} 

//******************** Game functions *******************************

//  These functions simply delegate to GameImpl's functions.
//  You probably don't want to change any of this code. 

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}
 
Game::~Game()
{
    delete m_impl;
}
 
bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}
 
int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
