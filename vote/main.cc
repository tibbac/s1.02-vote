/**
 * @author Filip-Daniel Danu
 * @brief  Programme principal
 */
#include "libvote.hh"

#include <cassert>
#include <getopt.h>
#include <sstream>

char const static optstring[]         = "hgc:";
struct option const static longopts[] = {
	{"help", no_argument, NULL, 'h'},
	{"generate", no_argument, NULL, 'g'},
	{"count", required_argument, NULL, 'c'},
	{NULL, 0, NULL, 0}};

/**
 * Montre comment utiliser le programme
 *
 * @param arg0  Le premier argument qui est passe au programme (effectivement
 *              le nom, l'emplacement du programme, situe dans
 *              <code>argv[0]</code>)
 */
static void usage(char *const arg0) {
	/* clang-format off */
	std::cerr
		<< "usage:  " << arg0 << " [--] ALGORITHM" << std::endl
		<< "        " << arg0 << " -g [-c COUNT]" << std::endl
		<< std::endl
		<< "options" << std::endl
		<< "\t-h, --help  shows this screen" << std::endl
	    << std::endl
		<< "\t-g, --generate  generates a new input file with random votes" << std::endl
	    << "\t                using candidate and participant lists from stdin" << std::endl
	    << std::endl
		<< "\t-c, --count COUNT  (use with -g) vote for a certain amount" << std::endl
		<< "\t                   of candidates" << std::endl
		<< std::endl
		<< "arguments" << std::endl
		<< "\tALGORITHM:" << std::endl
		<< "\t    two_round, majoritaire        two-round system" << std::endl
		<< "\t    ranked, alternatif            custom ranked voting system" << std::endl
		<< "\t    instant_runoff, preferentiel  instant-runoff voting system" << std::endl
		<< std::endl
		<< "examples" << std::endl
		<< "\tcat input1 | " << arg0 << " two_round >output1" << std::endl
		<< "\tcat input1 | " << arg0 << " -g -c 2 >input2" << std::endl
		<< std::endl;
	/* clang-format on */
}

int main(int argc, char *const argv[]) {
	std::string algorithm;

	i32  vote_count;
	bool generate;

	int opt;

	/* on a besoin d'au moins 1 argument pour getopt_long
	 * sans compter argv[0] */
	if (argc <= 1) {
		usage(argv[0]);
		return 1;
	}

	vote_count = 0;
	generate   = false;

	/* traitement de la ligne de commande via l'interface POSIX getopt_long */
	while ((opt = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) {
		switch (opt) {
		default:
		case '?':
		case ':':
			usage(argv[0]);
			return 1;
		case 'h':
			usage(argv[0]);
			return 0;
		case 'g':
			generate = true;
			break;
		case 'c':
			try {
				vote_count = std::stoul(optarg);
			} catch (std::invalid_argument const &e) {
				std::cerr << "invalid argument: " << optarg << std::endl;
				usage(argv[0]);
				return 1;
			} catch (std::out_of_range const &e) {
				std::cerr << "argument out of range: " << optarg << std::endl;
				usage(argv[0]);
				return 1;
			}
			break;
		}
	}

	/* on stocke en memoire tout ce qui est contenu dans le flux standard
	 * d'entree en supposant qu'il redirige vers un fichier on fait cela pour
	 * pouvoir manipuler le flux avec les methodes tellg() et seekg() de
	 * std::basic_istream on enveloppe ce buffer dans un nouveau flux, toujours
	 * en memoire, pour l'utiliser avec nos interfaces dans libvote */

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

	if (generate) {
		vote::generator::init();

		return (int)vote::generator::generate_vote(std::cout, candidates,
		                                           participants, vote_count);
	}

	/* il doit rester un dernier argument (le systeme de votes utilise) */
	if (optind == argc) {
		std::cerr << "missing argument: ALGORITHM" << std::endl;
		usage(argv[0]);
		return 1;
	} else if (optind < argc - 1) {
		std::cerr << "too many arguments" << std::endl;
		usage(argv[0]);
		return 1;
	}

	algorithm = argv[optind];

	vote::ballot b;
	if (!vote::get_ballot(candidates, participants, b)) {
		std::cerr << "invalid data" << std::endl;
		return 1;
	}

	if (algorithm == "two_round" || algorithm == "majoritaire") {
		if (!vote::algorithm::two_round(b)) {
			std::cerr << "two-round vote failed" << std::endl;
			return 1;
		}
	} else if (algorithm == "instant_runoff" || algorithm == "alternatif") {
		if (!vote::algorithm::instant_runoff(b)) {
			std::cerr << "instant-runoff vote failed" << std::endl;
			return 1;
		}
	} else if (algorithm == "ranked" || algorithm == "preferentiel") {
		if (!vote::algorithm::ranked(b)) {
			std::cerr << "ranked vote failed" << std::endl;
			return 1;
		}
	} else {
		std::cerr << "invalid argument for ALGORITHM: " << algorithm
				  << std::endl;
		return 1;
	}

	auto it = find_candidate(b, b.winners[0]);
	assert(it != b.candidates.end());
	std::cout << "c'est la glace " << it->name << " qui a gagne" << std::endl;

	return 0;
}

