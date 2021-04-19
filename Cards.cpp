#include "Cards.h"

#include <string>
#include <time.h>
#include <iostream>

// Card functions
Card::Card()
{
	this->number = 0;
	this->type = 0;
}
Card::Card(int number, int type)
{
	this->number = number;
	this->type = type;
}

int Card::getNumber()
{
	return this->number;
}
int Card::getType()
{
	return this->type;
}

std::string Card::toString()
{
	std::string to_str;

	to_str.append(std::to_string(number));
	to_str.append(",");
	to_str.append(std::to_string(type));

	return to_str;
}

// Deck functions

Deck::Deck()
{
}

Deck::Deck(std::list<Card> deck)
{
	this->deck = deck;
	this->default_deck = deck;
}

Deck::Deck(std::list<Card> deck, bool shuffle)
{
	this->deck = deck;
	this->default_deck = deck;

	if (shuffle) this->shuffleDeck();
}

Deck::Deck(std::list<Card> deck, std::vector<std::vector<Card>> hierarchy, bool shuffle)
{
	this->deck = deck;
	this->default_deck = deck;
	this->hierarchy = hierarchy;

	if (shuffle) this->shuffleDeck();
}

	// Card related functions
Card Deck::popTopCard()
{
	Card popped_card = this->deck.front();
	this->deck.pop_front();

	this->popped_cards.push_front(popped_card);

	return popped_card;
}

Card Deck::popBotCard()
{
	Card popped_card = this->deck.back();
	this->deck.pop_back();

	this->popped_cards.push_front(popped_card);

	return popped_card;
}

void Deck::insertCardOnTop(Card card)
{
	this->deck.push_front(card);
	this->inserted_cards.push_front(card);
}

void Deck::insertCardOnBot(Card card)
{
	this->deck.push_back(card);
	this->inserted_cards.push_front(card);
}

int Deck::eraseCard(Card card)
{
	int status = -1;

	for (auto it = deck.begin(); it != deck.end(); it++)
	{
		if (it->getNumber() == card.getNumber() && it->getType() == card.getType())
		{
			deck.erase(it);
			status = 0;
			break;
		}
	}

	return status;
}

Deck Deck::getCards(int amount){
	Deck cards = Deck();

	for (int x = 0; (x < amount && x < deck.size()); x++) cards.insertCardOnTop(popTopCard());

	cards.setCurrentDeckAsDefault();

	return cards;
}

Deck Deck::getCardsByType(int type)
{
	Deck cards = Deck();

	int size = deck.size();
	for (int x = 0; x < size; x++)
	{
		Card card = popTopCard();
		if (card.getType() == type) cards.insertCardOnTop(card);
	}

	cards.setCurrentDeckAsDefault();

	return cards;
}

Deck Deck::getCardsByType(int type, int amount)
{
	Deck cards = Deck();

	int size = deck.size();
	for (int x = 0; (x < amount && x < size); x++)
	{
		Card card = popTopCard();
		if (card.getType() == type) cards.insertCardOnTop(card);
	}

	cards.setCurrentDeckAsDefault();

	return cards;
}

int Deck::getHierarchyPosition(Card card)
{
	int position = 0;

	for (int x = 0; x < hierarchy.size(); x++)
	{
		for (int z = 0; z < hierarchy.at(x).size(); z++)
		{
			Card temp_card = hierarchy.at(x).at(z);

			if (card == temp_card)
			{
				position = x;
				return position;
			}
		}
	}

	return position;
}

int Deck::getLowestHierarchy()
{
	return (hierarchy.size() - 1);
}

	// Deck related functions
int Deck::size()
{
	return this->deck.size();
}

int Deck::default_size()
{
	return this->default_deck.size();
}

void Deck::shuffleDeck()
{
	std::srand(time(NULL));

	for (int z = 0; z < 5; z++)
	{
		std::list<Card> new_deck;

		int deck_size = this->deck.size();

		for (int x = 0; x < deck_size; x++)
		{
			Card card = this->deck.front();
			this->deck.pop_front();

			rand(); rand(); rand();
			int rand_number = (std::rand() % 2) + 1;

			if (rand_number == 1) new_deck.push_front(card);
			if (rand_number == 2) new_deck.push_back(card);
		}

		this->deck = new_deck;
	}
}

void Deck::sortDeck()
{
	
}

void Deck::refillDeck()
{

}

void Deck::resetDeck() {
	deck = default_deck;
	popped_cards = {};
}

	// Set
void Deck::setHierarchy(std::vector<std::vector<Card>> hierarchy) {
	this->hierarchy = hierarchy;
}

void Deck::setCurrentDeckAsDefault() {
	default_deck = deck;
}

	// Miscelaneous
std::string Deck::toString()
{
	std::string to_str = "";
	to_str.append("{");

	for (auto it = deck.begin(); it != deck.end(); it++)
	{
		if (it != deck.begin())
			to_str.append(",");
		
		to_str.append("{");
		to_str.append(std::to_string(it->getNumber()));
		to_str.append(",");
		to_str.append(std::to_string(it->getType()));
		to_str.append("}");
	}

	to_str.append("}");
	return to_str;
}