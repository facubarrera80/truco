#ifndef CARDS_H
#define CARDS_H

#include <array>
#include <list>
#include <vector>
#include <string>

class Card
{
public:
	Card();
	Card(int number, int type);

	int getNumber();
	int getType();

	std::string toString();

	friend bool operator== (const Card& lhs, const Card& rhs)
	{
		bool equal = true;

		if ((lhs.number != rhs.number) || (lhs.type != rhs.type)) equal = false;

		return equal;
	}
	friend bool operator!= (const Card& lhs, const Card& rhs) { return !(lhs == rhs); };

private:
	int number = 0;
	int type = 0;
};

class Deck
{
public:
	Deck();
	Deck(std::list<Card> deck);
	Deck(std::list<Card> deck, bool shuffle);
	Deck(std::list<Card> deck, std::vector<std::vector<Card>> hierarchy, bool shuffle);

	Card popTopCard();
	Card popBotCard();
	void insertCardOnTop(Card card);
	void insertCardOnBot(Card card);
	int eraseCard(Card card);

	Deck getCards(int amount);
	Deck getCardsByType(int type);
	Deck getCardsByType(int type, int amount);
	int getHierarchyPosition(Card card);

	int getLowestHierarchy();

	int size();
	int default_size();

	void shuffleDeck();
	void sortDeck(); // not developed
	void refillDeck(); // not developed
	void resetDeck();

	void setHierarchy(std::vector<std::vector<Card>> hierarchy);
	void setCurrentDeckAsDefault();

	std::list<Card> getPoppedCards(); // not developed

	std::string toString();

private:

	std::list<Card> default_deck;
	std::list<Card> deck;

	std::vector<std::vector<Card>> hierarchy;

	std::list<Card> popped_cards;
	std::list<Card> inserted_cards;

};

#endif