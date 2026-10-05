principal {
    x: int;
} {
    x = 0;
    se (0 & (x = 5)) entao escreva "no"; fimse
    escreva x; novalinha;
    se (1 || (x = 6)) entao escreva "sim"; fimse
    novalinha;
    escreva x; novalinha;
    se (1 & (x = 8)) entao escreva "ok"; fimse
    novalinha;
    escreva x; novalinha;
    se (0 || (x = 9)) entao escreva "ok"; fimse
    novalinha;
    escreva x; novalinha;
}
