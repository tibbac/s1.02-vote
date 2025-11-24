/**
 * @author Filip-Daniel Danu
 * @brief  Implementation de l'interface de la bibliotheque
 */
#include "libvote.hh"

#include <algorithm>
#include <cctype>

namespace vote {
namespace parser {
static bool is_last_name(std::string const &s) {
	for (char c : s) {
		if (isalpha(c) && islower(c)) {
			return false;
		}
	}
	return true;
}

bool parse_string(std::istream &stream, std::string &string) {
	bool success;
	i32  pos;

	for (; pos = stream.tellg(),
	       (success = (bool)std::getline(stream, string));) {
		/* si la ligne est vide, contient que des espaces, ou est un commentaire
		 * on l'ignore */
		if (string.empty() ||
		    string.find_first_not_of(' ') == std::string::npos ||
		    string.rfind("//", 0) == 0) {
			continue;
		}
		break;
	}
	if (!success) {
		stream.seekg(pos);
	}

	return success;
}

bool parse_int(std::istream &stream, i32 &integer) {
	bool        success;
	i32         pos;
	std::string s;

	pos = stream.tellg();
	if ((success = parse_string(stream, s))) {
		try {
			integer = std::stoi(s);
		} catch (std::invalid_argument const &e) {
			/* EXC(e, "failed to interpret '", s, "' as signed 32-bit integer");
			 */
			success = false;
			stream.seekg(pos);
		}
	}

	return success;
}

bool parse_choices(std::istream &stream, std::vector<std::string> &choices,
                   usize count) {
	bool        guess;
	i32         pos;
	std::string s;

	for (guess = (count == 0); guess || count > 0; guess && --count) {
		pos = stream.tellg();
		if (!parse_string(stream, s)) {
			return false;
		}
		/* une solution plus simple pourrait etre d'enlever cette clause et
		 * se baser entierement sur le parametre count en hardcodant le nombre
		 * de glaces, ou d'utiliser un autre format pour l'entree du programme,
		 * mais ceci marche directement :
		 */
		/* si on retrouve une ligne avec que des majuscules, on suppose qu'on
		 * est passe aux participants car cela ressemble a un nom de famille
		 * (pour eviter de hardcoder le nombre de glaces, 4) */
		if (guess && is_last_name(s)) {
			stream.seekg(pos);
			break;
		} else {
			choices.push_back(s);
		}
	}

	return true;
}

bool parse_participants(std::istream                    &stream,
                        std::vector<struct participant> &participants,
                        usize                            count) {
	bool        guess;
	std::string s;
	usize       i;

	for (i = 0, guess = (count == 0); guess || i < count; ++i) {
		i32                choice;
		struct participant p;

		if (!parse_string(stream, p.last_name)) {
			return guess;
		}
		if (!parse_string(stream, p.first_name)) {
			return false;
		}
		while (parse_int(stream, choice)) {
			if (choice < 1) {
				return false;
			}
			p.choices.push_back(choice - 1);
		}
		if (p.choices.size() == 0) {
			return false;
		}

		participants.push_back(p);
	}

	return true;
}
} // namespace parser

namespace algorithm {
struct choice {
	std::string name;
	u32         votes;
};

usize two_round(std::vector<std::string>              &choice_names,
                std::vector<struct participant> const &participants) {
	std::vector<struct choice> choices;

	for (std::string choice_name : choice_names) {
		struct choice c;
		c.name  = choice_name;
		c.votes = 0;
		choices.push_back(c);
	}

	for (struct participant p : participants) {
		for (u32 choice_index : p.choices) {
			if (choice_index < 0 || choice_index >= choices.size()) {
				return std::string::npos;
			}
			++choices[choice_index].votes;
		}
	}

	auto cmp = [](auto const &a, auto const &b) { return a.votes < b.votes; };
	return std::distance(choices.begin(),
	                     std::max_element(choices.begin(), choices.end(), cmp));
}

usize ranked(std::vector<std::string>              &choice_names,
             std::vector<struct participant> const &participants);

usize instant_runoff(std::vector<std::string>              &choice_names,
                     std::vector<struct participant> const &participants);
} // namespace algorithm
} // namespace vote

