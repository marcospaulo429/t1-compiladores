principal {
    i, soma, pares: int;
} {
    soma = 0; pares = 0;
    i = 1;
    enquanto (i <= 10) {
        soma = soma + i;
        se (i / 2 * 2 == i) entao
            pares = pares + 1;
        fimse
        i = i + 1;
    }
    escreva soma; novalinha;
    escreva pares; novalinha;
    se (soma > 100) entao
        escreva "grande";
    senao
        se (soma == 55) entao escreva "cinquenta e cinco"; senao escreva "outro"; fimse
    fimse
    novalinha;
    i = 3;
    enquanto (i) i = i - 1;
    escreva i; novalinha;
    enquanto (0) escreva "nunca";
    ;
}
