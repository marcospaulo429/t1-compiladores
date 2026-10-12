principal
{
    nota: int;
    conceito: car;
}
{
    leia nota;
    se (nota >= 90) entao
    {
        conceito = 'A';
    }
    senao
    {
        conceito = 'B';
    }
    fimse;

    escreva "Nota: ";
    escreva nota;
    novalinha;
    escreva "Conceito: ";
    escreva conceito;
    novalinha;
}
/* Comentario aberto que nao fecha ate o fim do arquivo