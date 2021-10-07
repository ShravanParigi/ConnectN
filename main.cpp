// main.cpp

#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

int main()
{
	HumanPlayer bp("Homer");
	SmartPlayer hp("Marge");
	Game g(7, 6, 4, &bp, &hp);
	g.play();
}
