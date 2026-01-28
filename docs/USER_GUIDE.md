# Guide Utilisateur Quoridor

Ce guide explique les contrôles clavier et souris pour la version actuelle du jeu Quoridor.

---

## Vue d'ensemble

**Quoridor** est un jeu de stratégie à deux joueurs sur un plateau 9×9. Chaque joueur contrôle un pion et dispose de 10 murs. L'objectif est d'atteindre la rangée opposée avant l'adversaire, tout en respectant la règle fondamentale : toujours laisser au moins un chemin valide pour chaque joueur.

---

## Contrôles de base

| Entrée | Utilisation |
|--------|-------------|
| **Souris** | Sélection de pions, déplacements, placement de murs |
| **Clavier** | Changement de mode, contrôle du jeu, sélection de difficulté |

---

## Modes de jeu

| Mode | Description |
|------|-------------|
| **Humain vs Humain** | Multijoueur local sur la même machine |
| **Humain vs IA** | Affrontez l'ordinateur à quatre niveaux de difficulté |

### Niveaux de difficulté IA

| Niveau | Profondeur | Bruit | Comportement |
|--------|------------|-------|--------------|
| **Facile** (1) | 1 | ±40 | ~50% coups sous-optimaux, idéal pour apprendre |
| **Normal** (2) | 1 | ±8 | ~15% erreurs, battable par la plupart des joueurs |
| **Difficile** (3) | 2 | ±3 | ~5% erreurs, nécessite une planification soignée |
| **Enfer** (4) | 4 | 0 | Jeu parfait, défi niveau expert |

---

## Contrôles du menu

| Touche | Action |
|--------|--------|
| `H` | Mode Humain vs Humain |
| `A` | Mode Humain vs IA |
| `1` / `2` / `3` / `4` | Difficulté : Facile / Normal / Difficile / Enfer |
| `Entrée` | Démarrer la partie |
| `Échap` | Quitter l'application |

---

## Interactions détaillées

### 1. Mode déplacement de pion (par défaut)

Le jeu démarre dans ce mode.

#### Sélectionner un pion
1. Survolez votre pion avec la souris
2. **Clic gauche** pour le sélectionner
3. Le pion sélectionné sera mis en surbrillance

#### Déplacer le pion
1. Une fois le pion sélectionné, survolez la case de destination souhaitée
2. **Clic gauche** sur la case
3. Si le mouvement est valide :
   - Le pion se déplace vers cette position
   - Le tour se termine
   - Le jeu passe au joueur suivant
4. Si le mouvement est invalide :
   - Le pion ne bouge pas
   - Un message d'erreur s'affiche

#### Désélectionner
- **Clic gauche** à nouveau sur le pion actuellement sélectionné pour le désélectionner

#### Mouvements valides
- **Orthogonal** : Une case vers le haut, bas, gauche ou droite
- **Saut direct** : Par-dessus un adversaire adjacent (si la case derrière est libre)
- **Saut diagonal** : Si le saut direct est bloqué par un mur ou le bord du plateau



---

### 2. Mode placement de mur

Dans ce mode, vous pouvez placer des obstacles sur le plateau.

#### Entrer dans le mode
- Appuyez sur la touche **`W`** de votre clavier
- Un message confirme le changement de mode
- Un aperçu semi-transparent du mur suit votre curseur

#### Aperçu du mur
- Déplacez votre souris sur le plateau
- L'aperçu du mur se positionne automatiquement aux intersections
- La couleur indique si le placement est valide (vert) ou non (rouge)

#### Tourner le mur
- Appuyez sur la touche **`O`**
- Bascule l'orientation entre **Horizontal** et **Vertical**



#### Confirmer le placement
1. Positionnez la souris à l'endroit souhaité
2. **Clic gauche** pour confirmer
3. Si le placement est valide :
   - Le mur est placé
   - Un mur est déduit de votre inventaire
   - Le tour se termine
   - Le jeu **retourne automatiquement en mode déplacement**
4. Si le placement est invalide :
   - Le mur n'est pas placé
   - Message : "Position de mur invalide !"

#### Quitter le mode
- Appuyez à nouveau sur **`W`** pour retourner manuellement au mode déplacement

#### Règles de placement
- Ne peut pas chevaucher un mur existant
- Ne peut pas croiser un mur existant (forme en +)
- Ne peut pas bloquer complètement le chemin d'un joueur
- Doit toujours laisser au moins un chemin vers l'objectif pour chaque joueur

---

### 3. Contrôle du jeu

| Touche | Action | Quand |
|--------|--------|-------|
| `R` | Réinitialiser la partie | À tout moment |
| `1`-`4` | Changer difficulté IA | En mode Humain vs IA |
| `Échap` | Retourner au menu | À tout moment |

---

## Conditions de victoire

| Joueur | Position de départ | Objectif |
|--------|-------------------|----------|
 | **Joueur 1** (Blanc) | Rangée du haut (y=0) | Atteindre la rangée du bas (y=8) |
| **Joueur 2** (Noir) | Rangée du bas (y=8) | Atteindre la rangée du haut (y=0) |

Lorsqu'un joueur atteint sa rangée objectif :
1. Un message de victoire s'affiche
2. Le jeu se termine
3. Appuyez sur `R` pour recommencer ou `Échap` pour le menu

---

## Conseils stratégiques

### Débutants (Facile/Normal)
- Concentrez-vous sur l'avancement de votre pion
- N'utilisez pas tous vos murs trop tôt
- Observez le chemin le plus court de l'adversaire

### Intermédiaires (Difficile)
- Placez des murs pour allonger le chemin adverse
- Gardez des murs pour la fin de partie
- Contrôlez le centre du plateau

### Experts (Enfer)
- Anticipez plusieurs coups à l'avance
- Créez des « corridors » pour forcer l'adversaire
- Équilibrez attaque (avancement) et défense (murs)

---

## Dépannage

| Problème | Solution |
|----------|----------|
| Le pion ne bouge pas | Vérifiez qu'il n'y a pas de mur ou d'adversaire bloquant |
| Impossible de placer un mur | Vérifiez le chevauchement ou si cela bloquerait un chemin |
| L'IA est trop forte | Réduisez la difficulté avec les touches 1-4 |
| L'IA est trop lente | Le mode Enfer peut prendre ~10ms par coup, c'est normal |

---

## Voir aussi

- [Documentation IA](AI_DOCU.md) — Détails de l'algorithme
- [Guide démo PVE](tests/PVE_DEMO_GUIDE.md) — Guide Joueur vs IA
- [Instructions d'installation](installation/INSTALLATION.md) — Compilation et exécution

---

**Dernière mise à jour** : 25 janvier 2026
