#!/bin/sh
# Roda os testes do compilador G-V1.
#
#   ok/NOME.g     programa correto
#   ok/NOME.in    entrada do programa (opcional)
#   ok/NOME.out   saida esperada ao executar o assembly no spim
#
#   erro/NOME.g   programa com erro
#   erro/NOME.err mensagem de erro esperada
#
# O simulador pode ser trocado com SPIM=/caminho/do/spim
#
# Pode ser rodado de qualquer lugar: ./testes/rodar_testes.sh
# ou com make test dentro de trabalho-final/

cd "$(dirname "$0")" || exit 1

G_V1=../trabalho-final/g-v1
if [ ! -x "$G_V1" ]; then
    make -C ../trabalho-final >/dev/null || exit 1
fi

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
for g in ok/*.g; do
    nome=$(basename "$g" .g)
    entrada=ok/$nome.in
    [ -f "$entrada" ] || entrada=/dev/null

    if ! "$G_V1" "$g" "$TMP/$nome.s" >"$TMP/$nome.log" 2>&1; then
        falha "$nome" "nao compilou: $(head -n 1 "$TMP/$nome.log")"
        continue
    fi

    if [ -n "$sem_spim" ]; then
        ok "$nome (so compilou)"
        continue
    fi

    # tira o cabecalho que o spim imprime antes da saida do programa
    # (o spim 8.0 do Debian/Ubuntu imprime mesmo com -quiet)
    "$SPIM" -quiet -file "$TMP/$nome.s" <"$entrada" 2>&1 \
        | grep -v -e '^SPIM Version' -e '^Copyright 1990' \
                  -e '^All Rights Reserved' -e '^See the file README' \
                  -e '^Loaded: ' >"$TMP/$nome.saida"

    if cmp -s "$TMP/$nome.saida" "ok/$nome.out"; then
        ok "$nome"
    else
        falha "$nome" "saida diferente"
        diff "ok/$nome.out" "$TMP/$nome.saida" | head -n 10
    fi
done

echo "programas com erro:"
for g in erro/*.g; do
    nome=$(basename "$g" .g)
    esperado=$(head -n 1 "erro/$nome.err")

    "$G_V1" "$g" "$TMP/$nome.s" >"$TMP/$nome.log" 2>&1
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
