#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Cards.h"

const std::list<Card> DEFAULT_DECK = { // 1 - ESPADAS, 2 - BASTO, 3 - ORO, 4 - COPA
											Card(1,1) , Card(2,1) , Card(3,1), Card(4,1) , Card(5,1) , Card(6,1) , Card(7,1) , Card(10,1) , Card(11,1) , Card(12,1),
											Card(1,2) , Card(2,2) , Card(3,2), Card(4,2) , Card(5,2) , Card(6,2) , Card(7,2) , Card(10,2) , Card(11,2) , Card(12,2),
											Card(1,3) , Card(2,3) , Card(3,3), Card(4,3) , Card(5,3) , Card(6,3) , Card(7,3) , Card(10,3) , Card(11,3) , Card(12,3),
											Card(1,4) , Card(2,4) , Card(3,4), Card(4,4) , Card(5,4) , Card(6,4) , Card(7,4) , Card(10,4) , Card(11,4) , Card(12,4)
};
const std::vector<std::vector<Card>> DEFAULT_DECK_HIERARCHY = {
											{ Card(1,1) },
											{ Card(1,2) },
											{ Card(7,1) },
											{ Card(7,3) },
											{ Card(3,1) , Card(3,2) , Card(3,3) , Card(3,4) },
											{ Card(2,1) , Card(2,2), Card(2,3) , Card(2,4) },
											{ Card(1,3), Card(1,4) },
											{ Card(12,1) , Card(12,2) , Card(12,3) , Card(12,4) },
											{ Card(11,1) , Card(11,2), Card(11,3) , Card(11,4) },
											{ Card(10,1), Card(10,2) , Card(10,3) , Card(10,4) },
											{ Card(7,2) , Card(7,4) },
											{ Card(6,1) , Card(6,2), Card(6,3) , Card(6,4) },
											{ Card(5,1), Card(5,2) , Card(5,3) , Card(5,4) },
											{ Card(4,1) , Card(4,2) , Card(4,3) , Card(4,4) }
};

#define CLEAR "cls"

#define MAX_PLAYERS 6
#define MIN_PLAYERS 2
#define MIN_TEAMS 2
#define MAX_TEAMS 3

#define INT 0
#define STR 1


// IOManager
   // output
#define MATCH 100
#define ROUND 101
#define SUBROUND 102
#define POINTS 103

   // request
#define NUM_PLAYERS 201
#define CHOICE 202
#define PLAY_CARD 203

  // error
#define INVALID 400


// Options
#define ENVIDO "envido"
#define	TRUCO_1 "truco"
#define TRUCO_2 "retruco"
#define TRUCO_3 "vale 4"
#define PLAY "jugar"
#define WITHDRAW "retirarse"
#define QUIERO "quiero"
#define NO_QUIERO "no quiero"

#define N_ENVIDO 1
#define	N_TRUCO_0 1
#define	N_TRUCO_1 2
#define N_TRUCO_2 3
#define N_TRUCO_3 4
#define N_PLAY 5
#define N_WITHDRAW 6
#define N_QUIERO 7
#define N_NO_QUIERO 8

// Points
#define P_ENVIDO_YES 2
#define P_ENVIDO_NO 1
#define P_TRUCO_1 1
#define P_TRUCO_2 2
#define P_TRUCO_3 3
#define P_ROUND 1

#endif