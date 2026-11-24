/* Variaveis globais, sombreamento e acesso a partir de funcoes */
global [
    g, h : int;
    c : car;
]
funcao [
    inicia() : int
    {
        g = 0;
        retorne 0;
    }
    incrementa() : int
    {
        g = g + 1;
        retorne g;
    }
    sombra(g : int) : int
    [ h : int; ]
    {
        h = g * 2;
        retorne h;
    }
]
principal
[ g : int; ]
{
    inicia();
    g = 100;
    h = 5;
    c = 'z';
    escreva incrementa(); novalinha;
    escreva sombra(7); novalinha;
    escreva g; novalinha;
    escreva h; novalinha;
    escreva c; novalinha;
}
