/**
 * @author Filip-Daniel Danu
 * @brief  Programme du vote majoritaire
 */
#include "libvote.hh"

#include <cassert>
#include <sstream>

int main() {
	std::string        buffer(std::istreambuf_iterator<char>(std::cin), {});
	std::istringstream stream(buffer);

	std::vector<struct vote::candidate>   candidates;
	std::vector<struct vote::participant> participants;

	if (!vote::parser::parse_candidates(stream, candidates, 0)) {
		std::cerr << "failed to parse candidates" << std::endl;
		return 1;
	}
	if (!vote::parser::parse_participants(stream, participants, 0)) {
		std::cerr << "failed to parse participants" << std::endl;
		return 1;
	}

	vote::ballot ballot;
	if (!vote::get_ballot(candidates, participants, ballot)) {
		std::cerr << "invalid data" << std::endl;
		return 1;
	}

	if (!vote::algorithm::two_round(ballot)) {
		std::cerr << "two-round vote failed" << std::endl;
		return 1;
	}

	auto it = find_candidate(ballot, ballot.winners[0]);
	assert(it != ballot.candidates.end());
	std::cout << "c'est la glace " << it->name << " qui a gagne" << std::endl;

	return 0;
}

