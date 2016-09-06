#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
//Desenvolvido por Patrick A. C. Guerra
/*
Sistema básico de controle de estoque apresentado em seminário em 15/03/2016.
OBS: 
-A biblioteca locale permite modificar os caracteres para um tabela diferente da padrão do c
-Verifique que é criada na memória RAM apenas um registro do tipo tipo_reg_estoque, 
e um do tipo_reg_fornecedor, no main, e as funções manipulam essas estruturas por ponteiros de endereço.
-fflush(stdin) é utilizado para limpar o buffer de caractere (local temporario 
em que fica armazenada a última tecla digitada)
*/

typedef struct Reg_Estoque{
	int codigo;
	char nome[20];
	int qtd_estoque;
	int cod_fornecedor;
	float preco;
}tipo_reg_estoque;

typedef struct Reg_Fornecedor{
	int cod_fornecedor;
	char nome[20];	
}tipo_reg_fornecedor;

FILE *p_estoque; 
FILE *p_fornecedor;
/* a função abre_arquivo_... que abre o arquivo com permissão de leitura, gravação e permite sobscrever ("alterar"),
que é a função que é chamada automaticamente ao executar o main; */
void abre_arquivo_estoque(void){
	p_estoque=fopen("Dados_Estoque.bin","r+b");
}

void abre_arquivo_fornecedor(void){
	p_fornecedor=fopen("Dados_Fornecedores.bin","r+b");
}
/*função limpa_... fecha o arquivo e abre com atributo de permissao de escrita e também limpa o arquivo, 
fecha novamente o arquivo, e chama 
a função abre_arquivo_...  */
void limpa_estoque(void){
	fclose(p_estoque);
	p_estoque=fopen("Dados_Estoque.bin","wb");
	fclose(p_estoque);
	abre_arquivo_estoque();	
}

void limpa_fornecedor(void){
	fclose(p_fornecedor);
	p_estoque=fopen("Dados_Fornecedores.bin","wb");
	fclose(p_fornecedor);
	abre_arquivo_fornecedor();	
}

//verifica o o codigo do último produto cadastrado
int localiza_cod_ultimo_produto(tipo_reg_estoque *estoque){
	int tmp_cod=0;
	//Coloca o leitor há um registro antes do final do arquivo binario
	fseek(p_estoque,(-1)*(int)sizeof(tipo_reg_estoque),SEEK_END);
	//Lê esse registro
	fread(estoque, sizeof(tipo_reg_estoque), 1, p_estoque);
	//Verifica se algum codigo foi encontrado difrente de zero
	if(estoque->codigo!=0) tmp_cod=estoque->codigo;
	//adiciona +1 ao ultimo codigo cadastrado e o retorna
	tmp_cod++;	
	return tmp_cod;
}

//verifica o o codigo do último fornecedor cadastrado
int localiza_cod_ultimo_fornecedor(tipo_reg_fornecedor *fornecedor){
	int tmp_cod=0;
	//Coloca o leitor há um registro antes do final do arquivo binario
	fseek(p_fornecedor,(-1)*(int)sizeof(tipo_reg_fornecedor),SEEK_END);
	//Lê esse registro
	fread(fornecedor, sizeof(tipo_reg_fornecedor), 1, p_fornecedor);
	//Verifica se algum codigo foi encontrado diferente de zero
	if(fornecedor->cod_fornecedor!=0) tmp_cod=fornecedor->cod_fornecedor;
	//adiciona +1 ao ultimo codigo cadastrado e o retorna
	tmp_cod++;	
	return tmp_cod;
}

int localiza_nome_fornecedor(tipo_reg_fornecedor *fornecedor,int cod){
	//Coloca o leitor no incio do arquivo
	rewind(p_fornecedor);
	//laço do while: execute o laço até atingir o fim do arquivo 
	//lê um registro para a memória e verifica se o registro atual possui código igual ao valor passado para a função na variavel cod
	//se encontrou retorna 1, senão retorna 0;
	do{
		fread(fornecedor, sizeof(tipo_reg_fornecedor), 1, p_fornecedor);
		if(fornecedor->cod_fornecedor==cod) {
			return 1;
		}
	}while(feof(p_fornecedor)==0);
	//feof(ponteiro_do_arquivo) verifica se chegou ao fim do arquivo, se sim retorna 1 senão chegou retorna 0;
	if(fornecedor->cod_fornecedor!=cod){
		return 0;
	}
}

void cadastra_produto(tipo_reg_estoque *estoque,tipo_reg_fornecedor *fornecedor){
	int continuar,verif_cad_fornecedor=-1;
	do{
		estoque->codigo=0;
		system("cls");
		system("color 2F");
		printf("		CONTROLE DE ESTOQUE");
		printf("\n\n--> Cadastro de Novos Produtos:\n\n");
		//chama função localiza_cod_ultimo_produto
		estoque->codigo=localiza_cod_ultimo_produto(estoque);
		printf("Digite o Nome do Produto:\n");
		fflush(stdin);
		scanf("%[A-Z a-z]s",estoque->nome);
		printf("Digite a Quantidade do Produto que há no Estoque:\n");	
		scanf("%d",&estoque->qtd_estoque);
		printf("Digite o Preço do Produto:\n");
		scanf("%f",&estoque->preco);
		printf("Digite o Código do Fornecedor do Produto:\n");
		fflush(stdin);
		scanf("%d",&estoque->cod_fornecedor);
		verif_cad_fornecedor=localiza_nome_fornecedor(fornecedor,estoque->cod_fornecedor);
		if(verif_cad_fornecedor==1){
			//posiciona o ponteiro no final do arquivo
			fseek(p_estoque, 0, SEEK_END);
			//escreve o registro no arquivo
			fwrite(estoque, sizeof(tipo_reg_estoque), 1, p_estoque);
			printf("\nProduto Cadastrado com sucesso!");	
		}else{
			system("cls");
			system("color 0C");
			printf("\nFornecedor Não Encontrado!");
			printf("\nCadastro Cancelado!");
		}
		printf("\n\nCadastrar outro (1-Sim / 0-Não): \n");
		fflush(stdin);
		scanf(" %d",&continuar);
	}while(continuar==1);	
}

void cadastra_fornecedor(tipo_reg_fornecedor *fornecedor){
	int continuar;
	do{
		fornecedor->cod_fornecedor=0;
		system("cls");
		system("color 2F");
		printf("		CONTROLE DE ESTOQUE");
		printf("\n\n--> Cadastro de Novos Fornecedores:\n\n");
		//chama função localiza_cod_ultimo_fornecedor
		fornecedor->cod_fornecedor=localiza_cod_ultimo_fornecedor(fornecedor);
		printf("Digite o Nome do Fornecedor:\n");
		fflush(stdin);
		scanf("%[A-Z a-z]s",fornecedor->nome);
		//posiciona o ponteiro no final do arquivo
		fseek(p_fornecedor, 0, SEEK_END);
		//escreve o registro no arquivo
		fwrite(fornecedor, sizeof(tipo_reg_fornecedor), 1, p_fornecedor);
		printf("\nFornecedor Cadastrado com sucesso!");
		printf("\n\nCadastrar outro (1-Sim / 0-Não): \n");
		fflush(stdin);
		scanf(" %d",&continuar);
	}while(continuar==1);	
}

void listar_produtos(tipo_reg_estoque *estoque,tipo_reg_fornecedor *fornecedor){
	char tmp;
	int encontrou_fornecedor;
	system("cls");
	system("color 2F");
	printf("		CONTROLE DE ESTOQUE");
	printf("\n\n--> Listagem de Produtos\n");
	printf("_____________________________________________________________________________");
	printf("\nCodigo     Nome               	  Quantidade     Preço        Fornecedor\n");
	printf("_____________________________________________________________________________");
	//Coloca o leitor no inicio do arquivo bianrio
	rewind(p_estoque);
	do{
		fread(estoque, sizeof(tipo_reg_estoque), 1, p_estoque);
		if(feof(p_estoque)==0) {
			encontrou_fornecedor=localiza_nome_fornecedor(fornecedor,estoque->cod_fornecedor);
			if(encontrou_fornecedor==1){
				printf("\n%6d     %-20s    %9d     R$%6.2f     %s ", estoque->codigo, estoque->nome, estoque->qtd_estoque, estoque->preco, fornecedor->nome);
			}else{
				printf("\n%6d     %-20s    %9d     R$%6.2f     Não Encontrado ", estoque->codigo, estoque->nome, estoque->qtd_estoque, estoque->preco);
			}
		}
	}while(feof(p_estoque)==0);
	printf("\n_____________________________________________________________________________");
	printf("\nPressione uma tecla para voltar ao menu...");
	fflush(stdin);
	scanf("%c",&tmp);
}

void listar_fornecedores(tipo_reg_fornecedor *fornecedor){
	char tmp;
	system("cls");
	system("color 2F");
	printf("		CONTROLE DE ESTOQUE");
	printf("\n\n--> Listagem de Fornecedores\n");
	printf("__________________________________________________");
	printf("\nCodigo     Nome do Fornecedor               	  \n");
	printf("__________________________________________________");
	//Coloca o leitor no inicio do arquivo bianrio
	rewind(p_fornecedor);
	//lê registros até chegar ao fim do arquivo
	do{
		fread(fornecedor, sizeof(tipo_reg_fornecedor), 1, p_fornecedor);
		if(feof(p_fornecedor)==0) {
			printf("\n%6d     %-20s", fornecedor->cod_fornecedor, fornecedor->nome);
		}
	}while(feof(p_fornecedor)==0);
	printf("\n__________________________________________________");
	printf("\nPressione uma tecla para voltar ao menu...");
	fflush(stdin);
	scanf("%c",&tmp);
}

void atualizar(int escolha, tipo_reg_estoque *estoque){
	int cod,continuar;
	do{
		system("cls");
		system("color 2F");
		printf("		CONTROLE DE ESTOQUE");
		printf("\n\n--> Atualizar Produto");
		printf("\nDigite o Código do Produto:");
		fflush(stdin);
		scanf("%d",&cod);
		rewind(p_estoque);
		do{
			fread(estoque, sizeof(tipo_reg_estoque), 1, p_estoque);
			if (cod==estoque->codigo) {
				printf("\nNome do Produto: %s",estoque->nome);
				printf("\nQuantidade: %d",estoque->qtd_estoque);
				printf("\nPreço: R$%.2f",estoque->preco);
				printf("\nFornecedor: %d",estoque->cod_fornecedor);
				if(escolha==0){
					printf("\nDigite o Novo Preço Para o Produto: ");
					scanf("%f",&estoque->preco);
					fseek(p_estoque,(-1)*(int)sizeof(tipo_reg_estoque),SEEK_CUR);
					fwrite(estoque, sizeof(tipo_reg_estoque), 1, p_estoque);
				}
				if(escolha==1){
					printf("\nDigite o Nova Quantidade de Estoque do Produto: ");
					scanf("%d",&estoque->qtd_estoque);
					fseek(p_estoque,(-1)*(int)sizeof(tipo_reg_estoque),SEEK_CUR);
					fwrite(estoque, sizeof(tipo_reg_estoque), 1, p_estoque);
				}
			}
		}while( feof(p_estoque)==0 && cod!=estoque->codigo );
		if(cod!=estoque->codigo){
			system("color 0C");
			printf("\n\nProduto não cadastrado!");
		}
		printf("\n\nAtualizar outro (1-Sim / 0-Não): \n");
		fflush(stdin);
		scanf(" %d",&continuar);
	}while(continuar==1);
}

main(){
/*Define a localidade de data e caracteres especiais para execução deste programa 
para padrão do sistema, inclusive ao utilizar números não inteiros, use 5,25
inteiro,casas decimais pois diferente do padrão c que usa ponto a virgula é padrão do sistema */
setlocale(LC_ALL,"");
//cria-se a estrutura Estoque na memória
tipo_reg_estoque Estoque;
//cria-se a estrutura Fornecedor na memória
tipo_reg_fornecedor Fornecedor;
//ponteiro que ira guardar endereço do tipo char
	char *tmp;
	int opcao;
	abre_arquivo_estoque();	
	abre_arquivo_fornecedor();
		do{
		opcao=-1;
		system("cls");
		system("color 3F");
		printf("============================================================\n");
		printf("||		----CONTROLE DE ESTOQUE----\n||");
		printf("\n||--> Opções: \n||");
		printf("\n||1 - Cadastrar Novo Produto");
		printf("\n||2 - Atualizar Preço dos Produtos");
		printf("\n||3 - Atualizar Estoque dos Produtos");
		printf("\n||4 - Listagem de todos os Produtos");
		printf("\n||5 - Cadastrar Novo Fornecedor");
		printf("\n||6 - Listagem de todos os Fornecedores");
		printf("\n||7 - Criar ou Limpar o Arquivo de Dados do Estoque");
		printf("\n||8 - Criar ou Limpar o Arquivo de Dados dos Fornecedores");
		printf("\n||\n||0 - Sair\n||");
		printf("\n||Digite a opção: ");
		scanf(" %d",&opcao);
		if(opcao!=0){
			switch(opcao){
				case 1: cadastra_produto(&Estoque,&Fornecedor); break;
				case 2: atualizar(0,&Estoque); break;
				case 3: atualizar(1,&Estoque); break;
				case 4: listar_produtos(&Estoque,&Fornecedor); break;
				case 5: cadastra_fornecedor(&Fornecedor); break;
				case 6: listar_fornecedores(&Fornecedor); break;
				case 7: limpa_estoque(); break;
				case 8: limpa_fornecedor(); break;
				default: 
				system("cls");
				system("color 0C");
				printf("\n||Opcao invalida!");
				printf("\n||Pressione uma tecla para voltar ao menu...");
				tmp = malloc(sizeof(char));
				fflush(stdin);
				scanf("%c",tmp);
				free(tmp);
				tmp=NULL;
				break;
			}
		}
	}while(opcao!=0);
	//fecha o arquivo de dados
	fclose(p_estoque);
	system("cls");
	system("color 1F");
	system("PAUSE");
}

