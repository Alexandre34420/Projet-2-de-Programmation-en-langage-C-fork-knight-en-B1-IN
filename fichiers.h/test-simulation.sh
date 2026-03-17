#!/bin/bash

# Test dédié à la simulation complète (partie 6)
# - Compile le binaire test-simulation
# - Exécute deux fois
# - Compare les fichiers output pour vérifier le déterminisme

set -u
set -o pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
ROOT_DIR="$SCRIPT_DIR"
BIN="$ROOT_DIR/test-simulation"
OUTDIR="$ROOT_DIR/output"
LOGDIR="$OUTDIR/logs"
CFLAGS="-Wall -Wextra -Werror -Iheaders -g"
FILES=(
    "scenario1_result.txt"
    "scenario2_result.txt"
    "scenario3_result.txt"
    "scenario4_result.txt"
    "scenario5_result.txt"
)

hint_missing_output() {
    local file="$1"
    local log="$2"
    echo "   Cause fréquente : exécution stoppée avant écriture ou nom de fichier différent"
    echo "   Log ($log), dernières lignes :"
    tail -12 "$log" 2>/dev/null || true
}

hint_diff_output() {
    local f1="$1"
    local f2="$2"
    echo "   Causes fréquentes : état global non remis à zéro ou aléatoire sans seed fixe"
    echo "   Diff (extrait) :"
    diff -u "$f1" "$f2" | head -30
}

hint_run_failure() {
    local log="$1"
    echo "   Échec pendant l'exécution : souvent dû à un segfault, une entrée invalide ou un assert déclenché"
    echo "   Log ($log), dernières lignes :"
    tail -20 "$log" 2>/dev/null || true
}

build_bin() {
    echo "[BUILD] Compilation simulation complète"
    if command -v make >/dev/null 2>&1; then
        (cd "$ROOT_DIR" && make clean > /dev/null 2>&1 || true)
        (cd "$ROOT_DIR" && make build-simulation)
    else
        (cd "$ROOT_DIR" && gcc $CFLAGS \
            src/player.c src/array-utils.c src/list-utils.c src/priority-queue.c \
            src/match.c src/simulation.c src/test-simulation.c \
            -o test-simulation)
    fi
    echo ""
}

run_and_snapshot() {
    local tag="$1"
    local log="$LOGDIR/run${tag}.log"
    echo "[RUN] Exécution #$tag"
    mkdir -p "$LOGDIR"
    if ! (cd "$ROOT_DIR" && "$BIN" >"$log" 2>&1); then
        echo "❌ Échec de l'exécution #$tag (voir $log)"
        hint_run_failure "$log"
        return 2
    fi
    local missing=0
    mkdir -p "$OUTDIR"
    for f in "${FILES[@]}"; do
        if [ -f "$OUTDIR/$f" ]; then
            cp "$OUTDIR/$f" "$OUTDIR/${f%.txt}-$tag.txt"
        else
            echo "⚠️  Fichier manquant après run$tag: $OUTDIR/$f"
            hint_missing_output "$f" "$log"
            missing=1
        fi
    done
    return $missing
}

compare_outputs() {
    local diff_found=0
    for f in "${FILES[@]}"; do
        local f1="$OUTDIR/${f%.txt}-1.txt"
        local f2="$OUTDIR/${f%.txt}-2.txt"
        if ! diff -q "$f1" "$f2" > /dev/null 2>&1; then
            echo "❌ Différences détectées entre $f1 et $f2"
            hint_diff_output "$f1" "$f2"
            diff_found=1
        else
            echo "✅ Identiques : $f1 vs $f2"
        fi
    done
    return $diff_found
}

main() {
    build_bin

    rm -f "$OUTDIR"/scenario*_result-1.txt "$OUTDIR"/scenario*_result-2.txt
    rm -f "$LOGDIR"/run*.log 2>/dev/null || true

    if ! run_and_snapshot 1; then
        echo "❌ Échec ou fichiers manquants après la première exécution"
        exit 1
    fi

    if ! run_and_snapshot 2; then
        echo "❌ Échec ou fichiers manquants après la seconde exécution"
        exit 1
    fi

    if compare_outputs; then
        echo ""
        echo "✅ Tous les tests simulation complète réussis"
        exit 0
    else
        echo ""
        echo "⚠️  Sorties non déterministes détectées"
        exit 1
    fi
}

main "$@"
