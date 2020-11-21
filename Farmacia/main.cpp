#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include "estrutura.h"
#include "util.h"

void CadastrarCliente(cliente* *listaClientes);
void CadastrarProduto(produto* *listaProdutos);
void ImprimeClientes(cliente *listaClientes);
void ImprimeProdutos(produto *listaProdutos);
void ImprimeVendidos(carrinho *listaVendidos);
void CarregarProdutos(produto* *listaProdutos);
void VenderProduto(produto* *listaProdutos, cliente *listaClientes, carrinho* *listaVendidos);
void CadastrarServico(servico* *listaServicos);
void ExecutarServico(servico *listaServicos);
void VerFaturamento();
void ImprimeProdutosPorValor(produto *listaProdutos);

int main()
{ 
    cliente *listaClientes = NULL;
    produto *listaProdutos = NULL;
    servico *listaServicos = NULL;
    carrinho *listaVendidos = NULL;

    int menu;

    CarregarProdutos(&listaProdutos);

    while (true)
    {
        puts("# MENU PRINCIPAL\n"); 
        puts("[1] Cadastrar Cliente");
        puts("[2] Imprimir Clientes");
        puts("[3] Cadastrar produto");
        puts("[4] Imprimir Produtos");
        puts("[5] Efetuar Venda");
        puts("[6] Cadastrar Servico");
        puts("[7] Executar Servico");
        puts("[8] Ver Faturamento");
        puts("[9] Imprimir Produtos de maior valor");
        puts("[10] Imprime Vendidos");
        puts("[0] Sair");

        printf("\n[-]: ");
        fflush(stdin);
        scanf("%d", &menu);

        system("cls");

        switch (menu)
        {
            case 0:
                exit(true);

            case 1:
                CadastrarCliente(&listaClientes);
                break;
            
            case 2:
                ImprimeClientes(listaClientes);
                break;
            case 3:
                CadastrarProduto(&listaProdutos);
                GravarEstoque(listaProdutos);
                break;  
            case 4:
                ImprimeProdutos(listaProdutos);
                break;
            case 5:
                VenderProduto(&listaProdutos, listaClientes, &listaVendidos);
                break;
            case 6:
                CadastrarServico(&listaServicos);
                break;
            case 7:
                ExecutarServico(listaServicos);
                break;
            case 8:
                VerFaturamento();
                break;
            case 9:
                ImprimeProdutosPorValor(listaProdutos);
                break;
            case 10:
                ImprimeVendidos(listaVendidos);
                break;
                
            default:
                puts("[Opcao invalida]\n");
                break;
        }
    }

    return 0;
}

void CadastrarCliente (cliente* *listaClientes)
{   
    cliente *no = (cliente*) malloc (sizeof(cliente));

    if (no != NULL)
    {        
        puts("# CADASTRO DE CLIENTE\n");

        printf("CPF: ");
        fflush(stdin);
        gets(no->pessoa.cpf);

        if (ExisteCliente(*listaClientes, no->pessoa.cpf))
        {
            free(no);
            puts("\n[CPF INVALIDO! Cliente ja cadastrado]\n");
            
            system("pause");
            system("cls");

            return;
        }

        printf("NOME: ");
        fflush(stdin);
        gets(no->pessoa.nome);

        printf("ENDERECO: ");
        fflush(stdin);
        gets(no->pessoa.endereco);

        no->prox = NULL;

        if (*listaClientes == NULL) 
        {
            *listaClientes = no;
        }
        else
        {
            cliente *aux = *listaClientes;

            while (aux->prox != NULL)
                aux = aux->prox;
            
            aux->prox = no;
        }
    } 
    else
    {
        puts("Erro ao alocar memoria");
    }

    puts("\n[CLIENTE CADASTRADO COM SUCESSO]\n");
    system("pause");
    system("cls");
}

void ImprimeClientes(cliente *listaClientes)
{
    int countLista = 0;

    if (listaClientes == NULL)
    {
        puts("[NENHUM CLIENTE CADASTRADO]\n");
    }
    else
    {
        while (listaClientes != NULL)
        {
            puts("----------------------------------------------------");
            printf("Cliente: %d", countLista + 1);
            printf("\nNome: %s", listaClientes->pessoa.nome);
            printf("\nCPF: %s\n", listaClientes->pessoa.cpf);
            puts("");

            listaClientes = listaClientes->prox;
            countLista++;
        }

        puts("----------------------------------------------------");
        printf("[?] Quantidade de clientes: %d\n\n", countLista);
    }

    system("pause");
    system("cls");
}

void CadastrarProduto(produto* *listaProdutos) 
{   
    produto *no = (produto*) malloc (sizeof(produto));

    puts("# CADASTRO DE PRODUTO\n");

    printf("CODIGO DO PRODUTO: ");
    scanf("%d", &no->info.codigo);

    bool existeProduto = BuscarProduto(*listaProdutos, no->info.codigo).codigo != -1 ? true : false;

    if (existeProduto || no->info.codigo < 0)
    {
        free(no);
        puts("\n[CODIGO INVALIDO! Produto ja cadastrado ou codigo negativo]\n");
        
        system("pause");
        system("cls");

        return;
    }

    printf("NOME DO PRODUTO: ");
    fflush(stdin);
    gets(no->info.nome);
    
    printf("FABRICANTE: ");
    fflush(stdin);
    gets(no->info.fabricante);

    printf("DATA DE VALIDADE: ");
    fflush(stdin);
    gets(no->info.data_validade);
    
    printf("DATA DE FABRICACAO ");
    fflush(stdin);
    gets(no->info.data_fabricacao);

    printf("PRECO: ");
    scanf("%f", &no->info.preco);

    no->proximo = NULL;

    if (*listaProdutos == NULL)
    {
        *listaProdutos = no;
    }
    else
    {
        produto *aux = *listaProdutos;
        
        while (aux->proximo != NULL)
            aux = aux->proximo;
            
        aux->proximo = no;
    }

    puts("\n[PRODUTO CADASTRADO COM SUCESSO]\n");

    system("pause");
    system("cls");
}

void CarregarProdutos(produto* *listaProdutos)
{
    FILE *arquivo = fopen("estoque.txt", "r");
    char *leuArquivo;
    char linha[200] = "\0";

    if (arquivo == NULL)
    {
        puts("[ERRO AO CARREGAR ARQUIVO DE ESTOQUE]\n");
        return;
    }

    while (!feof(arquivo))
    {
        leuArquivo = fgets(linha, 200, arquivo);
        
        if (!leuArquivo || strlen(linha) == 0) break; 

        produto *no = (produto*) malloc (sizeof(produto));

        if (no == NULL)
        {
            puts("\nErro ao alocar memoria");
            return;
        }

        no->info = ConveterTextoParaProduto(linha);
        no->proximo = NULL;

        if (*listaProdutos == NULL) 
        {
            *listaProdutos = no;
        }
        else
        {
            produto *aux = *listaProdutos;

            while (aux->proximo != NULL) 
                aux = aux->proximo;
            
            aux->proximo = no;
        }
    }

    fclose(arquivo);
}

void ImprimeProdutos(produto *listaProdutos)
{
    int count = 0;

    if (listaProdutos == NULL)
    {
        puts("[LISTA VAZIA]\n");
    }
    else
    {
        while (listaProdutos != NULL)
        {
            puts("----------------------------------------------------");
            printf("Codigo: %d", listaProdutos->info.codigo);
            printf("\nNome: %s", listaProdutos->info.nome);
            printf("\nPreco: %.2f", listaProdutos->info.preco);
            printf("\nData fabricacao: %s", listaProdutos->info.data_fabricacao);
            printf("\nData validade: %s", listaProdutos->info.data_validade);
            printf("\nFabricante: %s\n", listaProdutos->info.fabricante);
            puts("");

            listaProdutos = listaProdutos->proximo;
            count++;
        }

        puts("----------------------------------------------------");
        printf("[?] Quantidade de produtos: %d\n\n", count);
    }

    system("pause");
    system("cls");
}

void VenderProduto(produto* *listaProdutos, cliente *listaClientes, carrinho* *listaVendidos)
{
    if (*listaProdutos == NULL)
    {
        puts("[NENHUM PRODUTO CADASTRADO]\n");
    }
    else
    {
        char cpf[20] = "\0";

        printf("[-] CPF DO CLIENTE: ");
        fflush(stdin);
        gets(cpf);

        system("cls");
        
        if (ExisteCliente(listaClientes, cpf))
        {
            bool continuarMenu = true;
            carrinho *listaCarrinho = NULL;

            while (continuarMenu)
            {
                int carrinho;

                puts("# MENU DE VENDA\n");
                puts("[1] Adicionar ao carrinho");
                puts("[2] Finalizar compra");
                puts("[9] Cancelar");
                
                printf("\n[-]: ");
                scanf("%d", &carrinho);

                system("cls");

                switch (carrinho)
                {
                    case 1:
                        AdicionarAoCarrinho(&listaCarrinho, *listaProdutos, cpf);
                        break;
                    case 2:
                        FinalizarCompra(&listaCarrinho, listaProdutos, listaVendidos);
                        break;
                    case 9:
                        continuarMenu = false;
                        break;

                    default:
                        puts("[OPCAO INVALIDA]\n");
                        break;
                }
            }

            LimparCarrinho(&listaCarrinho);
        }
        else
        {
            puts("[CLIENTE NAO ENCONTRADO]\n");
        }
    }
    
    system("pause");
    system("cls");
}

void CadastrarServico(servico* *listaServicos)
{
    servico *no = (servico*) malloc (sizeof(servico));

    if (no != NULL)
    {
        puts("# CADASTRO DE SERVICO\n");

        printf("CODIGO DO SERVICO: ");
        scanf("%d", &no->info.codigo);

        if (ExisteServico(*listaServicos, no->info.codigo))
        {
            free(no);
            puts("\n[CODIGO INVALIDO! Servico ja cadastrado]\n");
            
            system("pause");
            system("cls");

            return;
        }
        
        printf("NOME DO SERVICO: ");
        fflush(stdin);
        gets(no->info.nome);

        printf("PRECO: ");
        scanf("%f", &no->info.preco);

        no->prox = NULL;

        if (*listaServicos == NULL) 
        {
            *listaServicos = no;
        }
        else
        {
            servico *aux = *listaServicos;

            while (aux->prox != NULL)
                aux = aux->prox;
            
            aux->prox = no;
        }

        puts("\n[SERVICO CADASTRADO COM SUCESSO]\n");
    } 
    else
    {
        puts("\n[ERRO AO ALOCAR MEMORIA]\n");
    }

    system("pause");
    system("cls");
}

void ExecutarServico(servico *listaServicos)
{
    if (listaServicos == NULL)
    {
        puts("[NENHUM SERVICO CADASTRADO]\n");
    }
    else
    {
        servico *auxLista = listaServicos;
        
        puts("# EXECUCAO DE SERVICO\n");

        while (auxLista != NULL)
        {
            puts("----------------------------------------------------");
            printf("Codigo: %d", auxLista->info.codigo);
            printf("\nNome: %s", auxLista->info.nome);
            printf("\nPreco: %.2f\n", auxLista->info.preco);
            puts("");

            auxLista = auxLista->prox;
        }

        puts("----------------------------------------------------");

        int codigoServico;

        printf("[-]: ");
        scanf("%d", &codigoServico);

        if (ExisteServico(listaServicos, codigoServico))
        {
            infoServico serv = BuscarServico(listaServicos, codigoServico);

            ContabilizarFaturamento(serv.preco);

            puts("\n[SERVICO EXECUTADO COM SUCESSO]\n");
        }
        else
        {
            puts("\n[SERVICO NAO ENCONTRADO]\n");
        }
        
    }

    system("pause");
    system("cls");    
}

void VerFaturamento()
{
    FILE *arquivo = fopen("faturamento.txt", "r");
    char *leuArquivo;
    char linha[20] = "\0";

    puts("# FATURAMENTO\n");

    if (arquivo == NULL)
    {
        puts("\n[ERRO AO LER ARQUIVO DE FATURAMENTO]\n");
    }
    else
    {
        leuArquivo = fgets(linha, 20, arquivo);

        printf("TOTAL FATURADO: R$ %s\n\n", (strlen(linha) > 0 ? linha : "0.00"));
    }

    fclose(arquivo);

    system("pause");
    system("cls");
}

void ImprimeProdutosPorValor(produto *listaProdutos)
{
    int maxRank = 5;

    if (listaProdutos == NULL)
    {
        puts("[LISTA VAZIA]");
    }
    else
    {
        puts("# TOP 5 VALOR\n");

        int tamanhoLista = TamanhoListaProdutos(listaProdutos);
        int tamanhoRank = (tamanhoLista > maxRank ? maxRank : tamanhoLista);

        int ignorados[tamanhoRank], countIgnorados = 0;

        float tabela[tamanhoRank][2];

        for (int i = 0; i < tamanhoRank; i++)
        {
            produto *aux = listaProdutos;
            float atual[2] = {0,0};

            while (aux != NULL)
            {
                if (aux->info.preco > atual[1] && !EhIgnorado(ignorados, countIgnorados, aux->info.codigo))
                {
                    atual[0] = (float)aux->info.codigo;
                    atual[1] = aux->info.preco;
                }

                aux = aux->proximo;
            }

            tabela[i][0] = atual[0];
            tabela[i][1] = atual[1];

            ignorados[i] = (int)tabela[i][0];
            countIgnorados++;
        }

        for (int i = 0; i < tamanhoRank; i++)
        {
            puts("------------------------------------------");
            printf("(%d) PRODUTO: %.0f // VALOR: %.2f\n", i+1, tabela[i][0], tabela[i][1]);
        }
    }
    
    puts("");
    system("pause");
    system("cls");
}

void ImprimeVendidos(carrinho *listaVendidos)
{
    if (listaVendidos == NULL)
    {
        puts("[NENHUM PRODUTO VENDIDO]\n");
    }
    else
    {
        while (listaVendidos != NULL)
        {
            puts("----------------------------------------------------");
            printf("\nNome: %s", listaVendidos->info.nomeProduto);
            printf("\nPreco: %.2f\n", listaVendidos->info.precoProduto);
            puts("");

            listaVendidos = listaVendidos->prox;
        }

        puts("----------------------------------------------------");
    }

    system("pause");
    system("cls");
}