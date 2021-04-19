#include "Structs.h"
#include "Constants.h"
#include "Truco_common_functions.h"
#include "Cards.h"
#include "IOManager.h"

Result truco(IOManager* manager, Match match, Round* round, int player_id)
{
	Result result;

	if ((*round).round_in_progress)
	{
		if ((*round).truco != (*round).new_truco)
		{
			// determine next player who to ask the question
			int next_player = match.getNextPlayer(player_id).id;

		Get_choice_truco:
			// get player's choice
			Options options = getOptions(*round);
			int choice = (*manager).request(CHOICE, options, next_player);
			if (!isChoiceValid(options, choice))
			{
				(*manager).output(INVALID);
				goto Get_choice_truco;
			}

			switch (choice)
			{
			case N_TRUCO_2:
				(*round).truco = (*round).new_truco;
				(*round).new_truco = N_TRUCO_2;
				truco(manager, match, round, next_player);
				break;

			case N_TRUCO_3:
				(*round).truco = (*round).new_truco;
				(*round).new_truco = N_TRUCO_3;
				truco(manager, match, round, next_player);
				break;

			case N_QUIERO:
				(*round).truco = (*round).new_truco;
				// continue the game
				break;

			case N_NO_QUIERO:
				(*round).winner = match.getTeam( match.getPlayer(player_id).team );
				// end the game
				(*round).round_in_progress = false;
				break;

			case N_ENVIDO:
				(*round).envido = true;
				(*round).envido_res = envido(manager, match, (*round), next_player);
				goto Get_choice_truco;
			}
		}

	}
	else
	{
		result.winner = (*round).winner;
		
		switch ((*round).truco)
		{
		case N_TRUCO_1:
			result.points = P_TRUCO_1;
			break;

		case N_TRUCO_2:
			result.points = P_TRUCO_2;
			break;

		case N_TRUCO_3:
			result.points = P_TRUCO_3;
			break;
		}
	}

	return result;
}