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



int tranforma(char v[]){
	int tam = strlen(v);
	int res = 0;
	int cas = 1;

	for (int i = tam - 1; i >= 0; i--){

		res = ((v[i] - 48) * cas) + res;
		cas = cas * 10;
	}
	return res;
}

//adm
int loginadm(tipoAdm adm);  //retorna 1 se o login foi efetuado, 0 se nao. 
int admconfig(tipoAdm adm);
int addcaso();		// retorna 1 se a insercao foi um sucesso, 0 se nao.

int cripto(int pass, int action); //funcao de criptografia de senhas, parametros ->> senha = senha a ser criptografada
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


	fclose(usuarios);
	fclose(casos);

	tipoJogador jogador;
	tipoAdm adm;

	int op = 0, tentativas = 3,resCode=-2;


	printf("\n\t\t\t\tWhere is Sergio Cabral in the World?\n\n");
	printf("Bem-vindo!!\n\n");
	printf("1 - Procurador\n");
	printf("2 - Detetive\n");
	printf("0 - Sair\n>>> ");
	scanf("%i", &op);

	do {
		switch (op) {

		case 1://ADM
			while (tentativas > 0) {

				printf("Procurador no teclado, digite seu nome...\n\n");
				fflush(stdin);
				gets_s(adm.nome);
				_strupr(adm.nome);
				printf("Otimo, agora digite sua senha...\n");
				printf("\t\tATENCAO VERIFIQUE SE NAO HA NINGUEM TE OBSERVANDO\n\n");
				fflush(stdin);
				scanf("%i", &adm.senha);
				system("cls");
				

				resCode = loginadm(adm);

				if (resCode == 1 ) {
					//login efetuado com sucesso
					tentativas = -1;
				}
				else if (resCode == -1){
					tentativas = -99;
					printf("Contate seu superior\n\n");
				}
				else if(resCode == 0) {
					//login falhou, senha ou usuario incorreto
					//atualiza as tentativas, no maximo 3 tentativas
					system("cls");
					tentativas--;
					printf("ERRO!, senha ou usuario incorreto\n\n");
					printf("Tentativas restantes ->> %i \n", tentativas);
				}
			}

			if (tentativas == -1) {
				//Menu do ADM
				admconfig(adm);

			}
			else{

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
	/*FUNCIONAMENTO DOS ARQUIVOS DE LOGIN ADM
	<USUARIO:senha>
	Ex.

	<IAGO:1234>
	
	OBS: ADMS TEM O NOME EM LETRA MAIUSCULA E jogadores em minuscula
	*/
	FILE *admdata = fopen("users.txt", "r+");
	if (!admdata){
		printf("ERRO: Arquivos do adm inacessiveis!!\n");
		return -1;
	}

	int tam = strlen(adm.nome);
	char c, usuarioArq[50],senhaArqV[8];
	int i = 0,pass=0;

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
				usuarioArq[i] = c;
				c = fgetc(admdata);
				i++;

			}
			usuarioArq[i] = '\0';

			if (strcmp(usuarioArq, adm.nome) != 0){
				// se o usuario pego do arquivo for diferente do digitado 	
				while (c != '<' && !feof(admdata)) {
					//desloco o cursor ate o proximo usuario cadastrado, ou ate o final do arq
					c = fgetc(admdata);

				}
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
					senhaArqV[i] = '\0';
					pass = tranforma(senhaArqV); // transformo o vetor de char para int
					pass = cripto(pass, 0);// descriptografar a senha obtida
					if (pass == adm.senha){
						
						system("cls");
						fclose(admdata);
						return 1;

					
					}
					else{
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

	printf("Bem vindo, %s \n\n",adm.nome);
	
	printf("1 - Adicionar caso\n");
	printf("2 - Alteras meus dados\n");
	printf("0 - Sair\n>>> ");
	scanf("%i", &op);

	switch (op)
	{

	case 1://ADD CASO

		addcaso();

		break;

	case 2://ALTERAR DADOS

		break;

	case 0://SAIR
		return 0;
		break;
	default:
		printf("Opcao invalida!\n");
		break;
	}
	
	
	system("pause");


	return 0;
}

int cripto(int pass, int action)
{
	int x;

	if (action == 1) {		// criptografar

		x = (pass * 2);



	}
	else {
		//descriptografar

		x = (pass / 2);


	}



	return x;

}
