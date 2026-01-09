/*
 * test-simulation.c
 *
 * Programme de test pour la validation finale (partie 6).
 *
 * Ce programme exécute 5 scénarios de simulation pour valider
 * l'intégration complète de votre système de matchmaking.
 *
 * Compilation et exécution directe via le script ./test-simulation.sh à lancer dans un terminal (Linux/Mac/Windows Git Bash).
 */

#include <stdio.h>
#include <stdlib.h>
#include "../headers/simulation.h"

// Prototypes des scénarios de test
void runScenario1(void);
void runScenario2(void);
void runScenario3(void);
void runScenario4(void);
void runScenario5(void);

int main(void) {
    printf("========================================\n");
    printf("  FORK KNIGHT - SIMULATION COMPLÈTE\n");
    printf("========================================\n\n");

    printf("Ce programme exécute 5 scénarios de test\n");
    printf("pour valider l'intégration de votre système.\n\n");

    // Scénario 1 : Test basique avec 50 joueurs
    runScenario1();

    // Scénario 2 : Test avec 100 joueurs
    runScenario2();

    // Scénario 3 : Stress test avec 1000 joueurs (voir input)
    runScenario3();

    // Scénario 4 : Convergence rapide (peu de rounds)
    runScenario4();

    // Scénario 5 : Stabilisation (beaucoup de rounds)
    runScenario5();

    printf("\n========================================\n");
    printf("  TOUS LES SCÉNARIOS TERMINÉS\n");
    printf("========================================\n");
    printf("\nVérifiez les fichiers dans output/ :\n");
    printf("  - scenario1_result.txt\n");
    printf("  - scenario2_result.txt\n");
    printf("  - scenario3_result.txt\n");
    printf("  - scenario4_result.txt\n");
    printf("  - scenario5_result.txt\n\n");

    return EXIT_SUCCESS;
}

// ============================================================
// Scénario 1 : Test basique (50 joueurs, 10 rounds)
// ============================================================
void runScenario1(void) {
    printf("----------------------------------------\n");
    printf("SCÉNARIO 1 : Test basique (50 joueurs)\n");
    printf("----------------------------------------\n");

    SimulationConfig config = {
        .inputPath = "input/players_50.txt",
        .outputPath = "output/scenario1_result.txt",
        .rounds = 10,
        .seed = 42,
        .maxWaitCyclesPriority = 10
    };

    printf("Configuration :\n");
    printf("  - Fichier d'entrée : %s\n", config.inputPath);
    printf("  - Fichier de sortie : %s\n", config.outputPath);
    printf("  - Nombre de rounds : %d\n", config.rounds);
    printf("  - Graine aléatoire : %u\n", config.seed);
    printf("  - Cycles d'attente max : %d\n\n", config.maxWaitCyclesPriority);

    runSimulation(&config);

    printf("\n✓ Scénario 1 terminé\n");
    printf("  Résultats : %s\n\n", config.outputPath);
}

// ============================================================
// Scénario 2 : Test intermédiaire (100 joueurs, 15 rounds)
// ============================================================
void runScenario2(void) {
    printf("----------------------------------------\n");
    printf("SCÉNARIO 2 : Test intermédiaire (100 joueurs)\n");
    printf("----------------------------------------\n");

    SimulationConfig config = {
        .inputPath = "input/players_100.txt",
        .outputPath = "output/scenario2_result.txt",
        .rounds = 15,
        .seed = 123,
        .maxWaitCyclesPriority = 12
    };

    printf("Configuration :\n");
    printf("  - Fichier d'entrée : %s\n", config.inputPath);
    printf("  - Fichier de sortie : %s\n", config.outputPath);
    printf("  - Nombre de rounds : %d\n", config.rounds);
    printf("  - Graine aléatoire : %u\n", config.seed);
    printf("  - Cycles d'attente max : %d\n\n", config.maxWaitCyclesPriority);

    runSimulation(&config);

    printf("\n✓ Scénario 2 terminé\n");
    printf("  Résultats : %s\n\n", config.outputPath);
}

// ============================================================
// Scénario 3 : Stress test (1000 joueurs, 20 rounds)
// ============================================================
void runScenario3(void) {
    printf("----------------------------------------\n");
    printf("SCÉNARIO 3 : Stress test (1000 joueurs)\n");
    printf("----------------------------------------\n");

    SimulationConfig config = {
        .inputPath = "input/players_100.txt", // Ajustez si vous avez un fichier 1000 joueurs
        .outputPath = "output/scenario3_result.txt",
        .rounds = 20,
        .seed = 999,
        .maxWaitCyclesPriority = 15
    };

    printf("Configuration :\n");
    printf("  - Fichier d'entrée : %s\n", config.inputPath);
    printf("  - Fichier de sortie : %s\n", config.outputPath);
    printf("  - Nombre de rounds : %d\n", config.rounds);
    printf("  - Graine aléatoire : %u\n", config.seed);
    printf("  - Cycles d'attente max : %d\n\n", config.maxWaitCyclesPriority);

    runSimulation(&config);

    printf("\n✓ Scénario 3 terminé\n");
    printf("  Résultats : %s\n\n", config.outputPath);
}

// ============================================================
// Scénario 4 : Convergence rapide (50 joueurs, 5 rounds)
// ============================================================
void runScenario4(void) {
    printf("----------------------------------------\n");
    printf("SCÉNARIO 4 : Convergence rapide (5 rounds)\n");
    printf("----------------------------------------\n");

    SimulationConfig config = {
        .inputPath = "input/players_50.txt",
        .outputPath = "output/scenario4_result.txt",
        .rounds = 5,
        .seed = 555,
        .maxWaitCyclesPriority = 8
    };

    printf("Configuration :\n");
    printf("  - Fichier d'entrée : %s\n", config.inputPath);
    printf("  - Fichier de sortie : %s\n", config.outputPath);
    printf("  - Nombre de rounds : %d\n", config.rounds);
    printf("  - Graine aléatoire : %u\n", config.seed);
    printf("  - Cycles d'attente max : %d\n\n", config.maxWaitCyclesPriority);

    runSimulation(&config);

    printf("\n✓ Scénario 4 terminé\n");
    printf("  Résultats : %s\n\n", config.outputPath);
}

// ============================================================
// Scénario 5 : Stabilisation (100 joueurs, 30 rounds)
// ============================================================
void runScenario5(void) {
    printf("----------------------------------------\n");
    printf("SCÉNARIO 5 : Stabilisation (30 rounds)\n");
    printf("----------------------------------------\n");

    SimulationConfig config = {
        .inputPath = "input/players_100.txt",
        .outputPath = "output/scenario5_result.txt",
        .rounds = 30,
        .seed = 777,
        .maxWaitCyclesPriority = 20
    };

    printf("Configuration :\n");
    printf("  - Fichier d'entrée : %s\n", config.inputPath);
    printf("  - Fichier de sortie : %s\n", config.outputPath);
    printf("  - Nombre de rounds : %d\n", config.rounds);
    printf("  - Graine aléatoire : %u\n", config.seed);
    printf("  - Cycles d'attente max : %d\n\n", config.maxWaitCyclesPriority);

    runSimulation(&config);

    printf("\n✓ Scénario 5 terminé\n");
    printf("  Résultats : %s\n\n", config.outputPath);
}