# Guide de Création de Tests

Ce guide explique comment créer de nouveaux tests unitaires pour le projet ENSTQuoridor en utilisant le framework de test Catch2.

---

## Prérequis

- Catch2 est déjà intégré au projet via vcpkg
- Tous les fichiers de test sont situés dans le répertoire `tests/`
- Les tests sont compilés dans un seul exécutable : `test_all`

---

## Étape 1 : Créer un nouveau fichier de test

Créez un nouveau fichier `.cpp` dans le répertoire `tests/` en suivant la convention de nommage `test_<composant>.cpp`.

**Exemple :** `tests/test_moncomposant.cpp`

```cpp
#include <catch2/catch_test_macros.hpp>
#include "model/MonComposant.hpp"

using namespace Quoridor;

TEST_CASE("Initialisation du composant", "[moncomposant]") {
    MonComposant composant;
    
    SECTION("Les valeurs par défaut sont correctement définies") {
        REQUIRE(composant.getValeur() == 0);
        REQUIRE(composant.estValide() == true);
    }
    
    SECTION("Le composant peut être modifié") {
        composant.setValeur(42);
        REQUIRE(composant.getValeur() == 42);
    }
}

TEST_CASE("Validation du composant", "[moncomposant][validation]") {
    MonComposant composant;
    
    SECTION("Une entrée invalide lève une exception") {
        REQUIRE_THROWS_AS(composant.setValeur(-1), std::invalid_argument);
    }
    
    SECTION("Une entrée valide est acceptée") {
        REQUIRE_NOTHROW(composant.setValeur(100));
    }
}
```

---

## Étape 2 : Ajouter le fichier de test à CMakeLists.txt

Ouvrez `tests/CMakeLists.txt` et ajoutez votre nouveau fichier de test à la commande `add_executable`.

**Avant :**
```cmake
add_executable(test_all
    test_board.cpp
    test_state.cpp
    test_rules.cpp
)
```

**Après :**
```cmake
add_executable(test_all
    test_board.cpp
    test_state.cpp
    test_rules.cpp
    test_moncomposant.cpp
)
```

---

## Étape 3 : Recompiler le projet

Après avoir ajouté votre fichier de test, recompilez le projet :

Suivez les instructions dans [Guide d'installation](../installation/INSTALLATION.md).

Et exécutez ce dont vous avez besoin.

---

## Macros de test Catch2

### Structure des tests
- **`TEST_CASE("description", "[tags]")`** — Définit un cas de test
- **`SECTION("description")`** — Divise un cas de test en sections (chaque section s'exécute indépendamment)

### Assertions
- **`REQUIRE(expression)`** — Échoue et arrête le test si l'expression est fausse
- **`CHECK(expression)`** — Échoue mais continue l'exécution du test
- **`REQUIRE_FALSE(expression)`** — Exige que l'expression soit fausse

### Tests d'exceptions
- **`REQUIRE_THROWS(expression)`** — Exige que l'expression lève une exception
- **`REQUIRE_THROWS_AS(expression, type_exception)`** — Exige un type d'exception spécifique
- **`REQUIRE_NOTHROW(expression)`** — Exige que l'expression ne lève pas d'exception

### Comparaisons
- **`REQUIRE(a == b)`** — Vérification d'égalité avec sortie détaillée
- **`REQUIRE(a != b)`** — Vérification d'inégalité
- **`REQUIRE(a < b)`**, **`REQUIRE(a > b)`** — Vérifications de comparaison

---

## Bonnes pratiques

1. **Utilisez des noms de test descriptifs** — Clarifiez ce qui est testé
2. **Utilisez des tags** — Organisez les tests avec des tags comme `[board]`, `[rules]`, `[integration]`
3. **Un concept par TEST_CASE** — Gardez les tests focalisés
4. **Utilisez SECTION pour les variations** — Testez différents scénarios avec la même configuration
5. **Testez les cas limites** — Incluez les conditions aux limites et les cas d'erreur
6. **Gardez les tests indépendants** — Chaque test doit pouvoir s'exécuter isolément

---

## Exemple : Fichier de test complet

```cpp
#include <catch2/catch_test_macros.hpp>
#include "model/Board.hpp"

using namespace Quoridor;

TEST_CASE("Validation du placement de mur", "[mur][validation]") {
    SECTION("Mur horizontal valide") {
        Wall mur = {{3, 3}, Orientation::Horizontal};
        REQUIRE(mur.position.x == 3);
        REQUIRE(mur.position.y == 3);
        REQUIRE(mur.orientation == Orientation::Horizontal);
    }
    
    SECTION("Mur vertical valide") {
        Wall mur = {{0, 0}, Orientation::Vertical};
        REQUIRE(mur.position.x == 0);
        REQUIRE(mur.position.y == 0);
        REQUIRE(mur.orientation == Orientation::Vertical);
    }
}

TEST_CASE("Détection de collision de mur", "[mur][collision]") {
    Wall mur1 = {{3, 3}, Orientation::Horizontal};
    Wall mur2 = {{3, 3}, Orientation::Horizontal};
    
    SECTION("Les murs identiques se chevauchent") {
        REQUIRE(mur1.overlaps(mur2) == true);
    }
    
    SECTION("Les positions différentes ne se chevauchent pas") {
        mur2.position = {4, 4};
        REQUIRE(mur1.overlaps(mur2) == false);
    }
}
```

---

## Ressources supplémentaires

- [Documentation Catch2](https://github.com/catchorg/Catch2/tree/devel/docs)
- [Tutoriel Catch2](https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md)
- Fichiers de test existants dans le répertoire `tests/` pour référence

---

## Dépannage

**Tests non découverts par CTest :**
- Assurez-vous que CMakeLists.txt inclut votre fichier de test
- Exécutez `cmake --preset <preset>` pour reconfigurer
- Recompilez le projet

**Erreurs de compilation :**
- Vérifiez que tous les en-têtes nécessaires sont inclus
- Vérifiez l'utilisation du namespace (`using namespace Quoridor;`)
- Assurez-vous que le composant testé fait partie de la bibliothèque `quoridor_model`

**Échecs de tests :**
- Utilisez le flag `--output-on-failure` avec ctest
- Exécutez un test individuel : `./test_all "Nom du test"`
- Utilisez `SECTION` pour isoler les scénarios en échec

---

**Dernière mise à jour** : 25 janvier 2026