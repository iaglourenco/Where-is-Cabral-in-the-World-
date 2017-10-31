#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NON_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define DELAY 0 // duracao do delay do slowprint


struct tipoPistas{

	char pista[100];
};

struct tipoCasos {

	int id;
	char descricao[120];
	int dificuldade;
	tipoPistas pistas[20];

};

struct tipoJogador {

	char nome[50];
	int nivel;

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

	<ADMIN:admin>

	OBS: ADMS TEM O NOME EM LETRA MAIUSCULA E jogadores em minuscula
	*/
	FILE *admdata = fopen("users.apc", "ab+");
	if (!admdata){
		printf("ERRO: Arquivos do adm inacessiveis!!\n");
		return -1;
	}

	int tam = strlen(adm.nome); // qtd de letras do nome digitado
	char c, usuarioArq[50], senhaArqV[8];
	int i = 0;
	tipoAdm verifica;// aqui ficara o usuario que vira do arquivo



	rewind(admdata);
	fread(&verifica, sizeof(tipoAdm), 1, admdata);
	printf("\n%s\n%s\n", verifica.nome, verifica.senha);
	system("pause");
	rewind(admdata);

	if (strlen(adm.senha) > 6){ // verificando se a senha eh valida
		fclose(admdata);
		return 0;
	}


	fread(&verifica, sizeof(tipoAdm), 1, admdata); // leio o primeiro dado do "users.apc" pois o adm sempre eh o primeiro
	
	if (strcmp(adm.nome, verifica.nome) == 0){
		// Nomes conferem, prossiga para a senha
		if (strcmp(adm.senha, verifica.senha) == 0){
			//Senhas conferem retorne OK
			fclose(admdata);
			return 1;
		}
		else{
			//Senha incorreta retorne erro
			fclose(admdata);
			return 0;
		}
	}
	else{
		//Nomes diferem retorne erro
		fclose(admdata);
		return 0;
	}


	




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
	
	tipoCasos casos;
	int contapistas=0;

	slowprint("Cadastro de Caso!", DELAY);
	printf("\n\n");
	//Formulario de prenchimento de casos

	slowprint("Bem vindo, Procurador  ", DELAY);
	printf("%s\n", admLog.nome);

	slowprint("Qual a dificuldade do caso:", DELAY);
	printf("\n\t");
	slowprint("1) Facil:", DELAY);
	printf("\n\t");
	slowprint("2) Medio:", DELAY);			//dificuldade
	printf("\n\t");
	slowprint("3) Dificil:", DELAY);
	printf("\nDigite o numero>> ");
	scanf("%i", casos.dificuldade);
	//==================================
	system("cls");
	slowprint("Descreva o caso em poucas palavras, pressione ENTER ao finalizar: |!| NO MAXIMO 100 CARACTERES |!| ", DELAY);
	printf("\n>>> ");
	fflush(stdin);
	gets_s(casos.descricao);			//Descricao
	//==================================
	system("cls");
	slowprint("Digite as pistas ,pressione ENTER ao final de cada uma, e ENTER duas vezes para finalizar", DELAY);
	printf("\n");
	while (contapistas < 20){
		slowprint("Pista atual = ", DELAY);
		printf("%i\n\n", contapistas+1);

	
	
	}




	}

	


	return 0;
}

int alterardados(tipoAdm adm){

	int op;
	FILE *admdata = fopen("users.apc", "rb+");
	if (!admdata){
		printf("ERRO: Nao foi possivel acessar os arquivos de usuario!");
		return 0;
	}
	char nomeAUX[50], senhaAUX[10];
	int tam = strlen(adm.nome), i = 0;

	tipoAdm teste;

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
			gets_s(adm.nome);
			_strupr(adm.nome);

			rewind(admdata);
			fwrite(&adm, sizeof(tipoAdm), 1, admdata); // reescrevo mudando o nome
				
			strcpy(admLog.nome, adm.nome);   //copio pra GLOBAL
			rewind(admdata);
			fread(&teste, sizeof(tipoAdm), 1, admdata);
			printf("\n%s\n%s\n", teste.nome,teste.senha);
			system("pause");

			break;


		case 2:
			printf("\n");
			slowprint("Digite a nova senha:", DELAY);
			printf("\n");
			slowprint("Maximo de 6 digitos!", DELAY);
			printf("\n");
			strcpy(senhaAUX, adm.senha);
			fflush(stdin);
			gets_s(adm.senha);

			rewind(admdata);
			fwrite(&adm, sizeof(tipoAdm), 1, admdata); // reescrevo mudando a senha

			strcpy(admLog.senha, adm.senha);   //copio pra GLOBAL
			rewind(admdata);
			fread(&teste, sizeof(tipoAdm), 1, admdata);
			printf("\n%s\n%s\n", teste.nome, teste.senha);
			system("pause");

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