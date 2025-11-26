/**
 * @author Filip-Daniel Danu
 * @author Raoux Mattéo
 * @brief  Code source du vote alternatif
 */
#include "libvote.hh"

#include <cassert>

namespace vote::algorithm {
static bool count_votes(struct ballot &ballot) {
	for (struct participant participant : ballot.participants) {
		usize rank;
		i32   id;

		for (rank = 0; rank < participant.votes.size(); ++rank) {
			id = participant.votes[rank];

			auto it = find_candidate(ballot, id);
			if (it != ballot.candidates.end() && !it->eliminated) {
				++it->votes;
				break;
			}

			/* si le candidat est elimine, on prend le prochain dans
			 * le classement */
		}
	}

	return true;
}

bool instant_runoff(struct ballot &ballot) {
	double frac;

	ballot.round = 0;

	do {
		if (ballot.round > 0) {
			print_vote(ballot);
		}

		++ballot.round;

		/* si on a deja compte les votes ce tour et il n'y avait pas de
		 * majorite absolue, on elimine la glace en derniere position */
		if (ballot.round > 1) {
			/* on trouve la premiere glace qui n'est pas eliminee dans le
			 * vecteur deja trie par ordre croissant de nombre de votes */
			auto it = std::find_if(
				ballot.candidates.begin(), ballot.candidates.end(),
				[](struct candidate const &c) { return !c.eliminated; });

			if (it != ballot.candidates.end()) {
				it->eliminated = true;
			}
		}

		/* on commence le tour */
		begin_round(ballot);
		if (!count_votes(ballot)) {
			return false;
		}

		/* tri des glaces par ordre croissant des votes */
		std::sort(ballot.candidates.begin(), ballot.candidates.end(),
		          compare_candidates);

		/* calcul du pourcentage de repartition de la glace en 1ere
		 * position */
		frac = get_vote_fraction(ballot, ballot.candidates.back());

		/* la boucle s'arrete lorsque la glace a la majorite absolue */
	} while (frac <= 0.5);

	ballot.winners.push_back(ballot.candidates.back().id);
	print_vote(ballot);

	return true;
}
} // namespace vote::algorithm

