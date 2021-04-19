
#include "Constants.h"
#include "Structs.h"
#include "Truco_common_functions.h"
#include "Cards.h"
#include "IOManager.h"

#include <time.h>
#include <stdlib.h>
#include <list>
#include <vector>

std::list<Player> getTurns(Match match, SubRound sub_round)
{
	// list to fill and return
	std::list<Player> turns;

	// variables
	bool found_first = false;
	bool list_complete = false;
	int first_id = 0;
	int next_id = 0;

	// setup first player id
	int first_player_id;
	if (match.round == 1) first_player_id = sub_round.starting_player.id;
	else first_player_id = match.rounds.back().sub_rounds.back().winner.id;

	// fill list
	for (auto it = match.players.begin();; it++) {

		// if we are on the end of the list AND havent found the first_id, break (player is not in the match list)
		if (!found_first && it == match.players.end()) break;
		// else, continue from the beginning
		else if (it == match.players.end()) it = match.players.begin();

		// if list is incomplete
		if (found_first && !list_complete)
		{
			// if next_id and first_id are equal, the loop has gone full circle and all players are in queue
			if (it->id == first_id) list_complete = true;
			
			// "else" prevent adding the first player again, this adds remaining players in order
			else if (next_id == it->id)
			{
				turns.push_back(Player{ it->id, it->hand, it->team });

				next_id++;
				if (next_id > match.number_of_players) next_id = 1;
			}
			
		}

		// if first player found
		if (!found_first && it->id == first_player_id)
		{
			found_first = true;
			turns.push_front(Player{ it->id, it->hand, it->team });
			first_id = it->id;

			next_id = first_id + 1;
			if (next_id > match.number_of_players) next_id = 1;
		}

		// if list is complete, break (we are done)
		if (list_complete) break;
	}

	return turns;
}

void fillTeams(Match *match)
{
	std::vector<Team> teams;
	
	int number_of_teams;
	if ((*match).number_of_players == MIN_PLAYERS) number_of_teams = MIN_TEAMS;
	else number_of_teams = (*match).number_of_players / 2;

	std::vector<Player> players = (*match).players;
	for (int x = 1; x <= number_of_teams; x++)
	{
		Team team;
		team.id = x;
		team.points = 0;

		bool found_first = false;

		for (auto it = players.begin(); it != players.end();)
		{
			if (found_first)
			{
				(*match).players.at(it->id - 1).team = team.id;
				team.players.push_back(Player{ it->id, it->hand, team.id });
			}

			else
			{
				if (it->id == team.id)
				{
					found_first = true;
					(*match).players.at(it->id - 1).team = team.id;
					team.players.push_back(Player{ it->id, it->hand, team.id });
				}
				else
				{
					it++;
					continue;
				}
			}

			for (int x = 0; x < number_of_teams; x++)
			{
				if (it == players.end()) break;
				else it++;
			}
		}

		teams.push_back(team);
	}

	(*match).teams = teams;
}
void playCard(IOManager* manager, Match* match, Round round, SubRound* sub_round, int player_id)
{
	Card card = (*manager).request(PLAY_CARD, (*match).getPlayer(player_id).hand, player_id);

	(*match).players.at(player_id - 1).hand.eraseCard(card);

	(*sub_round).cards_played.push_back( std::pair<int, Card>((*match).getPlayer(player_id).team, card) );
}
void addPoints(Match* match, Result result)
{
	for (int x = 0; x < (*match).teams.size(); x++)
	{
		if ((*match).teams.at(x).id == result.winner.id)
			(*match).teams.at(x).points += result.points;
	}
}

Result winner(Match match, Round round)
{
	/*
	*
	* This funtion returns a result object containing the winner team of the round and its respective
	* points. If there is no winner (in case of first round or second in some cases) then returns an empty 
	* Team object.
	* 
	*/

	Result result;
	result.winner.id = 0; // 0 means that there is no winner yet (since it is not a valid id)
	result.points = P_ROUND;

	std::vector<SubRound> sub_rounds = round.sub_rounds;

	// there can only be a winner in the second round
	if (sub_rounds.size() > 1)
	{
		bool first_draw = false;
		bool second_draw = false;
		bool third_draw = false;

		// loop through sub_rounds and their respective winner team
		for (int x = 0; x < sub_rounds.size(); x++)
		{
			SubRound sub_round = sub_rounds.at(x);
			int team_id = match.getPlayer(sub_round.winner.id).team; // round winner (if 0 then it was a draw)

			if (sub_round.id == 1 && team_id == 0)
			{
				first_draw = true;
			}
			else if (sub_round.id == 2)
			{
				// if draw
				if (team_id == 0)
					second_draw = true;
				// cant have 2 draws (so get who won)
				if (first_draw && !second_draw)
					result.winner = match.getTeam(team_id);
				if (!first_draw && second_draw)
					result.winner = match.getTeam(sub_rounds.at(x - 1).winner.team);

				// if last round winner is same as this round winner
				if (sub_rounds.at(x - 1).winner.team == team_id)
					result.winner = match.getTeam(team_id);
			}
			else if (sub_round.id == 3)
			{
				if (team_id == 0)
					third_draw = true;
				if (first_draw && second_draw)
				{
					// if there were 3 draws, then hand wins
					if (third_draw)
						result.winner = match.getTeam(round.starting_player.team);
					// else, winner of third round wins
					else
						result.winner = match.getTeam(team_id);
				}
				// its assumed that teams were tied, so third round winner wins
				else
					result.winner = match.getTeam(team_id);

			}
		}
	}

	return result;
}
Player sub_winner(Match match, SubRound sub_round)
{
	/*
	*
	* This funtion returs a Player object containing the player who won the subround acording
	* to the cards played. If there was a draw, the object contains an invalid object (id 0)
	* 
	*/

	Player result;

	std::vector<std::pair<int, int>> winners; // first: player_id, second: hierarchy position of card he played

	// iniciate a value with no points and hierarchy below the lowest
	winners.push_back(std::pair<int, int>(0, (match.deck.getLowestHierarchy() + 1) ));
	
	for (auto it = sub_round.cards_played.begin(); it != sub_round.cards_played.end(); it++)
	{
		// get the hierarchical position of the played card
		int hierarchy = match.deck.getHierarchyPosition(it->second);

		// set winner/s
		if (hierarchy < winners.at(0).second)
			winners.at(0) = std::pair<int, int>(it->first, hierarchy);
		else if (hierarchy == winners.at(0).second)
			winners.push_back(std::pair<int, int>(it->first, hierarchy));
	}

	// if draw, return and invalid tean (id 0)
	if (winners.size() == 2)
		result.id = 0;
	else
		result = match.getPlayer(winners.at(0).first);

	return result;
}

int main_truco()
{
	srand(time(NULL));

	Match match;
	IOManager manager;

	// request number of players
	while (true) {
		int number_of_players = manager.request(NUM_PLAYERS);

		if (number_of_players > MAX_PLAYERS || number_of_players < MIN_PLAYERS || number_of_players % 2 != 0) // 2 <= x <= 6 | x even number
		{
			manager.output(INVALID);
			continue;
		}

		match.number_of_players = number_of_players;
		break;
	}

	// create player classes
	for (int x = 1; x <= match.number_of_players; x++)
	{
		Player player;
		player.id = x;
		player.team = 0;

		match.players.push_back(player);	// initialize player with no team or points
	}

	// create teams
	fillTeams(&match);

	// rounds (until 30 points)
	match.match_in_progress = true;
	for (int round_num = 1;; round_num++)
	{
		for (int x = 0; x < match.teams.size(); x++)
		{
			Team team = match.teams.at(x);
			if (team.points >= 30)
				match.match_in_progress = false;
		}

		if (!match.match_in_progress) break;

		Round round;
		round.round_number = round_num;
		match.round = round_num;

		// create shuffled deck
		match.deck = Deck(DEFAULT_DECK, DEFAULT_DECK_HIERARCHY, true);

		// give cards to players
		for (int x = 0; x < match.number_of_players; x++)
		{
			match.players.at(x).hand = match.deck.getCards(3);
		}

		//				DEBUGGING ONLY
		manager.output(MATCH, match);
		//				DEBUGGING ONLY

		Result envido_res, truco_res, round_res;

		// start round
		round.round_in_progress = true;
		for (int sub_round_num = 1; sub_round_num <= 3; sub_round_num++)
		{
			Result test_winner = winner(match, round);
			if (test_winner.winner.id != 0)
			{
				round.round_in_progress = false;
				round_res = test_winner;
			}

			if (!round.round_in_progress) break;

			SubRound sub_round;
			sub_round.id = sub_round_num;

			// determine who starts the game and the turns
			if (sub_round_num == 1)
			{
				Player starting_player = match.players.at(rand() % match.number_of_players); // random player to start the game
				round.starting_player = starting_player;
				sub_round.starting_player = starting_player;
			}
			else
			{
				if (round.sub_rounds.back().winner.id == 0)
				{
					sub_round.starting_player = round.sub_rounds.back().starting_player;
				}
				else
					sub_round.starting_player = round.sub_rounds.back().winner;
			}

			sub_round.turns = getTurns(match, sub_round);

			// loop through all players' turns
			std::list<Player> turns = sub_round.turns;
			for (auto it = turns.begin(); it != turns.end(); it++)
			{
				if (!round.round_in_progress) break;

				Get_choice:
				// get players choice
				Options options = getOptions(round);
				int choice = manager.request(CHOICE, options, it->id);
				if (!isChoiceValid(options, choice))
				{
					manager.output(INVALID);
					goto Get_choice;
				}

				switch (choice)
				{
				case N_ENVIDO:
					round.envido = true;
					round.envido_res = envido(&manager, match, round, it->id);
					goto Get_choice;

				case N_TRUCO_1:
					round.new_truco = N_TRUCO_1;
					truco(&manager, match, &round, it->id);
					break;

				case N_TRUCO_2:
					round.new_truco = N_TRUCO_2;
					truco(&manager, match, &round, it->id);
					break;

				case N_TRUCO_3:
					round.new_truco = N_TRUCO_3;
					truco(&manager, match, &round, it->id);
					break;

				case N_PLAY:
					// continue
					break;

				case N_WITHDRAW:
					break;
				}

				if (round.round_in_progress) 
					playCard(&manager, &match, round, &sub_round, it->id);
			}

			sub_round.winner = sub_winner(match, sub_round);
			round.sub_rounds.push_back(sub_round);
		}
		round.round_in_progress = false;

		// end round
		round_res = winner(match, round);
		if (round.winner.id == 0)
			round.winner = round_res.winner;

		truco_res = truco(&manager, match, &round, NULL);
		round.truco_res = truco_res;

		addPoints(&match, round_res);
		addPoints(&match, round.truco_res);
		addPoints(&match, round.envido_res);

		match.rounds.push_back(round);

		manager.output(POINTS, match);
	}

	return 0;
}