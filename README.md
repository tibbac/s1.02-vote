# s1.02-vote

[Lien du dépôt GitHub](https://github.com/tibbac/s1.02-vote)

[Lien de la documentation générée par Doxygen](https://byte.cab/s1.02-vote/index.html)

## Sommaire

Ce dépôt contient notre travail pour la SAÉ 1.02 de M. Casali.

Nous avons les systèmes de votes suivants :

- Vote majoritaire à 2 tours
- Vote préférentiel
- Vote alternatif

## Membres du groupe

- [@tibbac](https://github.com/tibbac) : Filip-Daniel Danu
- [@NAIT0X](https://github.com/NAIT0X) : Matys Longo
- [@matteo2510](https://github.com/matteo2510) : Mattéo Raoux
- [@porcororo](https://github.com/porcororo) : Clément Zuber

## Utilisation

Exemple :

```sh
make release
cd out/vote
cat ../../tests/entree1.txt | ./vote -a majoritaire >sortie1.txt
diff ../../tests/oracle1.txt sortie1.txt
```

Voir `./vote -h` pour plus de détails sur la syntaxe.

