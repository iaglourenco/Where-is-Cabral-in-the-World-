#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NON_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define DELAY 15 // duracao do delay do slowprint



struct tipoPistas{

	char pista[100];
};

struct tipoSuspeitos{

	char nome[50];
	int sexo;   //1 masc, 2 fem
	char cabelo[50];
	char gosto[100];// joias,carros,pinturas....etc
	char caracteristica[100];// falta de 1 dedo, marcas na pele, cicatrizes, mania...etc

};

struct tipoCidade{

	char nome[50];
	char historia[1000];
	bool cidadefinal=false;	//Se eh a cidade onde o ladrao esta


};

struct tipoCasos {

	int id=0;
	char descricao[150];
	int dificuldade;
	tipoPistas pistas[20];
	tipoCidade cidades[20];
	tipoSuspeitos ladrao;
	int qtdcidades;
	int qtdPistas;
};


struct tipoSavepoint{
	int casoid;
	int tempo;
	int pontuacao;

};

struct tipoJogador {

	char nome[50];
	int nivel;
	int pontos;
	char senha[10];
	tipoSavepoint savepoint;

};

struct tipoAdm {

	char nome[50];
	char senha[10];
};

tipoAdm admLog; // GLOBAL do adm logado
tipoJogador jogadorLog; //GLOBAL do jogador logado

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

//jogador

int jogo(tipoJogador jogador, tipoCasos casos); //retorna 1 se ganhou, 0 se perdeu

int verificalogin(tipoJogador jogador); // verifica se o jogador tem um jogo salvo, retorna 1 se SIM, 0 se NAO

int loginjogador(tipoJogador jogador); // retorna 1 se login OK, 0 se nao

int salvarjogo(tipoJogador jogador); // salva o jogo para continuar depois, 1 se OK, 0 se ERRO

int rankingtop(tipoJogador jogador);
int rankingeral(tipoJogador jogador);




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
	FILE *suspeitos = fopen("suspeitos.apc", "rb+");
	if (!suspeitos){
		printf("ERRO: Arquivo de suspeitos nao encontrado!!\n");
		system("pause");
		exit(0);
	}


	fclose(suspeitos);
	fclose(usuarios); // Fecho os tres , essa abertura foi so pra verificacao
	fclose(casos);

	tipoJogador jogador;
	tipoAdm adm;
	tipoCasos caso;

	int op = -21, tentativas, resCode = -2;
	char cf;
	do {

		system("cls");
		printf("\n\t\t\t\t");
		slowprint("Quem roubou o Brasil?", DELAY);
		printf("\n\n");
		slowprint("Bem-vindo!!\n\nQuem eh voce?", DELAY);
		printf("\n\n");
		slowprint("1 - Procurador (ADM)", DELAY);
		printf("\n");
		slowprint("2 - Detetive (JOGADOR)", DELAY);
		printf("\n");
		slowprint("0 - Sair", DELAY);
		printf("\n>>> ");
		fflush(stdin);
		scanf("%i", &op);


		switch (op) {
#pragma region ADM
		case 1://ADM
			tentativas = 3;
			while (tentativas > 0) {
				resCode = -2;
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

#pragma endregion 
		case 2://JOGADOR
			resCode = -2;//reset dos erros
			slowprint("Detetive no teclado, digite seu usuario...", DELAY);
			printf("\n\n>>>");
			fflush(stdin);
			gets_s(jogador.nome);
			slowprint("Digite sua senha, se nao tiver uma senha cadastrada pressione ENTER... ", DELAY);
			printf("\n");
			fflush(stdin);
			if (gets_s(jogador.senha)==NULL){
				//inicio o jogo
				//escolho o caso aleatoriamente
				do{
					resCode = jogo(jogador, caso);
					if (resCode == 1){
						//ganhou

						slowprint("Parabens!!, jogar de novo? (S\N)", DELAY);
						printf("\n>>>");

					}
					else if (resCode == -1){
						//erro
						break;
					}
					else{
						//perdeu
						slowprint("Que pena, tentar novamente com outro caso? (S\N)", DELAY);
						printf("\n>>>");
					}


				} while (scanf("%c", &cf) != 's' || scanf("%c", &cf) != 'S');
			
			}
			else{
				//login 
				loginjogador(jogador);
			}

			break;

#pragma region SAIR
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
#pragma endregion

		default:
			slowprint("Opcao invalida", DELAY);
			printf("\n");
			break;
		}


	} while (op != 0);





}

//jogador
int jogo(tipoJogador jogador, tipoCasos caso){

	FILE *arqcaso = fopen("casos.apc", "rb");
	if (!arqcaso){
		printf("ERRO: Arquivo de casos nao encontrado!!");
		return -1;
	}

	if (caso.id == 0){
		//sorteia um novo caso

		caso.id = rand() % 5;//sorteia um caso de 1 a 5
	}
	

	





	return 0;
}

int loginjogador(tipoJogador jogador){

	return 0;

}



//adm
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
	int i = 0;
	tipoAdm verifica;// aqui ficara o usuario que vira do arquivo



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

	int op, resCode = -2;
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

			resCode = addcaso();

			switch (resCode){

			case 0://ERRO
				system("cls");
				slowprint("Erro ao cadastrar!", DELAY);
				printf("\n");
				op = -1;
				break;
			case 1://SUCESSO
				system("cls");
				slowprint("Cadastrado com sucesso!", DELAY);
				printf("\n");
				op = -1;
				break;
			case -1://CANCELADO
				system("cls");
				slowprint("Cadastro cancelado!", DELAY);
				printf("\n");
				slowprint("Pressione qualquer tecla para continuar...", DELAY);
				system("pause>nul");
				printf("\n");
				op = -1;

			}


			break;

		case 2://ALTERAR DADOS

			alterardados(adm);
			op = -1;
			break;

		case 0://SAIR
			return 0;
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

	FILE *arquivoCasos = fopen("casos.apc", "rb+");
	if (!arquivoCasos){
		return 0;
	}

	FILE *arquivoSuspeitos = fopen("suspeitos.apc", "rb+");
	if (!arquivoSuspeitos){
		return 0;
	}

	int qtd;
	fread(&qtd, sizeof(int), 1, arquivoCasos);

	tipoCasos casos;
	tipoSuspeitos suspeitos[10];
	int contapistas = 0, i = 0,contacidades=0,op;
	char cidade[100],pista[100], cf;

	system("cls");
	slowprint("Cadastro de Casos!", DELAY);
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
	scanf("%i", &casos.dificuldade);
	//==================================
	system("cls");
	slowprint("Descreva o caso em poucas palavras, pressione ENTER ao finalizar: |!| NO MAXIMO 100 CARACTERES |!| ", DELAY);
	printf("\n>>> ");
	fflush(stdin);
	gets_s(casos.descricao);//Descricao
	_strupr(casos.descricao);
	//==================================
	system("cls");
	slowprint("Insercao de pistas - Pressione ||ENTER|| ao final de cada uma, digite ||SAIR|| para finalizar", DELAY);
	printf("\n\n");
	slowprint("|!|MINIMO 7 pistas|!|", DELAY);
	printf("\n\n");
	slowprint("|!|MAXIMO 20 pistas|!|", DELAY);
	printf("\n\n");

	while (contapistas < 20){
		slowprint("Pista atual = ", DELAY);
		printf("%i\n\n", contapistas + 1);

		slowprint("Digite a pista:  [PRESSIONE ENTER AO TERMINAR]", DELAY);
		printf("\n");
		slowprint("[ESCREVA SAIR PARA FINALIZAR A INSERCAO DE PISTAS]", DELAY);
		printf("\n>>>");
		gets_s(pista);
		_strupr(pista);

		if (strcmp(pista, "SAIR") == 0){
			casos.qtdPistas = contapistas;
			break;
		}
		else{

			strcpy(casos.pistas[contapistas].pista, pista);

		}
		contapistas++;
		if (contapistas == 20){
			slowprint("NUMERO MAXIMO DE PISTAS ATINGIDO!", DELAY);
			printf("\n");
			casos.qtdPistas = contapistas;
			break;
		}


	}
	system("cls");
	slowprint("Insercao de cidades - Pressione ||ENTER|| ao final de cada uma, digite ||SAIR|| para finalizar",DELAY);
	printf("\n\n");
	slowprint("MAX 30 cidades!", DELAY);
	printf("\n\n");

	while (contacidades < 20){
		slowprint("Cidade de No - ", DELAY);
		printf("%i\n\n", contacidades + 1);

		slowprint("Digite o nome da cidade: [PRESSIONE ENTER AO TERMINAR]", DELAY);
		printf("\n>");
		slowprint("[ESCREVA SAIR PARA FINALIZAR A INSERCAO DE CIDADES]", DELAY);
		printf("\n>>>");
		fflush(stdin);
		gets_s(cidade);
		_strupr(cidade);

		if (strcmp(pista, "SAIR") == 0){
			casos.qtdcidades=contacidades;
			break;
		}
		else{

			strcpy(casos.cidades[contacidades].nome, pista);
			system("cls");
			slowprint("Digite a historia dessa cidade:", DELAY);
			printf("\n\n>>>");
			fflush(stdin);
			gets_s(casos.cidades[contapistas].historia);
			_strupr(casos.cidades[contapistas].historia);
			slowprint("Esta eh a cidade final do ladrao?, ou seja, a cidade em que o ladrao parou? (S/N)", DELAY);
			printf("\n");
			fflush(stdin);
			scanf("%c", &cf);

			if (cf == 's' || cf == 'S'){

				casos.cidades[contacidades].cidadefinal = true;
			}
			else{
				cf = NULL;
			}
		}
		contacidades++;
		if (contacidades == 20){
			slowprint("NUMERO MAXIMO DE CIDADES ATINGIDO!", DELAY);
			printf("\n");
			casos.qtdcidades = contacidades;
			break;
		}
	}
	do{

		fread(&suspeitos, sizeof(tipoSuspeitos), 1, arquivoSuspeitos);

		system("cls");
		slowprint("Selecione o ladrao:", DELAY);
		printf("\n\n");
		printf("1 - %s\n", suspeitos[0].nome);
		printf("2 - %s\n", suspeitos[1].nome);
		printf("3 - %s\n", suspeitos[2].nome);
		printf("4 - %s\n", suspeitos[3].nome);
		printf("5 - %s\n", suspeitos[4].nome);
		printf("6 - %s\n", suspeitos[5].nome);
		printf("7 - %s\n", suspeitos[6].nome);
		printf("8 - %s\n", suspeitos[7].nome);
		printf("9 - %s\n", suspeitos[8].nome);
		printf("10 - %s\n", suspeitos[9].nome);
		scanf("%i", &op);
	
			
	
	} while (op  < 1 || op > 10);
	


	system("cls");
	slowprint("Confirme os dados", DELAY);
	printf("\n\nDificuldade = %i\n\n", casos.dificuldade);
	printf("Descricao = %s\n\n", casos.descricao);
	printf("Pistas :\n\n");
	for (i = 0; i < contapistas; i++){

		printf("Pista No. %i - %s \n", i + 1, casos.pistas[i].pista);

	}
	for (i = 0; i < contacidades; i++){

		if (casos.cidades[i].cidadefinal){

			printf("[CIDADE FINAL] - Cidade No. %i - %s \n", i + 1, casos.cidades[i].nome);
		}
		printf("Cidade No. %i - %s \n", i + 1, casos.cidades[i].nome);

	}

	printf("Dossie do Ladrao:\n");
	printf("Nome: %s\n", suspeitos[op].nome);
	if (suspeitos[op].sexo == 1){
		printf("Sexo: Masculino");
	}
	else{
		printf("Sexo: Feminino");
	}
	printf("Gosto por : %s\n", suspeitos[op].gosto);
	printf("Cabelo : %s", suspeitos[op].cabelo);
	printf("Caracteristicas : %s", suspeitos[op].caracteristica);

	printf("\n");
	slowprint("Cadastrar?  (S/N)", DELAY);
	printf("\n>>>");
	scanf("%c", &cf);

	if (cf == 's' || cf == 'S'){

		casos.id = qtd++;
		if (!fwrite(&casos, sizeof(tipoCasos), 1, arquivoCasos)){
			return 0;	//erro ao cadastrar
		}
		else{
			return 1; //sucesso ao cadastrar
		}

	}
	else{
		return -1; // cancelado pelo usuario
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
			printf("\nNovo Nome: %s\nSenha: %s\n", teste.nome, teste.senha);
			system("pause");

			break;


		case 2:
			printf("\n");
			slowprint("Digite a nova senha:", DELAY);
			printf("\n");
			slowprint("Maximo de 6 digitos!", DELAY);
			printf("\n");
			fflush(stdin);
			gets_s(adm.senha);

			rewind(admdata);
			fwrite(&adm, sizeof(tipoAdm), 1, admdata); // reescrevo mudando a senha

			strcpy(admLog.senha, adm.senha);   //copio pra GLOBAL
			rewind(admdata);
			fread(&teste, sizeof(tipoAdm), 1, admdata);
			printf("\nNome%s\nNova Senha%s\n", teste.nome, teste.senha);
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