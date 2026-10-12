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
    escreva "Conceito que quebra
a linha aqui";
    escreva conceito;
    novalinha;
}