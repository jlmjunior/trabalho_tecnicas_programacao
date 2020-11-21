
infoProduto ConveterTextoParaProduto(char texto[200])
{
    infoProduto prod;

    char auxCodigo[10] = "\0";
    char auxPreco[10] = "\0";

    int y = 0;

    for (int i = 0; i < strlen(texto); i++)
    {
        if (texto[i] == ',')
        {
            y++;
            continue;
        }

        switch (y)
        {
            case 0:
                auxCodigo[strlen(auxCodigo)] = texto[i];
                break;
            case 1:
                prod.data_fabricacao[strlen(prod.data_fabricacao)] = texto[i];
                break;
            case 2:
                prod.data_validade[strlen(prod.data_validade)] = texto[i];
                break;
            case 3:
                prod.fabricante[strlen(prod.fabricante)] = texto[i];
                break;    
            case 4:
                prod.nome[strlen(prod.nome)] = texto[i];
                break;

            default:
                auxPreco[strlen(auxPreco)] = texto[i];
                break;
        }
    }

    prod.codigo = atoi(auxCodigo);
    prod.preco = atof(auxPreco);

    return prod;
}

infoProduto BuscarProduto(produto *listaProdutos, int codigo)
{
    infoProduto prod;

    prod.codigo = -1;

    if (listaProdutos == NULL)
    {
        return prod;
    }

    while (listaProdutos != NULL)
    {
        if (listaProdutos->info.codigo == codigo) return listaProdutos->info;

        listaProdutos = listaProdutos->proximo;
    }

    return prod;
}

infoServico BuscarServico(servico *listaServicos, int codigo)
{
    infoServico serv;

    serv.codigo = -1;

    if (listaServicos == NULL)
    {
        return serv;
    }

    while (listaServicos != NULL)
    {
        if (listaServicos->info.codigo == codigo) return listaServicos->info;

        listaServicos = listaServicos->prox;
    }

    return serv;
}

void ContabilizarFaturamento(float valor)
{
    FILE *arquivo = fopen("faturamento.txt", "r");
    char *leuArquivo;
    char linha[50] = "\0";

    if (arquivo == NULL)
    {
        puts("\n[ERRO AO LER ARQUIVO DE FATURAMENTO]\n");
        return;
    }

    char novoValor[50] = "\0";

    leuArquivo = fgets(linha, 200, arquivo);

    fclose(arquivo);

    arquivo = fopen("faturamento.txt", "w");
    
    if (leuArquivo || strlen(linha) > 0)
    {
        sprintf(novoValor, "%.2f", valor + atof(linha));
        fputs(novoValor, arquivo);
    }
    else
    {
        sprintf(novoValor, "%.2f", valor);
        fputs(novoValor, arquivo);
    }
    
    fclose(arquivo);
}

void GravarEstoque(produto *listaProdutos)
{
    if (listaProdutos == NULL)
    {
        FILE *arquivo = fopen("estoque.txt", "w");
        fclose(arquivo);
    }
    else
    {
        FILE *arquivo = fopen("estoque.txt", "w");

        if (arquivo == NULL)
        {
            puts("\n[FALHA AO CRIAR ARQUIVO DE ESTOQUE]\n");
            return;
        }

        while (listaProdutos != NULL)
        {
            char produtoFormatado[200];
            int resultado = 0;

            sprintf(produtoFormatado, "%d,%s,%s,%s,%s,%.2f%s"
                    ,listaProdutos->info.codigo
                    ,listaProdutos->info.data_fabricacao
                    ,listaProdutos->info.data_validade
                    ,listaProdutos->info.fabricante
                    ,listaProdutos->info.nome
                    ,listaProdutos->info.preco
                    ,(listaProdutos->proximo == NULL ? "" : "\n"));

            resultado = fputs(produtoFormatado, arquivo);

            if (resultado == EOF) puts("\n[FALHA AO GRAVAR NO ARQUIVO DE ESTOQUE]\n");
            
            listaProdutos = listaProdutos->proximo;
        }

        fclose(arquivo);
    }
}

bool ExisteCliente(cliente *listaClientes, char cpf[20])
{
    if (listaClientes == NULL)
    {
        return false;
    }

    while (listaClientes != NULL)
    {
        if (!strcmp(listaClientes->pessoa.cpf, cpf)) return true;

        listaClientes = listaClientes->prox;
    }

    return false;
}

bool ExisteServico(servico *listaServicos, int codigo)
{
    if (listaServicos == NULL)
    {
        return false;
    }

    while (listaServicos != NULL)
    {
        if (listaServicos->info.codigo == codigo) return true;

        listaServicos = listaServicos->prox;
    }

    return false;
}

bool ContemNoCarrinho(carrinho *listaCarrinho, int codigo)
{
    if (listaCarrinho == NULL)
    {
        return false;
    }

    while (listaCarrinho != NULL)
    {
        if (listaCarrinho->info.codigoProduto == codigo) return true;

        listaCarrinho = listaCarrinho->prox;
    }

    return false;
}

void EmitirComprovante(carrinho *listaCarrinho)
{
    char diretorio[40] = "\0";
    sprintf(diretorio, "./Comprovantes/comprovante_%ld.txt", time(NULL));

    FILE *arquivo = fopen(diretorio, "w");
    
    if (arquivo == NULL)
    {
        puts("Falha ao carregar arquivo de estoque");
        return;
    }

    time_t dataAtual = time(0);
    time(&dataAtual);

    float valorTotal = 0;
    char cabecalho[200] = "\0";

    sprintf(cabecalho, "#COMPROVANTE#\n\n%s\nCPF Cliente: %s\n", ctime(&dataAtual), listaCarrinho->info.cpf);

    fputs(cabecalho, arquivo);

    while (listaCarrinho != NULL)
    {
        char corpo[400] = "\0";

        sprintf(corpo, "\nCodigo: %d\nNome do produto: %s\nTarifa: R$ %.2f\n"
                ,listaCarrinho->info.codigoProduto
                ,listaCarrinho->info.nomeProduto
                ,listaCarrinho->info.precoProduto);
        strcat(corpo, "--------------------------------------------------------------");

        fputs(corpo, arquivo);
        
        valorTotal += listaCarrinho->info.precoProduto;

        listaCarrinho = listaCarrinho->prox;
    }

    char rodape[100] = "\0";

    sprintf(rodape, "\n\nTarifa total: R$ %.2f", valorTotal);

    fputs(rodape, arquivo);

    fclose(arquivo);

    puts("\n[COMPROVANTE GERADO]\n");

    ContabilizarFaturamento(valorTotal);
}

void AdicionarAoCarrinho(carrinho* *listaCarrinho, produto *listaProdutos , char cpf[20])
{
    int codigo;

    printf("[-] CODIGO DO PRODUTO: ");
    scanf("%d", &codigo);

    if (ContemNoCarrinho(*listaCarrinho, codigo))
    {
        puts("\n[PRODUTO JA ESTA NO CARRINHO]\n");

        system("pause");
        system("cls");

        return;
    }

    infoProduto prod = BuscarProduto(listaProdutos, codigo);

    if (prod.codigo != -1)
    {
        carrinho *no = (carrinho*) malloc (sizeof(carrinho));

        no->prox = NULL;
        no->info.codigoProduto = prod.codigo;
        no->info.precoProduto = prod.preco;
        strcpy(no->info.nomeProduto, prod.nome);
        strcpy(no->info.cpf, cpf);

        if (*listaCarrinho == NULL)
        {
            *listaCarrinho = no;
        }
        else
        {
            carrinho *aux = *listaCarrinho;
            
            while (aux->prox != NULL)
                aux = aux->prox;
                
            aux->prox = no;
        }

        puts("\n[PRODUTO ADICIONADO AO CARRINHO]\n");
    }
    else
    {
        puts("\n[PRODUTO NAO ENCONTRADO]\n");
    }

    system("pause");
    system("cls");
}

void ExcluirProduto(produto* *listaProdutos, int codigo)
{     
    produto *aux1 = *listaProdutos;
    produto *aux2 = aux1->proximo;
    
    if (aux1->info.codigo == codigo)
    {
        *listaProdutos = aux2;
        free(aux1);

        return;
    }

    while (aux2 != NULL)
    {
        if (aux2->info.codigo == codigo)
        {
            aux1->proximo = aux2->proximo;
            free(aux2);

            return;
        }

        aux1 = aux2;
        aux2 = aux2->proximo;		
    }
}

void LimparCarrinho(carrinho* *listaCarrinho)
{ 
    if (*listaCarrinho != NULL)
    {
	    carrinho *aux = *listaCarrinho;

        while( aux != NULL )
        {     
            *listaCarrinho = aux->prox;     
            free(aux);      
            aux = *listaCarrinho;               
		 }
	}      
}

void AdicionarVendido(carrinho* *listaVendidos, infoCarrinho produto)
{
    carrinho *no = (carrinho*) malloc (sizeof(carrinho));

    if (no != NULL)
    {
        no->info.codigoProduto = produto.codigoProduto;
        no->info.precoProduto = produto.precoProduto;
        strcpy(no->info.nomeProduto, produto.nomeProduto);
        strcpy(no->info.cpf, produto.cpf);

        no->prox = NULL;

        if (*listaVendidos == NULL) 
        {
            *listaVendidos = no;
        }
        else
        {
            carrinho *aux = *listaVendidos;

            while (aux->prox != NULL)
                aux = aux->prox;
            
            aux->prox = no;
        }
    }
    else 
    {

    }
}

void FinalizarCompra(carrinho* *listaCarrinho, produto* *listaProdutos, carrinho* *listaVendidos)
{
    carrinho *auxCarrinho = *listaCarrinho;

    if (*listaCarrinho == NULL)
    {
        puts("[CARRINHO VAZIO]\n");
    }
    else
    {
        EmitirComprovante(auxCarrinho);

        while (auxCarrinho != NULL)
        {
            AdicionarVendido(listaVendidos, auxCarrinho->info);
            ExcluirProduto(listaProdutos, auxCarrinho->info.codigoProduto);
            
            auxCarrinho = auxCarrinho->prox;
        }

        GravarEstoque(*listaProdutos);
        LimparCarrinho(listaCarrinho);

        puts("[COMPRA FINALIZADA COM SUCESSO]\n"); 
    }

    system("pause");
    system("cls");
}

int TamanhoListaProdutos(produto *listaProdutos)
{
    if (listaProdutos == NULL) return 0;

    int count = 0;

    while (listaProdutos != NULL)
    {
        listaProdutos = listaProdutos->proximo;
        count++;
    }

    return count;
}

bool EhIgnorado(int lista[], int tamanho, int codigo)
{
    for (int i = 0; i < tamanho; i++)
    {
        if (lista[i] == codigo) return true;
    }

    return false;
}