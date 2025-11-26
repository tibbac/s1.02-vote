/**
 * @author Filip-Daniel Danu
 * @brief  Code source du vote majoritaire a 2 tours
 */
#include "libvote.hh"

#include <cassert>

namespace vote::algorithm {
static bool count_votes(struct ballot &ballot) {
	for (struct participant participant : ballot.participants) {
		i32 id;

		/* 2 tours : 1 vote au premier tour, 1 vote au deuxieme tour */
		if (participant.votes.size() != 2) {
			return false;
		}

		assert(ballot.round >= 1 && ballot.round <= 2);
		id = participant.votes[ballot.round - 1];

		if (!is_eliminated(ballot, id)) {
			auto it = find_candidate(ballot, id);
			if (it != ballot.candidates.end()) {
				++it->votes;
			}
		}
	}

	return true;
}

bool two_round(struct ballot &ballot) {
	double frac;
	usize  i;

	/* premier tour */
	ballot.round = 1;
	begin_round(ballot);
	if (!count_votes(ballot)) {
		return false;
	}

	std::sort(ballot.candidates.begin(), ballot.candidates.end(),
	          compare_candidates);
	frac = get_vote_fraction(ballot, ballot.candidates.back());

	print_vote(ballot);

	/* majorite absolue donc la glace gagne */
	if (frac > 0.5) {
		ballot.winners.push_back(ballot.candidates.back().id);
		return true;
	}

	/* sinon, second tour */
	/* on garde seulement les deux glaces en face */
	for (i = 0; i < ballot.candidates.size() - 2; ++i) {
		ballot.candidates[i].eliminated = true;
	}

	++ballot.round;
	begin_round(ballot);
	if (!count_votes(ballot)) {
		return false;
	}

	std::sort(ballot.candidates.begin(), ballot.candidates.end(),
	          compare_candidates);
	ballot.winners.push_back(ballot.candidates.back().id);

	print_vote(ballot);

	return true;
}
} // namespace vote::algorithm

