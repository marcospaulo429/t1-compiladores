principal {
    n, f: int;
} {
    leia n;
    f = 1;
    enquanto (n > 1) {
        f = f * n;
        n = n - 1;
    }
    escreva f;
    novalinha;
}
