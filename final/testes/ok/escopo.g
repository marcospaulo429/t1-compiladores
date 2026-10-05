principal {
    x, y: int;
} {
    x = 1;
    y = 2;
    {
        x: int;
        z: int;
    } {
        x = 10;
        z = 20;
        escreva x; escreva " "; escreva y; escreva " "; escreva z; novalinha;
        {
            y: int;
        } {
            y = 99;
            x = x + 1;
            escreva y; escreva " "; escreva x; novalinha;
        }
        escreva y; novalinha;
    }
    escreva x; novalinha;
    {
        w: int;
    } {
        escreva w; novalinha;
        w = 5;
    }
    {
        q: int;
    } {
        escreva q; novalinha;
    }
}
