#include <stdio.h>   // Entrada e saída padrão
#include <stdlib.h>  // Funções de alocação e liberação de memória
#include <string.h>  // Manipulação de strings

// ---------------------------------------------------------
//              CÓDIGO DA ILHA – NÍVEL MESTRE
// ---------------------------------------------------------
// Sistema de inventário completo com:
// - Lista encadeada dinâmica
// - Salvamento e carregamento em arquivo
// - Busca binária otimizada
// - Ordenação alfabética
// ---------------------------------------------------------

#define MAX_NOME 30
#define MAX_TIPO 20
#define ARQ_SAVE "mochila.txt" // Nome do arquivo de save

// ---------------------------------------------------------
// Estrutura do item (nó da lista encadeada)
// ---------------------------------------------------------
typedef struct item {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    struct item *prox;
} Item;

// ---------------------------------------------------------
// Declarações das funções principais
// ---------------------------------------------------------
Item* criarItem(const char *nome, const char *tipo, int quantidade);
void inserirItemOrdenado(Item **inicio);
void removerItem(Item **inicio);
void listarItens(const Item *inicio);
void buscarItemBinario(const Item *inicio);
void salvarArquivo(const Item *inicio);
void carregarArquivo(Item **inicio);
void liberarLista(Item **inicio);

// Funções auxiliares internas
int contarItens(const Item *inicio);
void preencherVetor(const Item *inicio, Item **vetor);
int comparar(const void *a, const void *b);

// ---------------------------------------------------------
// Função principal (menu principal do Mestre)
// ---------------------------------------------------------
int main() {
    Item *inicio = NULL;
    int opcao;

    // Tenta carregar os dados automaticamente no início
    carregarArquivo(&inicio);

    do {
        printf("\n==============================================\n");
        printf("      CODIGO DA ILHA - NIVEL MESTRE\n");
        printf("==============================================\n");
        printf("1. Adicionar item (ordenado)\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item (binária)\n");
        printf("5. Salvar inventário\n");
        printf("0. Sair do jogo\n");
        printf("----------------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                inserirItemOrdenado(&inicio);
                break;
            case 2:
                removerItem(&inicio);
                break;
            case 3:
                listarItens(inicio);
                break;
            case 4:
                buscarItemBinario(inicio);
                break;
            case 5:
                salvarArquivo(inicio);
                break;
            case 0:
                printf("\nSalvando progresso...\n");
                salvarArquivo(inicio);
                liberarLista(&inicio);
                printf("Saída concluída. Até a próxima, Mestre!\n");
                break;
            default:
                printf("\nOpção inválida.\n");
        }
    } while (opcao != 0);

    return 0;
}

// ---------------------------------------------------------
// Cria um novo item alocado dinamicamente
// ---------------------------------------------------------
Item* criarItem(const char *nome, const char *tipo, int quantidade) {
    Item *novo = (Item*) malloc(sizeof(Item));
    if (!novo) {
        printf("\nErro de alocação!\n");
        return NULL;
    }
    strcpy(novo->nome, nome);
    strcpy(novo->tipo, tipo);
    novo->quantidade = quantidade;
    novo->prox = NULL;
    return novo;
}

// ---------------------------------------------------------
// Insere item na lista de forma ordenada por nome
// ---------------------------------------------------------
void inserirItemOrdenado(Item **inicio) {
    char nome[MAX_NOME], tipo[MAX_TIPO];
    int quantidade;

    printf("\nNome do item: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Tipo: ");
    fgets(tipo, MAX_TIPO, stdin);
    tipo[strcspn(tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &quantidade);
    getchar();

    Item *novo = criarItem(nome, tipo, quantidade);
    if (!novo) return;

    // Inserção ordenada (alfabética)
    if (*inicio == NULL || strcmp(novo->nome, (*inicio)->nome) < 0) {
        novo->prox = *inicio;
        *inicio = novo;
    } else {
        Item *atual = *inicio;
        while (atual->prox && strcmp(novo->nome, atual->prox->nome) > 0) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }

    printf("\nItem '%s' inserido de forma ordenada!\n", nome);
}

// ---------------------------------------------------------
// Remove item da lista por nome
// ---------------------------------------------------------
void removerItem(Item **inicio) {
    if (*inicio == NULL) {
        printf("\nInventário vazio.\n");
        return;
    }

    char nome[MAX_NOME];
    printf("\nNome do item a remover: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    Item *atual = *inicio, *anterior = NULL;

    while (atual && strcmp(atual->nome, nome) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if (!atual) {
        printf("\nItem '%s' não encontrado.\n", nome);
        return;
    }

    if (!anterior)
        *inicio = atual->prox;
    else
        anterior->prox = atual->prox;

    free(atual);
    printf("\nItem '%s' removido com sucesso!\n", nome);
}

// ---------------------------------------------------------
// Lista todos os itens da mochila
// ---------------------------------------------------------
void listarItens(const Item *inicio) {
    if (!inicio) {
        printf("\nA mochila está vazia.\n");
        return;
    }

    printf("\n--- Mochila do Mestre ---\n");
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("----------------------------------------------\n");

    const Item *atual = inicio;
    while (atual) {
        printf("%-20s | %-15s | %d\n",
               atual->nome, atual->tipo, atual->quantidade);
        atual = atual->prox;
    }
}

// ---------------------------------------------------------
// Busca item com base em busca binária simulada
// ---------------------------------------------------------
void buscarItemBinario(const Item *inicio) {
    if (!inicio) {
        printf("\nMochila vazia.\n");
        return;
    }

    int qtd = contarItens(inicio);
    Item **vetor = (Item**) malloc(qtd * sizeof(Item*));
    if (!vetor) {
        printf("\nErro de memória.\n");
        return;
    }

    preencherVetor(inicio, vetor);
    qsort(vetor, qtd, sizeof(Item*), comparar);

    char nome[MAX_NOME];
    printf("\nNome do item a buscar: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int ini = 0, fim = qtd - 1, meio, encontrado = 0;

    while (ini <= fim) {
        meio = (ini + fim) / 2;
        int cmp = strcmp(vetor[meio]->nome, nome);
        if (cmp == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   vetor[meio]->nome, vetor[meio]->tipo, vetor[meio]->quantidade);
            encontrado = 1;
            break;
        } else if (cmp < 0) ini = meio + 1;
        else fim = meio - 1;
    }

    if (!encontrado)
        printf("\nItem '%s' não encontrado.\n", nome);

    free(vetor);
}

// ---------------------------------------------------------
// Salva o inventário atual em arquivo de texto
// ---------------------------------------------------------
void salvarArquivo(const Item *inicio) {
    FILE *f = fopen(ARQ_SAVE, "w");
    if (!f) {
        printf("\nErro ao abrir arquivo para salvar.\n");
        return;
    }

    const Item *atual = inicio;
    while (atual) {
        fprintf(f, "%s;%s;%d\n", atual->nome, atual->tipo, atual->quantidade);
        atual = atual->prox;
    }

    fclose(f);
    printf("\nInventário salvo em '%s'.\n", ARQ_SAVE);
}

// ---------------------------------------------------------
// Carrega inventário do arquivo (se existir)
// ---------------------------------------------------------
void carregarArquivo(Item **inicio) {
    FILE *f = fopen(ARQ_SAVE, "r");
    if (!f) {
        printf("\nNenhum save encontrado. Começando novo jogo.\n");
        return;
    }

    char linha[100], nome[MAX_NOME], tipo[MAX_TIPO];
    int quantidade;

    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%[^;];%[^;];%d", nome, tipo, &quantidade) == 3) {
            Item *novo = criarItem(nome, tipo, quantidade);
            if (!novo) continue;

            // Inserção ordenada
            if (*inicio == NULL || strcmp(novo->nome, (*inicio)->nome) < 0) {
                novo->prox = *inicio;
                *inicio = novo;
            } else {
                Item *atual = *inicio;
                while (atual->prox && strcmp(novo->nome, atual->prox->nome) > 0)
                    atual = atual->prox;
                novo->prox = atual->prox;
                atual->prox = novo;
            }
        }
    }

    fclose(f);
    printf("\nSave carregado com sucesso!\n");
}

// ---------------------------------------------------------
// Libera toda a memória alocada
// ---------------------------------------------------------
void liberarLista(Item **inicio) {
    Item *atual = *inicio;
    while (atual) {
        Item *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    *inicio = NULL;
}

// ---------------------------------------------------------
// Contagem de elementos da lista
// ---------------------------------------------------------
int contarItens(const Item *inicio) {
    int cont = 0;
    while (inicio) {
        cont++;
        inicio = inicio->prox;
    }
    return cont;
}

// ---------------------------------------------------------
// Copia os ponteiros da lista para um vetor auxiliar
// ---------------------------------------------------------
void preencherVetor(const Item *inicio, Item **vetor) {
    int i = 0;
    while (inicio) {
        vetor[i++] = (Item*)inicio;
        inicio = inicio->prox;
    }
}

// ---------------------------------------------------------
// Função de comparação usada no qsort
// ---------------------------------------------------------
int comparar(const void *a, const void *b) {
    Item *ia = *(Item**)a;
    Item *ib = *(Item**)b;
    return strcmp(ia->nome, ib->nome);
}
