#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NON_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define DELAY 15 // duracao do delay do slowprint



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
	char senha[10];
};

tipoAdm admLog; // GLOBAL do adm logado
tipoJogador jogadorLog;

// printa devagar
void slowprint(char s[], int delay){

	int tam = strlen(s);
	int i;

	for (i = 0; i < tam; i++){

		printf("%c", s[i]);
		Sleep(delay);

	}

}



//adm
int loginadm(tipoAdm adm);  //retorna 1 se o login foi efetuado, 0 se nao. 

int admconfig(tipoAdm adm);	// retorna 0 quando pedido pra sair

int addcaso();	// retorna 1 se a insercao foi um sucesso, 0 se nao.

int alterardados(tipoAdm adm); //retorna 1 se a alteracao foi feita, 0 se nao

//===============================================================================================


void main() {

	system("color 0A");
	// Abertura da referencia dos arquivos
	FILE *casos = fopen("casos.apc", "rb+");
	if (!casos){
		printf("ERRO: Arquivo de casos nao encontrado!!\n");
		system("pause");
		exit(0);		// fechar programa
	}

	FILE *usuarios = fopen("users.apc", "rb+");
	if (!usuarios){
		printf("ERRO: Arquivo de usuarios nao encontrado!!\n");
		system("pause");
		exit(0); // fechar programa
	}

	

	fclose(usuarios); // Fecho os dois , essa abertura foi so pra verificacao
	fclose(casos);

	tipoJogador jogador;
	tipoAdm adm;

	int op = 0, tentativas, resCode = -2;
	do {

		system("cls");
		printf("\n\t\t\t\t");
		slowprint("Onde esta Sergio Cabral no Brasil?", DELAY);
		printf("\n\n");
		slowprint("Bem-vindo!!\n\nQuem eh voce?", DELAY);
		printf("\n\n");
		slowprint("1 - Procurador (ADM)", DELAY);
		printf("\n");
		slowprint("2 - Detetive (JOGADOR)", DELAY);
		printf("\n");
		slowprint("0 - Sair", DELAY);
		printf("\n>>> ");
		scanf("%i", &op);


		switch (op) {

		case 1://ADM
			tentativas = 3;
			while (tentativas > 0) {

				slowprint("Procurador no teclado, digite seu nome...", DELAY);
				printf("\n\n>>>");
				fflush(stdin);
				gets_s(adm.nome);
				strcpy(admLog.nome, adm.nome);
				_strupr(adm.nome);
				_strupr(admLog.nome);
				slowprint("Otimo, agora digite sua senha...", DELAY);
				printf("\n\t\t");
				slowprint("ATENCAO VERIFIQUE SE NAO HA NINGUEM TE OBSERVANDO", DELAY);
				printf("\n\n>>>");
				fflush(stdin);
				gets_s(adm.senha);
				system("cls");


				resCode = loginadm(adm);

				if (resCode == 1) {
					//login efetuado com sucesso
					tentativas = -1;
				}
				else if (resCode == -1){
					tentativas = -99;
					slowprint("Contate seu superior!", DELAY);
					printf("\n\n");
				}
				else if (resCode == 0) {
					//login falhou, senha ou usuario incorreto
					//atualiza as tentativas, no maximo 3 tentativas
					system("cls");
					tentativas--;
					slowprint("ERRO!, senha ou usuario incorreto", DELAY);
					printf("\n");
					slowprint("NO MAXIMO 6 CARACTERES SEM NUMEROS!", DELAY);
					printf("\n");
					printf("Tentativas restantes ->> %i \n", tentativas);

				}
			}

			if (tentativas == -1) {
				//Menu do ADM
				admconfig(adm);


			}
			else{


				system("cls");
				slowprint("Tentativas esgotadas!", DELAY);
				printf("\nPressione qualquer tecla para voltar ao menu...");
				system("pause>nul");

			}


			break;

		case 2://JOGADOR
			slowprint("Detetive no teclado, digite seu usuario...", DELAY);
			printf("\n\n>>>");
			gets_s(jogador.nome);

			break;


		case 0://SAIR
			system("cls");
			slowprint("Obrigado por jogar!!", DELAY);
			printf("\nAlunos:\n\n");
			slowprint(" Iago Lourenco - 156160116", DELAY);
			printf("\n");
			slowprint(" Gabriel Souza - ", DELAY);
			printf("\n");
			slowprint("Pressione qualquer tecla para sair...", DELAY);
			system("pause>nul");
			break;


		default:
			slowprint("Opcao invalida", DELAY);
			printf("\n");
			break;
		}


	} while (op != 0);





}

int loginadm(tipoAdm adm) {			//retorna 1 se o login foi efetuado, 0 se nao.
	/*FUNCIONAMENTO DOS ARQUIVOS DE LOGIN ADM
	<USUARIO:senha>
	Ex.

	<IAGO:iago123>

	OBS: ADMS TEM O NOME EM LETRA MAIUSCULA E jogadores em minuscula
	*/
	FILE *admdata = fopen("users.apc", "ab+");
	if (!admdata){
		printf("ERRO: Arquivos do adm inacessiveis!!\n");
		return -1;
	}

	int tam = strlen(adm.nome); // qtd de letras do nome
	char c, usuarioArq[50], senhaArqV[8];
	int i = 0;


	if (strlen(adm.senha) > 6){ // verificando se a senha eh valida
		fclose(admdata);
		return 0;
	}

	fwrite(&adm, sizeof(tipoAdm), 1, admdata);
	
	c = fgetc(admdata);
	while (!feof(admdata))			//ate o final do arquivo
	{

		if (c == '<' || c == '>'){
			// se for os separador pego o proximo caracter

			c = fgetc(admdata);

		}
		else
		{
			i = 0;
			while (c != ':')
			{
				usuarioArq[i] = c; // pego o nome cadastrado no arquivo
				c = fgetc(admdata);
				i++;

			}
			usuarioArq[i] = '\0'; // fecho a string

			if (strcmp(usuarioArq, adm.nome) != 0){
				// se o usuario pego do arquivo for diferente do digitado 	
				return 0; // retorno o erro
			}

			else {
				//se nao verifico a senha

				if (c == ':') {//pego do separador pra frente
					i = 0;
					c = fgetc(admdata);
					while (c != '>'){//ate o proximo usuario

						senhaArqV[i] = c;
						c = fgetc(admdata);
						i++;
					}
					senhaArqV[i] = '\0'; // fecho a string

					// vejo se a senha digitada eh igual a cadastrada
					if (strcmp(senhaArqV,adm.senha) == 0){
					 //se sim retorno sucesso
						system("cls");
						fclose(admdata);
						return 1;


					}
					else{
						// se nao retorno o erro
						fclose(admdata);
						return 0;
					}


				}

			}


		}


	}

	fclose(admdata);
	return 0;




}

int admconfig(tipoAdm adm) {
	//menu de configuracao do adm

	int op;
	do{
		system("cls");
		slowprint("Bem vindo, ", DELAY);
		printf("%s \n\n", admLog.nome);

		slowprint("1 - Adicionar caso", DELAY);
		printf("\n");
		slowprint("2 - Alterar meus dados", DELAY);
		printf("\n");
		slowprint("0 - Sair", DELAY);
		printf("\n>>> ");
		scanf("%i", &op);


		switch (op)
		{

		case 1://ADD CASO

			addcaso();

			break;

		case 2://ALTERAR DADOS

			op = alterardados(adm);

			break;

		case 0://SAIR
			return 0;
			break;
		case -1:
			op = -1;
			break;
		default:
			slowprint("Opcao invalida!", DELAY);
			printf("\n");
			break;
		}

	} while (op == -1);
	system("pause");


	return 0;
}

int addcaso(){
	//Insercao de novos casos
	/*
	FUNCIONAMENTO:

	1 - Cada caso tem,pelo menos, 7 pistas que levam a resolucao
	2 - Cada caso tem um nivel de dificuldade
	3 - Cada caso tem sua descricao


	FORMATACAO DO ARQUIVO casos.txt


	#<numero do caso: INT>
	d<dificuldade: INT>
	p<No DE PISTAS: INT>

	<

	1~~~~~~;		\
	2~~~~~~;		 \
	3				  \
	4				   \	Pistas
	.				   /	do caso  ->>  ';' define o fim da pista
	.				  /
	.				 /
	n				/

	>

	%

	~~~~~~~~~~
	~~~~~~~~~~		//		Descricao do
	~~~~~~~~		//			caso
	~~~~~~~~~~


	%


	*/


	tipoCasos casos;

	slowprint("Cadastro de Caso!", DELAY);
	printf("\n\n");







	return 0;
}

int alterardados(tipoAdm adm){

	int op;
	FILE *admdata = fopen("users.txt", "r+");
	if (!admdata){
		printf("ERRO: Arquivos de usuario nao encontrados!");
		return 0;
	}
	char nomeAUX[50], senhaAUX[10];
	int tam = strlen(adm.nome), i = 0;

	do{
		system("cls");
		slowprint("Alterar Dados", DELAY);
		printf("\n\n");

		slowprint("1 - Mudar nome", DELAY);
		printf("\n");
		slowprint("2 - Mudar senha", DELAY);
		printf("\n");
		slowprint("0 - Sair", DELAY);
		printf("\n>>>");
		scanf("%i", &op);

		switch (op){

		case 1:
			
			printf("\n");
			slowprint("Digite o novo nome:", DELAY);
			printf("\n");
			fflush(stdin);
			gets_s(admLog.nome);
			_strupr(admLog.nome);

			fgetc(admdata);  // passo pelo '<'


			fclose(admdata);
			//Descobrir como substituir no arquivo o nome
			break;


		case 2:
			printf("\n");
			slowprint("Digite a nova senha:", DELAY);
			printf("\n");
			slowprint("4 digitos", DELAY);
			printf("\n");
			strcpy(senhaAUX,adm.senha);
			scanf("%i", &adm.senha);


			fclose(admdata);
			//Descobrir como substituir no arquivo o senha
			break;

		case 0://SAIR
			fclose(admdata);
			return -1;
			break;

		default:
			slowprint("Opcao invalida!", DELAY);
			printf("\n");
			break;

		}

	} while (op != 0);

	fclose(admdata);
	return -1;

}
