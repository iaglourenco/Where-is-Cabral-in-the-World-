#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NON_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>





struct tipoPistas{ // definicao da pista

	char pista[100];
};

struct tipoSuspeitos{

	char nome[50];//string
	int sexo;   //1 masc, 2 fem
	char cabelo[50]; //string
	char gosto[100];// joias,carros,pinturas....etc
	char caracteristica[100];// falta de 1 dedo, marcas na pele, cicatrizes, mania...etc

};

struct tipoCidade{

	char nome[50];
	char historia[1000];
	bool cidadefinal = false;	//Se eh a cidade onde o ladrao esta


};

struct tipoCasos {

	int id = 0;
	char descricao[1000];
	int dificuldade;
	tipoPistas pistas[20];
	tipoCidade cidades[20];
	tipoSuspeitos ladrao;
	int qtdcidades;
	int qtdPistas;
};


struct tipoSavepoint{
	int casoid = 0;
	int cidadeatual;
	int pistatual;
	int pontuacao;

};

struct tipoJogador {

	char nome[50];
	int nivel = 1;
	int pontos;
	char senha[10];
	tipoSavepoint savepoint;

};

struct tipoAdm {

	char nome[50];
	char senha[10];
};

int DELAY = 15; //GLOBAL do delay do print
tipoAdm admLog; // GLOBAL do adm logado
tipoJogador jogadorLog; //GLOBAL do jogador logado

// printa devagar
void slowprint(char s[], int delay){

	int tam = strlen(s);
	int i;

	for (i = 0; i < tam; i++){

		printf("%c", s[i]);
		Sleep(delay);
		//if (i % 94 == 0 && i != 0){
		//	printf("\n");
		//	}

	}

}

void config();

//adm
int loginadm(tipoAdm adm);  //retorna 1 se o login foi efetuado, 0 se nao. 

int admconfig(tipoAdm adm);	// retorna 0 quando pedido pra sair

int addcaso();	// retorna 1 se a insercao foi um sucesso, 0 se nao.

int alterardados(tipoAdm adm); //retorna 1 se a alteracao foi feita, 0 se nao

//===============================================================================================

//jogador

int jogo(tipoJogador jogador, tipoCasos casos); //retorna 1 se ganhou, 0 se perdeu
int viajar(tipoCasos caso, int cidadeatual);
int loginjogador(tipoJogador jogador); // retorna 1 se login OK, 0 se nao
int investigar(tipoCasos caso, int pistatual, bool cidadefinal); // retorna a qtd de pistas restantes
int viajar(tipoCasos caso, int cidadeatual); //retorna a cidade
void salvar(tipoJogador jogador, tipoCasos caso);
void dossies();
int ordemDePrisao(tipoCasos caso); // retorna a quantidade de acertos, se =5 YOU WIN, se nao Dinheiro-=acertos

int rankingtop(); //retorna 0
int rankingeral();// retorna 0




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

	/*
	tipoAdm nvi;
	fflush(stdin);
	gets(nvi.nome);
	_strupr(nvi.nome);
	fflush(stdin);
	gets(nvi.senha);
	fwrite(&nvi, sizeof(tipoAdm), 1, usuarios);
	*/




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
		slowprint(".:||Where is Sergio Cabral Crew in BRAZIL?||:.", DELAY);
		printf("\n\n");
		rankingtop();
		slowprint("Bem-vindo!!\n\nQuem eh voce?", DELAY);
		printf("\n\n");
		slowprint("1 - Procurador (ADM)", DELAY);
		printf("\n");
		slowprint("2 - Detetive (JOGADOR)", DELAY);
		printf("\n");
		slowprint("3 - Configuracoes", DELAY);
		printf("\n");
		slowprint("4 - Ranking", DELAY);
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

#pragma region JOGADOR
		case 2://JOGADOR
			resCode = -2;//reset dos erros
			slowprint("Detetive no teclado, digite seu usuario...", DELAY);
			printf("\n\n>>>");
			fflush(stdin);
			gets_s(jogador.nome);
			slowprint("Digite sua senha, se nao tiver uma senha cadastrada pressione ENTER... ", DELAY);
			printf("\n>>>");
			fflush(stdin);
			gets_s(jogador.senha);
			if (strcmp(jogador.senha, "\0") == 0){//ENTER
				//inicio o jogo
				jogador.savepoint.casoid = 0;
				jogador.savepoint.pontuacao = 1000;
				jogador.pontos = 1000;
				jogadorLog = jogador;
				do{
					//loop do jogo 
					resCode = jogo(jogador, caso);
					fflush(stdin);
					if (resCode == 1){
						//ganhou
						if (jogador.nivel < 3 || jogadorLog.nivel<3){
							jogador.nivel++;
							jogadorLog.nivel++;
						}
						slowprint("Parabens!!, jogar de novo? (S-N)", DELAY);
						printf("\n>>>");

					}
					else if (resCode == -1){
						//erro
						break;
					}
					else if (resCode == -99){
						break;
					}
					else if (jogador.savepoint.casoid == 0 || resCode == -123){
						//perdeu

						slowprint("Que pena, jogar novamente? (S-N)", DELAY);
						printf("\n>>>");

					}

					fflush(stdin);
					scanf("%c", &cf);
				} while (cf == 's' || cf == 'S');

			}
			else{
				//login 					

				resCode = loginjogador(jogador);

				switch (resCode){

				case 0:
					//erro login
					system("cls");
					slowprint("Erro! Usuario ou senha incorreto!", DELAY);
					printf("\n");
					slowprint("Deseja iniciar um jogo? (S-N) ", DELAY);
					fflush(stdin);
					scanf("%c", &cf);

					if (cf == 's' || cf == 'S'){
						jogador.savepoint.casoid = 0;
						jogador.savepoint.pontuacao = 1000;
						jogador.pontos = 1000;
						jogadorLog = jogador;
						fflush(stdin);
						//inicio o jogo
						do{
							resCode = jogo(jogador, caso);
							if (resCode == 1){
								//ganhou

								if (jogador.nivel < 3 || jogadorLog.nivel<3){
									jogador.nivel++;
									jogadorLog.nivel++;
								}
								slowprint("Parabens!!, jogar de novo? (S-N)", DELAY);
								printf("\n>>>");
							}
							else if (resCode == -1){
								//erro
								break;
							}
							else if (resCode = -99){
								break;
							}
							else if (jogador.savepoint.casoid == 0 || resCode == -123){
								//perdeu
								slowprint("Que pena, jogar novamente? (S-N)", DELAY);
								printf("\n>>>");
							}

							fflush(stdin);
							scanf("%c", &cf);
						} while (cf == 's' || cf == 'S');


					}

					break;

				case 1:
					// jogador logado
					jogadorLog.pontos = jogadorLog.savepoint.pontuacao;
					jogador = jogadorLog;
					//inicio o jogo
					do{
						resCode = jogo(jogador, caso);
						if (resCode == 1){
							//ganhou
							if (jogador.nivel < 3 || jogadorLog.nivel<3){
								jogador.nivel++;
								jogadorLog.nivel++;
							}
							slowprint("Parabens!!, jogar de novo? (S-N)", DELAY);
							printf("\n>>>");
						}
						else if (resCode == -1){
							//erro
							break;
						}
						else if (resCode = -99){
							break;
						}
						else if (jogador.savepoint.casoid == 0 || resCode == -123){
							//perdeu
							slowprint("Que pena, jogar novamente? (S-N)", DELAY);
							printf("\n>>>");
						}

						fflush(stdin);
						scanf("%c", &cf);
					} while (cf == 's' || cf == 'S');

					break;

				case -1:
					system("cls");
					printf("ERRO AO INICIAR O JOGO\n");
					system("pause");
					break;

				}

			}

			break;
#pragma endregion 

#pragma region CONFIG

		case 3:
			system("cls"); // configuracoes do jogo
			config();




			break;
		case 4:
			rankingeral();		//ranking
			break;

#pragma endregion

#pragma region SAIR
		case 0://SAIR
			system("cls");
			slowprint("Obrigado por jogar!!", DELAY);
			printf("\nFeito por:\n\n");
			slowprint(" Iago Lourenco - 156160116", DELAY);
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
	FILE *tuto = fopen("tutorial.txt", "r");
	

	tipoCasos readedCase, loadedCase;
	char cf, tutorial[1000];
	int option, cidadeatual = 0, pistatual = 0, resCode = 0, i, strikes = 0, die = 0;
	rewind(arqcaso);

	i = 0;
	while (!feof(tuto)){
		tutorial[i] = fgetc(tuto);
		i++;
	}
	tutorial[i] = '\0';

	system("cls");
	slowprint("Bem vindo ", DELAY);
	printf("%s\n\n", jogador.nome);
	if (jogador.savepoint.casoid == 0){// se nao ha jogo salvo ofereco o tutorial
		slowprint("Deseja ver o tutorial?(S-N)", DELAY);
		printf("\n>>>");
		fflush(stdin);
		scanf("%c", &cf);
		if (cf == 's' || cf == 'S'){
			if (!tuto){
				printf("Erro ao exibir tutorial!\n");
			}
			slowprint(tutorial, DELAY);
			printf("\n");
			system("pause");
		}
	}





	if (jogador.savepoint.casoid == 0 || strlen(jogadorLog.senha) == 0){
		//sorteia um novo caso
		//sem jogo salvo
		do{

			caso.id = 1 + rand() % (1 + rand() % (1 + rand() % 100));//rand nao funciona direito
			// por ter uma seed os casos sorteados seguem um padrao
			rewind(arqcaso);
			while (caso.id != readedCase.id && feof(arqcaso) == false){
				fread(&readedCase, sizeof(tipoCasos), 1, arqcaso);
			}

		} while (readedCase.dificuldade != jogador.nivel);

		caso = readedCase;
	}
	else{
		//com jogo salvo
		//procura o caso

		while (!feof(arqcaso)){

			fread(&readedCase, sizeof(tipoCasos), 1, arqcaso);

			if (readedCase.id == jogador.savepoint.casoid){
				jogadorLog.pontos = jogador.savepoint.pontuacao; //load das informacoes
				cidadeatual = jogador.savepoint.cidadeatual;
				pistatual = jogador.savepoint.pistatual;
				caso = readedCase;
				break;
			}
		}

	}



	system("cls");
	slowprint("Descricao do caso: ", DELAY);
	printf("\n");
	slowprint("CAPITAO FERNANDES - ", DELAY);
	slowprint(caso.descricao, DELAY);
	printf("\n");
	system("pause");
	do{
		//loop do jogo

		system("cls");
		slowprint("Cidade Atual - ", DELAY);
		printf("%s\n\n\t", caso.cidades[cidadeatual].nome);
		slowprint(caso.cidades[cidadeatual].historia, DELAY);
		printf("\n");
		printf("\n\n\n\n\n\t\t\t\t\t\tDinheiro:R$ %i,00", jogadorLog.pontos);

		slowprint("\n\n\t1 - Viajar", DELAY);
		printf("\n\t");
		slowprint("2 - Interrogar", DELAY);
		printf("\n\t");
		slowprint("3 - Salvar jogo", DELAY);
		printf("\n\t");
		slowprint("4 - Criar ordem de prisao", DELAY);
		printf("\n\t");
		slowprint("5 - Dossies", DELAY);
		printf("\n\t");
		slowprint("6 - Descricao do caso", DELAY);
		printf("\n\t");
		slowprint("7 - Como jogar", DELAY);
		printf("\n\t");
		slowprint("0 - Sair do jogo", DELAY);
		printf("\n\nOpcao>>>");
		scanf("%i", &option);


		switch (option){

		case 1://viajar
			cidadeatual = viajar(caso, cidadeatual);
			break;
		case 2://investigar


			if (caso.cidades[cidadeatual].cidadefinal == true){
				die++;//se inverstigar 3 vezes na cidade em que o ladrao esta vc eh assasinado :)
				if (die > 3){
					system("cls");
					slowprint("CAPITAO FERNANDES - EH COM MUITO PESAR QUE INFORMO QUE O DETETIVE ", DELAY);
					slowprint(jogadorLog.nome, DELAY);
					printf("\n");
					slowprint("FOI ENCONTRADO MORTO NA CIDADE DE ", DELAY);
					slowprint(caso.cidades[cidadeatual].nome, DELAY);
					printf("\n");
					slowprint("A PERICIA DISSE QUE O ASSASINO TINHA ENVOLVIMENTOS COM O SUSPEITO QUE O DETETIVE ESTAVA INVESTIGANDO ", DELAY);
					printf("\n");
					system("pause");
					system("cls");
					slowprint("VOCE FOI MORTO, RESSUCITE E TENTE NA PROXIMA", DELAY);
					Sleep(1000);
					printf("\n");
					system("pause");
					return -123;

				}

			}
			if (jogadorLog.pontos <= 0){//vejo se tem dinheiro ainda
				break;
			}
			pistatual = investigar(caso, pistatual, caso.cidades[cidadeatual].cidadefinal);
			break;
		case 3://Salvar jogo
			jogadorLog.savepoint.cidadeatual = cidadeatual;
			jogadorLog.savepoint.pistatual = pistatual;// salvo as informacoes necessarias

			salvar(jogador, caso);
			break;
		case 4://Ordem de prisao

			resCode = ordemDePrisao(caso);
			jogadorLog.pontos -= resCode;
			system("cls");
			slowprint("Ordem de prisao enviada! ", DELAY);
			printf("\n");
			slowprint("CAPITAO FERNANDES - POLICIAIS A PROCURA!", DELAY);
			Sleep(5000);
			if (resCode == 5){
				//ganhou

				system("cls");
				printf("\n");
				slowprint("CAPITAO FERNANDES - O LADRAO FOI PEGO!!", DELAY);
				Sleep(1000);
				printf("\n");
				return 1;
			}
			else{
				strikes++;//se errar 3 vezes, vc eh demitido por prender um inocente :)
				//errar eh humano, errar 3 vezes eh burrice :)

				if (strikes > 3){
					printf("\n");
					slowprint("CAPITAO FERNANDES - CAPTURAMOS UM INDIVIDUO, MAS NAO ERA O CULPADO", DELAY);
					printf("\n");
					slowprint("CAPITAO FERNANDES - VOCE PRENDEU UM INOCENTE, O VERDADEIRO LADRAO SE ENTREGOU", DELAY);
					printf("\n\n");
					slowprint("CAPITAO FERNANDES - O VERDADEIRO LADRAO ERA: ", DELAY);
					slowprint(caso.ladrao.nome, DELAY);
					printf("\n\n");
					slowprint("CAPITAO FERNANDES - VOCE ESTA DEMITIDO", DELAY);
					printf("\n\n");
					Sleep(1000);
					system("pause");
					return -123;

				}
				jogadorLog.pontos -= 100; // toda busca gera gasto, pagamento do policiais :)
				printf("\n");
				slowprint("CAPITAO FERNANDES - AS BUSCAS NAO RETORNARAM NADA", DELAY);
				printf("\n");
				Sleep(1000);

			}

			break;
		case 5://Dossies
			dossies(); //ficha de todos os suspeitos
			break;
		case 6:
			system("cls");
			slowprint(caso.descricao, DELAY);
			printf("\nPressione qualquer tecla para voltar...", DELAY);
			system("pause>nul");
			break;

		case 7://tutorial
			system("cls");
			i = 0;
			/*while (!feof(tuto)){
				tutorial[i] = fgetc(tuto);
				i++;
			}
			tutorial[i] = '\0';*/
			slowprint(tutorial, DELAY);
			printf("\n");
			system("pause");

			break;

		case 0://sair
			system("cls");
			slowprint("Tem certeza? (S-N)", DELAY);
			printf("\n>>>");
			fflush(stdin);
			scanf("%c", &cf);

			if (cf == 's' || cf == 'S'){
				slowprint("Apagando historico, Aguarde...", DELAY);
				printf("\n");
				Sleep(5000);
				slowprint("APAGADO", DELAY);  // delay so pra irritar msm :)
				Sleep(2000);
				system("cls");
			}
			else{
				option = -99;
			}

			break;

		default:
			slowprint("Opcao invalida!", DELAY);
			Sleep(1000);
			break;
		}


		if (jogadorLog.pontos <= 0){
			system("cls");
			slowprint(" VOCE GASTOU DEMAIS, A PF FALIU ", DELAY); // gastou muito, RUA
			Sleep(1000);
			break;
		}

	} while (option != 0);//tempo restante aqui



	return -99;
}

//procs do menu do jogo
//feito
int viajar(tipoCasos caso, int cidadeatual){
	system("cls");


	int i, cidadeescolhida;

	slowprint("Viajar:", DELAY);
	printf("\n");
	slowprint("Destinos>\n", DELAY);
	if (cidadeatual == caso.qtdcidades - 1){
		//caso seja a ultima cidade disponivel so eh possivel voltar
		printf("\n\t1 - %s\n", caso.cidades[cidadeatual - 2].nome);
		printf("\t2 - %s\n", caso.cidades[cidadeatual - 1].nome);
		printf("\t0 - Cancelar\n\n");
		slowprint("Escolha uma cidade: ", DELAY);
		printf("\n>>>");
		fflush(stdin);
		scanf("%i", &cidadeescolhida);

		if (cidadeescolhida == 1){
			jogadorLog.pontos -= 100 + rand() % 200;
			cidadeescolhida = cidadeatual - 2;
		}
		else if (cidadeescolhida == 2){
			cidadeescolhida = cidadeatual - 1;
			jogadorLog.pontos -= 100 + rand() % 200;
		}
		else
			return cidadeatual;
	}
	else if (cidadeatual == 0){
		//caso seja a primeira cidade so eh possivel ir pra frente
		printf("\n\t1 - %s\n", caso.cidades[cidadeatual + 2].nome);
		printf("\t2 - %s\n", caso.cidades[cidadeatual + 1].nome);
		printf("\t0 - Cancelar\n\n");
		slowprint("Escolha uma cidade: ", DELAY);
		printf("\n>>>");
		fflush(stdin);
		scanf("%i", &cidadeescolhida);

		if (cidadeescolhida == 1){
			cidadeescolhida = cidadeatual + 2;
			jogadorLog.pontos -= 100 + rand() % 200;
		}
		else if (cidadeescolhida == 2){
			cidadeescolhida = cidadeatual + 1;
			jogadorLog.pontos -= 100 + rand() % 200;
		}
		else
			return cidadeatual;
	}
	else{
		//se nao pra frente e para tras
		printf("\n\t1 - %s\n", caso.cidades[cidadeatual + 1].nome);
		printf("\t2 - %s\n", caso.cidades[cidadeatual - 1].nome);
		printf("\t0 - Cancelar\n\n");
		slowprint("Escolha uma cidade: ", DELAY);
		printf("\n>>>");
		fflush(stdin);
		scanf("%i", &cidadeescolhida);

		if (cidadeescolhida == 1){
			cidadeescolhida = cidadeatual + 1;
			jogadorLog.pontos -= 100 + rand() % 200;
		}
		else if (cidadeescolhida == 2){
			cidadeescolhida = cidadeatual - 1;
			jogadorLog.pontos -= 100 + rand() % 200;
		}
		else
			return cidadeatual;
	}


	return cidadeescolhida;
}

//feito
void salvar(tipoJogador jogador, tipoCasos caso){
	system("cls");

	FILE *users = fopen("users.apc", "rb+");
	if (!users){

		printf("Erro ao salvar!!\n");
		system("pause");
	}

	tipoJogador aux;
	int cont = 0;
	jogador = jogadorLog;
	jogador.savepoint.cidadeatual = jogadorLog.savepoint.cidadeatual; // save das informacoes
	jogador.savepoint.pistatual = jogadorLog.savepoint.pistatual;
	jogador.savepoint.casoid = caso.id;
	jogador.savepoint.pontuacao = jogadorLog.pontos;


	fseek(users, sizeof(tipoAdm), SEEK_SET);//depois do adm

	while (!feof(users)){

		fread(&aux, sizeof(tipoJogador), 1, users); // leio o usuario

		if (strcmp(aux.nome, jogador.nome) == 0){// vejo se o nome eh igual
			rewind(users);	//se sim
			fseek(users, sizeof(tipoAdm), SEEK_SET);//depois do adm
			fseek(users, sizeof(tipoJogador)*cont, SEEK_SET); //coloco o cursor antes do usuario lido, baseado no cont
			fwrite(&jogador, sizeof(tipoJogador), 1, users);
			printf("Jogo Salvo!\n");
			fclose(users);
			Sleep(3000);
			cont = -1;
			break;
		}
		if (cont == -1)
			break;
		cont++;//conto quantos usuarios eu li
	}
	if (cont != -1){// nao achei nenhum nome 
		//crio novo cadatro
		slowprint("Crie uma senha:", DELAY);
		printf("\n");
		slowprint("MAXIMO 10 CARACTERES", DELAY);
		printf("\n>>>");
		fflush(stdin);
		gets(jogador.senha);
		fwrite(&jogador, sizeof(tipoJogador), 1, users);
		printf("Jogo Salvo\n");
		fclose(users);
		Sleep(3000);
	}
}

//feito
int investigar(tipoCasos caso, int pistatual, bool cidadefinal){
	system("cls");


	jogadorLog.pontos -= 0 + rand() % 10;		//custo para interrogar, pague as pessoas e elas falarao :)


	if (cidadefinal == true){

		slowprint("Cuidado voce esta perto!", DELAY);
		printf("\n");
		system("pause");
		return pistatual;
	}

	if (pistatual >= caso.qtdPistas || (rand() % 2) == 0){	//as vezes eles pegam seu dinheiro e nao falam nada :)
		// BR eh foda

		slowprint("Prefiro nao comentar", DELAY);
		printf("\n");
		system("pause");
	}
	else{
		slowprint(caso.pistas[pistatual].pista, DELAY);
		printf("\n");
		system("pause");
		pistatual++;
	}

	return pistatual;



}

//feito
void dossies(){

	//lista todos os suspeitos
	FILE *suspeitos = fopen("suspeitos.apc", "rb");
	tipoSuspeitos dossie[10];
	int i = 0, op = 0;

	while (i < 10){

		fread(&dossie[i], sizeof(tipoSuspeitos), 1, suspeitos);
		i++;
	}
	while (op != -1){
		system("cls");
		slowprint("Dossies:", DELAY);
		printf("\n");

		slowprint("Escolha um perfil:", DELAY);
		printf("\n");
		for (i = 0; i < 10; i++){

			printf("%i - %s\n", i + 1, dossie[i].nome);
		}
		printf("0 - Voltar");
		printf("\n>>>");
		fflush(stdin);
		scanf("%i", &op);
		op--;
		if (op == -1){
			break;
		}
		system("cls");

		printf("Nome: %s\n", dossie[op].nome);
		printf("Sexo: ");
		if (dossie[op].sexo == 1){
			printf("Masculino\n");
		}
		else{
			printf("Feminino\n");
		}
		printf("Gosto por: %s\n", dossie[op].gosto);
		printf("Caracteristicas: %s\n", dossie[op].caracteristica);
		printf("Cabelo: %s\n", dossie[op].cabelo);
		printf("Pressione qualquer tecla para voltar ao jogo...\n");
		system("pause>nul");
		fclose(suspeitos);
	}

}

//feito
int ordemDePrisao(tipoCasos caso){

	FILE *suspeitos = fopen("suspeitos.apc", "rb");
	tipoSuspeitos dossie[10], ordemDePrisao;
	int i = 0, op = -1, pontos = 0, op1 = 1;

	while (i < 10){

		fread(&dossie[i], sizeof(tipoSuspeitos), 1, suspeitos);
		i++;
	}

	strcpy(ordemDePrisao.cabelo, " ");
	strcpy(ordemDePrisao.nome, " ");				//zero todas as informaçoes
	strcpy(ordemDePrisao.caracteristica, " ");
	strcpy(ordemDePrisao.gosto, " ");
	ordemDePrisao.sexo = 0;

	do{
		system("cls");
		slowprint("Ordem de prisao!", DELAY);
		printf("\n");

		slowprint("Defina as informacoes:", DELAY);
		printf("\n\n");

		slowprint("1 - Nome: ", DELAY);
		printf("%s\n", ordemDePrisao.nome);
		slowprint("2 - Sexo: ", DELAY);
		if (ordemDePrisao.sexo == 1){
			slowprint("Masculino", DELAY);
			printf("\n");
		}
		else if (ordemDePrisao.sexo == 2){
			slowprint("Feminino", DELAY);
			printf("\n");
		}
		else{
			slowprint(" ", DELAY);
			printf("\n");
		}
		slowprint("3 - Cabelo: ", DELAY);
		printf("%s\n", ordemDePrisao.cabelo);
		slowprint("4 - Caracteristicas: ", DELAY);
		printf("%s\n", ordemDePrisao.caracteristica);
		slowprint("5 - Gosto por: ", DELAY);
		printf("%s\n\n\n", ordemDePrisao.gosto);
		slowprint("6 - Criar ordem", DELAY);
		printf("\n");
		slowprint("0 - Voltar", DELAY);
		printf("\n>>>");
		fflush(stdin);
		scanf("%i", &op1);

		switch (op1)
		{

		case 1://NOME
			do{
				system("cls");
				i = 0;
				while (i < 10){
					printf("%i - %s\n", i + 1, dossie[i].nome);
					i++;
				}
				printf("0 - Voltar\n>>>");
				fflush(stdin);
				scanf("%i", &op);
				op--;
				if (op != -1){
					if (op < 9){
						strcpy(ordemDePrisao.nome, dossie[op].nome);
						break;
					}
				}

			} while (op != -1);

			break;
		case 2://SEXO
			do{
				system("cls");
				slowprint("1 - Masculino", DELAY);
				printf("\n");
				slowprint("2 - Feminino", DELAY);
				printf("\n>>>");
				fflush(stdin);
				scanf("%i", &op);

				if (op == 1 || op == 2){
					ordemDePrisao.sexo = op;
					break;
				}


				printf("OPCAO INVALIDA\n");
				Sleep(500);
			} while (op != 1 || op != 2);
			break;
		case 3:
			do{
				system("cls");
				i = 0;
				while (i < 10){
					if (i == 6 || i == 8 || i == 9){
						i++;
					}
					else{
						printf("%i - %s\n", i + 1, dossie[i].cabelo);
						i++;
					}
				}
				printf("0 - Voltar\n>>>");
				fflush(stdin);
				scanf("%i", &op);
				op--;
				if (op != -1){
					if (op < 9){
						strcpy(ordemDePrisao.cabelo, dossie[op].cabelo);
						break;
					}
				}

			} while (op != -1);
			break;
		case 4:
			do{
				system("cls");
				i = 0;
				while (i < 10){
					printf("%i - %s\n", i + 1, dossie[i].caracteristica);
					i++;
				}
				printf("0 - Voltar\n>>>");
				fflush(stdin);
				scanf("%i", &op);
				op--;
				if (op != -1){
					if (op < 9){
						strcpy(ordemDePrisao.caracteristica, dossie[op].caracteristica);
						break;
					}
				}

			} while (op != -1);
			break;
		case 5:
			do{
				system("cls");
				i = 0;
				while (i < 10){
					printf("%i - %s\n", i + 1, dossie[i].gosto);
					i++;
				}
				printf("0 - Voltar\n>>>");
				fflush(stdin);
				scanf("%i", &op);
				op--;
				if (op != -1){
					if (op < 9){
						strcpy(ordemDePrisao.gosto, dossie[op].gosto);
						break;
					}
				}

			} while (op != -1);
			break;
		case 6:
			if (strcmp(caso.ladrao.nome, ordemDePrisao.nome) == 0){			//cada acerto vale 1 ponto acertou tudo YOU WIN
				pontos++;													// se nao acertar tudo perde dinheiro :)
			}
			else{
				pontos--;
			}

			if (caso.ladrao.sexo == ordemDePrisao.sexo){
				pontos++;
			}
			else{
				pontos--;
			}

			if (strcmp(caso.ladrao.gosto, ordemDePrisao.gosto) == 0){
				pontos++;
			}
			else{
				pontos--;
			}

			if (strcmp(caso.ladrao.caracteristica, ordemDePrisao.caracteristica) == 0){
				pontos++;
			}
			else{
				pontos--;
			}
			if (strcmp(caso.ladrao.cabelo, ordemDePrisao.cabelo) == 0){
				pontos++;
			}
			else{
				pontos--;
			}
			fclose(suspeitos);
			return pontos;// se acertar tudo ganha

			break;
		case 0:
			fclose(suspeitos);
			return 0;
			break;
		default:
			slowprint("OPCAO INVALIDA", DELAY);
			printf("\n");
			Sleep(500);
			break;
		}



	} while (op1 != 0);



	fclose(suspeitos);
	return 0;
}

//feito
int loginjogador(tipoJogador jogador){


	FILE *arqUsers = fopen("users.apc", "rb");
	if (!arqUsers){
		return -1;
	}
	tipoJogador readedUser;
	tipoAdm aux;

	fread(&aux, sizeof(tipoAdm), 1, arqUsers);// o primeiro eh o ADM

	while (!feof(arqUsers)){

		fread(&readedUser, sizeof(tipoJogador), 1, arqUsers);

		if (strcmp(readedUser.nome, jogador.nome) == 0){
			//nome encontrado , verifica a senha

			if (strcmp(readedUser.senha, jogador.senha) == 0){
				// Senha batem retorne OK
				jogadorLog = readedUser;

				fclose(arqUsers);
				return 1;

			}
			else{
				//Senha incorreta retorne erro
				fclose(arqUsers);
				return 0;
			}




		}
		else{
			//Nomes diferem continue ate feof

			continue;

		}


	}

	fclose(arqUsers);
	return 0;

}

int rankingeral(){

	FILE *arqRanking = fopen("users.apc", "rb");
	if (!arqRanking){
		printf("ERRO: Arquivos de ranking inacessiveis\n");
		Sleep(2000);
		return -1;
	}
	tipoJogador readedPlayers[1000];
	tipoJogador aux;
	tipoAdm a;
	int qtd = 0;
	fread(&a, sizeof(tipoAdm), 1, arqRanking);
	while (!feof(arqRanking)){
		fread(&readedPlayers[qtd], sizeof(tipoJogador), 1, arqRanking); // leio os usuarios
		qtd++;
	}
	qtd--;
	int op, i, j;



	do{
		system("cls");
		slowprint("Ranking", DELAY);
		printf("\n");
		slowprint("1 - Nome", DELAY);
		printf("\n");
		slowprint("2 - Nivel", DELAY);
		printf("\n");
		slowprint("3 - Dinheiro", DELAY);
		printf("\n");
		slowprint("0 - Voltar", DELAY);
		printf("\n>>>");
		scanf("%i", &op);

		switch (op){


		case 1://nome
			system("cls");
			slowprint("Nome			Nivel			Dinheiro", DELAY);
			printf("\n________________________________________________________\n");

			for (i = 0; i < qtd; i++){
				//bubble sort
				for (j = 0; j < qtd - 1; j++){
					if (strcmp(readedPlayers[j].nome, readedPlayers[j + 1].nome) > 0){

						aux = readedPlayers[j];
						readedPlayers[j] = readedPlayers[j + 1];
						readedPlayers[j + 1] = aux;

					}
				}
			}

			for (i = 0; i < qtd; i++){

				printf("\n%s			%i			%i\n", readedPlayers[i].nome, readedPlayers[i].nivel, readedPlayers[i].savepoint.pontuacao);
				Sleep(500);
			}


			printf("\nPressione qualquer tecla para voltar...");
			system("pause>nul");
			break;

		case 2://nivel
			system("cls");
			slowprint("Posicao		Nome			Nivel			Dinheiro", DELAY);
			printf("\n________________________________________________________________________\n");

			for (i = 0; i < qtd; i++){
				//bubble sort
				for (j = 0; j < qtd - 1; j++){
					if (readedPlayers[j].nivel < readedPlayers[j + 1].nivel){

						aux = readedPlayers[j];
						readedPlayers[j] = readedPlayers[j + 1];
						readedPlayers[j + 1] = aux;

					}
				}
			}

			for (i = 0; i < qtd; i++){

				printf("\n%i		%s			%i			%i\n", i + 1, readedPlayers[i].nome, readedPlayers[i].nivel, readedPlayers[i].savepoint.pontuacao);
				Sleep(500);
			}


			printf("\nPressione qualquer tecla para voltar...");
			system("pause>nul");

			break;

		case 3://money
			system("cls");
			slowprint("Posicao		Nome			Nivel			Dinheiro", DELAY);
			printf("\n________________________________________________________________________\n");

			for (i = 0; i < qtd; i++){
				//bubble sort
				for (j = 0; j < qtd - 1; j++){
					if (readedPlayers[j].savepoint.pontuacao < readedPlayers[j + 1].savepoint.pontuacao){

						aux = readedPlayers[j];
						readedPlayers[j] = readedPlayers[j + 1];
						readedPlayers[j + 1] = aux;

					}
				}
			}

			for (i = 0; i < qtd; i++){

				printf("\n%i		%s			%i			%i\n", i + 1, readedPlayers[i].nome, readedPlayers[i].nivel, readedPlayers[i].savepoint.pontuacao);
				Sleep(500);
			}


			printf("\nPressione qualquer tecla para voltar...");
			system("pause>nul");
			break;

		case 0:
			return 0;
			break;

		default:
			slowprint("Opcao invalida", DELAY);
			printf("\n");
		}

	} while (op != 0);

	fclose(arqRanking);
	return 0;



}

int rankingtop(){

	FILE *rkg = fopen("users.apc", "rb");

	// msmo do ranking geral mas limitado a 5 usuarios ou menos
	tipoJogador readedPlayers[1000], aux;
	int qtd = 0, i = 0, j;
	tipoAdm a;
	fread(&a, sizeof(tipoAdm), 1, rkg);
	while (!feof(rkg)){

		fread(&readedPlayers[qtd], sizeof(tipoJogador), 1, rkg);
		qtd++;
	}
	qtd--;
	slowprint("\n\n\t\t\t\t\t  ||TOP 5 - Nivel||\n\n", DELAY);
	slowprint("\n\t\t\t\tPosicao    Nome        Nivel    Dinheiro", DELAY);
	printf("\n\t\t\t\t________________________________________\n");

	for (i = 0; i < qtd; i++){

		for (j = 0; j < qtd - 1; j++){
			if (readedPlayers[j].nivel < readedPlayers[j + 1].nivel){

				aux = readedPlayers[j];
				readedPlayers[j] = readedPlayers[j + 1];
				readedPlayers[j + 1] = aux;

			}
		}
	}

	i = 0;
	while (i < qtd && i < 5){
		printf("\n\t\t\t\t%i          %s        %i             %i\n", i + 1, readedPlayers[i].nome, readedPlayers[i].nivel, readedPlayers[i].savepoint.pontuacao);
		Sleep(500);
		i++;
	}
	printf("\n\n");
	fclose(rkg);
	return 0;
}


void config(){

	int op, aux;
	char bkg, frg, cf, color[] = { 'c', 'o', 'l', 'o', 'r', ' ', '0', 'a', '\0' };
	do{
		system("cls");
		slowprint("Configuracoes", DELAY);
		printf("\n\n");
		slowprint("1 - Definir velocidade de exibicao do texto", DELAY);
		printf("\n");
		slowprint("2 - Mudar cores", DELAY);
		printf("\n");
		slowprint("0 - Sair", DELAY);
		printf("\n\n>>>");



		scanf("%i", &op);


		switch (op){

		case 1://mudar o delay do slowprint
			aux = DELAY;
			slowprint("Digite o valor em milisegundos, entre cada letra:", DELAY);
			printf("\n>>>");
			fflush(stdin);
			scanf("%i", &DELAY);

			slowprint("Salvar configuracao?(S-N)", DELAY);
			printf("\n>>>");
			fflush(stdin);
			scanf("%c", &cf);
			if (cf == 's' || cf == 'S'){
				break;
			}
			else{
				DELAY = aux;

			}

			break;

		case 2:// mudar a cor do CMD
			slowprint("Qual cor voce quer mudar?", DELAY);
			printf("\n\n");
			slowprint("1 - Background (FUNDO)", DELAY);
			printf("\n");
			slowprint("2 - Texto (LETRAS)", DELAY);
			printf("\n>>>");
			fflush(stdin);
			scanf("%i", &op);

			switch (op){

			case 1:
				slowprint("Cores disponiveis", DELAY);
				printf("\n\n");
				slowprint("1 - Preto", DELAY);
				printf("\n");
				slowprint("2 - Azul", DELAY);
				printf("\n");
				slowprint("3 - Verde", DELAY);
				printf("\n");
				slowprint("4 - Verde-agua", DELAY);
				printf("\n");
				slowprint("5 - Vermelho", DELAY);
				printf("\n");
				slowprint("6 - Roxo", DELAY);
				printf("\n");
				slowprint("7 - Amarelo", DELAY);
				printf("\n");
				slowprint("8 - Branco", DELAY);
				printf("\n>>>");
				fflush(stdin);
				scanf("%c", &bkg);
				bkg--;
				color[6] = bkg;
				system(color);
				slowprint("Salvar configuracao?(S-N)", DELAY);
				printf("\n>>>");
				fflush(stdin);
				scanf("%c", &cf);
				if (cf == 's' || cf == 'S'){
					break;
				}
				else{
					system("color 0a");

				}
				break;

			case 2:
				slowprint("Cores disponiveis", DELAY);
				printf("\n\n");
				slowprint("1 - Preto", DELAY);
				printf("\n");
				slowprint("2 - Azul", DELAY);
				printf("\n");
				slowprint("3 - Verde", DELAY);
				printf("\n");
				slowprint("4 - Verde-agua", DELAY);
				printf("\n");
				slowprint("5 - Vermelho", DELAY);
				printf("\n");
				slowprint("6 - Roxo", DELAY);
				printf("\n");
				slowprint("7 - Amarelo", DELAY);
				printf("\n");
				slowprint("8 - Branco", DELAY);
				printf("\n>>>");
				fflush(stdin);
				scanf("%c", &frg);
				frg--;
				color[7] = frg;
				system(color);
				slowprint("Salvar configuracao?(S-N)", DELAY);
				printf("\n>>>");
				fflush(stdin);
				scanf("%c", &cf);
				if (cf == 's' || cf == 'S'){
					break;
				}
				else{
					system("color 0a");

				}
				break;

			case 0:
				break;

			default:
				slowprint("OPCAO INVALIDA", DELAY);
				printf("\n");
				Sleep(500);

			}
		}
	} while (op != 0);


}

//adm
int loginadm(tipoAdm adm) {			//retorna 1 se o login foi efetuado, 0 se nao.
	/*FUNCIONAMENTO DOS ARQUIVOS DE USUARIOS
	<USUARIO:senha>
	Ex.

	struct	tipoAdm{nome=ADMIN;senha=admin;};
	struct	tipoJogador{...};

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

	tipoCasos casos, aux;
	tipoSuspeitos suspeitos[10];
	int contapistas = 0, i = 0, contacidades = 0, op;
	bool endFile;
	char cidade[100], pista[100], cf;
	bool cadastrada = false;
	int qtd = 1, cont = 0;
	endFile = feof(arquivoCasos);
	while (!feof(arquivoCasos)){// pego o id do ultimo cadastrado 
		fread(&aux, sizeof(tipoCasos), 1, arquivoCasos);
		qtd++; //conto os casos
		cont++;
	}
	if (endFile == true){
		qtd = 0;// nenhum caso cadastrado
	}

	casos.id = qtd++;

	system("cls");
	slowprint("Cadastro de Casos!", DELAY);
	//Formulario de prenchimento de casos
	printf("\n\n");
	slowprint("Bem vindo, Procurador  ", DELAY);
	printf("%s\n", admLog.nome);
	printf("\n\t\t\t\t\tCasos cadastrados = %i\n\n", cont - 1);

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

	while (contapistas < 20 || contapistas < 7){
		slowprint("Pista atual = ", DELAY);
		printf("%i\n\n", contapistas + 1);

		slowprint("Digite a pista:  [PRESSIONE ENTER AO TERMINAR]", DELAY);
		printf("\n");
		slowprint("[ESCREVA SAIR PARA FINALIZAR A INSERCAO DE PISTAS]", DELAY);
		printf("\n>>>");
		gets_s(pista);
		_strupr(pista);

		if (strcmp(pista, "SAIR") == 0){//vejo se eh sair
			if (contapistas < 7){
				continue;
			}
			else{
				casos.qtdPistas = contapistas;
				break;
			}
		}
		else{

			strcpy(casos.pistas[contapistas].pista, pista); //salvo na struct

		}
		contapistas++;
		if (contapistas == 20){	// numero maximo atingido
			slowprint("NUMERO MAXIMO DE PISTAS ATINGIDO!", DELAY);
			printf("\n");
			Sleep(1000);
			casos.qtdPistas = contapistas;
			break;
		}


	}
	system("cls");
	slowprint("Insercao de cidades - Pressione ||ENTER|| ao final de cada uma, digite ||SAIR|| para finalizar", DELAY);
	printf("\n\n");
	slowprint("MAX 30 cidades!", DELAY);
	printf("\n\n");

	while (contacidades < 30 || contacidades < 3){ // igual ao contapistas
		slowprint("Cidade de No - ", DELAY);
		printf("%i\n\n", contacidades + 1);

		slowprint("Digite o nome da cidade: [PRESSIONE ENTER AO TERMINAR]", DELAY);
		printf("\n>");
		slowprint("[ESCREVA SAIR PARA FINALIZAR A INSERCAO DE CIDADES]", DELAY);
		printf("\n>>>");
		fflush(stdin);
		gets_s(cidade);
		_strupr(cidade);

		if (contacidades == 30){

			slowprint("NUMERO MAXIMO DE CIDADES ATINGIDO!!", DELAY);
			Sleep(1000);
			printf("\n");
			break;
		}
		if (strcmp(cidade, "SAIR") == 0){
			if (contacidades < 3){
				continue;
			}
			else{
				casos.qtdcidades = contacidades;
				break;
			}
		}
		else{

			strcpy(casos.cidades[contacidades].nome, cidade);
			system("cls");
			slowprint("Digite a historia dessa cidade:", DELAY);
			printf("\n\n>>>");
			fflush(stdin);
			gets_s(casos.cidades[contacidades].historia);
			_strupr(casos.cidades[contacidades].historia);
			if (cadastrada == false){
				slowprint("Esta eh a cidade final do ladrao?, ou seja, a cidade em que o ladrao parou? (S/N)", DELAY);
				printf("\n");
				fflush(stdin);
				scanf("%c", &cf);

				if (cf == 's' || cf == 'S'){

					casos.cidades[contacidades].cidadefinal = true;
					cadastrada = true;
				}
				else{
					cf = NULL;
				}
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
	for (i = 0; i < 10; i++){
		//pego os suspeitos do arquivo
		fread(&suspeitos[i], sizeof(tipoSuspeitos), 1, arquivoSuspeitos);
	}

	do{//printo
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
		printf("10 - %s\n>>>", suspeitos[9].nome);
		scanf("%i", &op);

	} while (op  < 1 || op > 10);
	op--;
	casos.ladrao = suspeitos[op];


	system("cls");
	slowprint("Confirme os dados", DELAY);//peco a confirmacao dos dados inseridos 	
	//printo a struct inteira

	printf("\n\nDificuldade = %i\n\n", casos.dificuldade);

	printf("Descricao = %s\n\n", casos.descricao);

	printf("Pistas :\n\n");
	for (i = 0; i < contapistas; i++){

		printf("Pista No. %i - %s \n", i + 1, casos.pistas[i].pista);

	}
	printf("\n");
	printf("Cidades :\n\n");
	for (i = 0; i < contacidades; i++){

		if (casos.cidades[i].cidadefinal == true){

			printf("[CIDADE FINAL] - Cidade No. %i - %s \n", i + 1, casos.cidades[i].nome);
		}
		else{
			printf("Cidade No. %i - %s \n", i + 1, casos.cidades[i].nome);
		}
	}

	printf("\nDossie do Ladrao:\n\n");
	printf("Nome: %s\n", suspeitos[op].nome);
	if (suspeitos[op].sexo == 1){
		printf("Sexo: Masculino\n");
	}
	else{
		printf("Sexo: Feminino\n");
	}
	printf("Gosto por : %s\n", suspeitos[op].gosto);
	printf("Cabelo : %s\n", suspeitos[op].cabelo);
	printf("Caracteristicas : %s\n\n", suspeitos[op].caracteristica);

	printf("\n");
	slowprint("Cadastrar?  (S/N)", DELAY);
	printf("\n>>>");
	fflush(stdin);
	scanf("%c", &cf);

	if (cf == 's' || cf == 'S'){

		if (!fwrite(&casos, sizeof(tipoCasos), 1, arquivoCasos)){
			fclose(arquivoCasos);
			return 0;	//erro ao cadastrar
		}
		else{
			fclose(arquivoCasos);
			return 1; //sucesso ao cadastrar
		}

	}
	else{
		fclose(arquivoCasos);
		return -1; // cancelado pelo usuario
	}

	fclose(arquivoCasos);
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