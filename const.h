#ifndef CONST_H
#define CONST_H

#include <vector>
#include <QtCore>
using namespace std;

const bool CLOSED=false;
const bool OPENED=true;
const bool PRESENT=true;
const bool ABSENT=false;

const int MENU=1;
const int PAUSE=2;
const int PLAY=3;
const int GAME_OVER=4;

const vector<int> for_buttons{30, 50, 30, 40, 15, 10, 10, 90 ,8, 5, 14, 23, 10};
const vector<qreal> for_cards_y{8, 22, 36, 50, 64, 78, 14};
const vector<qreal> for_cards_y2={6, 17, 28, 39, 50, 61, 72, 83, 11};

#endif // CONST_H

