#ifndef IOMANAGER_H
#define IOMANAGER_H

#include "Structs.h"
#include "Cards.h"

class IOManager
{

public:
	IOManager();

	int request(int request_id);
	int request(int request_id, Options options, int player_id);
	Card request(int request_id, Deck deck, int player_id);

	int output(int output_id);
	int output(int output_id, Match match);
	int output(int output_id, Round round);
	int output(int output_id, SubRound sub_round);

private:

};

#endif