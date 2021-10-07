// Player.cpp

#include "provided.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class HumanPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class BadPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};


class SmartPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
    void determineBestComputerMove(Scaffold& s, int color, int n, int& bestMove, int& rating, int depth);
    void determineBestHumanMove(Scaffold& s, int color, int n, int& bestMove, int& rating, int depth);
    int rater(Scaffold& s, int color, int col, int n);
    int totaldepth;
};

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    int c = 0;
    while (c<1 || c>s.cols() || s.checkerAt(c, s.levels()) != VACANT){
        //cout << "It is " << name() << "'s turn to play!";
        cout << "human player's move" << endl;
        cout << "enter a valid column number(1 to " << s.cols() << "):";
        cin >> c;
        cout << endl;
    }
    return c;
}

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    int c = 1;
    cerr << "computer's move" << endl;
    while (s.checkerAt(c, s.levels()) != VACANT) {
        c++;
    }
    return c;
}



int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    Scaffold scaff = s;
    int bestMove = -1, rating = -2;
    int depth = 0;
    totaldepth = 8;
    determineBestComputerMove(scaff, color, N, bestMove, rating, depth);
    cerr << bestMove << endl;
    return bestMove;
}

int SmartPlayerImpl::rater(Scaffold& scaffold, int color, int col, int n) {
    int lev = scaffold.levels();
    while (scaffold.checkerAt(col, lev) == VACANT)
        lev--;
    //cout << lastc << ", " << lastl << endl;
    //cout << "checking for " << piece<<endl;

    //checks row of last played element
    int c = 0;
    for (int i = 1; i <= scaffold.cols(); i++) {
        if (scaffold.checkerAt(i, lev) == color)
            c++;
        else
            c = 0;
        if (c == n) {
            return 1;
        }
    }

    //checks column of last played element
    c = 0;
    for (int i = 1; i <= scaffold.levels() && scaffold.checkerAt(col, i) != VACANT; i++) {
        if (scaffold.checkerAt(col, i) == color)
            c++;
        else
            c = 0;
        if (c == n) {
            return 1;
        }
    }

    //checks backward diagonal of last played element
    c = 0;
    int t = col + lev;
    for (int lev2 = t - 1; lev2 > 0 && t - lev2 <= scaffold.cols(); lev2--) {
        if (scaffold.checkerAt(t - lev2, lev2) == color)
            c++;
        else
            c = 0;
        if (c == n) {
            return 1;
        }
    }

    //checks forward diagonal of last played element
    c = 0;
    int d = lev - col;
    for (int col2 = 1; col2 <= scaffold.cols() && col2 + d <= scaffold.levels(); col2++) {
        if (scaffold.checkerAt(col2, col2 + d) == color)
            c++;
        else
            c = 0;
        if (c == n) {
            return 1;
        }
    }

    if (scaffold.numberEmpty() == 0) {
        return 0;
    }

    return -1;
}

void SmartPlayerImpl::determineBestComputerMove(Scaffold& s, const int color, int n, int& bestMove, int& rating, int depth) {
    if (depth == totaldepth) {
        rating = 0;
        return;
    }
    depth++;
    int cols = s.cols() + 1;
    vector<int> moves(cols, -1*totaldepth);
    for (int i = 1; i < moves.size(); i++) {
        if (s.makeMove(i, color)) {
            moves[i] = rater(s, color, i, n) * (totaldepth - depth); // stores rating
            if (moves[i] < 0) { // if rating is not win or tie
                int humanRate = 0;
                int move;
                determineBestHumanMove(s, (color - 1) * -1, n, move, humanRate, depth);
                moves[i] = humanRate;
            }
            s.undoMove();
        }
    }
    if (depth == 1) {
        for (int i = 1; i < moves.size(); i++) {
            cerr << moves[i] << ", ";
        }
    }
    int choice = 1;
    vector<int> choices; // vector to store all posible best move options
    for (int i = 1; i < moves.size(); i++) {
        if (moves[choice] < moves[i]) {
            choice = i;
            choices.clear();
            choices.push_back(choice);
        } if (moves[choice] == moves[i]) {
            choices.push_back(i);
        }
    }
    srand(time(NULL));
    bestMove = choices[rand() % choices.size()]; // pick random of best moves
    rating = moves[bestMove];
}

void SmartPlayerImpl::determineBestHumanMove(Scaffold& s, const int color, int n, int& bestMove, int& rating, int depth) {
    if (depth == totaldepth) {
        rating = 0;
        return;
    }
    depth++;
    int cols = s.cols() + 1;
    vector<int> moves(cols, totaldepth);
    for (int i = 1; i < moves.size(); i++) {
        if (s.makeMove(i, color)) {
            moves[i] = rater(s, color, i, n) * -1 * (totaldepth - depth); // stores rating
            if (moves[i] > 0) { // if rating is not win or tie
                int humanRate = 0;
                int move;
                determineBestComputerMove(s, (color * -1) + 1, n, move, humanRate, depth);
                moves[i] = humanRate;
            }
            s.undoMove();
        }
    }
    int choice = 1;
    vector<int> choices; // vector to store all possible best move options
    for (int i = 1; i < moves.size(); i++) {
        if (moves[choice] > moves[i]) {
            choice = i;
            choices.clear();
            choices.push_back(choice);
        } if (moves[choice] == moves[i]) {
            choices.push_back(i);
        }
    }
    srand(time(NULL));
    bestMove = choices[rand() % choices.size()]; // pick random of best moves
    rating = moves[bestMove];
}

//******************** Player derived class functions *************************
//  These functions simply delegate to the Impl classes' functions.
//  You probably don't want to change any of this code. 

HumanPlayer::HumanPlayer(string nm)
 : Player(nm)
{
    m_impl = new HumanPlayerImpl;
}
 
HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}
 
int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
 : Player(nm)
{
    m_impl = new BadPlayerImpl;
}
 
BadPlayer::~BadPlayer()
{
    delete m_impl;
}
 
int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
 : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}
 
SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}
 
int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

