/**
 * @author Filip-Daniel Danu
 * @brief  Implementation de l'interface de la bibliotheque
 */
#include "libvote.hh"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>

namespace vote {
bool get_ballot(std::vector<struct candidate>   candidates,
                std::vector<struct participant> participants,
                struct ballot                  &b) {
	for (struct candidate &candidate : candidates) {
		candidate.votes      = 0;
		candidate.last_votes = 0;
		candidate.eliminated = false;
	}

	/* verification de l'entree utilisateur */
	for (struct participant participant : participants) {
		for (i32 id : participant.votes) {
			if (id < 1 || (usize)id > candidates.size()) {
				return false;
			}
		}
	}

	b.candidates   = candidates;
	b.participants = participants;
	b.round        = 1;

	return true;
}

std::vector<struct candidate>::iterator find_candidate(struct ballot &ballot,
                                                       i32 candidate_id) {
	auto pred = [candidate_id](struct candidate const &c) {
		return c.id == candidate_id;
	};
	auto it =
		std::find_if(ballot.candidates.begin(), ballot.candidates.end(), pred);
	return it;
}

bool is_winner(struct ballot const &ballot, i32 candidate_id) {
	auto it =
		std::find(ballot.winners.begin(), ballot.winners.end(), candidate_id);
	return it != ballot.winners.end();
}

bool is_eliminated(struct ballot &ballot, i32 candidate_id) {
	auto it = find_candidate(ballot, candidate_id);
	return it != ballot.candidates.end() && it->eliminated;
}

void print_vote(struct ballot &ballot) {
	std::vector<struct candidate> candidates;

	candidates = ballot.candidates;
	std::sort(candidates.begin(), candidates.end(), compare_candidates);
	std::reverse(candidates.begin(), candidates.end());

	std::cerr << "Round " << ballot.round << std::endl;

	for (struct vote::candidate c : candidates) {
		if (is_winner(ballot, c.id)) {
			std::cerr << "Winner: ";
		}

		std::cerr << "(id: " << c.id << ") " << c.name << " (" << std::round(1000 * algorithm::get_vote_fraction(ballot, c)) / 10 << "%): " << c.votes
				  << " votes";

		if (c.last_votes > 0) {
			std::cerr << " (" << c.last_votes << " last place votes)";
		}

		std::cerr << std::endl;
	}

	std::cerr << std::endl;
}

namespace parser {
static bool is_last_name(std::string const &s) {
	for (char c : s) {
		if (isalpha(c) && islower(c)) {
			return false;
		}
	}
	return true;
}

bool read_string(std::istream &stream, std::string &string) {
	for (;;) {
		if (!std::getline(stream, string)) {
			return false;
		}

		/* si la ligne est vide, contient que des espaces, ou est un commentaire
		 * on l'ignore */
		if (string.empty() ||
		    string.find_first_not_of(' ') == std::string::npos ||
		    string.rfind("//", 0) == 0) {
			continue;
		}

		return true;
	}
}

bool parse_int(std::istream &stream, i32 &integer) {
	std::streampos pos;
	std::string    s;

	pos = stream.tellg();

	if (read_string(stream, s)) {
		try {
			integer = std::stoi(s);
			return true;
		} catch (std::invalid_argument const &e) {
		} catch (std::out_of_range const &e) {
		}

		stream.seekg(pos, std::ios::beg);
	}

	return false;
}

bool parse_candidates(std::istream                  &stream,
                      std::vector<struct candidate> &candidates, usize count) {
	bool guess;
	i32  i;

	for (i = 1, guess = (count == 0); guess || (count > 0);
	     ++i, guess && --count) {

		std::streampos pos;
		std::string    s;

		pos = stream.tellg();
		if (!read_string(stream, s)) {
			return false;
		}

		/* une solution plus simple pourrait etre d'enlever cette clause et
		 * se baser entierement sur le parametre count en hardcodant le nombre
		 * de glaces, ou d'utiliser un autre format pour l'entree du programme,
		 * mais ceci marche directement :
		 */
		/*
		 * si on retrouve une ligne avec que des majuscules, on suppose qu'on
		 * est passe aux participants car cela ressemble a un nom de famille
		 * (pour eviter de hardcoder le nombre de glaces, 4)
		 */
		if (guess && is_last_name(s)) {
			stream.seekg(pos, std::ios::beg);
			break;
		} else {
			struct candidate candidate;

			candidate.id         = i;
			candidate.name       = s;
			candidate.votes      = 0;
			candidate.last_votes = 0;
			candidate.eliminated = false;

			candidates.push_back(candidate);
		}
	}

	return true;
}

bool parse_participants(std::istream                    &stream,
                        std::vector<struct participant> &participants,
                        usize                            count) {
	bool guess;

	for (guess = (count == 0); guess || (count > 0); --count) {
		struct participant participant;
		std::string        tmp;
		i32                id;

		if (!read_string(stream, participant.last_name)) {
			return guess && stream.eof();
		}
		if (!read_string(stream, participant.first_name)) {
			return false;
		}
		while (parse_int(stream, id)) {
			if (id < 1) {
				return false;
			}
			participant.votes.push_back(id);
		}
		if (participant.votes.size() == 0) {
			return false;
		}

		participants.push_back(participant);
	}

	return true;
}
} // namespace parser

namespace algorithm {
void begin_round(struct ballot &ballot) {
	for (struct candidate &candidate : ballot.candidates) {
		if (!candidate.eliminated) {
			candidate.votes = 0;
		}
	}

	ballot.winners.clear();
}

double get_vote_fraction(struct ballot          &ballot,
                         struct candidate const &candidate) {
	usize sum;

	sum = 0;
	for (struct candidate c : ballot.candidates) {
		if (!is_eliminated(ballot, c.id)) {
			sum += c.votes;
		}
	}

	return (double)candidate.votes / (double)sum;
}
} // namespace algorithm

namespace generator {
void init() { std::srand(std::time(0)); }

bool generate_vote(std::ostream                        &stream,
                   std::vector<struct candidate> const &candidates,
                   std::vector<struct participant>     &participants,
                   usize                                vote_count) {

	std::vector<i32> vote_indices(candidates.size());
	usize            i;

	vote_count = std::min(vote_count, candidates.size());

	for (i = 0; i < vote_indices.size(); ++i) {
		vote_indices[i] = i + 1;
	}

	stream << "// fichier genere automatiquement" << std::endl << std::endl;

	stream << "// glaces" << std::endl << std::endl;

	for (struct candidate candidate : candidates) {
		stream << candidate.name << std::endl;
	}

	stream << std::endl << "// participants" << std::endl << std::endl;

	for (struct participant &participant : participants) {
		usize j;
		usize n;

		stream << participant.last_name << std::endl;
		stream << participant.first_name << std::endl;

		if (vote_count == 0) {
			n = 1 + (u32)std::rand() % vote_indices.size();
		} else {
			n = vote_count;
		}

		participant.votes = vote_indices;

		for (j = 0; j < n; ++j) {
			std::swap(participant.votes[j],
			          participant.votes[std::rand() % vote_indices.size()]);
		}

		participant.votes.resize(n);

		for (j = 0; j < n; ++j) {
			stream << participant.votes[j] << std::endl;
		}

		stream << std::endl;
	}

	stream << std::endl;

	return bool(stream);
}
} // namespace generator
} // namespace vote

