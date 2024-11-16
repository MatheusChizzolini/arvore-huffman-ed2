#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tadarvh.h"

int main(void) {
    Tree *tree = NULL;
    char fraseDecodificada[512] = "";

    recuperaArvorePeloBinario(&tree);
    
	decodificaFrase(tree, fraseDecodificada);
	printf("%s", fraseDecodificada);

    return 0;
}
