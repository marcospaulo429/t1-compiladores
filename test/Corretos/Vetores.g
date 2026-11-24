/* Vetores locais e globais de int e car */
global [ v[4] : int; ]
principal
[ i : int; w[3] : car; ]
{
    i = 0;
    enquanto (i < 4) { v[i] = i * i; i = i + 1; }
    i = 0;
    enquanto (i < 4) { escreva v[i]; escreva ' '; i = i + 1; }
    novalinha;
    w[0] = 'a'; w[1] = 'b'; w[2] = 'c';
    escreva w[2]; escreva w[1]; escreva w[0]; novalinha;
    v[v[1] + 1] = 42;
    escreva v[2]; novalinha;
}
