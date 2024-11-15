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
    char palavra[15], codigoHuffman[10];
    struct registro *prox;
};
typedef struct registro Registro;

Registro *novoRegistro(char palavra[15]) {
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
    tree->esq = tree->esq = NULL;

    return tree;
}

Forest *criaNodo(int simbolo, int freq) {
    Forest *forest = (Forest *)malloc(sizeof(Forest));

    forest->tree = criaFolha(simbolo, freq);
    forest->prox = NULL;

    return forest;
}

void apagaNodo(Forest **cabeca, int simbolo) {
    Forest *ant, *atual;

    if (*cabeca != NULL) {
        if ((*cabeca)->tree->simbolo == simbolo) {
            atual = *cabeca;
            *cabeca = (*cabeca)->prox;
            free(atual);
        }
        else {
            ant = atual = *cabeca;
            while (atual->prox != NULL && atual->tree->simbolo != simbolo) {
                ant = atual;
                atual = atual->prox;
            }

            if (atual != NULL) {
                ant->prox = atual->prox;
                free(atual);
            }
        }
    }
}

void insereOrdenadoNaFloresta(Forest **forest, Registro *tabela) {
    Forest *nodo, *ant, *atual;

    while (tabela != NULL) {
        nodo = criaNodo(tabela->simbolo, tabela->freq);
        if (*forest == NULL)
            *forest = nodo;          
        else {
            if (nodo->tree->freq <= (*forest)->tree->freq) {
                nodo->prox = *forest;
                *forest = nodo;
            }
            else {
                atual = ant = *forest;
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

        tabela = tabela->prox;
    }
}

void exibeFloresta(Forest *forest) {
    while (forest != NULL) {
        printf("%d | %d\n", forest->tree->freq, forest->tree->simbolo);
        forest = forest->prox;
    }
}

void insereRegistro(Registro **tabela, char palavra[15]) {
	Registro *registro, *aux;
	
	registro = novoRegistro(palavra);
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

Registro* buscaPalavraNaTabela(Registro *tabela, char palavra[15]) {
    Registro *atual = tabela;
    while (atual != NULL && strcmp(atual->palavra, palavra) != 0)
        atual = atual->prox;

    return atual;
}

void exibeTabela(Registro *tabela) {
    while (tabela != NULL) {
        printf("SIMBOLO: %d\tPALAVRA: '%s'\tFREQUENCIA: %d\n", tabela->simbolo, tabela->palavra, tabela->freq);
        tabela = tabela->prox;
    }
}

Registro *separaEmPalavras(char frase[128]) {
    Registro *tabela = NULL, *aux;
    int i, j, simbolo = 1;
    char palavra[15];

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
            aux = buscaPalavraNaTabela(tabela, palavra);
            if (aux == NULL) {
                insereRegistro(&tabela, palavra);
                aux = buscaPalavraNaTabela(tabela, palavra);
                aux->simbolo = simbolo++;
            }
            else {
                aux->freq++;
            }
        }
    }

    return tabela;
}

void geraArvoreDeHuffman(Forest **cabeca) {}

void exibeArvoreDeHuffman(Tree *raiz) {}
