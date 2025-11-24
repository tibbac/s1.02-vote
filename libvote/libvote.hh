/**
 * @author Filip-Daniel Danu
 * @brief  Bibliotheque de code boilerplate pour les systemes de vote
 */
#pragma once

#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

#define EXC(...)

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

template <typename T> constexpr T min(T x, T y) { return x < y ? x : y; }

template <typename T> constexpr T max(T x, T y) { return x > y ? x : y; }

namespace vote {
struct participant {
	std::string      last_name;
	std::string      first_name;
	std::vector<u32> choices;
};

namespace parser {
/**
 * Lit une chaine de caracteres.
 * Si la ligne est precedee de "//", elle est consideree comme un commentaire
 * et est ignore par la fonction, renvoyant la prochaine ligne.
 *
 * @param stream Flux d'entree
 * @param string La chaine de caracteres
 * @return Valeur booleenne indicative du succes de la fonction
 */
bool parse_string(std::istream &stream, std::string &string);

/**
 * Lit un entier.
 * Meme remarques que la fonction parse_string.
 *
 * @param stream Flux d'entree
 * @param string L'entier
 * @see parse_string()
 * @return Valeur booleenne indicative du succes de la fonction
 */
bool parse_int(std::istream &stream, i32 &integer);

/**
 * Traite les noms de glaces contenues dans un flux. Si le format est incorrect,
 * la fonction echoue.
 *
 * @param stream    Flux d'entree
 * @param choices   Vecteur de sortie
 * @param count     Nombre de glaces a traiter. Si ce nombre est 0, on essaie
 *                  de deviner quand il faut arreter pour passer aux candidats.
 * @param remainder La derniere chaine de caracteres lue lorsque 'count' est
 *                  affecte a 0 et que le nom d'un participant est detecte,
                    cette chaine correspondra donc au nom du participant.
 * @return Valeur booleenne indicative du succes de la fonction
 */
bool parse_choices(std::istream &stream, std::vector<std::string> &choices,
                   usize count);

/**
 * Traite les noms de participants contenus dans un flux. Si le format est
 * incorrect, la fonction echoue.
 *
 * @param stream       Flux d'entree
 * @param participants Vecteur de sortie
 * @param count        Nombre de participants a traiter. S'il est 0, on traite
 * @param remainder    La chaine de caracteres 'remainder' renvoyee par la
                       fonction parse_choices().
 * le flux jusqu'a qu'il soit vide.
 * @return Valeur booleenne indicative du succes de la fonction
 */
bool parse_participants(std::istream                    &stream,
                        std::vector<struct participant> &participants,
                        usize                            count);
} // namespace parser

namespace algorithm {
/**
 * Systeme de vote majoritaire 2 tours.
 *
 * @param choices      Noms des glaces
 * @param participants Participants du vote
 * @return L'indice de la glace dans le vecteur 'choices' qui a gagne.
 */
usize two_round(std::vector<std::string>              &choices,
                std::vector<struct participant> const &participants);

/**
 * Systeme de vote preferentiel.
 *
 * @param choices      Noms des glaces
 * @param participants Participants du vote
 * @return L'indice de la glace dans le vecteur 'choices' qui a gagne.
 */
usize ranked(std::vector<std::string>              &choices,
             std::vector<struct participant> const &participants);

/**
 * Systeme de vote alternatif.
 *
 * @param choices      Noms des glaces
 * @param participants Participants du vote
 * @return L'indice de la glace dans le vecteur 'choices' qui a gagne.
 */
usize instant_runoff(std::vector<std::string>              &choices,
                     std::vector<struct participant> const &participants);
} // namespace algorithm
} // namespace vote

