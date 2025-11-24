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

template <typename T> constexpr T min(T x, T y) { return x < y ? x : y; }

template <typename T> constexpr T max(T x, T y) { return x > y ? x : y; }

namespace vote {
struct participant {
	std::string last_name;
	std::string first_name;
	i32         glace;
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
 * @param stream Flux d'entree
 * @param choices Vecteur de sortie
 * @param count  Nombre de glaces a traiter. Si ce nombre est 0, on essaie
 *               de deviner quand il faut arreter pour passer aux candidats.
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
 * @param count        Nombre de candidats a traiter. S'il est 0, on traite le
 *                     flux jusqu'a qu'il soit vide.
 * @return Valeur booleenne indicative du succes de la fonction
 */
bool parse_participants(std::istream                    &stream,
                        std::vector<struct participant> &participants,
                        usize                            count);
} // namespace parser

namespace algorithm {} // namespace algorithm
} // namespace vote

