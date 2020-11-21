typedef struct
{   
    char nome[40];
    char cpf[20];
    char endereco[100];
    char telefone[20];

} cadastro;
       
typedef struct nodoCliente
{
    cadastro pessoa;    
    struct nodoCliente* prox; 

} cliente;

typedef struct
{
    int codigo;
    float preco;
    char nome[45] = "\0";
    char fabricante[45] = "\0";
    char data_validade[11] = "\0";
    char data_fabricacao[11] = "\0";
    // int desconto; podemos pensar nisso depois

} infoProduto;

typedef struct nodoProduto {   

    infoProduto info;
    struct nodoProduto *proximo;

} produto;

typedef struct
{   
    int codigoProduto;
    float precoProduto;
    char nomeProduto[45] = "\0";
    char cpf[20] = "\0";

} infoCarrinho;     

typedef struct nodoCarrinho
{
    infoCarrinho info;    
    struct nodoCarrinho* prox; 

} carrinho;

typedef struct
{
    int codigo;
    float preco;
    char nome[45] = "\0";

} infoServico;

typedef struct nodoServico {   

    infoServico info;
    struct nodoServico *prox;

} servico;