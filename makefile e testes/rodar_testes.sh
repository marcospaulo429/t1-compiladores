#!/bin/sh
# Roda os testes do compilador G-V1.
#
#   testes/ok/NOME.g     programa correto
#   testes/ok/NOME.in    entrada do programa (opcional)
#   testes/ok/NOME.out   saida esperada ao executar o assembly no spim
#
#   testes/erro/NOME.g   programa com erro
#   testes/erro/NOME.err trecho que deve aparecer na mensagem de erro
#
# O simulador pode ser trocado com SPIM=/caminho/do/spim

cd "$(dirname "$0")" || exit 1

SPIM=${SPIM:-spim}
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT

passou=0
falhou=0

ok() {
    passou=$((passou + 1))
    printf '  ok     %s\n' "$1"
}

falha() {
    falhou=$((falhou + 1))
    printf '  FALHOU %s (%s)\n' "$1" "$2"
}

if ! command -v "$SPIM" >/dev/null 2>&1; then
    echo "spim nao encontrado: so serao verificados a compilacao e os erros"
    sem_spim=1
fi

echo "programas corretos:"
for g in testes/ok/*.g; do
    nome=$(basename "$g" .g)
    entrada=testes/ok/$nome.in
    [ -f "$entrada" ] || entrada=/dev/null

    if ! ./g-v1 "$g" "$TMP/$nome.s" >"$TMP/$nome.log" 2>&1; then
        falha "$nome" "nao compilou: $(head -n 1 "$TMP/$nome.log")"
        continue
    fi

    if [ -n "$sem_spim" ]; then
        ok "$nome (so compilou)"
        continue
    fi

    "$SPIM" -file "$TMP/$nome.s" <"$entrada" 2>&1 \
        | grep -v '^Loaded: ' >"$TMP/$nome.saida"

    if cmp -s "$TMP/$nome.saida" "testes/ok/$nome.out"; then
        ok "$nome"
    else
        falha "$nome" "saida diferente"
        diff "testes/ok/$nome.out" "$TMP/$nome.saida" | head -n 10
    fi
done

echo "programas com erro:"
for g in testes/erro/*.g; do
    nome=$(basename "$g" .g)
    esperado=$(head -n 1 "testes/erro/$nome.err")

    ./g-v1 "$g" "$TMP/$nome.s" >"$TMP/$nome.log" 2>&1
    status=$?

    if [ $status -eq 0 ]; then
        falha "$nome" "deveria falhar e compilou"
    elif [ -e "$TMP/$nome.s" ]; then
        falha "$nome" "gerou assembly mesmo com erro"
    elif grep -qF -- "$esperado" "$TMP/$nome.log"; then
        ok "$nome"
    else
        falha "$nome" "mensagem diferente: $(head -n 1 "$TMP/$nome.log")"
    fi
done

echo
echo "$passou passaram, $falhou falharam"

[ "$falhou" -eq 0 ]
