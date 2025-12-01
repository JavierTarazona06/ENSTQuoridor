# Rapport des modifications Modèle (T2.1 & T2.2)

Ce document détaille l'état d'avancement des tâches T2.1 et T2.2 de l'Itération 2, concernant la gestion et la validation des murs dans le modèle.

## T2.1 - Gestion des murs dans `Board`

**Statut : Terminée et Testée**

Cette tâche consistait à étendre la classe `Board` pour supporter le stockage et la gestion basique des murs.

### Implémentation (`include/model/Board.hpp`) :
- **Structures de données** :
  - `enum class Orientation { Horizontal, Vertical }`
  - `struct Wall { Position pos; Orientation orientation; }`
- **Attributs** :
  - `std::vector<Wall> placedWalls_` : Stockage dynamique des murs.
  - `std::array<int, NUM_PLAYERS> wallsRemaining_` : Compteur décrémental (10 murs par défaut).
- **Méthodes implémentées** :
  - `placeWall(wall, playerIndex)` : Ajout avec validation défensive (limites, chevauchements, compteur).
  - `getWallsRemaining(playerIndex)` : Accesseur du compteur.
  - `isWallAt(pos, orientation)` : Vérification d'existence.
  - `getWalls()` : Accesseur pour la Vue.

### Tests (`tests/test_board.cpp`) :
Les tests couvrent :
- Le placement valide (horizontal/vertical).
- La détection de chevauchement (exact et partiel).
- La limite de 10 murs par joueur.
- Les limites du plateau (exceptions levées).

---

## T2.2 - Règles de validation des murs (`Rules`)

**Statut : Terminée et Testée**

Cette tâche visait à implémenter la logique métier complète pour valider un coup "poser un mur" avant de l'appliquer.

### Implémentation (`include/model/Rules.hpp`) :
- **Méthode principale** :
  - `static bool isValidWallPlacement(const Board& board, const Wall& wall, int playerIndex)`
  
### Logique de validation :
1. **Limites (0-7)** : Vérifie que le mur est dans la grille des inter-cases.
2. **Disponibilité** : Vérifie `board.getWallsRemaining() > 0`.
3. **Chevauchements** :
   - Vérifie qu'aucun mur n'existe déjà à la position donnée (collision).
   - Vérifie qu'aucun mur perpendiculaire ne croise le nouveau mur (croisement).
   - Vérifie qu'aucun mur de même orientation ne chevauche partiellement (recouvrement).
4. **Anti-blocage (Pathfinding)** :
   - Vérifie que **chaque joueur** conserve au moins un chemin vers sa ligne d'arrivée.
   - **Note technique** : Pour satisfaire cette exigence de T2.2, un algorithme BFS (`pathExists`) a été implémenté directement dans `Rules.cpp`. Cela anticipe partiellement la tâche **T2.3 (Pathfinder)**, mais garantit que la validation est fonctionnelle dès maintenant. Une refactorisation vers une classe `Pathfinder` dédiée reste possible pour T2.3.

### Tests (`tests/test_rules.cpp`) :
Une suite de tests complète a été ajoutée pour vérifier :
- Tous les cas de validité (placement libre).
- Tous les cas d'invalidité (hors limites, chevauchements variés).
- Le cas critique d'enfermement d'un joueur (le placement doit être refusé).

---

## Guide d'intégration pour l'équipe

### Vue (`Render2D`)
- Utilisez `board.getWalls()` pour récupérer la liste des murs à dessiner.
- Chaque mur a une `pos` (top-left) et une `orientation`.
- Dessinez les murs Horizontaux entre `(x, y)` et `(x+2, y)`.
- Dessinez les murs Verticaux entre `(x, y)` et `(x, y+2)`.

### Contrôleur (`InputHandler` / `Server`)
Lorsqu'un joueur tente de poser un mur :
1. **Pré-validation** : Appelez `Rules::isValidWallPlacement(board, wall, player)`.
2. **Action** :
   - Si `true` : Appelez `board.placeWall(wall, player)`. Le coup est valide, passez au tour suivant.
   - Si `false` : Refusez le coup, affichez un message d'erreur (ex: "Mur invalide" ou "Bloque le chemin"), et ne changez pas de tour.
