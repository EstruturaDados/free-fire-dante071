#include <stdio.h>   // Entrada e saída padrão
#include <stdlib.h>  // Funções de alocação de memória
#include <string.h>  // Manipulação de strings

// ---------------------------------------------------------
//              CÓDIGO DA ILHA – NÍVEL AVENTUREIRO
// ---------------------------------------------------------
// Sistema de inventário dinâmico com lista encadeada.
// Permite cadastrar, remover, listar e buscar itens.
// Usa alocação dinâmica de memória (malloc / free).
// ---------------------------------------------------------

#define MAX_NOME 30
#define MAX_TIPO 20

// ---------------------------------------------------------
// Estrutura de dados – Nó da lista encadeada
// ---------------------------------------------------------
typedef struct item {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    struct item *prox; // Ponteiro para o próximo item
} Item;

// ---------------------------------------------------------
// Declarações de funções
// ---------------------------------------------------------
Item* criarItem(const char *nome, const char *tipo, int quantidade);
void inserirItem(Item **inicio);
void removerItem(Item **inicio);
void listarItens(const Item *inicio);
void buscarItem(const Item *inicio);
void liberarLista(Item **inicio);

// ---------------------------------------------------------
// Função principal (menu interativo)
// ---------------------------------------------------------
int main() {
    Item *inicio = NULL; // Lista começa vazia
    int opcao;

    do {
        printf("\n==============================================\n");
        printf("     CODIGO DA ILHA - NIVEL AVENTUREIRO\n");
        printf("==============================================\n");
        printf("1. Adicionar item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item\n");
        printf("0. Sair\n");
        printf("----------------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1:
                inserirItem(&inicio);
                break;
            case 2:
                removerItem(&inicio);
                break;
            case 3:
                listarItens(inicio);
                break;
            case 4:
                buscarItem(inicio);
                break;
            case 0:
                printf("\nSalvando progresso e saindo...\n");
                liberarLista(&inicio);
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ---------------------------------------------------------
// Cria um novo item dinamicamente e retorna o ponteiro
// ---------------------------------------------------------
Item* criarItem(const char *nome, const char *tipo, int quantidade) {
    Item *novo = (Item*) malloc(sizeof(Item));
    if (!novo) {
        printf("\nErro de alocação de memória!\n");
        return NULL;
    }
    strcpy(novo->nome, nome);
    strcpy(novo->tipo, tipo);
    novo->quantidade = quantidade;
    novo->prox = NULL;
    return novo;
}

// ---------------------------------------------------------
// Insere um novo item no início da lista
// ---------------------------------------------------------
void inserirItem(Item **inicio) {
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

    novo->prox = *inicio; // Novo item aponta para o anterior
    *inicio = novo;       // Início agora é o novo item

    printf("\nItem '%s' adicionado com sucesso!\n", nome);
}

// ---------------------------------------------------------
// Remove item da lista por nome
// ---------------------------------------------------------
void removerItem(Item **inicio) {
    if (*inicio == NULL) {
        printf("\nA mochila está vazia.\n");
        return;
    }

    char nome[MAX_NOME];
    printf("\nNome do item a remover: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    Item *atual = *inicio;
    Item *anterior = NULL;

    while (atual != NULL && strcmp(atual->nome, nome) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("\nItem '%s' não encontrado.\n", nome);
        return;
    }

    if (anterior == NULL)
        *inicio = atual->prox;  // Removendo o primeiro
    else
        anterior->prox = atual->prox;

    free(atual);
    printf("\nItem '%s' removido com sucesso!\n", nome);
}

// ---------------------------------------------------------
// Lista todos os itens cadastrados
// ---------------------------------------------------------
void listarItens(const Item *inicio) {
    if (inicio == NULL) {
        printf("\nA mochila está vazia.\n");
        return;
    }

    printf("\n--- Mochila do Aventureiro ---\n");
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("----------------------------------------------\n");

    const Item *atual = inicio;
    while (atual != NULL) {
        printf("%-20s | %-15s | %d\n",
               atual->nome, atual->tipo, atual->quantidade);
        atual = atual->prox;
    }
}

// ---------------------------------------------------------
// Busca item por nome (varredura linear)
// ---------------------------------------------------------
void buscarItem(const Item *inicio) {
    if (inicio == NULL) {
        printf("\nA mochila está vazia.\n");
        return;
    }

    char nome[MAX_NOME];
    printf("\nNome do item a buscar: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    const Item *atual = inicio;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   atual->nome, atual->tipo, atual->quantidade);
            return;
        }
        atual = atual->prox;
    }

    printf("\nItem '%s' não encontrado na mochila.\n", nome);
}

// ---------------------------------------------------------
// Libera toda a memória alocada pela lista
// ---------------------------------------------------------
void liberarLista(Item **inicio) {
    Item *atual = *inicio;
    while (atual != NULL) {
        Item *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    *inicio = NULL;
}
