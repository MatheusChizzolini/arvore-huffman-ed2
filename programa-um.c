#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tadarvh.h"

int main(void) {
    Tree *tree = NULL;
	Forest *forest = NULL;
	Registro *tabela = NULL;
    char frase[512] = "amar e sonhar sonhar e viver viver e curtir curtir e amar", huffman[8] = "";
	char fraseACodificar[512] = "curtir amar e viver sonhar", fraseCodificada[512] = "";

    tabela = separaEmPalavras(frase);
	geraFloresta(&forest, tabela);
    tree = geraArvoreDeHuffman(&forest);
	geraCodigosHuffman(&tabela, tree, huffman, 0);
	gravaTabelaEmBinario(tabela);
	codificaFrase(tabela, fraseACodificar, fraseCodificada);
	gravaFraseCodificadaEmBinario(fraseCodificada);
	
	printf("Frase principal:\n%s\n", frase);
	exibeTabela(tabela);
	exibeArvoreDeHuffman(tree, 0);
	printf("\nFrase a ser codificada:\n%s\n", fraseACodificar);
	printf("\nFrase codificada:\n%s\n", fraseCodificada);
	
    return 0;
}

