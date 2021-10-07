// Scaffold.cpp
// Changes need to be made before submitting
#include "provided.h"
#include <vector>
#include <stack>
#include <iostream>
using namespace std;


class ScaffoldImpl
{
public:
    ScaffoldImpl(int nColumns, int nLevels); 
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const; 
    void display() const; 
    bool makeMove(int column, int color); 
    int undoMove();  
private:
    vector<vector <int> > scaffold;
    int m_col, m_lev, emptySpaces;
    stack<int> columns;
};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
    :m_col(nColumns), m_lev(nLevels), emptySpaces(m_col * m_lev), scaffold(nLevels, vector<int>(nColumns, -1))
{
    if (m_col < 1 || m_lev < 1) {
        exit(1);
    }
}

int ScaffoldImpl::cols() const
{
    return m_col;  //  This is not always correct; it's just here to compile
}

int ScaffoldImpl::levels() const
{
    return m_lev;  //  This is not always correct; it's just here to compile
}

int ScaffoldImpl::numberEmpty() const
{
    return emptySpaces;  //  This is not always correct; it's just here to compile
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
    if (column < 1 || level < 1 || column > m_col || level > m_lev){
        return -2; // change later
    }
    //cout << column << ", " << level << endl;
    return scaffold[level-1][column-1];  //  This is not always correct; it's just here to compile
}

void ScaffoldImpl::display() const
{
    for (int i = m_lev-1; i >= 0; i--) { // backwards for loop to iterate through the 2d vector in backwards order of rows since row 0 is the last row to print.
        for (int j = 0; j < m_col; j++) {
            cout << '|';
            switch (scaffold[i][j]) {
            case VACANT:
                cout << ' ';
                break;
            case BLACK:
                printf("\033[1;36m");
                cout << "B"; //Change back to B
                printf("\033[0m");
                break;
            case RED:
                printf("\033[1;31m");
                cout << "R"; //Change back to R
                printf("\033[0m");
                break;
            }
        }
        cout << '|' << endl;
    }
    for (int j = 0; j < m_col; j++) {
        cout << "+-";
    }
    cout << "+" << endl;
}

bool ScaffoldImpl::makeMove(int column, int color)
{
    //cout << "Made(" << color <<  "): " << column << endl;
    if (column < 1 || column > m_col) {
        return false;
    }
    if (scaffold[m_lev - 1][column - 1] != VACANT) {  
        return false;
    }
    int i = 0;
    while (scaffold[i][column - 1] != VACANT) {
        i++;
    }
    scaffold[i][column - 1] = color;
    columns.push(column - 1); // pushes move column to stack for undo reference
    emptySpaces--;
    return true;
}

int ScaffoldImpl::undoMove()
{
        if (columns.empty()) {
            return 0;
        }
        int col = columns.top();
        //cout << "Undo: " << col << endl;
        columns.pop(); // pops top element off move column when undoing 
        int i = m_lev - 1;
        while (scaffold[i][col] == VACANT) {
            i--;
        }
        scaffold[i][col] = VACANT;
        emptySpaces++;
        return col;
}

//******************** Scaffold functions *******************************

//  These functions simply delegate to ScaffoldImpl's functions.
//  You probably don't want to change any of this code. 

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}
 
Scaffold::~Scaffold()
{
    delete m_impl;
}
 
Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}
 
Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}
 
int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}
 
void Scaffold::display() const
{
    m_impl->display();
}
 
bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}
 
int Scaffold::undoMove()
{
    return m_impl->undoMove();
}
