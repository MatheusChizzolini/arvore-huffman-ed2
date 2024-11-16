#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tadarvh.h"

int main(void) {
    Tree *tree = NULL;
	Forest *forest = NULL;
	Registro *tabela = NULL;
    char frase[256] = "amar e sonhar sonhar e viver viver e curtir curtir e amar", huffman[8] = "";
	char fraseACodificar[256] = "curtir e amar e viver sonhar", fraseCodificada[512] = "";

	// Criação da tabela de registros, da floresta e da árvore
    tabela = separaEmPalavras(frase);
	geraFloresta(&forest, tabela);
    tree = geraArvoreDeHuffman(&forest);
	geraCodigosHuffman(&tabela, tree, huffman, 0);
	gravaTabelaEmBinario(tabela);
	codificaFrase(tabela, fraseACodificar, fraseCodificada);
	gravaFraseCodificadaEmBinario(fraseCodificada);
	
	// Exibição da tabela de registros e da árvore
	exibeTabela(tabela);
	exibeArvoreDeHuffman(tree, 0);
	
    return 0;
}
