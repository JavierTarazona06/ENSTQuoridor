# Documentation technique de l'algorithme IA

## Architecture
- **Module** : `model/AI.{hpp,cpp}`
- **Entrées** : `Board`, `State` (joueur courant, positions, murs restants)
- **Sortie** : `Move` (déplacement de pion ou pose de mur)
- **Stratégie** : Minimax avec élagage Alpha-Bêta, profondeur dépendante de la difficulté, injection de bruit pour simuler des erreurs volontaires.

## Niveaux de difficulté
- **Easy** : profondeur 1, bruit élevé (40) 	1 chance sur 2 de jouer sous-optimal
- **Normal** : profondeur 1, bruit modéré (8) 	occasionnellement perfectible
- **Hard** : profondeur 2, bruit faible (3) 	rarement des erreurs
- **Hell** : profondeur 4, bruit nul (0) 	jeu quasi parfait

Paramètres : `DifficultyConfig` dans `AI.cpp` (profondeur + bruit). Le bruit est ajouté à l'évaluation sauf pour les états terminaux.

## Boucle de décision
1. Récupérer le joueur courant via `State`.
2. Générer l'ensemble des coups légaux (`getAllValidMoves`) :
	 - Déplacements de pion (voisins et sauts autorisés par `Rules::isValidMove`).
	 - Poses de mur pertinentes (chemin le plus court adverse + proximité adversaire) filtrées par `Rules::isValidWallPlacement`.
3. Pour chaque coup, simuler sur une copie de `Board`, appeler `minimax` (profondeur dépend de la difficulté, alternance max/min) et conserver le meilleur score.
4. Retourner le coup avec le score maximal (fallback : premier coup si aucun score trouvé).

## Minimax + Alpha-Bêta
- **maximizingPlayer** : l'IA (joueur courant au début).
- **minimizingPlayer** : l'adversaire.
- Arrêt : profondeur 0 ou état terminal (score max/min renvoyé par `evaluate`).
- Élagage : coupure quand `beta <= alpha`.
- Score retourné : `evaluateWithNoise` (évaluation + bruit contrôlé).

## Heuristique d'évaluation (`evaluate`)
Composée de cinq facteurs pondérés (`EvalWeights` dans `AI.cpp`) :
1. **Distance vers l'objectif** (poids 10) : `(dist_adv - dist_moi)` via `Pathfinder::getShortestPathDistance`.
2. **Bonus fin de partie** (poids 15, non linéaire) : récompense forte si `dist <= 3`, pénalise si l'adversaire est proche.
3. **Avantage murs** (poids 2) : murs restants pour moi vs adversaire.
4. **Mobilité** (poids 3) : nombre de coups de pion valides (4 voisins) pour moi vs adversaire.
5. **Efficacité des murs** (poids 5) : allongement du chemin adverse vs base, pénalité symétrique si mes murs me gênent.

Gestion des terminaux :
- Victoire IA 		`+inf` (INT_MAX)
- Victoire adverse 	`-inf` (INT_MIN)
- Chemin bloqué IA 	`INT_MIN + 1`
- Chemin bloqué adverse `INT_MAX - 1`

## Aléa et bruit
- Générateur `std::mt19937` thread_local.
- Bruit uniforme dans `[-noise, noise]` appliqué au score non terminal (`evaluateWithNoise`).
- Easy/Normal/Hard/Hell modulent `noiseLevel` pour simuler des erreurs.

## Génération des coups
- **Pions** : exploration locale 5x5 centrée sur le pion, validée par `Rules::isValidMove`.
- **Murs** :
	- Blocage du plus court chemin adverse (chemin obtenu via `Pathfinder::getShortestPath`).
	- "Box" autour de l'adversaire (carré 2x2 autour de sa position).
	- Filtrage par `Rules::isValidWallPlacement` + set pour éviter les doublons.

## Points d'extension
- Ajuster `EvalWeights` pour rééquilibrer agressivité/défense.
- Modifier profondeurs/noise dans `DifficultyConfig` pour affiner chaque niveau.
- Ajouter une fenêtre de transposition ou un tri de coups pour accélérer le minimax.
- Étendre la sélection de murs (défense proche du joueur, heuristiques de couloir).

## Fichiers de référence
- [include/model/AI.hpp](../include/model/AI.hpp)
- [src/model/AI.cpp](../src/model/AI.cpp)
