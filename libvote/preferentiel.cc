/**
 * @author Filip-Daniel Danu
 * @brief  Code source du vote preferentiel (methode de Coombs)
 */
#include "libvote.hh"

#include <cassert>

namespace vote::algorithm {
static bool count_votes(struct ballot &ballot) {
	for (struct participant participant : ballot.participants) {
		usize rank;
		i32   id;

		/* calcul des voix en premiere place de l'ordre de preference */
		for (rank = 0; rank < participant.votes.size(); ++rank) {
			id = participant.votes[rank];

			auto it = find_candidate(ballot, id);
			if (it != ballot.candidates.end() && !it->eliminated) {
				++it->votes;
				break;
			}
		}

		/* calcul des voix en derniere place de l'ordre de preference */
		for (rank = participant.votes.size() - 1; rank >= 0; --rank) {
			id = participant.votes[rank];

			auto it = find_candidate(ballot, id);
			if (it != ballot.candidates.end() && !it->eliminated) {
				++it->last_votes;
				break;
			}
		}
	}

	return true;
}

bool ranked(struct ballot &ballot) {
	std::vector<usize> last_votes;
	double             frac;

	ballot.round = 0;

	do {
		if (ballot.round > 0) {
			print_vote(ballot);
		}

		++ballot.round;

		/* si une glace n'a pas encore eu la majorite absolue, on elimine la
		 * glace avec le plus de votes en dernier rang */
		if (ballot.round > 1) {
			std::sort(ballot.candidates.begin(), ballot.candidates.end(),
			          compare_candidates_last);

			ballot.candidates.back().eliminated = true;
		}

		begin_round(ballot);
		if (!count_votes(ballot)) {
			return false;
		}

		std::sort(ballot.candidates.begin(), ballot.candidates.end(),
		          compare_candidates);

		frac = get_vote_fraction(ballot, ballot.candidates.back());

		/* lorsque la glace en premiere place a la majorite absolue
		 * elle gagne */
	} while (frac <= 0.5);

	ballot.winners.push_back(ballot.candidates.back().id);
	print_vote(ballot);

	return true;
}
} // namespace vote::algorithm

