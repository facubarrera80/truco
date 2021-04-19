#include "Structs.h"
#include "Constants.h"
#include "Truco_common_functions.h"
#include "Cards.h"
#include "IOManager.h"

#include <vector>
#include <list>

int getPoints(Deck cards)
{
	/*
	* 
	* this function gets a deck of 3 cards (***NOT TESTED***) and determines the 4 different posible combinations of envido points
	* then returns the highest one
	* 
	*/

	int return_points = 0;

	int points[4];
	memset(&points, 0, sizeof(points)); // set points to 0

	// x represents the card type, it iterates 4 times for each type
	for (int x = 0; x < 4; x++)
	{
		Deck cards_copy = cards;
		Deck same_type_cards = cards_copy.getCardsByType(x + 1);

		int values[2];

		// switch through the amount of same type cards and respond according to the situation
		switch (same_type_cards.size())
		{
		case 0:
			points[x] = 0;
			continue;
			break;

		case 1:
			values[0] = same_type_cards.popTopCard().getNumber();
			if (values[0] > 7)
				points[x] = 0;
			else
				points[x] = values[0];
			break;

		case 2:
			values[0] = same_type_cards.popTopCard().getNumber();
			values[1] = same_type_cards.popTopCard().getNumber();

			if (values[0] > 7 && values[1] > 7)
				points[x] = 20;

			else if (values[0] > 7 && values[1] <= 7)
				points[x] = 20 + values[1];
			else if (values[1] > 7 && values[0] <= 7)
				points[x] = 20 + values[0];

			else if (values[0] <= 7 && values[1] <= 7)
				if (values[0] == 7 && values[1] == 7)
					points[x] = 20 + 6 + 7;
				else
					points[x] = 20 + values[0] + values[1];
			break;

		case 3:
			/* en caso de flor*/
			break;
		}
	}

	// get the highest points
	for (int x = 0; x < 4; x++)
	{
		if (points[x] > return_points)
			return_points = points[x];
	}

	return return_points;
}

std::vector<Team> getWinners(Match match)
{
	/*
	* 
	* this funtions returns a vector containing the teams who won the envido (if there is 2 or more, there was a draw)
	* 
	* everyone's points are hold in the variable points and then are compared. most points wins, and in case of draw starting player's team wins
	* 
	*/

	std::list<std::pair<Player, int>> points; // hold points of every player

	// fill points
	for (auto it = match.players.begin(); it != match.players.end(); it++)
	{
		int env_points = getPoints(it->hand);
		points.push_back( std::pair<Player,int> (Player{ it->id, it->hand, it->team }, env_points) );
	}

	// determine winner/s (draw)
	std::vector<Team> winner;
	int temp_points = 0;
	for (auto team = points.begin(); team != points.end(); team++)
	{
		if (team->second == temp_points)
			winner.push_back(match.getTeam((team->first).id));

		else if (team->second > temp_points)
		{
			winner.clear();
			winner.push_back(match.getTeam((team->first).id));
		}
	}

	return winner;
}

Result envido(IOManager* manager, Match match, Round round, int player_id)
{
	/*
	*
	* this funtion determines the winner of an envido (in case it is called during the game by a player)
	*
	* the envido is called by a player during his turn, and the player next to it decides whether he wants in or not
	* if he accepts, everyone says their points and whoever has the most points wins 2 points. In case of draw, starting round player wins 2 points.
	* if he rejects, the team of the player who called the envido wins 1 point
	*
	*/

	if (round.sub_rounds.size() > 0) return Result(); // not playable after first sub_round

	std::vector<Team> winners;
	Result result;

	// determine player who to ask the question
	int next_player = match.getNextPlayer(player_id).id;

	Get_choice_envido:
	// get options for the envido
	round.envido_in_progress = true;
	Options options = getOptions(round);

	int choice = (*manager).request(CHOICE, options, next_player);
	if (!isChoiceValid(options, choice))
	{
		(*manager).output(INVALID);
		goto Get_choice_envido;
	}

	switch (choice)
	{
	// if envido is accepted
	case N_QUIERO:

		// determine the winner/s (draw) of the envido
		winners = getWinners(match);

		switch (winners.size())
		{
		case 1:
			// if only one, save that
			result = Result{ winners.front(), P_ENVIDO_YES };
			break;

		case 2:
		case 3:
			// if more than one (draw), starting player's team wins
			for (int x = 0; x < match.teams.size(); x++)
			{
				int team = round.starting_player.team;
				for (auto it = winners.begin(); it != winners.end(); it++)
				{
					if (it->id == team)
					{
						result = Result{ Team{ it->id, it->players, it->points }, P_ENVIDO_YES };
						break;
					}
				}

				team++;
				if (team > match.teams.size())
					team = 1;
			}
			break;
		}

		break;

	// if envido is not accepted, player's team wins
	case N_NO_QUIERO:

		result = Result{match.getTeam( match.getPlayer(player_id).team ), P_ENVIDO_NO };

		break;
	}

	return result;
}