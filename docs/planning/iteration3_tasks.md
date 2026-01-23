# Itération 3 : Intelligence Artificielle et Configurabilité
**Dates**: 16 décembre 2025 -- 5 janvier 2026 (3 semaines)
**Équipe**: Javier Tarazona, Tianyi Liang

---

## 🎯 Objectifs de l'itération
- Implémenter IA Minimax/Négamax avec élagage alpha-beta
- Créer une heuristique d'évaluation pertinente
- Intégrer 3 niveaux de difficulté (Facile, Normal, Difficile)
- Ajouter un menu principal et gestion des états de jeu (SceneManager)
- Mode Humain vs IA fonctionnel

---

## 📝 Liste des Tâches (Format GitHub Issues)

### Modèle (IA & Logique)

#### **TASK 23: Infrastructure IA et Bot Aléatoire (Niveau Facile)**
- **Estimation**: 6 heures
- **Description**:
  Créer la classe de base `AI` et implémenter une stratégie simple pour le niveau "Facile". L'IA doit être capable de générer tous les coups valides et d'en choisir un (aléatoirement pour commencer).
- **Critères d'acceptation**:
  - [ ] Infrastructure: Créer la classe `AI` (`src/model/AI.cpp`)
  - [ ] Implémentation: Méthode `getBestMove(Board, State, Difficulty)` retournant un `Move` ou `Wall`
  - [ ] Logique: Générer la liste complète des coups valides (déplacements et murs)
  - [ ] Validation: L'IA instanciée ne fait pas crasher le programme
  - [ ] Validation: Niveau Facile joue des coups aléatoires mais légaux

---

#### **TASK 24: Fonction d'Évaluation Heuristique**
- **Estimation**: 8 heures
- **Description**:
  Implémenter une fonction d'évaluation `evaluate(Board, Player)` pour estimer la qualité d'une position. Cette fonction est cruciale pour le Minimax.
- **Critères d'acceptation**:
  - [ ] Implémentation: Calculer la distance BFS/A* vers l'objectif pour les deux joueurs
  - [ ] Algorithme: Score = (Distance Adversaire - Distance Soi) + Bonus Murs
  - [ ] Validation: Une position gagnante retourne un score maximal
  - [ ] Validation: Avancer vers l'objectif augmente le score

---

#### **TASK 25: Algorithme Minimax avec Alpha-Beta**
- **Estimation**: 12 heures
- **Description**:
  Implémenter l'algorithme Minimax récursif avec élagage Alpha-Beta pour optimiser la recherche de coups.
- **Critères d'acceptation**:
  - [ ] Implémentation: Algorithme Minimax récursif
  - [ ] Optimisation: Intégrer l'élagage Alpha-Beta
  - [ ] Validation: À profondeur 1, l'IA prend une victoire immédiate
  - [ ] Validation: À profondeur 2, l'IA bloque une victoire immédiate de l'adversaire
  - [ ] Fonctionnalité: Les niveaux Moyen (prof 2) et Difficile (prof 4) sont jouables

---

#### **TASK 26: Optimisation Pathfinder (A*)**
- **Estimation**: 4 heures
- **Description**:
  Améliorer le Pathfinder (actuellement BFS) vers A* (A-Star) ou optimiser le BFS existant pour qu'il soit très rapide, car il sera appelé des milliers de fois par l'IA.
- **Critères d'acceptation**:
  - [ ] Implémentation: Algorithme A* utilisant la distance de Manhattan comme heuristique
  - [ ] Validation: A* trouve toujours le chemin le plus court (comparaison avec BFS)
  - [ ] Performance: A* est plus rapide que BFS sur un plateau vide
  - [ ] Intégrité: Vérifier qu'il n'y a aucune régression sur la validation des placements de murs

---

### Contrôleur (Logique de Jeu)

#### **TASK 27: SceneManager (Gestion des États)**
- **Estimation**: 6 heures
- **Description**:
  Introduire un `SceneManager` pour gérer les transitions entre les différents écrans : Menu Principal, Jeu, Fin de Partie.
- **Critères d'acceptation**:
  - [ ] Enum `GameState` (Menu, Playing, GameOver)
  - [ ] Boucle principale modifiée pour déléguer l'update/render à la scène active
  - [ ] Transition fluide entre Menu et Jeu
  - [ ] Possibilité de revenir au Menu depuis le jeu (touche Échap)

---

#### **TASK 28: Intégration Tour de l'IA**
- **Estimation**: 4 heures
- **Description**:
  Intégrer l'IA dans la boucle de jeu `Game`. Lorsque c'est le tour de l'IA, le jeu doit demander un coup à l'IA, attendre (ou calculer), puis appliquer le coup.
- **Critères d'acceptation**:
  - [ ] Détection du tour de l'IA (Player 2 en mode HvIA)
  - [ ] Appel asynchrone ou gestion du temps pour ne pas geler l'UI (optionnel, ou simple "loading")
  - [ ] Application automatique du coup choisi par l'IA
  - [ ] Alternance correcte : Humain -> IA -> Humain

---

#### **TASK 29: Configuration et Difficulté**
- **Estimation**: 2 heures
- **Description**:
  Ajouter une structure pour stocker les paramètres de la partie avant le lancement (Mode de jeu, Difficulté IA).
- **Critères d'acceptation**:
  - [ ] Struct `GameConfig`
  - [ ] Stockage du mode (PvP ou PvE)
  - [ ] Stockage de la difficulté (Easy, Normal, Hard)
  - [ ] Passer cette config à l'initialisation de `Game`

---

### Vue (Interface Graphique)

#### **TASK 30: Menu Principal**
- **Estimation**: 6 heures
- **Description**:
  Créer une vue pour le menu principal permettant de choisir le mode de jeu et de lancer la partie.
- **Critères d'acceptation**:
  - [ ] Affichage du titre "Quoridor"
  - [ ] Bouton "Humain vs Humain"
  - [ ] Bouton "Humain vs IA"
  - [ ] Sélecteur de difficulté (si HvIA sélectionné)
  - [ ] Bouton "Quitter"
  - [ ] Navigation fonctionnelle (clic souris)

#### **TASK 31: Écran de Fin de Partie et Feedback**
- **Estimation**: 4 heures
- **Description**:
  Améliorer l'écran de fin de partie et ajouter un feedback visuel pour les coups de l'IA.
- **Critères d'acceptation**:
  - [ ] Écran "Game Over" avec vainqueur affiché
  - [ ] Bouton "Rejouer" et "Menu Principal"
  - [ ] (Optionnel) Surligner le dernier coup joué par l'IA pour que le joueur comprenne l'action

---

### Tests et Documentation

#### **TASK 32: Tests Unitaires - IA & Heuristiques**
- **Estimation**: 4 heures
- **Description**:
  Écrire des tests unitaires exhaustifs pour valider le comportement de base de l'IA, la génération de coups et la fonction d'évaluation.
- **Critères d'acceptation**:
  - [ ] Fichier `tests/test_ai_core.cpp` créé
  - [ ] **Test**: Génération de coups (tous les coups valides sont listés)
  - [ ] **Test**: Pas de coups invalides (hors limites, murs, collisions)
  - [ ] **Test**: Bot Facile retourne toujours un coup valide
  - [ ] Fichier `tests/test_heuristics.cpp` créé
  - [ ] **Test**: Score(Gagnant) > Score(Neutre) > Score(Perdant)
  - [ ] **Test**: Symétrie (le score est inversement proportionnel pour l'adversaire)
  - [ ] **Test**: Impact des murs sur le score (bonus léger)

#### **TASK 33: Tests Avancés - Minimax & Scénarios**
- **Estimation**: 6 heures
- **Description**:
  Valider l'intelligence du Minimax et le comportement global de l'IA dans des situations complexes via des scénarios prédéfinis.
- **Critères d'acceptation**:
  - [ ] Fichier `tests/test_minimax.cpp` créé
  - [ ] **Test Minimax**: Profondeur 1 trouve victoire immédiate
  - [ ] **Test Minimax**: Profondeur 2 bloque victoire immédiate
  - [ ] **Test Minimax**: Profondeur 4 (6) sont jouables
  - [ ] **Test Performance**: Comparaison Minimax vs Alpha-Beta (nombre de nœuds)
  - [ ] Fichier `tests/test_ai_scenarios.cpp` créé
  - [ ] **Scénario 1 (Défense)**: L'IA pose un mur critique pour sauver la partie
  - [ ] **Scénario 2 (Sûreté)**: L'IA ne s'enferme pas elle-même
  - [ ] **Scénario 3 (Efficacité)**: L'IA choisit le chemin le plus court en l'absence d'obstacles

#### **TASK 34: Documentation Itération 3**
- **Estimation**: 2 heures
- **Description**:
  Mettre à jour la documentation pour inclure les détails sur l'IA et le guide d'utilisation des menus.
- **Critères d'acceptation**:
  - [ ] Mise à jour `README.md` (nouvelles fonctionnalités IA)
  - [ ] Mise à jour `docs/USER_GUIDE.md` (menus, niveaux de difficulté)
  - [ ] Documentation technique de l'algorithme IA (Architecture, Heuristique)

---

## 📊 Résumé de l'effort

| Membre de l'équipe | Heures Estimées | Tâches (Globalement) |
|--------------------|-----------------|------------------|
| **Tianyi** (IA Core)| ~34 heures      | IA, Heuristiques, Minimax, Pathfinder, Tests |
| **Javier** (UI/UX) | ~24 heures      | SceneManager, Intégration, Menus, Config, Docs |
| **Total**          | ~58 heures      | |
