#include <stdio.h>   // Entrada e saída padrão
#include <stdlib.h>  // Funções utilitárias (malloc, free, etc.)
#include <string.h>  // Manipulação de strings (strcmp, strcpy, etc.)

// ---------------------------------------------------------
//              CÓDIGO DA ILHA – NÍVEL NOVATO
// ---------------------------------------------------------
// Sistema básico de gerenciamento da mochila inicial do jogador.
// Permite cadastrar, remover, listar e buscar itens coletados.
// ---------------------------------------------------------

#define TAM_MAX 10  // Capacidade máxima da mochila
#define MAX_NOME 30
#define MAX_TIPO 20

// ---------------------------------------------------------
// Estrutura que representa cada item da mochila
// ---------------------------------------------------------
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} Item;

// ---------------------------------------------------------
// Declarações de funções
// ---------------------------------------------------------
void inserirItem(Item mochila[], int *qtd);
void removerItem(Item mochila[], int *qtd);
void listarItens(const Item mochila[], int qtd);
void buscarItem(const Item mochila[], int qtd);

// ---------------------------------------------------------
// Função principal com menu interativo
// ---------------------------------------------------------
int main() {
    Item mochila[TAM_MAX];
    int qtd = 0;
    int opcao;

    do {
        printf("\n==============================================\n");
        printf("     CODIGO DA ILHA - NIVEL NOVATO\n");
        printf("==============================================\n");
        printf("1. Adicionar item à mochila\n");
        printf("2. Remover item da mochila\n");
        printf("3. Listar todos os itens\n");
        printf("4. Buscar item por nome\n");
        printf("0. Sair do jogo\n");
        printf("----------------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1:
                inserirItem(mochila, &qtd);
                break;
            case 2:
                removerItem(mochila, &qtd);
                break;
            case 3:
                listarItens(mochila, qtd);
                break;
            case 4:
                buscarItem(mochila, qtd);
                break;
            case 0:
                printf("\nEncerrando... Boa sorte na ilha!\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// ---------------------------------------------------------
// Função para adicionar um item na mochila
// ---------------------------------------------------------
void inserirItem(Item mochila[], int *qtd) {
    if (*qtd >= TAM_MAX) {
        printf("\nMochila cheia! Remova um item antes de adicionar outro.\n");
        return;
    }

    Item novo;
    printf("\nNome do item: ");
    fgets(novo.nome, MAX_NOME, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // Remove \n

    printf("Tipo (arma, munição, cura...): ");
    fgets(novo.tipo, MAX_TIPO, stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar();

    mochila[*qtd] = novo;
    (*qtd)++;

    printf("\nItem '%s' adicionado com sucesso!\n", novo.nome);
}

// ---------------------------------------------------------
// Função para remover item por nome
// ---------------------------------------------------------
void removerItem(Item mochila[], int *qtd) {
    if (*qtd == 0) {
        printf("\nA mochila está vazia.\n");
        return;
    }

    char nome[MAX_NOME];
    printf("\nInforme o nome do item a remover: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int pos = -1;
    for (int i = 0; i < *qtd; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("\nItem '%s' não encontrado.\n", nome);
        return;
    }

    for (int i = pos; i < *qtd - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    (*qtd)--;
    printf("\nItem '%s' removido com sucesso!\n", nome);
}

// ---------------------------------------------------------
// Função para listar todos os itens da mochila
// ---------------------------------------------------------
void listarItens(const Item mochila[], int qtd) {
    if (qtd == 0) {
        printf("\nA mochila está vazia.\n");
        return;
    }

    printf("\n--- Itens na Mochila (%d/%d) ---\n", qtd, TAM_MAX);
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("----------------------------------------------\n");

    for (int i = 0; i < qtd; i++) {
        printf("%-20s | %-15s | %d\n",
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade);
    }
}

// ---------------------------------------------------------
// Função de busca sequencial por nome
// ---------------------------------------------------------
void buscarItem(const Item mochila[], int qtd) {
    if (qtd == 0) {
        printf("\nA mochila está vazia.\n");
        return;
    }

    char nome[MAX_NOME];
    printf("\nNome do item a buscar: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    for (int i = 0; i < qtd; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return;
        }
    }

    printf("\nItem '%s' não encontrado na mochila.\n", nome);
}
