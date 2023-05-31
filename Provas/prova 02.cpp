#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum Cor
{
  VERMELHO = 1,
  PRETO = 0
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
  node->pai = NULL;
  node->esq = NULL;
  node->dir = NULL;
  node->cor = VERMELHO;

  return node;
}

void rot_esq(No **raiz, No *atual)
{
  No *filho = atual->dir;
  if (filho == NULL)
  {
    fprintf(stderr, "Rotação errada\n");
    return;
  }
  atual->dir = filho->esq;
  if (filho->esq != NULL)
  {
    filho->esq->pai = atual;
  }

  filho->esq = atual;
  filho->pai = atual->pai;
  atual->pai = filho;

  if (filho->pai == NULL)
  {
    *raiz = filho;
    filho->cor = PRETO;
  }
  else
  {
    if (filho->pai->esq == atual)
    {
      filho->pai->esq = filho;
    }
    else
    {
      filho->pai->dir = filho;
    }
    return;
  }
}

void rot_dir(No **raiz, No *atual)
{
  No *filho = atual->esq;
  if (filho == NULL)
  {
    fprintf(stderr, "Rotação errada\n");
    return;
  }
  atual->esq = filho->dir;
  if (filho->dir != NULL)
  {
    filho->dir->pai = atual;
  }

  filho->dir = atual;
  filho->pai = atual->pai;
  atual->pai = filho;

  if (filho->pai == NULL)
  {
    *raiz = filho;
    filho->cor = PRETO;
  }
  else
  {
    if (filho->pai->dir == atual)
    {
      filho->pai->dir = filho;
    }
    else
    {
      filho->pai->esq = filho;
    }
    return;
  }
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
  printf("%d %d\n", node->num, node->cor);
  imprimir(node->esq, tab + 1);
}

void remover_no(No **raiz, No *no_remover)
{
  if (*raiz == NULL)
  {
    return;
  }
  // Encontrar o nó a ser removido
  if (no_remover->pai->dir == no_remover)
  {
    no_remover->pai->dir = no_remover->esq;
    if (no_remover->esq != NULL)
    {
      no_remover->esq->pai = no_remover->pai;
    }

    return;
  }
  else
  {
    no_remover->pai->esq = no_remover->dir;
    if (no_remover->dir != NULL)
    {
      no_remover->dir->pai = no_remover->pai;
    }
    return;
  }
}

void remocao(No **raiz, No *node)
{
  // 1
  if (node->cor == VERMELHO)
  {
    printf("\nVERMELHO\n");
    remover_no(raiz, node);
    return;
  }
  // 2.1
  No *irmao = NULL;
  if (node->pai->dir != node && node->pai->dir != NULL && node->pai->dir->cor == PRETO)
  {
    irmao = node->pai->dir;
    if (irmao->dir == NULL && irmao->esq == NULL)
    {
      node->pai->cor = PRETO;
      irmao->cor = VERMELHO;
      remover_no(raiz, node);
      return;
    }
    else if (irmao->dir == NULL && irmao->esq != NULL && irmao->esq->cor == VERMELHO)
    {
      node->pai->cor = PRETO;
      irmao->cor = VERMELHO;
      remover_no(raiz, node);
      return;
    }
  }
  else if (node->pai->esq != node && node->pai->esq != NULL && node->pai->esq->cor == PRETO)
  {
    irmao = node->pai->esq;
    if (irmao->dir == NULL && irmao->esq == NULL)
    {
      node->pai->cor = PRETO;
      irmao->cor = VERMELHO;
      remover_no(raiz, node);
      return;
    }
    else if (irmao->esq == NULL && irmao->dir != NULL && irmao->dir->cor == VERMELHO)
    {
      node->pai->cor = PRETO;
      irmao->cor = VERMELHO;
      remover_no(raiz, node);
      return;
    }
  }
  // 2.2
  if (node->pai->esq != node && node->pai->esq != NULL && node->pai->esq->cor == PRETO)
  {
    irmao = node->pai;
    if (irmao->esq->cor == VERMELHO)
    {
      if (node->pai->cor == PRETO)
      {
        irmao->esq->cor = PRETO;
      }
      else
      {
        irmao->esq->cor = VERMELHO;
      }
      rot_dir(raiz, node->pai);
      remover_no(raiz, node);
      return;
    }
  }
  else if (node->pai->dir != node && node->pai->dir != NULL && node->pai->dir->cor == PRETO)
  {
    irmao = node->pai;
    if (irmao->dir->cor == VERMELHO)
    {
      if (node->pai->cor == PRETO)
      {
        irmao->dir->cor = PRETO;
      }
      else
      {
        irmao->dir->cor = VERMELHO;
      }
      rot_esq(raiz, node->pai);
      remover_no(raiz, node);
      return;
    }
  }
  /// 2.3
  if (node->pai->esq != node && node->pai->esq != NULL && node->pai->esq->cor == VERMELHO)
  {
    rot_dir(raiz, node->pai);
    remover_no(raiz, node);
    if (node->pai->cor == PRETO)
    {
      node->pai->esq->cor = VERMELHO;
    }
    return;
  }
  else if (node->pai->dir != node && node->pai->dir != NULL && node->pai->dir->cor == VERMELHO)
  {
    rot_esq(raiz, node->pai);
    remover_no(raiz, node);
    if (node->pai->cor == PRETO)
    {
      node->pai->dir->cor = VERMELHO;
    }
    return;
  }
}

No *maior(No *node)
{
  if (node == NULL)
    return NULL;
  if (node->dir == NULL)
    return node;
  return maior(node->dir);
}

No *menor(No *node)
{
  if (node == NULL)
    return NULL;
  if (node->esq == NULL)
    return node;
  return menor(node->esq);
}

void remocao_de_fato(No **raiz, No *node, int num)
{
  if (node == NULL)
  {
    return;
  }

  remocao_de_fato(raiz, (node)->esq, num);
  remocao_de_fato(raiz, (node)->dir, num);

  No *removido = node;
  No *aux = NULL;
  if (node->num == num)
  {

    aux = maior(node->esq);

    if (aux == NULL)
    {
      aux = menor(node->dir);
    }
    if (aux == NULL)
    {
      aux = removido;
    }
    // ele é o proprio caso
    if (removido == aux)
    {
      if (removido->pai == NULL)
      {
        *raiz = NULL;
        return;
      }
      else
      {
        remocao(raiz, removido);

        return;
      }
    }
    else
    {
      // não é o proprio caso, existe um maior ou menor
      remocao(raiz, aux);

      aux->pai = removido->pai;
      aux->esq = removido->esq;
      aux->dir = removido->dir;
      aux->cor = removido->cor;

      if (removido->esq != NULL)
      {
        removido->esq->pai = aux;
      }

      if (removido->dir != NULL)
      {
        removido->dir->pai = aux;
      }
      if (removido->pai == NULL)
      {
        *raiz = aux;
        aux->cor = PRETO;
      }
      else
      {
        if (removido->pai->dir == removido)
        {
          removido->pai->dir = aux;
        }
        else
        {
          removido->pai->esq = aux;
        }
      }
      return;
    }
  }
}

void balanceamento(No **raiz, No *node)
{
  if (node->pai == NULL)
  {
    node->cor = PRETO;
    return;
  }

  if (node->pai->cor == PRETO)
  {

    return;
  }

  No *avo = node->pai->pai;
  No *tio = NULL;
  if (avo->dir != node->pai)
  {
    tio = avo->dir;
  }
  if (avo->esq != node->pai)
  {
    tio = avo->esq;
  }

  if (tio != NULL)
  {
    if (tio->cor == VERMELHO && node->pai->cor == VERMELHO)
    {
      tio->cor = PRETO;
      node->pai->cor = PRETO;
      if (avo->pai != NULL)
      {
        avo->cor = VERMELHO;
      }
      return;
    }
  }
  if ((tio == NULL && node->pai->cor == VERMELHO) ||
      ((tio->cor == PRETO) && node->pai->cor == VERMELHO))
  {

    if (node->pai == avo->esq && node == node->pai->esq)
    {
      rot_dir(raiz, avo);
      node->pai->cor = PRETO;
      avo->cor = VERMELHO;
      return;
    }
    if (node->pai == avo->dir && node == node->pai->dir)
    {
      rot_esq(raiz, avo);
      node->pai->cor = PRETO;
      avo->cor = VERMELHO;
      return;
    }
    if (node->pai == avo->esq && node == node->pai->dir)
    {
      rot_esq(raiz, node->pai);
      rot_dir(raiz, avo);
      node->cor = PRETO;
      avo->cor = VERMELHO;
      return;
    }
    if (node->pai == avo->dir && node == node->pai->esq)
    {
      rot_dir(raiz, node->pai);
      rot_esq(raiz, avo);
      node->cor = PRETO;
      avo->cor = VERMELHO;
      return;
    }
  }
}

void balanceamento_total(No **raiz, No *node)
{
  if (node == NULL)
  {
    return;
  }

  if (node->cor == VERMELHO)
  {
    balanceamento(raiz, node);
  }
  balanceamento_total(raiz, node->dir);
  balanceamento_total(raiz, node->esq);
}

void imprimir_cores(No *node)
{
  if (node == NULL)
    return;

  imprimir_cores(node->esq);

  if (node->cor == VERMELHO)
    printf("Nó %d: Vermelho\n", node->num);
  else
  {
    printf("Nó %d: Preto\n", node->num);
  }

  imprimir_cores(node->dir);
}

int main(int argc, char const *argv[])
{
  int op = 0;
  No *root = NULL;
  No *no = NULL;
  int num = 0;

  do
  {
    printf("----- MENU -----\n");
    printf("1 - Inserir no.\n");
    printf("2 - Imprimir arvore.\n");
    printf("3 - Remover no.\n");
    printf("0 - Sair\n");

    printf("Opcao: ");
    scanf("%d", &op);

    switch (op)
    {
    case 1:
      printf("Digite um numero: ");
      scanf("%d", &num);
      no = criar_no(num);
      inserir_no(&root, no);
      balanceamento_total(&root, root);
      break;

    case 2:
      imprimir(root);
      break;

    case 3:
      printf("Digite o numero a ser removido: ");
      scanf("%d", &num);
      remocao_de_fato(&root, root, num);
      balanceamento_total(&root, root);
      break;

    case 0:
      printf("Saindo...\n");
      break;

    default:
      printf("Opcao invalida\n");
      break;
    }
  } while (op != 0);

  return 0;
}
