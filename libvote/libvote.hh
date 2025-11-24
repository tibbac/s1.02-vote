/**
 * @author Filip-Daniel Danu
 * @brief  Bibliotheque de code boilerplate pour les systemes de vote
 */
#pragma once

#include <cstdint>
#include <iostream>
#include <limits>
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
 * Structure representant le prenom et le nom de famille d'un participant
 */
struct participant_name {
	/**
	 * Nom de famille du participant
	 */
	std::string last_name;
	/**
	 * Prenom du participant
	 */
	std::string first_name;
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
	 * Choix du participant dans le vote
	 */
	std::vector<u32> choices;
};

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
bool parse_string(std::istream &stream, std::string &string);

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
 * @param stream   Flux d'entree
 * @param choices  Vecteur de sortie
 * @param count    Nombre de glaces a traiter. Si ce nombre est 0, on essaie
 *                 de deviner quand il faut arreter pour passer aux candidats.
 * @return  Valeur booleenne indicative du succes de la fonction
 */
bool parse_choices(std::istream &stream, std::vector<std::string> &choices,
                   usize count);

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
 * Systeme de votes majoritaire 2 tours.
 *
 * @param choices       Noms des glaces
 * @param participants  Participants du vote
 * @return  L'indice de la glace dans le vecteur <code>choices</code> qui a
 * gagne.
 */
usize two_round(std::vector<std::string> const        &choices,
                std::vector<struct participant> const &participants);

/**
 * Systeme de votes preferentiel.
 *
 * @param choices       Noms des glaces
 * @param participants  Participants du vote
 * @return  L'indice de la glace dans le vecteur <code>choices</code> qui a
 * gagne.
 */
usize ranked(std::vector<std::string> const        &choices,
             std::vector<struct participant> const &participants);

/**
 * Systeme de votes alternatif.
 *
 * @param choices       Noms des glaces
 * @param participants  Participants du vote
 * @return  L'indice de la glace dans le vecteur <code>choices</code> qui a
 * gagne.
 */
usize instant_runoff(std::vector<std::string> const        &choices,
                     std::vector<struct participant> const &participants);
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
 * Les choix de chaque participant seront aleatoires.
 *
 * @param stream             Le flux de sortie (peut etre un
 *                           <code>std::ofstream</code>)
 * @param choices            Noms des glaces
 * @param participant_names  Noms des participants
 * @param multiple_choices   Indique si un participant peut voter pour
 *                           plusieurs glaces
 * @return
 */
bool generate_participants(
	std::ostream &stream, std::vector<std::string> const &choices,
	std::vector<struct participant_name> const &participant_names,
	bool                                        multiple_choices);
} // namespace generator
} // namespace vote

