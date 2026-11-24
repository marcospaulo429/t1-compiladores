/* Funcoes: chamada, recursao e retorno de car */
funcao [
    fat(n : int) : int
    {
        se (n <= 1) entao retorne 1; fimse
        retorne n * fat(n - 1);
    }
    soma(a : int, b : int) : int
    {
        retorne a + b;
    }
    letra(n : int) : car
    {
        retorne 'a';
    }
]
principal {
    escreva fat(5); novalinha;
    escreva soma(3, soma(4, 5)); novalinha;
    escreva fat(soma(1, 2)); novalinha;
}
