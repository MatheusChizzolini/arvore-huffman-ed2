struct tree {
    int simbolo, freq;
    struct tree *esq, *dir;
};
typedef struct tree Tree;

struct forest {
    struct tree *tree;
    struct forest *prox;
};
typedef struct forest Forest;

struct registro {
    int freq, simbolo;
    char palavra[16], codigoHuffman[8];
    struct registro *prox;
};
typedef struct registro Registro;

struct bits {
    unsigned char b7 : 1;
    unsigned char b6 : 1;
    unsigned char b5 : 1;
    unsigned char b4 : 1;
    unsigned char b3 : 1;
    unsigned char b2 : 1;
    unsigned char b1 : 1;
    unsigned char b0 : 1;
};
typedef struct bits Bits;

union byte {
    struct bits bit;
    unsigned char codigo;
};
typedef union byte Byte;

Registro *criaRegistro(char palavra[]) {
    Registro *registro = (Registro *)malloc(sizeof(Registro));
    
    registro->freq = 1;
    registro->simbolo = 0;
	strcpy(registro->palavra, palavra);
	strcpy(registro->codigoHuffman, "");
	registro->prox = NULL;

	return registro;
}

Tree *criaFolha(int simbolo, int freq) {
    Tree *tree = (Tree *)malloc(sizeof(Tree));

    tree->simbolo = simbolo;
    tree->freq = freq;
    tree->esq = tree->dir = NULL;

    return tree;
}

Forest *criaNodo(int simbolo, int freq) {
    Forest *forest = (Forest *)malloc(sizeof(Forest));

    forest->tree = criaFolha(simbolo, freq);
    forest->prox = NULL;

    return forest;
}

void insereOrdenado(Forest **cabeca, Forest *nodo) {
    Forest *ant, *atual;

    if (*cabeca == NULL)
        *cabeca = nodo;          
    else {
        if (nodo->tree->freq <= (*cabeca)->tree->freq) {
            nodo->prox = *cabeca;
            *cabeca = nodo;
        }
        else {
            ant = atual = *cabeca;
            while (atual->prox != NULL && nodo->tree->freq > atual->tree->freq) {
                ant = atual;
                atual = atual->prox;
            }

            if (nodo->tree->freq <= atual->tree->freq) {
                nodo->prox = atual;
                ant->prox = nodo;
            }
            else
                atual->prox = nodo;
        }
    }
}

void geraFloresta(Forest **cabeca, Registro *tabela) {
    Forest *nodo;

    while (tabela != NULL) {
        nodo = criaNodo(tabela->simbolo, tabela->freq);
        insereOrdenado(&*cabeca, nodo);

        tabela = tabela->prox;
    }
}

void insereRegistro(Registro **tabela, char palavra[]) {
	Registro *registro, *aux;
	
	registro = criaRegistro(palavra);
	if (*tabela == NULL) {
		*tabela = registro;
	}
    else {
		aux = *tabela;
		while (aux->prox != NULL)
			aux = aux->prox;
			
		aux->prox = registro;
	}
}

Registro* buscaPorPalavra(Registro *tabela, char palavra[]) {
    Registro *atual = tabela;

    while (atual != NULL && strcmp(atual->palavra, palavra) != 0)
        atual = atual->prox;

    return atual;
}

Registro* buscaPorSimbolo(Registro *tabela, int simbolo) {
    Registro *atual = tabela;

    while (atual != NULL && atual->simbolo != simbolo)
        atual = atual->prox;

    return atual;
}

Registro *separaEmPalavras(char frase[]) {
    Registro *tabela = NULL, *aux;
    int i, j, simbolo = 1;
    char palavra[16];

    for (i = 0; i < strlen(frase); i++) {
        j = 0;
        if (frase[j] != '\0') {
            if (frase[i] != ' ') {
                palavra[j++] = frase[i];
                while (i < strlen(frase) && frase[i + 1] != ' ')
                    palavra[j++] = frase[++i];
            }
            else
                palavra[j++] = frase[i];
            
            palavra[j] = '\0';
            aux = buscaPorPalavra(tabela, palavra);
            if (aux == NULL) {
                insereRegistro(&tabela, palavra);
                aux = buscaPorPalavra(tabela, palavra);
                aux->simbolo = simbolo++;
            }
            else {
                aux->freq++;
            }
        }
    }

    return tabela;
}

void exibeTabela(Registro *tabela) {
    int y = 5;
    gotoxy(1, 4); printf("SIMBOLO");
    gotoxy(14, 4); printf("PALAVRA");
    gotoxy(27, 4); printf("FREQUENCIA");
    gotoxy(40, 4); printf("HUFFMAN");

    while (tabela != NULL) {
        gotoxy(4, y); printf("%d", tabela->simbolo);
        gotoxy(14, y); printf("'%s'", tabela->palavra);
        gotoxy(31, y); printf("%d", tabela->freq);
        gotoxy(41, y); printf("%s", tabela->codigoHuffman);
        y++;

        tabela = tabela->prox;
    }

    printf("\n\n");
}

Tree *geraArvoreDeHuffman(Forest **cabeca) {
    Forest *atual, *prox, *nodo;

    while ((*cabeca)->prox != NULL) {
        atual = *cabeca;
        prox = atual->prox;
        
        nodo = criaNodo(-1, atual->tree->freq + prox->tree->freq);
        nodo->tree->esq = atual->tree;
        nodo->tree->dir = prox->tree;

        insereOrdenado(&*cabeca, nodo);

        *cabeca = (*cabeca)->prox->prox;
        free(atual);
        free(prox);
    }

    return (*cabeca)->tree;
}

void exibeArvoreDeHuffman(Tree *raiz, int n) {
    int i;

    if (raiz != NULL) {
        exibeArvoreDeHuffman(raiz->esq, n + 1);
        for (i = 0; i < 5 * n; i++)
            printf(" ");
        
        printf("(%d, %d)\n", raiz->simbolo, raiz->freq);
        exibeArvoreDeHuffman(raiz->dir, n + 1);
    }
}

void geraCodigosHuffman(Registro **tabela, Tree *tree, char huffman[], int i) {
    Registro *aux;

    if (tree != NULL) {
        if (tree->esq == NULL && tree->dir == NULL) {
            huffman[i] = '\0';
            aux = buscaPorSimbolo(*tabela, tree->simbolo);
            strcpy(aux->codigoHuffman, huffman);
        }
        else {
            huffman[i] = '0';
            geraCodigosHuffman(&*tabela, tree->esq, huffman, i + 1);
            huffman[i] = '1';
            geraCodigosHuffman(&*tabela, tree->dir, huffman, i + 1);
        }
    }
}

void gravaTabelaEmBinario(Registro *tabela) {
    FILE *arq = fopen("tabela-registros.dat", "wb");

    if (arq != NULL) {
        while (tabela != NULL) {
            fwrite(tabela, sizeof(Registro), 1, arq);
            tabela = tabela->prox;
        }
    }
    
    fclose(arq);
}

void codificaFrase(Registro *tabela, char fraseACodificar[], char fraseCodificada[]) {
    Registro *aux;
    int i, j, mod;
    char palavra[16];

    for (i = 0; i < strlen(fraseACodificar); i++) {
        j = 0;
        if (fraseACodificar[j] != '\0') {
            if (fraseACodificar[i] != ' ') {
                palavra[j++] = fraseACodificar[i];
                while (i < strlen(fraseACodificar) && fraseACodificar[i + 1] != ' ')
                    palavra[j++] = fraseACodificar[++i];
            }
            else
                palavra[j++] = fraseACodificar[i];
            
            palavra[j] = '\0';
            aux = buscaPorPalavra(tabela, palavra);
            if (aux != NULL)
                strcat(fraseCodificada, aux->codigoHuffman);
        }
    }

    // Preenchendo o resto da frase codificada com 0 para completar o byte
    mod = strlen(fraseCodificada) % 8;
    i = 0;
    if (mod > 0) {
        while (i < (8 - mod)) {
            strcat(fraseCodificada, "0");
            i++;
        }
    }
}

void gravaFraseCodificadaEmBinario(char fraseCodificada[]) {
    int i = 0;
    Byte byte;
    FILE *arq = fopen("frase-codificada.dat", "wb");

    while (i < strlen(fraseCodificada)) {
        byte.bit.b0 = fraseCodificada[i];
        byte.bit.b1 = fraseCodificada[i + 1];
        byte.bit.b2 = fraseCodificada[i + 2];
        byte.bit.b3 = fraseCodificada[i + 3];
        byte.bit.b4 = fraseCodificada[i + 4];
        byte.bit.b5 = fraseCodificada[i + 5];
        byte.bit.b6 = fraseCodificada[i + 6];
        byte.bit.b7 = fraseCodificada[i + 7];

        i += 8;
        fwrite(&byte.codigo, sizeof(char), 1, arq);
    }
    
    fclose(arq);
}

void recuperaArvorePeloBinario(Tree **raiz) {
    int i;
    Tree *atual, *nodo;
    Registro *registro;
    FILE *arq = fopen("tabela-registros.dat", "rb");

    if (arq != NULL) {
        if (*raiz == NULL)
            *raiz = criaFolha(-1, -1);

        fread(registro, sizeof(Registro), 1, arq);
        while (!feof(arq)) {
            atual = *raiz;
            for (i = 0; i < strlen(registro->codigoHuffman); i++) {
                if (registro->codigoHuffman[i] == '0') {
                    if (atual->esq == NULL) {
                        nodo = criaFolha(-1, -1);
                        atual->esq = nodo;
                    }

                    atual = atual->esq;
                }
                else {
                    if (registro->codigoHuffman[i] == '1') {
                        if (atual->dir == NULL) {
                            nodo = criaFolha(-1, -1);
                            atual->dir = nodo;
                        }

                        atual = atual->dir;
                    }
                }

                if (i == strlen(registro->codigoHuffman) - 1) {
                    atual->simbolo = registro->simbolo;
                    atual->freq = registro->freq;
                }
            }
            
            fread(registro, sizeof(Registro), 1, arq);
        }
    }

    fclose(arq);
}

Registro *recuperaTabelaPeloBinario(void) {
    Registro registro, *novoRegistro, *tabela = NULL, *aux;
    FILE *ptr = fopen("tabela-registros.dat", "rb");

    if (ptr != NULL) {
        fread(&registro, sizeof(Registro), 1, ptr);
        while (!feof(ptr)) {
            novoRegistro = criaRegistro(registro.palavra);
            novoRegistro->simbolo = registro.simbolo;
            novoRegistro->freq = registro.freq;
            strcpy(novoRegistro->codigoHuffman, registro.codigoHuffman);

            if (tabela == NULL) {
                tabela = novoRegistro;
            }
            else {
                aux = tabela;
                while (aux->prox != NULL)
                    aux = aux->prox;
                
                aux->prox = novoRegistro;
            }

            fread(&registro, sizeof(Registro), 1, ptr);
        }
    }

    fclose(ptr);
    return tabela;
}

void decodificaFrase(Tree *raiz, char fraseDecodificada[]) {
    int i = 0;
    char fraseCodificada[512] = "";
    Byte byte;
    Tree *atual;
    Registro *tabela, *aux;
    FILE *arq = fopen("frase-codificada.dat", "rb");

    if (arq != NULL) {
        fread(&byte.codigo, sizeof(char), 1, arq);
        while (!feof(arq)) {
            fraseCodificada[i] = byte.bit.b0 + '0';
            fraseCodificada[i + 1] = byte.bit.b1 + '0';
            fraseCodificada[i + 2] = byte.bit.b2 + '0';
            fraseCodificada[i + 3] = byte.bit.b3 + '0';
            fraseCodificada[i + 4] = byte.bit.b4 + '0';
            fraseCodificada[i + 5] = byte.bit.b5 + '0';
            fraseCodificada[i + 6] = byte.bit.b6 + '0';
            fraseCodificada[i + 7] = byte.bit.b7 + '0';
            
            fread(&byte.codigo, sizeof(char), 1, arq);
            i = i + 8;
        }

        fraseCodificada[i] = '\0';
        atual = raiz;
        for (i = 0; i < strlen(fraseCodificada); i++) {
            if (fraseCodificada[i] == '0')
                atual = atual->esq;
            else
                if (fraseCodificada[i] == '1')
                    atual = atual->dir;
            
            if (atual->esq == NULL && atual->dir == NULL) {
                tabela = recuperaTabelaPeloBinario();
                aux = buscaPorSimbolo(tabela, atual->simbolo);
                strcat(fraseDecodificada, aux->palavra);
                
                atual = raiz;
            }
        }
    }

    fclose(arq);
    printf("Frase codificada recuperada do arquivo binario:\n%s\n", fraseCodificada);
}
