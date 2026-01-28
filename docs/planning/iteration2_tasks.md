# Itération 2 : Règles complètes et placement de murs
**Dates**: 2 décembre -- 15 décembre 2025 (2 semaines)

## Objectifs de l'itération
- Implémenter règles complètes de déplacement (sauts par-dessus adversaire)
- Placement de murs avec validation (chevauchement, limites)
- Vérification chemin restant (BFS ou A*)
- Condition de victoire
- Interface améliorée (affichage murs, HUD basique)

---

## Tâches - Modèle

### T2.1 - Gestion des murs dans Board
**Estimation**: 4 heures

**Description**:
Étendre la classe `Board` pour gérer les murs horizontaux et verticaux. Implémenter le stockage des murs placés et maintenir le compteur de murs restants par joueur (10 murs par joueur selon les règles standard).

**Critères d'acceptation**:
- [ ] Structure de données pour stocker murs horizontaux et verticaux (vecteurs ou grille)
- [ ] Méthode `placeWall(x, y, orientation, playerIndex)` avec validation de base
- [ ] Méthode `getWallsRemaining(playerIndex)` retourne le nombre de murs disponibles
- [ ] Méthode `isWallAt(x, y, orientation)` pour vérifier présence d'un mur
- [ ] Tests unitaires pour placement et récupération de murs
- [ ] Initialisation: chaque joueur commence avec 10 murs

---

### T2.2 - Règles de validation des murs
**Estimation**: 6 heures

**Description**:
Implémenter dans `Rules` la validation complète du placement de murs : vérifier les limites du plateau, détecter les chevauchements avec murs existants, et s'assurer qu'un mur ne bloque pas complètement un joueur.

**Critères d'acceptation**:
- [ ] Méthode `Rules::isValidWallPlacement(board, x, y, orientation, playerIndex)` implémentée
- [ ] Validation: coordonnées dans les limites du plateau (0-8 pour position, orientation correcte)
- [ ] Validation: pas de chevauchement avec murs existants (même position/orientation ou croisement)
- [ ] Validation: joueur a encore des murs disponibles
- [ ] Validation: mur ne crée pas de blocage complet (voir T2.3)
- [ ] Tests unitaires couvrant cas valides et invalides (≥10 tests)
- [ ] Tests cas limites: coins, bords du plateau

---

### T2.3 - Pathfinder avec BFS
**Estimation**: 8 heures

**Description**:
Créer une classe `Pathfinder` utilisant l'algorithme BFS (Breadth-First Search) pour vérifier qu'un joueur peut toujours atteindre sa ligne d'arrivée. Cette vérification doit être appelée avant de valider définitivement un placement de mur.

**Critères d'acceptation**:
- [ ] Classe `Pathfinder` créée dans `src/model/Pathfinder.cpp`
- [ ] Méthode `hasPathToGoal(board, playerIndex, hypotheticalWall)` implémentée
- [ ] BFS explore les cases adjacentes accessibles (pas de murs bloquants)
- [ ] Retourne `true` si au moins un chemin existe vers la rangée d'arrivée
- [ ] Gestion des murs hypothétiques pour validation avant placement
- [ ] Tests unitaires: cas avec chemin unique, plusieurs chemins, pas de chemin
- [ ] Performance acceptable: vérification en <10ms pour plateau 9×9

---

### T2.4 - Règles de déplacement avec sauts
**Estimation**: 6 hours

**Description**:
Étendre `Rules::isValidMove()` pour supporter les sauts par-dessus l'adversaire selon les règles de Quoridor : saut simple si case libre derrière, sauts diagonaux si mur bloque le saut droit.

**Critères d'acceptation**:
- [ ] Déplacement simple orthogonal (déjà implémenté en Itération 1)
- [ ] Saut droit: si adversaire sur case adjacente et case derrière libre
- [ ] Saut diagonal: si adversaire adjacent et mur bloque saut droit, deux options diagonales possibles
- [ ] Validation: destination dans limites plateau et non occupée
- [ ] Validation: pas de mur bloquant entre cases
- [ ] Tests unitaires: ≥8 scénarios (saut simple, diagonal gauche/droite, cas invalides)
- [ ] Documentation des règles de saut en commentaires

---

### T2.5 - Condition de victoire
**Estimation**: 2 heures

**Description**:
Implémenter la détection de victoire dans `State` ou `Rules`. Un joueur gagne lorsqu'il atteint la rangée opposée (y=8 pour joueur 0, y=0 pour joueur 1).

**Critères d'acceptation**:
- [ ] Méthode `Rules::checkVictory(board, playerIndex)` ou équivalent dans `State`
- [ ] Retourne `true` si le pion du joueur est sur sa ligne d'arrivée
- [ ] `State::setGameStatus()` appelée avec `Player1Won` ou `Player2Won`
- [ ] Tests unitaires: victoire joueur 0, victoire joueur 1, pas de victoire
- [ ] Intégration dans la boucle de jeu pour arrêter la partie

---

## Tâches - Vue

### T2.6 - Rendu des murs
**Estimation**: 5 heures

**Description**:
Ajouter à `Render2D` la capacité de dessiner les murs horizontaux et verticaux sous forme de rectangles colorés. Les murs doivent être visuellement distincts de la grille et des pions.

**Critères d'acceptation**:
- [ ] Méthode `Render2D::drawWalls(board)` implémentée
- [ ] Murs horizontaux: rectangles ~3px hauteur, largeur = CELL_SIZE
- [ ] Murs verticaux: rectangles largeur ~3px, hauteur = CELL_SIZE
- [ ] Couleur distincte (ex: brun/marron ou noir épais)
- [ ] Position correcte entre les cases selon coordonnées grille
- [ ] Tests visuels: murs affichés correctement aux positions attendues
- [ ] Pas de chevauchement visuel avec grille/pions

---

### T2.7 - HUD (Heads-Up Display)
**Estimation**: 4 heures

**Description**:
Créer une zone d'affichage d'informations en haut ou à côté du plateau montrant : joueur actif, nombre de murs restants par joueur, et messages d'état (victoire, coup invalide).

**Critères d'acceptation**:
- [ ] Zone HUD définie (position fixe hors grille de jeu)
- [ ] Texte "Joueur 1" ou "Joueur 2" avec indicateur visuel (couleur pion)
- [ ] Affichage "Murs restants: X" pour le joueur actif
- [ ] Méthode `Render2D::drawHUD(state)` implémentée
- [ ] Utilise fonts lisibles (SFML sf::Text)
- [ ] Tests visuels: HUD se met à jour correctement après chaque coup

---

### T2.8 - Messages d'erreur
**Estimation**: 3 heures

**Description**:
Afficher des messages temporaires à l'écran lorsqu'un coup invalide est tenté (déplacement impossible, mur invalide). Le message doit être clair et disparaître après quelques secondes.

**Critères d'acceptation**:
- [ ] Méthode `Render2D::showMessage(text, duration, color)` implémentée
- [ ] Message affiché dans zone visible (centre écran ou en haut)
- [ ] Couleur rouge pour erreurs, verte pour confirmations
- [ ] Timer pour faire disparaître le message après 2-3 secondes
- [ ] Exemples messages: "Déplacement invalide", "Mur bloque le chemin", "Plus de murs disponibles"
- [ ] Tests: messages s'affichent et disparaissent correctement

---

## Tâches - Contrôleur

### T2.9 - Mode placement de murs
**Estimation**: 6 heures

**Description**:
Étendre `InputHandler` pour supporter le placement de murs. L'utilisateur doit pouvoir sélectionner une position (clic souris), puis choisir l'orientation (touche clavier ou second clic).

**Critères d'acceptation**:
- [ ] Touche 'W' (ou bouton UI) active le mode "placement mur"
- [ ] Touche 'M' retourne au mode "déplacement pion"
- [ ] En mode mur: clic sur une position montre aperçu mur horizontal
- [ ] Touche 'R' ou molette souris change orientation (horizontal ↔ vertical)
- [ ] Clic valide le placement (appel `Rules::isValidWallPlacement`)
- [ ] Si invalide: affiche message erreur (via T2.8)
- [ ] Si valide: mur placé, murs restants décrémenté, tour passe
- [ ] Indicateur visuel mode actif (curseur différent ou texte HUD)

---

### T2.10 - Intégration partie complète HvH
**Estimation**: 4 heures

**Description**:
Connecter tous les composants (déplacement, sauts, murs, victoire) dans le contrôleur `Server` pour permettre une partie Humain vs Humain complète du début à la fin.

**Critères d'acceptation**:
- [ ] Boucle de jeu alterne joueurs après chaque coup valide
- [ ] Détection de victoire arrête la partie et affiche message
- [ ] Coups invalides ne changent pas le tour
- [ ] Partie jouable de A à Z sans crash
- [ ] Tests manuels: compléter 2-3 parties jusqu'à victoire
- [ ] Réinitialisation possible (touche 'N' pour nouvelle partie)

---

## Tâches - Tests

### T2.11 - Tests de placement de murs
**Estimation**: 3 heures

**Description**:
Écrire des tests unitaires exhaustifs pour la validation des murs : cas valides, chevauchements, limites du plateau, blocages de chemin.

**Critères d'acceptation**:
- [ ] Fichier `test_walls.cpp` créé (ou extension de `test_rules.cpp`)
- [ ] ≥12 tests couvrant :
  - Placement valide mur horizontal/vertical
  - Chevauchement avec mur existant
  - Hors limites plateau
  - Pas assez de murs restants
  - Mur bloquant chemin (doit échouer)
- [ ] Tous les tests passent
- [ ] Couverture code ≥85% pour `Rules::isValidWallPlacement`

---

### T2.12 - Tests de pathfinding
**Estimation**: 3 heures

**Description**:
Tester `Pathfinder` avec des configurations variées : chemins multiples, chemin unique, cul-de-sac, murs formant labyrinthe.

**Critères d'acceptation**:
- [ ] Fichier `test_pathfinder.cpp` créé
- [ ] ≥8 tests couvrant :
  - Plateau vide (toujours chemin)
  - 1 seul chemin possible
  - Plusieurs chemins (vérifier qu'un est trouvé)
  - Pas de chemin (totalement bloqué)
  - Murs en labyrinthe complexe
- [ ] Tests de performance: <10ms par vérification
- [ ] Tous les tests passent

---

### T2.13 - Tests de sauts
**Estimation**: 2 heures

**Description**:
Tester les règles de saut (droit et diagonal) avec différentes configurations de pions et murs.

**Critères d'acceptation**:
- [ ] Fichier `test_jumps.cpp` créé (ou extension de `test_rules.cpp`)
- [ ] ≥6 tests :
  - Saut droit valide (adversaire devant, case libre derrière)
  - Saut diagonal valide (mur bloque saut droit)
  - Saut invalide (pas d'adversaire adjacent)
  - Saut invalide (case destination occupée)
  - Saut diagonal invalide (pas de mur bloquant)
- [ ] Tous les tests passent

---

## Tâches - Documentation

### T2.14 - Documentation règles complètes
**Estimation**: 2 heures

**Description**:
Mettre à jour le README et créer un document décrivant les règles implémentées (déplacements, sauts, murs, victoire) avec exemples visuels si possible.

**Critères d'acceptation**:
- [ ] Section "Règles du jeu" dans README.md
- [ ] Description placement murs (orientation, limites)
- [ ] Description sauts (conditions, types)
- [ ] Diagramme ou ASCII art montrant exemples (optionnel)
- [ ] Relecture par équipe, aucune ambiguïté

---

### T2.15 - Mise à jour guide utilisateur
**Estimation**: 1 heure

**Description**:
Ajouter au guide utilisateur les instructions pour placer des murs, changer de mode, et comprendre les messages d'erreur.

**Critères d'acceptation**:
- [ ] Section "Placement de murs" dans `docs/installation/game.md`
- [ ] Touches/commandes documentées (W, M, R, etc.)
- [ ] Exemples de messages d'erreur et signification
- [ ] Captures d'écran ou description visuelle HUD

---

## Résumé de l'effort

| Catégorie | Nombre de tâches | Temps total estimé |
|-----------|------------------|-------------------|
| Modèle | 5 | 26 heures |
| Vue | 3 | 12 heures |
| Contrôleur | 2 | 10 heures |
| Tests | 3 | 8 heures |
| Documentation | 2 | 3 heures |
| **TOTAL** | **15** | **59 heures** |

**Répartition suggérée** (2 personnes, 2 semaines):
- Semaine 1: T2.1-T2.7 (focus Modèle + Vue de base)
- Semaine 2: T2.8-T2.15 (Contrôleur, Tests, Documentation, intégration)

---

## Critères de succès de l'itération

Partie jouable de A à Z en mode local (2 humains)
Victoire détectée et affichée
Tous les cas de règles validés par tests unitaires (>20 tests)
Aucun placement de mur ne peut bloquer définitivement un joueur
Interface claire avec HUD et messages d'erreur

---

## Risques identifiés

**Complexité validation murs + pathfinding**  
→ Mitigation: Implémenter BFS simple d'abord; tester cas par cas; prévoir debug intensif

**Performance pathfinding en temps réel**  
→ Mitigation: Optimiser BFS (early exit), limiter recherche à zone pertinente

**Intégration UI/UX (mode placement murs)**  
→ Mitigation: Prototyper rapidement, tester avec utilisateur réel, itérer sur feedback
