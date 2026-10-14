/* INTCONST pode ter sinal (+ ou -), mas em "n-1" o '-' e operador */
principal {
    a, b, n: int;
} {
    n = 10;
    a = +5;
    b = -3;
    escreva n-1; novalinha;
    escreva n - 1; novalinha;
    escreva n+1; novalinha;
    escreva a*-3; novalinha;
    escreva (a)-1; novalinha;
    escreva -b+-2; novalinha;
    escreva a - -b; novalinha;
    escreva 7-2; novalinha;
}
