#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <vector>
#include <list>

#include "Constants.h"
#include "Cards.h"

struct Player 
{
	std::string toString()
	{
		std::string to_str = "";

		to_str.append("{");
		to_str.append(std::to_string(id));
		to_str.append(",");
		to_str.append(hand.toString());
		to_str.append(",");
		to_str.append(std::to_string(team));
		to_str.append("}");

		return to_str;
	}

	int id;
	Deck hand;
	int team;
};

struct Team
{
	int id = 0;
	std::vector<Player> players;
	int points;
};

struct Result
{
	Team winner = Team();
	int points = 0;
};

struct SubRound
{
	int id;
	std::list< std::pair<int, Card> > cards_played;
	
	Player starting_player;
	std::list<Player> turns;

	Player winner;
};

struct Round 
{
	int round_number;
	bool round_in_progress = true;

	bool envido = false;
	bool envido_in_progress = false;
	int truco = N_TRUCO_0;
	int new_truco = N_TRUCO_0;

	Player starting_player;
	std::vector<SubRound> sub_rounds;

	Result envido_res, truco_res;

	Team winner;
};

struct Match
{
	Player getPlayer(int id)
	{
		Player player = {};
		for (auto it = players.begin(); it != players.end(); it++)
			if (it->id == id) player = Player{ it->id, it->hand, it->team };
		return player;
	}
	Player getNextPlayer(int id)
	{
		int next_player = id;
		if ((id + 1) > number_of_players) next_player = 1;
		else next_player++;

		return getPlayer(next_player);
	}
	Team getTeam(int id)
	{
		Team team = {};
		for (auto it = teams.begin(); it != teams.end(); it++)
			if (it->id == id) team = Team{ it->id, it->players, it->points };
		return team;
	}

	bool match_in_progress;

	int round = 0;
	int number_of_players = 0;
	Deck deck;
	std::vector<Player> players;
	std::vector<Team> teams;
	std::vector<Round> rounds;
};

typedef std::vector<std::pair<int, std::string>> Options;

#endif