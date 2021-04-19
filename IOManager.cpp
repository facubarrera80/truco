#include "IOManager.h"

#include <stdlib.h>

#include "Constants.h"
#include "Cards.h"
#include "Structs.h"

IOManager::IOManager()
{
}

int IOManager::request(int request_id)
{
	// TEMPORARY //
	if (request_id == NUM_PLAYERS) return 2; // TEMPORARY FOR TEST ONLY //
	// TEMPORARY //

	return 0;
}

int IOManager::request(int request_id, Options options, int player_id)
{
	int choice;

	printf("(%i) - Options:\n", player_id);
	for (auto it = options.begin(); it != options.end(); it++)
	{
		printf("%d - %s\n", it->first, (it->second).c_str());
	}

	printf("> ");
	scanf_s("%i", &choice);

	return choice;
}

Card IOManager::request(int request_id, Deck deck, int player_id)
{
	Card card;
	std::vector<Card> cards;

	printf("(%i) - Options:\n", player_id);
	int deck_size = deck.size();
	for (int x = 1; x <= deck_size; x++)
	{
		Card card = deck.popTopCard();
		cards.push_back(card);
		printf("%d - %d,", x, card.getNumber());
		switch (card.getType())
		{
		case 1:
			printf("espada");
			break;
		case 2:
			printf("basto");
			break;
		case 3:
			printf("oro");
			break;
		case 4:
			printf("copa");
			break;
		}
		printf("\n");
	}

	int choice = 0;

	printf("> ");
	scanf_s("%d", &choice);

	return cards.at(choice-1);
}

int IOManager::output(int output_id)
{

	if (output_id == INVALID)
		printf("invalid\n");

	return 0;
}

int IOManager::output(int output_id, Match match)
{
	if (output_id == MATCH)
	{
		printf("Round %i\n", match.round);
		for (int x = 0; x < match.players.size(); x++)
		{
			Player player = match.players.at(x);
			printf("P%i (%i) - {", player.id, match.teams.at(x).points);
			int size = player.hand.size();
			for (int z = 0; z < size; z++)
			{
				if (z > 0)
					printf(" , ");

				Card card = player.hand.popTopCard();
				printf("%d ", card.getNumber());
				switch (card.getType())
				{
				case 1:
					printf("espada");
					break;
				case 2:
					printf("basto");
					break;
				case 3:
					printf("oro");
					break;
				case 4:
					printf("copa");
					break;
				}
			}
			printf("}\n");
		}
	}
	if (output_id == POINTS)
	{
		Round round = match.rounds.back();
		printf("ROUND WINNER = %d - for 1 point\n", round.winner.id);
		printf("ENVIDO WINNER = %d - for %d points\n", round.envido_res.winner.id, round.envido_res.points);
		printf("TRUCO WINNER = %d - for %d points\n", round.truco_res.winner.id, round.truco_res.points);
	}

	return 0;
}

int IOManager::output(int output_id, Round round)
{
	return 0;
}

int IOManager::output(int output_id, SubRound sub_round)
{
	return 0;
}
