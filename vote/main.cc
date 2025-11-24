#include <getopt.h>
#include <sstream>

#include "libvote.hh"

char const static optstring[]         = "a:h";
struct option const static longopts[] = {
	{"help", no_argument, NULL, 'h'},
	{"system", required_argument, NULL, 's'},
	{NULL, 0, NULL, 0}};

static void usage(char *const arg0) {
	std::cerr << "usage: " << arg0 << " -a algorithm" << std::endl
			  << "       " << "cat input | " << arg0 << " >output" << std::endl;
}

int main(int argc, char *const argv[]) {
	std::string algorithm;
	int         opt;

	if (argc <= 1) {
		usage(argv[0]);
		return 1;
	}

	while ((opt = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) {
		switch (opt) {
		default:
		case '?':
		case ':':
			usage(argv[0]);
			return 1;
		case 'a':
			algorithm = std::string(optarg, strlen(optarg));
			if (algorithm != "two_round") {
				std::cerr << "unknown voting algorithm '" << algorithm << "'"
						  << std::endl;
				usage(argv[0]);
				return 1;
			}
			break;
		case 'h':
			usage(argv[0]);
			return 0;
		}
	}

	std::string        buffer(std::istreambuf_iterator<char>(std::cin), {});
	std::istringstream stream(buffer);
	for (;;) {
		if (!std::getline(std::cin, buffer)) {
			if (std::cin.eof()) {
				break;
			}
			return 1;
		}
	}

	std::vector<std::string> choices;
	if (!vote::parser::parse_choices(stream, choices, 0)) {
		std::cerr << "failed to parse choices" << std::endl;
		return 1;
	}

	std::vector<struct vote::participant> participants;
	if (!vote::parser::parse_participants(stream, participants, 0)) {
		std::cerr << "failed to parse participants" << std::endl;
		return 1;
	}

	usize i;
	if (algorithm == "two_round") {
		i = vote::algorithm::two_round(choices, participants);
	} else {
		return 1;
	}
	std::cout << "c'est la glace " << choices[i] << " qui a gagne" << std::endl;

	return 0;
}

