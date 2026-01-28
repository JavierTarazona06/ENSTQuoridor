# Itération 4 : Packaging, polish et préparation soutenance

**Dates** : 6 janvier – 14 janvier 2026 (1 semaine)

---

## À faire

### L4.1 Packaging

- [ ] **[PACK-01] Windows ZIP portable**
  - Configurer CPack pour générer `Quoridor-1.0-Windows.zip`
  - Inclure `quoridor_game.exe`, DLLs SFML, dossier `assets/`
  - Tester double-clic sur machine Windows propre
  - **Estimation** : 2h

- [ ] **[PACK-02] Linux tarball / AppImage**
  - Générer archive `.tar.gz` avec binaire + libs + assets
  - (Optionnel) Créer AppImage via `linuxdeploy`
  - Tester sur Ubuntu/Debian
  - **Estimation** : 3h

- [ ] **[PACK-03] macOS bundle (best-effort)**
  - Configurer `MACOSX_BUNDLE` dans CMake
  - Embarquer frameworks SFML dans `.app`
  - Générer `.dmg` via CPack DragNDrop
  - **Estimation** : 3h

---

### L4.2 Assets et polish UI

- [ ] **[UI-01] Textures/sprites plateau et pions**
  - Créer ou sourcer sprites épurés pour grille, pions, murs
  - Intégrer dans `Render2D` (optionnel : garder formes géométriques)
  - **Estimation** : 2h

- [ ] **[UI-02] Polices et lisibilité**
  - Vérifier rendu polices sur toutes plateformes
  - Ajuster tailles/couleurs HUD et messages
  - **Estimation** : 1h

- [ ] **[UI-03] Sons (optionnel)**
  - Ajouter sons déplacement pion, placement mur, victoire
  - Intégrer via SFML Audio
  - **Estimation** : 2h

- [ ] **[UI-04] Animations simples (optionnel)**
  - Animation transition pion (interpolation position)
  - Feedback visuel placement mur
  - **Estimation** : 2h

---

### L4.3 Documentation

- [ ] **[DOC-01] README.md complet**
  - Description projet, prérequis, instructions compilation
  - Instructions d'exécution, modes de jeu, contrôles
  - **Estimation** : 1h

- [ ] **[DOC-02] Guide utilisateur (USER_GUIDE.md)**
  - Contrôles détaillés, modes de jeu, niveaux IA
  - Captures d'écran
  - **Estimation** : 1h

- **[DOC-03] Documentation technique IA**
  - Architecture Minimax, heuristiques, paramètres difficulté
  - **Terminé** : `docs/AI_Documentation.md`

- [ ] **[DOC-04] Diagramme UML classes**
  - Générer diagramme MVC (Model/View/Controller)
  - Inclure dans `docs/` ou slides soutenance
  - **Estimation** : 1h

- [ ] **[DOC-05] Commentaires code**
  - Vérifier Doxygen-style sur classes principales
  - Nettoyer TODOs et code mort
  - **Estimation** : 1h

---

### L4.4 Tests finaux

- [ ] **[TEST-01] Tests intégration HvH**
  - Scénario partie complète Humain vs Humain
  - Vérifier victoire, restart, retour menu
  - **Estimation** : 1h

- [ ] **[TEST-02] Tests intégration HvIA**
  - Scénario partie complète Humain vs IA (tous niveaux)
  - Vérifier temps réponse IA < 3s
  - **Estimation** : 1h

- [ ] **[TEST-03] Tests multi-plateforme**
  - Lancer sur Windows, Linux, (macOS si disponible)
  - Valider rendu, contrôles, assets chargés
  - **Estimation** : 2h

- [ ] **[TEST-04] Correction bugs identifiés**
  - Traiter bugs critiques trouvés lors des tests
  - Geler fonctionnalités le 10 janvier
  - **Estimation** : 2h

---

### L4.5 Soutenance

- [ ] **[SOUT-01] Diaporama (15-20 slides)**
  - Contexte et objectifs
  - Architecture MVC
  - IA : Minimax + α-β, heuristiques
  - Démo live
  - Résultats tests
  - Packaging multi-plateforme
  - Rétrospective et améliorations futures
  - **Estimation** : 3h

- [ ] **[SOUT-02] Démo préparée**
  - Scénario démo fluide (HvH + HvIA)
  - Backup : vidéo pré-enregistrée si crash
  - **Estimation** : 1h

- [ ] **[SOUT-03] Répétition orale**
  - Timing 10-15 minutes + questions
  - Anticiper questions techniques (IA, packaging, tests)
  - **Estimation** : 2h

---

## En cours

*(Déplacer les tâches ici une fois commencées)*

---

## Terminé

- [x] **[DOC-03] Documentation technique IA** — `docs/AI_Documentation.md`
- [x] **[UI-05] GameConfig struct** — Encapsulation mode + difficulté
- [x] **[UI-06] Info box mode/difficulté** — Affichage in-game
- [x] **[CTRL-01] Menu cliquable** — Sélection mode par clic souris

---

## 🚫 Bloqué

*(Tâches bloquées avec raison)*

---

## 📊 Résumé

| Catégorie     | À faire | En cours | Terminé |
|---------------|---------|----------|---------|
| Packaging     | 3       | 0        | 0       |
| UI/Polish     | 4       | 0        | 2       |
| Documentation | 4       | 0        | 1       |
| Tests         | 4       | 0        | 0       |
| Soutenance    | 3       | 0        | 0       |
| **Total**     | **18**  | **0**    | **3**   |

---

## ⏰ Planning suggéré

| Date       | Focus                                      |
|------------|--------------------------------------------|
| 6-7 jan    | Packaging Windows + Linux                  |
| 8 jan      | Documentation README + Guide utilisateur   |
| 9 jan      | Tests intégration + correction bugs        |
| 10 jan     | **Feature freeze** — Diaporama soutenance  |
| 11-12 jan  | Bug fixes uniquement + répétitions         |
| 13-14 jan  | Répétition finale + backup démo            |
| **15 jan** | **Soutenance**                             |
