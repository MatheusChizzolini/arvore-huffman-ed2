#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tadarvh.h"

int main(void) {
    Registro *tabela = NULL;
    Forest *forest = NULL;
    char frase[128] = "amar e sonhar sonhar e viver viver e curtir curtir e amar";

    tabela = separaEmPalavras(frase);
    exibeTabela(tabela);
    printf("\n");
    
    insereOrdenadoNaFloresta(&forest, tabela);
    exibeFloresta(forest);
    
    return 0;
}
