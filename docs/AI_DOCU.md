# Documentation Technique de l'Intelligence Artificielle

Ce document décrit l'architecture et l'implémentation de l'algorithme IA du jeu Quoridor.

---

## 📋 Vue d'ensemble

| Aspect | Détail |
|--------|--------|
| **Module** | `model/AI.{hpp,cpp}` |
| **Entrées** | `Board` (état du plateau), `State` (joueur courant) |
| **Sortie** | `Move` (déplacement de pion ou pose de mur) |
| **Algorithme** | Minimax avec élagage Alpha-Bêta |
| **Particularité** | Injection de bruit pour simuler des erreurs |

---

## 🎯 Niveaux de difficulté

L'IA propose quatre niveaux de difficulté, contrôlés par deux paramètres :
- **Profondeur** : Nombre de coups analysés à l'avance
- **Bruit** : Randomisation ajoutée aux scores d'évaluation

### Configuration (`DifficultyConfig` dans `AI.cpp`)

```cpp
namespace DifficultyConfig {
    constexpr int EASY_DEPTH = 1;     // Profondeur 1
    constexpr int NORMAL_DEPTH = 1;   // Profondeur 1
    constexpr int HARD_DEPTH = 2;     // Profondeur 2
    constexpr int HELL_DEPTH = 4;     // Profondeur 4

    constexpr int EASY_NOISE = 40;    // Bruit élevé
    constexpr int NORMAL_NOISE = 8;   // Bruit modéré
    constexpr int HARD_NOISE = 3;     // Bruit faible
    constexpr int HELL_NOISE = 0;     // Aucun bruit
}
```

### Caractéristiques par niveau

| Niveau | Profondeur | Bruit | Taux d'erreur | Temps/coup | Comportement |
|--------|------------|-------|---------------|------------|--------------|
| **Facile** | 1 | ±40 | ~50% | ~150μs | Voit les menaces immédiates mais choisit souvent au hasard |
| **Normal** | 1 | ±8 | ~15% | ~150μs | Voit les menaces immédiates, erreurs occasionnelles |
| **Difficile** | 2 | ±3 | ~5% | ~800μs | Planifie à l'avance, erreurs rares |
| **Enfer** | 4 | 0 | 0% | ~10ms | Calcul profond, jeu parfait |

---

## 🔄 Boucle de décision

```
┌─────────────────────────────────────────────────────────────┐
│                    getBestMove()                            │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  1. Récupérer le joueur courant via State                  │
│                     ↓                                       │
│  2. Générer tous les coups légaux (getAllValidMoves)       │
│     ├── Déplacements de pion (5x5 autour du pion)          │
│     └── Placements de murs (stratégiquement filtrés)       │
│                     ↓                                       │
│  3. Pour chaque coup :                                      │
│     ├── Simuler sur une copie du Board                     │
│     ├── Appeler minimax(profondeur, α, β, ...)            │
│     └── Conserver le meilleur score                        │
│                     ↓                                       │
│  4. Retourner le coup avec le score maximal                │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### Pseudo-code

```cpp
Move AI::getBestMove(Board& board, State& state, Difficulty difficulty) {
    int player = state.getCurrentPlayer();
    int depth = getDepthForDifficulty(difficulty);
    int noise = getNoiseForDifficulty(difficulty);
    
    int bestScore = -∞;
    Move bestMove;
    
    for (Move move : getAllValidMoves(board, player)) {
        Board nextBoard = board.copy();
        applyMove(nextBoard, move, player);
        
        int score = minimax(nextBoard, depth-1, -∞, +∞, false, player, noise);
        
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    
    return bestMove;
}
```

---

## 🌳 Algorithme Minimax avec Alpha-Bêta

### Principe

Le Minimax explore l'arbre des possibilités en alternant entre :
- **Maximisation** : L'IA choisit le coup qui maximise son score
- **Minimisation** : L'adversaire choisit le coup qui minimise le score de l'IA

L'élagage Alpha-Bêta optimise la recherche en coupant les branches inutiles.

### Implémentation

```cpp
int AI::minimax(Board board, int depth, int α, int β, 
                bool maximizing, int playerIndex, int noise) {
    
    // Cas de base : feuille de l'arbre
    if (depth == 0) {
        return evaluateWithNoise(board, playerIndex, noise);
    }
    
    // Vérifier l'état terminal
    int eval = evaluate(board, playerIndex);
    if (eval == INT_MAX || eval == INT_MIN) {
        return eval;  // Victoire/défaite
    }
    
    int currentPlayer = maximizing ? playerIndex : opponent(playerIndex);
    vector<Move> moves = getAllValidMoves(board, currentPlayer);
    
    if (maximizing) {
        int maxEval = INT_MIN;
        for (Move move : moves) {
            Board next = applyMove(board, move);
            int eval = minimax(next, depth-1, α, β, false, playerIndex, noise);
            maxEval = max(maxEval, eval);
            α = max(α, eval);
            if (β <= α) break;  // Élagage bêta
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (Move move : moves) {
            Board next = applyMove(board, move);
            int eval = minimax(next, depth-1, α, β, true, playerIndex, noise);
            minEval = min(minEval, eval);
            β = min(β, eval);
            if (β <= α) break;  // Élagage alpha
        }
        return minEval;
    }
}
```

### Visualisation de l'élagage

```
                    MAX (IA)
                   /    \
                 /        \
              MIN          MIN
             / | \        / | \
            5  3  ?      2  ?  ?
                  ↑          ↑
               Élagué     Élagué
               (α=5)      (β=2)
```

---

## 📊 Fonction d'évaluation

La fonction `evaluate()` évalue la qualité d'une position pour un joueur. Elle combine cinq facteurs pondérés.

### Poids d'évaluation (`EvalWeights`)

```cpp
namespace EvalWeights {
    constexpr int DISTANCE_DIFF = 10;      // Différence de distance
    constexpr int WALL_ADVANTAGE = 2;      // Avantage en murs
    constexpr int MOBILITY = 3;            // Liberté de mouvement
    constexpr int ENDGAME_BONUS = 15;      // Bonus fin de partie
    constexpr int WALL_EFFECTIVENESS = 5;  // Efficacité des murs
}
```

### Facteurs détaillés

#### 1. Différence de distance (Poids: ×10)
```cpp
score += (dist_adversaire - dist_moi) * DISTANCE_DIFF;
```
- Plus je suis proche de mon objectif par rapport à l'adversaire, mieux c'est
- Calculé via `Pathfinder::getShortestPathDistance` (BFS)

#### 2. Bonus fin de partie (Poids: ×15, non-linéaire)
```cpp
if (dist_moi <= 3) {
    score += (4 - dist_moi)² * ENDGAME_BONUS;
}
if (dist_adversaire <= 3) {
    score -= (4 - dist_adversaire)² * ENDGAME_BONUS;
}
```
- Récompense exponentielle quand proche de la victoire
- Pénalité exponentielle quand l'adversaire est proche

#### 3. Avantage en murs (Poids: ×2)
```cpp
score += (mes_murs - murs_adversaire) * WALL_ADVANTAGE;
```
- Avoir plus de murs = plus de flexibilité stratégique

#### 4. Mobilité (Poids: ×3)
```cpp
score += (ma_mobilité - mobilité_adversaire) * MOBILITY;
```
- Nombre de mouvements valides disponibles
- Plus d'options = meilleure flexibilité tactique

#### 5. Efficacité des murs (Poids: ×5)
```cpp
// Combien mes murs ont allongé le chemin adverse
wall_effectiveness = dist_adversaire - distance_base_adversaire;
score += wall_effectiveness * WALL_EFFECTIVENESS;

// Pénalité si les murs adverses m'ont ralenti
score -= my_wall_penalty * WALL_EFFECTIVENESS;
```

### États terminaux

| État | Score |
|------|-------|
| Victoire IA | `INT_MAX` (+∞) |
| Défaite IA | `INT_MIN` (-∞) |
| Chemin IA bloqué | `INT_MIN + 1` |
| Chemin adversaire bloqué | `INT_MAX - 1` |

---

## 🎲 Système de bruit

Le bruit permet de créer des niveaux de difficulté battables en simulant des "erreurs" humaines.

### Principe

```cpp
int evaluateWithNoise(Board& board, int player, int noiseLevel) {
    int baseScore = evaluate(board, player);
    
    // Ne pas ajouter de bruit aux états terminaux
    if (isTerminal(baseScore)) return baseScore;
    
    // Ajouter bruit uniforme dans [-noise, +noise]
    if (noiseLevel > 0) {
        uniform_int_distribution<> dist(-noiseLevel, noiseLevel);
        baseScore += dist(generator);
    }
    
    return baseScore;
}
```

### Calcul de probabilité d'erreur

Soient deux coups A (optimal, score $S_A$) et B (sous-optimal, score $S_B$) avec $\Delta = S_A - S_B$.

Après injection de bruit $N \in [-n, +n]$ :
- Score A devient : $S_A + N_A$
- Score B devient : $S_B + N_B$

L'IA choisit B si : $S_B + N_B > S_A + N_A$, soit $N_B - N_A > \Delta$

La différence $X = N_B - N_A$ suit une **distribution triangulaire** sur $[-2n, +2n]$.

**Probabilité de choisir le coup sous-optimal :**

$$P(X > \Delta) = \frac{(2n - \Delta)^2}{8n^2} \quad \text{pour } 0 \leq \Delta \leq 2n$$

### Exemples pratiques

#### Facile (noise = 40)
| Différence Δ | Probabilité d'erreur |
|--------------|---------------------|
| 0 | 50% |
| 10 | 32% |
| 20 | 18% |
| 40 | 2% |
| 80 | 0% |

#### Normal (noise = 8)
| Différence Δ | Probabilité d'erreur |
|--------------|---------------------|
| 0 | 50% |
| 5 | 24% |
| 10 | 7% |
| 16 | 0% |

---

## 🧩 Génération des coups

### Mouvements de pion

```cpp
vector<Position> getValidPawnMoves(Board& board, int player) {
    Position pos = board.getPawnPosition(player);
    vector<Position> moves;
    
    // Explorer zone 5x5 autour du pion (couvre les sauts)
    for (int r = pos.y - 2; r <= pos.y + 2; r++) {
        for (int c = pos.x - 2; c <= pos.x + 2; c++) {
            if (Rules::isValidMove(board, player, pos.y, pos.x, r, c)) {
                moves.push_back({c, r});
            }
        }
    }
    return moves;
}
```

### Placements de murs (optimisé)

Pour éviter d'évaluer tous les 128 emplacements possibles, l'IA filtre stratégiquement :

```cpp
vector<Wall> getRelevantWalls(Board& board, int player) {
    set<Wall> walls;
    int opponent = 1 - player;
    
    // 1. Murs bloquant le plus court chemin adverse
    vector<Position> path = Pathfinder::getShortestPath(board, opponent);
    for (auto& step : path) {
        addBlockingWalls(walls, step);
    }
    
    // 2. Murs autour de l'adversaire (box strategy)
    Position oppPos = board.getPawnPosition(opponent);
    for (int r = oppPos.y - 1; r <= oppPos.y; r++) {
        for (int c = oppPos.x - 1; c <= oppPos.x; c++) {
            walls.insert({{c, r}, Horizontal});
            walls.insert({{c, r}, Vertical});
        }
    }
    
    // 3. Filtrer par validité
    vector<Wall> valid;
    for (auto& w : walls) {
        if (Rules::isValidWallPlacement(board, w, player)) {
            valid.push_back(w);
        }
    }
    return valid;
}
```

---

## 📈 Complexité et performances

### Complexité temporelle

| Composant | Complexité |
|-----------|------------|
| Évaluation | O(V + E) pour BFS, ~O(81) pour le plateau |
| Génération de coups | O(25) pion + O(~20) murs filtrés |
| Minimax (sans élagage) | O(b^d) où b ≈ 30, d = profondeur |
| Minimax (avec α-β) | O(b^(d/2)) dans le meilleur cas |

### Temps de réponse mesurés

| Profondeur | Branching factor | Temps typique |
|------------|------------------|---------------|
| 1 | ~30 | ~150μs |
| 2 | ~30 | ~800μs |
| 4 | ~30 | ~8-10ms |

---

## 🔧 Points d'extension

### Améliorations possibles

1. **Table de transposition**
   - Mémoriser les positions déjà évaluées
   - Éviter les calculs redondants

2. **Tri des coups**
   - Évaluer d'abord les coups prometteurs
   - Améliorer l'efficacité de l'élagage α-β

3. **Iterative deepening**
   - Approfondir progressivement la recherche
   - Permettre une réponse en temps limité

4. **Ouvertures pré-calculées**
   - Stocker les meilleurs coups pour le début de partie
   - Réponse instantanée

### Paramètres ajustables

```cpp
// Dans AI.cpp

// Modifier les poids d'évaluation
namespace EvalWeights {
    DISTANCE_DIFF = 10;    // ↑ pour prioriser l'avancement
    WALL_ADVANTAGE = 2;    // ↑ pour préserver les murs
    MOBILITY = 3;          // ↑ pour favoriser la flexibilité
    ENDGAME_BONUS = 15;    // ↑ pour accélérer les fins de partie
    WALL_EFFECTIVENESS = 5; // ↑ pour encourager les murs efficaces
}

// Modifier les niveaux de difficulté
namespace DifficultyConfig {
    EASY_DEPTH = 1;    // ↑ pour un Easy plus intelligent
    EASY_NOISE = 40;   // ↓ pour un Easy moins aléatoire
    // etc.
}
```

---

## 📁 Fichiers de référence

| Fichier | Description |
|---------|-------------|
| [`include/model/AI.hpp`](../include/model/AI.hpp) | Déclarations de l'interface IA |
| [`src/model/AI.cpp`](../src/model/AI.cpp) | Implémentation complète |
| [`tests/test_ai_core.cpp`](../tests/test_ai_core.cpp) | Tests de l'infrastructure IA |
| [`tests/test_minimax.cpp`](../tests/test_minimax.cpp) | Tests du minimax et heuristiques |
| [`tests/test_heuristics.cpp`](../tests/test_heuristics.cpp) | Tests de la fonction d'évaluation |

---

**Dernière mise à jour** : 25 janvier 2026
