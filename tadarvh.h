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
};
typedef struct registro Registro;

// Função que recebe uma frase e a separa em palavras para colocar na tabela de registros, ao mesmo tempo contando a frequência.
void separaEmPalavras(char frase[]);