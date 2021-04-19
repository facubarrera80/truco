
#include "Constants.h"
#include "Structs.h"
#include "Truco_common_functions.h"

Options getOptions(Round round)
{
	Options options;

	// first sub_round options
	if (round.sub_rounds.size() == 0)
	{
		if (round.envido_in_progress == true) {
			options.push_back(std::pair<int, std::string>(N_QUIERO, QUIERO));
			options.push_back(std::pair<int, std::string>(N_NO_QUIERO, NO_QUIERO));
		}
		else if (round.truco != round.new_truco) {
			switch (round.new_truco) {
			case N_TRUCO_1:
				options.push_back(std::pair<int, std::string>(N_TRUCO_2, TRUCO_2));
				options.push_back(std::pair<int, std::string>(N_QUIERO, QUIERO));
				options.push_back(std::pair<int, std::string>(N_NO_QUIERO, NO_QUIERO));
				if (!round.envido)
					options.push_back(std::pair<int, std::string>(N_ENVIDO, ENVIDO));
				break;
			case N_TRUCO_2:
				options.push_back(std::pair<int, std::string>(N_TRUCO_3, TRUCO_3));
				options.push_back(std::pair<int, std::string>(N_QUIERO, QUIERO));
				options.push_back(std::pair<int, std::string>(N_NO_QUIERO, NO_QUIERO));
				break;
			case N_TRUCO_3:
				options.push_back(std::pair<int, std::string>(N_QUIERO, QUIERO));
				options.push_back(std::pair<int, std::string>(N_NO_QUIERO, NO_QUIERO));
				break;
			}
		}
		else {
			if (!round.envido)
				options.push_back(std::pair<int, std::string>(N_ENVIDO, ENVIDO));
			switch (round.truco) {
			case N_TRUCO_0:
				options.push_back(std::pair<int, std::string>(N_TRUCO_1, TRUCO_1));
				break;
			case N_TRUCO_1:
				options.push_back(std::pair<int, std::string>(N_TRUCO_2, TRUCO_2));
				break;
			case N_TRUCO_2:
				options.push_back(std::pair<int, std::string>(N_TRUCO_3, TRUCO_3));
				break;
			}
			options.push_back(std::pair<int, std::string>(N_PLAY, PLAY));
		}
	}

	// other sub_rounds options
	else
	{
		if (round.truco != round.new_truco) {
			switch (round.new_truco) {
			case N_TRUCO_1:
				options.push_back(std::pair<int, std::string>(N_TRUCO_2, TRUCO_2));
				options.push_back(std::pair<int, std::string>(N_QUIERO, QUIERO));
				options.push_back(std::pair<int, std::string>(N_NO_QUIERO, NO_QUIERO));
				break;
			case N_TRUCO_2:
				options.push_back(std::pair<int, std::string>(N_TRUCO_3, TRUCO_3));
				options.push_back(std::pair<int, std::string>(N_QUIERO, QUIERO));
				options.push_back(std::pair<int, std::string>(N_NO_QUIERO, NO_QUIERO));
				break;
			case N_TRUCO_3:
				options.push_back(std::pair<int, std::string>(N_QUIERO, QUIERO));
				options.push_back(std::pair<int, std::string>(N_NO_QUIERO, NO_QUIERO));
				break;
			}
		}
		else {
			switch (round.truco) {
			case N_TRUCO_0:
				options.push_back(std::pair<int, std::string>(N_TRUCO_1, TRUCO_1));
				break;
			case N_TRUCO_1:
				options.push_back(std::pair<int, std::string>(N_TRUCO_2, TRUCO_2));
				break;
			case N_TRUCO_2:
				options.push_back(std::pair<int, std::string>(N_TRUCO_3, TRUCO_3));
				break;
			}
			options.push_back(std::pair<int, std::string>(N_PLAY, PLAY));
		}
	}

	options.push_back(std::pair<int, std::string>(N_WITHDRAW, WITHDRAW));

	return options;
}

bool isChoiceValid(Options options, int choice)
{
	bool is_valid = false;

	for (auto it = options.begin(); it != options.end(); it++)
	{
		if (it->first == choice)
			is_valid = true;
	}

	return is_valid;
}