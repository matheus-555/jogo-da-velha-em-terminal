#include <stdio.h>
#include "lib/velha/velha.h"

int main(void)
{
    velha_t jogo;
    
    velha_init(&jogo);
    velha_main(&jogo);

    return 0;
}
