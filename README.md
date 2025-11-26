# s1.02-vote

[Lien du dépôt GitHub](https://github.com/tibbac/s1.02-vote)

[Lien de la documentation générée par Doxygen](https://byte.cab/s1.02-vote/index.html)

## Sommaire

Ce dépôt contient notre travail pour la SAÉ 1.02 de M. Casali.

Nous avons les systèmes de votes suivants :

- Vote majoritaire à 2 tours
- Vote préférentiel : méthode de Coombs
- Vote alternatif

## Structure

Nous utilisons la template SUBDIRS de Qt Creator avec 5 projets :

- libvote : bibliothèque statique contenant du code partagé en commun avec tous les projets
- vote : programme CLI pour réunir les trois systèmes de vote en une interface console
- vote_majoritaire : programme de vote majoritaire à deux tours
- vote_preferentiel : programme de méthode de Coombs
- vote_alternatif : programme de vote alternatif

Un makefile est présent pour compiler le projet avec seulement qmake.

## Membres du groupe

- [@tibbac](https://github.com/tibbac) : Filip-Daniel Danu
- [@NAIT0X](https://github.com/NAIT0X) : Matys Longo
- [@matteo2510](https://github.com/matteo2510) : Mattéo Raoux
- [@porcororo](https://github.com/porcororo) : Clément Zuber

## Utilisation

Exemple :

```sh
make release
cat tests/majoritaire/entree1.txt | ./out/vote/vote -a majoritaire >sortie1.txt
diff tests/majoritaire/oracle1.txt sortie1.txt
cat tests/alternatif/entree2.txt | ./out/vote_alternatif/vote_alternatif
# et ainsi de suite
```

Voir `./vote -h` pour plus de détails sur la syntaxe.

