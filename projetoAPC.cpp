#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NON_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Onde no mundo esta o dinheiro do Brasil?


struct tipoCasos {

	char descricao[100];
	int id;
	int dificuldade;
};

struct tipoJogador {
	
	char nome[50];
	int level;

};

struct tipoAdm {

	char nome[50];
	int senha;
};



//adm
int loginadm(tipoAdm adm);  //retorna 1 se o login foi efetuado, 0 se nao. 
void admconfig();
int addcaso();		// retorna 1 se a insercao foi um sucesso, 0 se nao.

void cripto(int pass, int action); //funcao de criptografia de senhas, parametros ->> senha = senha a ser criptografada
								   //   ->> action = acao que sera feita, ou seja,
								   //       1 para criptografar 0 para descriptografar



void main() {

	// Abertura da referencia dos arquivos
	FILE *casos = fopen("casos.txt", "r+");
	if (!casos)
		printf("ERRO: Arquivo de casos nao encontrado!!\n");
	FILE *usuarios = fopen("users.txt", "r+");
	if (!usuarios)
		printf("ERRO: Arquivo de usuarios nao encontrado!!\n");


	tipoJogador jogador;
	tipoAdm adm;
	
	int op=0,tentativas=3;


	printf("Where is Sergio Cabral in the World?\n\n");
	printf("Bem-vindo,");
	printf("Se voce for um Procurador Geral da Republica digite 1\n");
	printf("Se voce for um detetive em campo digite 2\n");
	printf("Digite 0 para sair\n");
	scanf("%i", &op);

	do {
		switch (op) {

		case 1://ADM
			while (tentativas > 0) {

				printf("Procurador no teclado, digite seu nome...\n\n");
				gets_s(adm.nome);
				printf("Otimo, agora digite sua senha...\n");
				printf("\t\tATENCAO VERIFIQUE SE NAO HA NINGUEM TE OBSERVANDO\n\n");
				scanf("%i", &adm.senha);
				system("cls");

				if (loginadm(adm) == 1) {
					//login efetuado com sucesso
					tentativas = -1;
				}
				else {
					//login falhou, senha ou usuario incorreto
					//atualiza as tentativas, no maximo 3 tentativas
					system("cls");
					tentativas--;
					printf("ERRO!, senha ou usuario incorreto\n\n");
					printf("Tentativas restantes ->> %i \n", tentativas);
				}
			}

			if (tentativas = -1) {
				//Menu do ADM
				admconfig();

			}
			else {

				system("cls");
				printf("Tentativas esgotadas\n");

			}


			break;

		case 2://JOGADOR
			printf("Detetive no teclado, digite seu usuario...\n\n");
			gets_s(jogador.nome);

			break;


		case 0://SAIR
			system("cls");
			break;


		default:
			printf("Opcao invalida\n");
			break;
		}
		

	} while (op != 0);
		

		


}

int loginadm(tipoAdm adm) {			//retorna 1 se o login foi efetuado, 0 se nao.
/*FUNCIONAMENTO DOS ARQUIVOS DE LOGIN
		<usuario:senha>
		Ex.

		<iago:1234>		
	*/

	

	FILE *admdata = fopen("data.txt", "r+");
	if (!admdata)
		printf("ERRO: Arquivos do adm inacessiveis!!\n");

	int tam = strlen(adm.nome);
	char c,usuarioArq[50];
	int i=0;


	while (!feof(admdata))			//ate o final do arquivo
	{

		c = fgetc(admdata);

		if (c == '<' || c == '>'){
		// se for os separador pego o proximo caracter
	
		c = fgetc(admdata);
			
		}
		else
		{

			while (c != ':')
			{
				usuarioArq[i] = c;
				c = fgetc(admdata);
				i++;

			}

			if(strcmp(usuarioArq,adm.nome) != 0){
			// se o usuario pego do arquivo for diferente do digitado 	
				while (c != '<' || !feof(admdata)) {
					//desloco o cursor ate o proximo usuario cadastrado, ou ate o final do arq
					c = fgetc(admdata);

				}
			}

			else {
				//se nao verifico a senha

				if (c == ':') {

					c = fgetc(admdata);
				}

				// preciso trasformar o char em int e verificar a senha
				

			}


		}


	}

	return 0;




}

void admconfig() {
	//menu de configuracao do adm




}

void cripto(int pass, int action)
{

	if (action == 1) {		// criptografar
						




	}
	else {
	
					//descriptografar
	
	}
	




}
