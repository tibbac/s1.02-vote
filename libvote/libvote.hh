/**
 * @author Filip-Daniel Danu
 * @brief  Bibliotheque de code boilerplate pour les systemes de vote
 */
#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef ptrdiff_t isize;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t   usize;

/**
 * Espace de nommage principal de la bibliotheque
 */
namespace vote {
/**
 * Structure representant un candidat
 */
struct candidate {
	/**
	 * Identifiant de la glace
	 */
	i32 id;
	/**
	 * Nom de la glace
	 */
	std::string name;
	/**
	 * Nombre de voix accumulees dans un vote
	 */
	usize votes;
	/**
	 * Pour la methode de Coombs (vote preferentiel) : on calcule toutes les
	 * voix pour lesquelles la glace est en derniere place dans l'ordre de
	 * classement des participants
	 */
	usize last_votes;
	/**
	 * Indique si la glace a ete elimine dans un vote
	 */
	bool eliminated;
};

/**
 * Structure representant un participant d'un vote
 */
struct participant {
	/**
	 * Nom de famille du participant
	 */
	std::string last_name;
	/**
	 * Prenom du participant
	 */
	std::string first_name;
	/**
	 * Les identifiants des glaces pour lesquels le participant a vote
	 */
	std::vector<i32> votes;
};

/**
 * Structure representant un vote organise avec des glaces et des
 * participants ayant votes pour ces glaces
 */
struct ballot {
	/**
	 * Toutes les glaces du vote
	 */
	std::vector<struct candidate> candidates;
	/**
	 * Tous les participants du vote
	 */
	std::vector<struct participant> participants;
	/**
	 * Glace gagnantes
	 */
	std::vector<i32> winners;
	/**
	 * Le tour actuel du vote. Apres avoir applique un systeme de votes,
	 * ce nombre deviendra le nombre total de tours effectues.
	 */
	u32 round;
};

/**
 * Initialise une structure de vote pour pouvoir commencer un vote
 *
 * @param candidates    Les glaces
 * @param participants  Les participants du vote
 * @param ballot  Le vote
 * @return  Valeur booleenne indiquant si la structure a ete cree ou s'il
 *          y a eu une erreur
 */
bool get_ballot(std::vector<struct candidate>   candidates,
                std::vector<struct participant> participants,
                struct ballot                  &ballot);

/**
 * Trouve une glace dans une structure de vote a l'aide de son identifiant
 *
 * @param ballot        Le vote
 * @param candidate_id  L'identifiant de la glace
 * @return  Un iterateur appartenant au vecteur <code>ballot.candidates</code>
 */
std::vector<struct candidate>::iterator find_candidate(struct ballot &ballot,
                                                       i32 candidate_id);

/**
 * Determine si une glace est gagnante d'un vote a l'aide de son identifiant
 *
 * @param ballot        Le vote
 * @param candidate_id  L'identifiant de la glace
 * @return  Valeur booleenne indiquant si la glace est gagnante ou non
 */
bool is_winner(struct ballot const &ballot, i32 candidate_id);

/**
 * Determine si une glace a ete elimine d'un vote a l'aide de son identifiant
 *
 * @param ballot        Le vote
 * @param candidate_id  L'identifiant de la glace
 * @return  Valeur booleenne indiquant si la glace est eliminee ou non
 */
bool is_eliminated(struct ballot &ballot, i32 candidate_id);

/**
 * Affiche un vote en cours.
 *
 * @param ballot  Le vote a afficher
 */
void print_vote(struct ballot &ballot);

/**
 * Fonction pour comparer des candidats entre eux selon leurs nombres de
 * votes
 *
 * @param lhs  Cote gauche
 * @param rhs  Cote droit
 * @return  Renvoie la condition <code>lhs.votes < rhs.votes</code>
 */
inline bool compare_candidates(struct candidate const &lhs,
                               struct candidate const &rhs) {
	return lhs.votes < rhs.votes;
}

/**
 * Fonction pour comparer des candidats entre eux selon leurs nombres de
 * votes en derniere place (methode de Coombs)
 *
 * @param lhs  Cote gauche
 * @param rhs  Cote droit
 * @return  Renvoie la condition
 *          <code>lhs.last_votes < rhs.last_votes</code>
 */
inline bool compare_candidates_last(struct candidate const &lhs,
                                    struct candidate const &rhs) {
	return lhs.last_votes < rhs.last_votes;
}

/**
 * Utilites d'analyse d'entrees
 */
namespace parser {
/**
 * Lit une chaine de caracteres.
 * Si la ligne est precedee de "//", elle est consideree comme un commentaire
 * et est ignore par la fonction, renvoyant la prochaine ligne.
 *
 * @param stream  Flux d'entree
 * @param string  La chaine de caracteres
 * @return  Valeur booleenne indicative du succes de la fonction
 */
bool read_string(std::istream &stream, std::string &string);

/**
 * Lit un entier.
 * Meme remarques que la fonction parse_string.
 *
 * @param stream   Flux d'entree
 * @param integer  L'entier renvoye
 * @see  parse_string()
 * @return  Valeur booleenne indicative du succes de la fonction
 */
bool parse_int(std::istream &stream, i32 &integer);

/**
 * Traite les noms de glaces contenues dans un flux. Si le format est incorrect,
 * la fonction echoue.
 *
 * @param stream      Flux d'entree
 * @param candidates  Vecteur de sortie
 * @param count       Nombre de glaces a traiter. Si ce nombre est 0, on essaie
 *                    de deviner quand il faut arreter pour passer aux
 * participants.
 * @return  Valeur booleenne indicative du succes de la fonction
 */
bool parse_candidates(std::istream                  &stream,
                      std::vector<struct candidate> &candidates, usize count);

/**
 * Traite les noms de participants contenus dans un flux. Si le format est
 * incorrect, la fonction echoue.
 *
 * @param stream        Flux d'entree
 * @param participants  Vecteur de sortie
 * @param count         Nombre de participants a traiter. S'il est 0, on traite
 *                      les lignes suivantes jusqu'a la fin du flux.
 * @return  Valeur booleenne indicative du succes de la fonction
 */
bool parse_participants(std::istream                    &stream,
                        std::vector<struct participant> &participants,
                        usize                            count);
} // namespace parser

/**
 * Systemes de votes
 */
namespace algorithm {
/**
 * Commence un tour d'un vote en reinitialisant toutes les voix des glaces
 * qui ne sont pas encore eliminees pour les recompter
 *
 * @param ballot  Le vote
 */
void begin_round(struct ballot &ballot);

/**
 * Calcule le pourcentage des voix qu'une glace possede
 *
 * @param ballot     Le vote
 * @param candidate  La glace
 * @return  Nombre flottant normalise entre 0 et 1
 */
double get_vote_fraction(struct ballot          &ballot,
                         struct candidate const &candidate);

/**
 * Systeme de votes majoritaire 2 tours.
 *
 * @param ballot  Le vote
 * @return  Valeur booleenne indiquant que le systeme de votes a ete
 *          effectue avec succes
 */
bool two_round(struct ballot &ballot);

/**
 * Systeme de votes preferentiel.
 *
 * @param ballot  Le vote
 * @return  Valeur booleenne indiquant que le systeme de votes a ete
 *          effectue avec succes
 */
bool ranked(struct ballot &ballot);

/**
 * Systeme de votes alternatif.
 *
 * @param ballot  Le vote
 * @return  Valeur booleenne indiquant que le systeme de votes a ete
 *          effectue avec succes
 */
bool instant_runoff(struct ballot &ballot);
} // namespace algorithm

/**
 * Utilites de generation de votes aleatoires
 */
namespace generator {
/**
 * Initialisation du generateur de nombres pseudo-aleatoires
 */
void init();

/**
 * Generation de fichier d'entree pour un vote.
 * Toutes les voix seront choisies aleatoirement.
 *
 * @param stream        Le flux de sortie
 * @param candidates    Glaces
 * @param participants  Participants
 * @param vote_count    Si cet argument est 0, chaque participant
 *                      pourra voter pour une ou plusieurs glaces. Sinon,
 *                      il correspond au nombre exact de glaces
 *                      individuelles que chaque participant devra avoir vote
 *                      pour.
 * @return Valeur booleenne indiquant le succes de la generation
 */
bool generate_vote(std::ostream                        &stream,
                   std::vector<struct candidate> const &candidates,
                   std::vector<struct participant>     &participants,
                   usize                                vote_count);
} // namespace generator
} // namespace vote

