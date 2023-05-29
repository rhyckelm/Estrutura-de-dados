#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

enum Cor
{
  VERMELHO,
  PRETO
};

typedef struct node
{
  struct node *esq;
  struct node *dir;
  struct node *pai;
  int num;
  enum Cor cor;
} No;

No *criar_no(int num)
{
  No *node = (No *)malloc(sizeof(No));

  node->num = num;
  node->esq = NULL;
  node->dir = NULL;

  return node;
}

int eh_vermelho(No *node)
{
  if (node == NULL)
  {
    return 0;
  }

  return node->cor == VERMELHO ? 1 : 0;
}

void pintar(No *node)
{
  if (node->dir != NULL && node->dir->cor == VERMELHO)
  {
    return;
  }

  if (node->esq != NULL && node->esq->cor == VERMELHO)
  {
    return;
  }

  node->cor = VERMELHO;
  if (node->dir != NULL)
    node->dir->cor = PRETO;
  if (node->esq != NULL)
    node->esq->cor = PRETO;
}

void rot_esq(No **node)
{
  No *atual = *node;
  No *filho = atual->dir;
  if (filho == NULL)
  {
    fprintf(stderr, "Rotação errada\n");
    return;
  }

  atual->dir = filho->esq;
  filho->esq = atual;
  filho->cor = atual->cor;
  atual->cor = VERMELHO;
  *node = filho;
}

void rot_dir(No **node)
{
  No *atual = *node;
  No *filho = atual->esq;
  if (filho == NULL)
  {
    fprintf(stderr, "Rotação errada\n");
    return;
  }

  atual->esq = filho->dir;
  if (filho->dir != NULL)
    filho->dir->pai = atual;

  filho->dir = atual;
  filho->cor = atual->cor;
  atual->cor = VERMELHO;
  *node = filho;
  atual->pai = filho; // Atualização do pai após a rotação
}

void inserir_no(No **node, No *novo)
{
  if (*node == NULL)
  {
    *node = novo;
    return;
  }

  if ((*node)->num == novo->num)
  {
    printf("Número já existe\n");
    return;
  }

  if ((*node)->num < novo->num)
  {
    if ((*node)->dir == NULL)
    {
      (*node)->dir = novo;
      novo->pai = *node;
    }
    else
    {
      inserir_no(&(*node)->dir, novo);
    }
  }
  else
  {
    if ((*node)->esq == NULL)
    {
      (*node)->esq = novo;
      novo->pai = *node;
    }
    else
    {
      inserir_no(&(*node)->esq, novo);
    }
  }

  if (eh_vermelho((*node)->dir) && !eh_vermelho((*node)->esq))
    rot_esq(node);

  if (eh_vermelho((*node)->esq) && eh_vermelho((*node)->esq->esq))
    rot_dir(node);

  if (eh_vermelho((*node)->dir) && eh_vermelho((*node)->esq))
    pintar(*node);
}

void imprimir(No *node, int tab = 0)
{
  if (node == NULL)
    return;

  imprimir(node->dir, tab + 1);
  for (int i = 0; i < tab; i++)
  {
    printf("\t");
  }
  printf("%d\n", node->num);
  imprimir(node->esq, tab + 1);
}

void remover_no(No **raiz, int num)
{
  if (*raiz == NULL)
  {
    printf("A árvore está vazia.\n");
    return;
  }

  // Encontrar o nó a ser removido
  No *atual = *raiz;
  No *pai = NULL;
  No *no_remover = NULL;
  No *filho = NULL;

  while (atual != NULL)
  {
    if (num == atual->num)
    {
      no_remover = atual;
      break;
    }

    pai = atual;

    if (num < atual->num)
      atual = atual->esq;
    else
      atual = atual->dir;
  }

  if (no_remover == NULL)
  {
    printf("O nó não foi encontrado na árvore.\n");
    return;
  }

  // Determinar o filho do nó a ser removido
  if (no_remover->esq != NULL)
    filho = no_remover->esq;
  else
    filho = no_remover->dir;

  if (filho != NULL)
    filho->pai = pai;

  // Remover o nó da árvore
  if (pai == NULL)
    *raiz = filho;
  else if (no_remover == pai->esq)
    pai->esq = filho;
  else
    pai->dir = filho;

  free(no_remover);
}

void balanceamento(No **raiz)
{
  No *node = *raiz;

  while (node != NULL && node != *raiz && node->cor == PRETO)
  {
    No *pai = node->pai;
    No *irmao;

    if (node == pai->esq)
    {
      irmao = pai->dir;

      if (irmao->cor == VERMELHO)
      {
        // Caso 1: Irmão vermelho
        irmao->cor = PRETO;
        pai->cor = VERMELHO;
        rot_esq(&pai);
        irmao = pai->dir;
      }

      if ((irmao->esq == NULL || irmao->esq->cor == PRETO) &&
          (irmao->dir == NULL || irmao->dir->cor == PRETO))
      {
        // Caso 2: Pai, irmão e sobrinhos pretos
        irmao->cor = VERMELHO;
        node = pai;
      }
      else
      {
        if (irmao->dir == NULL || irmao->dir->cor == PRETO)
        {
          // Caso 3: Pai esquerdo, sobrinho direito preto
          irmao->esq->cor = PRETO;
          irmao->cor = VERMELHO;
          rot_dir(&irmao);
          irmao = pai->dir;
        }

        // Caso 4: Sobrinho direito vermelho
        irmao->cor = pai->cor;
        pai->cor = PRETO;
        irmao->dir->cor = PRETO;
        rot_esq(&pai);
        node = *raiz;
      }
    }
    else
    {
      irmao = pai->esq;

      if (irmao->cor == VERMELHO)
      {
        // Caso 1: Irmão vermelho
        irmao->cor = PRETO;
        pai->cor = VERMELHO;
        rot_esq(&pai);
        irmao = pai->esq;
      }

      if ((irmao->esq == NULL || irmao->esq->cor == PRETO) &&
          (irmao->dir == NULL || irmao->dir->cor == PRETO))
      {
        // Caso 2: Pai, irmão e sobrinhos pretos
        irmao->cor = VERMELHO;
        node = pai;
      }
      else
      {
        if (irmao->esq == NULL || irmao->esq->cor == PRETO)
        {
          // Caso 3: Pai direito, sobrinho esquerdo preto
          irmao->dir->cor = PRETO;
          irmao->cor = VERMELHO;
          rot_esq(&irmao);
          irmao = pai->esq;
        }

        // Caso 4: Sobrinho esquerdo vermelho
        irmao->cor = pai->cor;
        pai->cor = PRETO;
        irmao->esq->cor = PRETO;
        rot_dir(&pai);
        node = *raiz;
      }
    }
  }
  if (node != NULL)
    node->cor = PRETO;
}

void imprimir_cores(No *node)
{
  if (node == NULL)
    return;

  imprimir_cores(node->esq);

  if (node->cor == VERMELHO)
    printf("Nó %d: Vermelho\n", node->num);
  else
    printf("Nó %d: Preto\n", node->num);

  imprimir_cores(node->dir);
}

int main(int argc, char const *argv[])
{
  srand(time(NULL));

  No *no = criar_no(5);
  No *root = no;

  no = criar_no(6);
  inserir_no(&root, no);
  no = criar_no(4);
  inserir_no(&root, no);
  no = criar_no(8);
  inserir_no(&root, no);
  no = criar_no(1);
  inserir_no(&root, no);
  no = criar_no(3);
  inserir_no(&root, no);
  no = criar_no(7);
  inserir_no(&root, no);

  imprimir(root);
  balanceamento(&root);
  imprimir(root);
  imprimir_cores(root);

  printf("================\n");

  return 0;
}
