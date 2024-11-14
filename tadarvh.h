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
        printf("PALAVRA: '%s'\tFREQUENCIA: %d\n", tabela->palavra, tabela->freq);
        tabela = tabela->prox;
    }
}

// Função que recebe uma frase e a separa em palavras para colocar na tabela de registros, ao mesmo tempo contando a frequência.
Registro *separaEmPalavras(char frase[128]) {
    Registro *tabela = NULL, *aux;
    int i, j;
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
            // printf("%s", palavra);
            aux = buscaPalavraNaTabela(tabela, palavra);
            if (aux == NULL) {
                insereRegistro(&tabela, palavra);
                // printf("%s\n", tabela->palavra);
            }
            else {
                aux->freq++;
            }
        }
    }

    return tabela;
}
