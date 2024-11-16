#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tadarvh.h"

int main(void) {
    Registro *tabela = NULL;
    Forest *forest = NULL;
    Tree *tree = NULL;
    char frase[128] = "amar e sonhar sonhar e viver viver e curtir curtir e amar", huffman[8] = "";
	char fraseACodificar[32] = "curtir e amar e viver sonhar", fraseCodificada[512] = "";

	// Criacao da tabela de registro, da floresta e da arvore
    tabela = separaEmPalavras(frase);
	geraFloresta(&forest, tabela);
    tree = geraArvoreDeHuffman(&forest);
	geraCodigoDeHuffman(&tabela, tree, huffman, 0);
	gravaTabelaEmDAT(tabela);
	codificaFrase(tabela, fraseACodificar, fraseCodificada);
	gravaCodigoEmDAT(fraseCodificada);
	
	// Exibicao
	exibeTabela(tabela);
	exibeArvoreDeHuffman(tree, 0);
	
    return 0;
}

