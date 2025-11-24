/**
 * @auteur Filip-Daniel Danu
 * @brief Programme principal
 */
#include <getopt.h>
#include <sstream>

#include "libvote.hh"

char const static optstring[]         = "h";
struct option const static longopts[] = {
	{"help", no_argument, NULL, 'h'},
	{NULL, 0, NULL, 0}};

/**
 * Montre comment utiliser le programme
 *
 * @param arg0  Le premier argument qui est passe au programme (effectivement
 *              le nom, l'emplacement du programme, situe dans <code>argv[0]</code>)
 */
static void usage(char *const arg0) {
	std::cerr << "usage:  " << arg0 << " [--] algorithm" << std::endl <<
			     "        " << "cat input | " << arg0 << " algorithm >output" << std::endl <<
                 std::endl <<
                 "arguments" << std::endl <<
                 "\talgorithm: majoritaire/preferentiel/alternatif" << std::endl;
}

int main(int argc, char *const argv[]) {
	std::string algorithm;
	int         opt;

	/* on a besoin d'au moins argument (sans compter argv[0]) */
	if (argc <= 1) {
		usage(argv[0]);
		return 1;
	}

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
		}
	}

	/* il doit rester un dernier argument (le systeme de votes utilise) */
	if (optind == argc) {
		std::cerr << "missing argument: algorithm" << std::endl;
		usage(argv[0]);
		return 1;
	} else if (optind < argc - 1) {
		std::cerr << "too many arguments" << std::endl;
		usage(argv[0]);
		return 1;
	}

	algorithm = argv[optind];

	/* on stocke en memoire tout ce qui est contenu dans le flux standard d'entree
	 * en supposant qu'il redirige vers un fichier
	 * on fait cela pour pouvoir manipuler le flux avec les methodes tellg() et
	 * seekg() de std::basic_istream
	 * on enveloppe ce buffer dans un nouveau flux, toujours en memoire, pour
	 * l'utiliser avec nos interfaces dans libvote */

	std::string        buffer(std::istreambuf_iterator<char>(std::cin), {});
	std::istringstream stream(buffer);

	while (!std::cin.eof()) {
		if (!std::getline(std::cin, buffer)) {
			return 1;
		}
	}

	std::vector<std::string> choices;
	std::vector<struct vote::participant> participants;

	if (!vote::parser::parse_choices(stream, choices, 0)) {
		std::cerr << "failed to parse choices" << std::endl;
		return 1;
	}
	if (!vote::parser::parse_participants(stream, participants, 0)) {
		std::cerr << "failed to parse participants" << std::endl;
		return 1;
	}

	usize i;
	if (algorithm == "two_round" || algorithm == "majoritaire") {
		i = vote::algorithm::two_round(choices, participants);
	} else {
		return 1;
	}
	std::cout << "c'est la glace " << choices[i] << " qui a gagne" << std::endl;

	return 0;
}

