/* Fibonacci recursivo e vetor passado em chamada recursiva */
funcao [
    fib(n : int) : int
    {
        se (n < 2) entao retorne n; fimse
        retorne fib(n - 1) + fib(n - 2);
    }
    somaRec(v[] : int, n : int) : int
    {
        se (n == 0) entao retorne 0; fimse
        retorne v[n - 1] + somaRec(v, n - 1);
    }
]
principal
[ a[4] : int; ]
{
    escreva fib(10); novalinha;
    a[0] = 10; a[1] = 20; a[2] = 30; a[3] = 40;
    escreva somaRec(a, 4); novalinha;
}
