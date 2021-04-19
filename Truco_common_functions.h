#ifndef TRUCO_COMMON_FUNCTIONS_H
#define TRUCO_COMMON_FUNCTIONS_H

#include "Constants.h"
#include "Structs.h"
#include "IOManager.h"

Options getOptions(Round round);
bool isChoiceValid(Options options, int choice);

Result envido(IOManager* manager, Match match, Round round, int player_id);
Result truco(IOManager* manager, Match match, Round* round, int player_id);

#endif