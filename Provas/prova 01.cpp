#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct ingresso
{
    int data;
    struct ingresso *prox;
};

struct cliente
{
    char nome[30];
    int cpf;
    struct ingresso *ingresso;
    struct cliente *prox;
    struct cliente *ant;
};

struct cliente *cabeca = nullptr;

bool cpf_existe(int cpf)
{
    struct cliente *aux = cabeca;
    while (aux != nullptr)
    {
        if (aux->cpf == cpf)
        {
            return true;
        }
        aux = aux->prox;
    }
    return false;
}

bool data_existe(int data)
{
    struct cliente *aux = cabeca;
    while (aux != nullptr)
    {
        ingresso *bilhete = aux->ingresso;
        while (bilhete != nullptr)
        {
            if (bilhete->data == data)
            {
                return true;
            }
            bilhete = bilhete->prox;
        }
        aux = aux->prox;
    }
    // Implemente sua lógica de validação de data aqui
    // Retorne true se a data for válida, false caso contrário
    return false;
}

void adicionar()
{
    struct cliente *novo = new cliente;
    novo->ingresso = nullptr;

    printf("Digite o nome: ");
    scanf("%s", novo->nome);

    int cpf = 0;
    do
    {
        printf("Digite o cpf: ");
        scanf("%d", &cpf);
        if (cpf_existe(cpf))
        {
            printf("CPF já cadastrado. Tente novamente com outro.\n");
        }
    } while (cpf_existe(cpf));
    novo->cpf = cpf;

    int data = 0;
    do
    {
        printf("Digite a data do ingresso: ");
        scanf("%d", &data);
        if (data_existe(data))
        {
            printf("Essa data já exite. Tente novamente.\n");
        }
    } while (data_existe(data));
    struct ingresso *newTicket = new ingresso;
    newTicket->data = data;
    newTicket->prox = nullptr;

    novo->ingresso = newTicket;
    novo->prox = nullptr;

    if (cabeca == nullptr)
    {
        cabeca = novo;
    }
    else
    {
        struct cliente *aux = cabeca;
        while (aux->prox != nullptr)
        {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}

void imprimir()
{
    cliente *aux = cabeca;
    while (aux != nullptr)
    {
        printf("Nome: %s\nCpf: %d\n", aux->nome, aux->cpf);
        if (aux->ingresso != nullptr)
        {
            printf("Ingressos:\n");
            ingresso *bilhete = aux->ingresso;
            while (bilhete != nullptr)
            {
                printf("Data: %d\n", bilhete->data);
                bilhete = bilhete->prox;
            }
        }
        printf("==========\n");
        aux = aux->prox;
    }
}

struct cliente *buscar()
{
    struct cliente *aux = cabeca;
    int cpf = 0;
    printf("Digite o cpf: ");
    scanf("%d", &cpf);

    while (aux != nullptr)
    {
        if (aux->cpf == cpf)
        {
            return aux;
        }
        aux = aux->prox;
    }
}

void insertion_sort()
{
    // Cria uma nova lista encadeada de ingressos ordenados
    ingresso *ordenar = nullptr;

    // Percorre a lista original de clientes
    cliente *aux = cabeca;
    while (aux != nullptr)
    {
        // Remove o primeiro ingresso da lista de ingressos do cliente
        ingresso *bilhete = aux->ingresso;
        aux->ingresso = bilhete->prox;

        // Insere o ingresso na posição correta na lista ordenada
        if (ordenar == nullptr || bilhete->data < ordenar->data)
        {
            bilhete->prox = ordenar;
            ordenar = bilhete;
        }
        else
        {
            ingresso *temp = ordenar;
            while (temp->prox != nullptr && temp->prox->data < bilhete->data)
            {
                temp = temp->prox;
            }
            bilhete->prox = temp->prox;
            temp->prox = bilhete;
        }

        // Avança para o próximo nó da lista original
        aux = aux->prox;
    }

    // Atualiza a lista de ingressos de cada cliente na lista original
    aux = cabeca;
    while (aux != nullptr && ordenar != nullptr)
    {
        aux->ingresso = ordenar;
        ordenar = ordenar->prox;
        aux = aux->prox;
    }

    // Libera a lista ordenada
    ingresso *temp;
    while (ordenar != nullptr)
    {
        temp = ordenar;
        ordenar = ordenar->prox;
        delete temp;
    }
}

void inserir_ingresso()
{
    // encontra o nó com o cpf especificado
    cliente *aux = buscar();

    // cria um novo ingresso e adiciona à lista de ingressos para este nó
    int data = 0;
    do
    {
        printf("Digite a data do ingresso: ");
        scanf("%d", &data);
        if (data_existe(data))
        {
            printf("Essa data já exite. Tente novamente.\n");
        }
    } while (data_existe(data));
    struct ingresso *newTicket = new ingresso;
    newTicket->data = data;
    newTicket->prox = nullptr;

    // Se a lista de ingressos ainda não existir, cria o primeiro nó
    if (aux->ingresso == nullptr)
    {
        aux->ingresso = newTicket;
    }
    else
    {
        // Encontra o último nó na lista de ingressos
        ingresso *lastTicket = aux->ingresso;
        while (lastTicket->prox != nullptr)
        {
            lastTicket = lastTicket->prox;
        }

        // Adiciona o novo ingresso ao final da lista de ingressos
        lastTicket->prox = newTicket;
    }
}

typedef struct list
{
    cliente *inicio = nullptr;
    cliente *fim = nullptr;
    int tamanho = 0;
} lista;

void anexar_inicio(lista *nova, lista *lista)
{
    if ((lista == NULL) || (lista->tamanho == 0))
    {
        return;
    }
    nova->tamanho += lista->tamanho;
    nova->inicio->ant = lista->fim;
    lista->fim->prox = nova->inicio;
    nova->inicio = lista->inicio;
}

void anexar_fim(lista *nova, lista *lista)
{
    if ((lista == NULL) || (lista->tamanho == 0))
    {
        return;
    }
    nova->tamanho += lista->tamanho;
    nova->fim->prox = lista->inicio;
    lista->inicio->ant = nova->fim;
    nova->fim = lista->fim;
}

void remover(lista *lst, cliente *elemento)
{
    if ((lst == NULL) || (elemento == NULL) || (lst->tamanho == 0))
    {
        return;
    }

    if ((lst->inicio == elemento) && (lst->fim == elemento))
    {
        lst->tamanho = 0;
        lst->inicio = NULL;
        lst->fim = NULL;
    }

    lst->tamanho--;

    if (lst->inicio == elemento)
    {
        lst->inicio = lst->inicio->prox;
        lst->inicio->ant = NULL;
        elemento->prox = NULL;
        return;
    }

    if (lst->fim == elemento)
    {
        lst->fim = lst->fim->ant;
        lst->fim->prox = NULL;
        elemento->ant = NULL;
        return;
    }

    cliente *antElem = elemento->ant;
    cliente *proxElem = elemento->prox;
    antElem->prox = proxElem;
    proxElem->ant = antElem;

    elemento->ant = NULL;
    elemento->prox = NULL;
}

void inserir_inicio(lista *lst, cliente *elemento)
{
    if ((lst == NULL) || (elemento == NULL))
    {
        return;
    }

    if (lst->tamanho == 0)
    {
        lst->inicio = elemento;
        lst->fim = elemento;
        lst->tamanho = 1;
        elemento->ant = NULL;
        elemento->prox = NULL;
        return;
    }

    elemento->ant = NULL;
    elemento->prox = lst->inicio;
    lst->inicio->ant = elemento;
    lst->inicio = elemento;
    lst->tamanho++;
}

void inserir_fim(lista *lst, cliente *elemento)
{
    if ((lst == NULL) || (elemento == NULL))
    {
        return;
    }

    if (lst->tamanho == 0)
    {
        lst->inicio = elemento;
        lst->fim = elemento;
        lst->tamanho = 1;
        elemento->ant = NULL;
        elemento->prox = NULL;
        return;
    }

    elemento->prox = NULL;
    elemento->ant = lst->fim;
    lst->fim->prox = elemento;
    lst->fim = elemento;
    lst->tamanho++;
}

void quick_sort(lista *lst)
{
    if ((lst == NULL) || (lst->tamanho <= 1))
    {
        return;
    }

    int tam_metade = lst->tamanho / 2;
    cliente *pivo = lst->inicio;
    for (int i = 0; i < tam_metade; i++)
    {
        pivo = pivo->prox;
    }
    lista menores;
    lista maiores;

    cliente *aux = lst->inicio;
    cliente *elemento = NULL;
    while (aux != NULL)
    {
        elemento = aux;
        aux = aux->prox;
        if (elemento == pivo)
        {
            continue;
        }
        remover(lst, elemento);
        if (elemento->cpf < pivo->cpf)
        {
            inserir_inicio(&menores, elemento);
        }
        else
        {
            inserir_fim(&maiores, elemento);
        }
    }

    quick_sort(&menores);
    anexar_inicio(lst, &menores);

    quick_sort(&maiores);
    anexar_fim(lst, &maiores);
}

void divide(lista *lista1, lista *lista2)
{
    int tam_metade = lista1->tamanho / 2;
    cliente *inicio2 = lista1->inicio;
    for (int i = 0; i < tam_metade; i++)
    {
        inicio2 = inicio2->prox;
    }

    cliente *fim1 = inicio2->ant;
    fim1->prox = NULL;
    inicio2->ant = NULL;

    lista2->inicio = inicio2;
    lista2->fim = lista1->fim;
    lista2->tamanho = lista1->tamanho - tam_metade;

    lista1->fim = fim1;
    lista1->tamanho = tam_metade;
}

int main(int argc, char const *argv[])
{
    lista *lst = new lista;
    int op = 0;

    do
    {
        printf("----- MENU -----\n");
        printf("1 - comprar ingresso.\n");
        printf("2 - imprimir bilhete\n");
        printf("3 - Comprar outro ingresso.\n");
        printf("4 - Quick Sort.\n");
        printf("0 - Sair\n");

        printf("Digite a opcao desejada: ");
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            adicionar();
            break;
        case 2:
            imprimir();
            break;
        case 3:
            inserir_ingresso();
            insertion_sort();
            break;
        case 4:
            quick_sort(lst);
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida.\n");
        }
    } while (op != 0);

    return 0;
}
