/**
 * @author Filip-Daniel Danu
 * @brief  Implementation de l'interface de la bibliotheque
 */
#include "libvote.hh"

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

	for (; (success = (bool)std::getline(stream, string));) {
		/* si la ligne est vide, contient que des espaces, ou est un commentaire
		 * on l'ignore */
		if (string.empty() ||
		    string.find_first_not_of(' ') == std::string::npos ||
		    string.rfind("//", 0) == 0) {
			continue;
		}
		break;
	}

	return success;
}

bool parse_int(std::istream &stream, i32 &integer) {
	bool        success;
	std::string s;

	if ((success = parse_string(stream, s))) {
		try {
			integer = std::stoi(s);
		} catch (std::invalid_argument const &e) {
			success = false;
		}
	}

	return success;
}

bool parse_choices(std::istream &stream, std::vector<std::string> &choices,
                   usize count) {
	bool        check_participant;
	bool        success;
	std::string s;

	check_participant = count == 0;
	for (; (check_participant || count > 0) &&
	       (success = parse_string(stream, s));
	     --count) {
		/* une solution plus simple pourrait etre d'enlever cette clause et
		 * se baser entierement sur le parametre count en hardcodant le nombre
		 * de glaces, ou d'utiliser un autre format pour l'entree du programme,
		 * mais ceci marche directement :
		 */
		/* si on retrouve une ligne avec que des majuscules, on suppose qu'on
		 * est passe aux participants car cela ressemble a un nom de famille
		 * (pour eviter de hardcoder le nombre de glaces, 4) */
		if (check_participant && is_last_name(s)) {
			break;
		} else {
			choices.push_back(s);
		}
	}
	return success;
}

bool parse_participants(std::istream                    &stream,
                        std::vector<struct participant> &participants,
                        usize                            count) {
	std::string s;
	bool        success;

	if (count == 0) {
		count = std::numeric_limits<usize>::max();
	}
	for (; count > 0 && (success = parse_string(stream, s)); --count) {
		struct participant p;

		participants.push_back(p);
	}

	return success;
}
} // namespace parser
} // namespace vote

