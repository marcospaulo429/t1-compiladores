/* Vetor passado por referencia: a funcao altera o vetor do chamador */
funcao [
    zera(v[] : int, n : int) : int
    [ i : int; ]
    {
        i = 0;
        enquanto (i < n) { v[i] = 0; i = i + 1; }
        retorne 0;
    }
    soma(v[] : int, n : int) : int
    [ i, s : int; ]
    {
        i = 0; s = 0;
        enquanto (i < n) { s = s + v[i]; i = i + 1; }
        retorne s;
    }
    troca(v[] : int, i : int, j : int) : int
    [ t : int; ]
    {
        t = v[i]; v[i] = v[j]; v[j] = t;
        retorne 0;
    }
    elemento(x : int) : int
    {
        x = x + 1;
        retorne x;
    }
]
principal
[ a[5] : int; k : int; ]
{
    a[0] = 1; a[1] = 2; a[2] = 3; a[3] = 4; a[4] = 5;
    escreva soma(a, 5); novalinha;
    troca(a, 0, 4);
    escreva a[0]; escreva a[4]; novalinha;
    escreva elemento(a[1]); escreva a[1]; novalinha;
    zera(a, 5);
    escreva soma(a, 5); novalinha;
}
