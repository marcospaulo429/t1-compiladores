principal
{
    nota: int;
    conceito: car;
}
{
    leia nota;
    se (nota >= 90) entao
    {
        conceito = @;
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