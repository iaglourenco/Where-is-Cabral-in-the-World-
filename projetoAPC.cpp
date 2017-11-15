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
	char descricao[1000];
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
	int nivel=0;
	int pontos=0;
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
int viajar(tipoCasos caso, int cidadeatual);
int loginjogador(tipoJogador jogador); // retorna 1 se login OK, 0 se nao
int investigar(tipoCasos caso, int pistatual, bool cidadefinal);
int viajar(tipoCasos caso, int cidadeatual);
void salvar(tipoJogador jogador, tipoCasos caso);
void dossies();

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

#pragma region JOGADOR
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
				do{
					resCode = jogo(jogador, caso);
					
					if (resCode == 1){
						//ganhou
						jogador.nivel++;
						slowprint("Parabens!!, jogar de novo? (S-N)", DELAY);
						printf("\n>>>");

					}
					else if (resCode == -1){
						//erro
						break;
					}
					else{
						//perdeu
						jogador.nivel--;
						slowprint("Que pena, tentar novamente com outro caso? (S-N)", DELAY);
						printf("\n>>>");
					}


				} while (scanf("%c", &cf) != 's' || scanf("%c", &cf) != 'S');
			
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

						//inicio o jogo
						do{
							resCode = jogo(jogador, caso);
							if (resCode == 1){
								//ganhou

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
							else{
								//perdeu
								slowprint("Que pena, tentar novamente com outro caso? (S-N)", DELAY);
								printf("\n>>>");
							}

							fflush(stdin);
						} while (scanf("%c", &cf) != 's' || scanf("%c", &cf) != 'S');


					}

					break;

				case 1:
					// logado
					
					jogadorLog = jogador; //att a global
					//inicio o jogo
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
						else if (resCode = -99){
							break;
						}
						else{
							//perdeu
							slowprint("Que pena, tentar novamente com outro caso? (S\N)", DELAY);
							printf("\n>>>");
						}

						fflush(stdin);
					} while (scanf("%c", &cf) != 's' || scanf("%c", &cf) != 'S');

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

	tipoCasos readedCase,loadedCase;
	char cf;
	int option,cidadeatual=0,pistatual=0,resCode=0;
#pragma region CONFIGURACAO INICIAL

	
	rewind(arqcaso);

	do{
		fread( &readedCase, sizeof( tipoCasos),1,arqcaso);

		if (readedCase.id == caso.id){
			// caso encontrado
			//inicia jogo
			caso = readedCase;
			break;
		}
		else{
			// le o proximo
			fread(&readedCase, sizeof(tipoCasos), 1, arqcaso);
		}

	} while (!feof(arqcaso));

	system("cls");

	slowprint("Bem vindo ", DELAY);
	printf("%s\n\n", jogador.nome);
	if (jogador.savepoint.casoid == 0){
		slowprint("Deseja ver o tutorial?",DELAY);
		printf("\n>>>");
		scanf("%c", &cf);
		if (cf == 's' || cf == 'S'){
			system("PRINT tutorial.txt");
			printf("\n");
			system("pause");
		}
	}

#pragma endregion
	if (caso.id == 0){
		//sorteia um novo caso
		caso.id = 1;
		//caso.id = rand() % 5;//sorteia um caso de 1 a 5
	}

	
	system("cls");
	slowprint(caso.descricao, DELAY);
	printf("\n");
	system("pause");
	do{
		//loop do jogo

		system("cls");
		slowprint("Cidade Atual - ", DELAY);
		printf("%s\n\n\t",caso.cidades[cidadeatual].nome);
		slowprint(caso.cidades[cidadeatual].historia,DELAY);
		printf("\n");
		Sleep(3000);

		slowprint("\n\n\t1 - Viajar", DELAY);
		printf("\n\t");
		slowprint("2 - Investigar", DELAY);
		printf("\n\t");
		slowprint("3 - Salvar jogo", DELAY);
		printf("\n\t");
		slowprint("4 - Criar ordem de prisao", DELAY);
		printf("\n\t");
		slowprint("5 - Dossies", DELAY);
		printf("\n\t");
		slowprint("0 - Sair do jogo", DELAY);
		printf("\n\nOpcao>>>");
		scanf("%i", &option);


		switch (option){

		case 1://viajar
			cidadeatual = viajar(caso,cidadeatual);
			break;
		case 2://investigar
		
			pistatual = investigar(caso, pistatual, caso.cidades[cidadeatual].cidadefinal);
			break;
		case 3://Salvar jogo
			salvar(jogador, caso);
			break;
		case 4://Ordem de prisao
			


			break;
		case 5://Dossies
			dossies();
			break;
		case 0://sair
			system("cls");
			slowprint("Tem certeza?", DELAY);
			fflush(stdin);
			scanf("%c", &cf);

			if (cf == 's' || cf == 'S'){
				slowprint("Apagando historico...",DELAY);
				Sleep(5000);
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

		
		








	} while (option != 0);//tempo restante aqui

	
	





	return -99;
}

//procs do menu do jogo
//feito
int viajar(tipoCasos caso,int cidadeatual){
	system("cls");

	int i,cidadeescolhida;

	slowprint("Viajar:", DELAY);
	printf("\n");
	slowprint("Destinos>", DELAY);
	if (cidadeatual == caso.qtdcidades){
		//caso seja a ultima cidade disponivel so eh possivel voltar
		printf("1 - %s\n", caso.cidades[cidadeatual - 2].nome);
		printf("2 - %s\n", caso.cidades[cidadeatual - 1].nome);
		slowprint("Escolha uma cidade: ", DELAY);
		printf("\n");
		fflush(stdin);
		scanf("%i", &cidadeescolhida);

		if (cidadeescolhida == 1)
			cidadeescolhida = cidadeatual - 2;
		else
			cidadeescolhida = cidadeatual - 1;

	}
	else if (cidadeatual == 0){
		//caso seja a primeira cidade so eh possivel ir pra frente
		printf("1 - %s\n", caso.cidades[cidadeatual + 2].nome);
		printf("2 - %s\n", caso.cidades[cidadeatual + 1].nome);
		slowprint("Escolha uma cidade: ", DELAY);
		printf("\n");
		fflush(stdin);
		scanf("%i", &cidadeescolhida);

		if (cidadeescolhida == 1)
			cidadeescolhida = cidadeatual + 2;
		else
			cidadeescolhida = cidadeatual + 1;
	}
	else{
		//se nao pra frente e para tras
		printf("\n\n1 - %s\n", caso.cidades[cidadeatual + 1].nome);
		printf("2 - %s\n", caso.cidades[cidadeatual - 1].nome);
		slowprint("Escolha uma cidade: ", DELAY);
		printf("\n");
		fflush(stdin);
		scanf("%i", &cidadeescolhida);

		if (cidadeescolhida == 1)
			cidadeescolhida = cidadeatual + 1;
		else
			cidadeescolhida = cidadeatual - 1;
	}


	return cidadeescolhida;
}

//feito
void salvar(tipoJogador jogador,tipoCasos caso){
	system("cls");

	FILE *users = fopen("users.apc","rb+");
	if (!users){

		printf("Erro ao salvar!!\n");
		system("pause");
	}

	tipoJogador aux;
	int cont=0;

	jogador.savepoint.casoid = caso.id;
	jogador.savepoint.pontuacao = jogador.pontos;
	//jogador.savepoint.tempo = ;

	slowprint("Crie uma senha:",DELAY);
	printf("\n");
	slowprint("MAXIMO 10 CARACTERES", DELAY);
	printf("\n>>>");
	fflush(stdin);
	gets(jogador.senha);

	fseek(users, sizeof(tipoAdm), SEEK_SET);//depois do adm

	while (!feof(users)){

		fread(&aux, sizeof(tipoJogador), 1, users);

		if (strcmp(aux.nome, jogador.nome) == 0){
			rewind(users);
			fseek(users, sizeof(tipoJogador)*cont,SEEK_SET);
			fwrite(&jogador, sizeof(tipoJogador), 1, users);
			printf("Jogo Salvo!\n");
			fclose(users);
			Sleep(3000);
			break;
		}
		cont++;
	}

	fwrite(&jogador, sizeof(tipoJogador), 1, users);
	printf("Jogo Salvo\n");
	fclose(users);
	Sleep(3000);

}

//feito
int investigar(tipoCasos caso,int pistatual,bool cidadefinal){
	system("cls");



	
	if (cidadefinal == true){

		slowprint("Cuidado voce esta perto!",DELAY);
		printf("\n");
		system("pause");
		return pistatual;
	}

	if (pistatual >= caso.qtdPistas){

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

	FILE *suspeitos = fopen("suspeitos.apc", "rb");
	tipoSuspeitos dossie[10];
	int i=0,op=-1;

	while (i < 10){

		fread(&dossie[i], sizeof(tipoSuspeitos), 1, suspeitos);
		i++;
	}
	

	slowprint("Dossies:", DELAY);
	printf("\n");

	slowprint("Escolha um perfil:", DELAY);
	printf("\n");
	for (i = 0; i < 10; i++){

		printf("%i - %s\n", i+1,dossie[i].nome);
	}
	printf("\n>>>");
	fflush(stdin);
	scanf("%i", &op);
	op--;

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


int ordemDePrisao(tipoCasos caso){




	return 0;
}

//feito
int loginjogador(tipoJogador jogador){


	FILE *arqUsers = fopen("users.apc", "rb");
	if (!arqUsers){
		return -1;
	}
	tipoJogador readedUser;

	fread(&readedUser, sizeof(tipoJogador), 1, arqUsers);// o primeiro eh o ADM

	while (!feof(arqUsers)){

		fread(&readedUser, sizeof(tipoJogador), 1, arqUsers);

		if ( strcmp( readedUser.nome , jogador.nome ) == 0 ){
			//nome encontrado , verifica a senha

			if (strcmp(readedUser.senha, jogador.senha) == 0 ){
				// Senha batem retorne OK
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

	tipoCasos casos,aux;
	tipoSuspeitos suspeitos[10];
	int contapistas = 0, i = 0, contacidades = 0, op,endFile;
	char cidade[100], pista[100], cf;

	int qtd=1;
	while (!feof(arquivoCasos)){// pego o id do ultimo cadastrado 
		fread(&aux, sizeof(tipoCasos), 1, arquivoCasos);
		endFile = feof(arquivoCasos);
	}
	if (endFile == true){
		qtd = 1;
	}
	else{
		qtd = aux.id;
	}
	

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

	while (contapistas < 20 && contapistas < 7){
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
			Sleep(1000);
			casos.qtdPistas = contapistas;
			break;
		}


	}
	system("cls");
	slowprint("Insercao de cidades - Pressione ||ENTER|| ao final de cada uma, digite ||SAIR|| para finalizar",DELAY);
	printf("\n\n");
	slowprint("MAX 30 cidades!", DELAY);
	printf("\n\n");

	while (contacidades < 30 && contacidades < 3){
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

			slowprint("NUMERO MAXIMO DE CIDADES ATINGIDO!!",DELAY);
			Sleep(1000);
			printf("\n");
			break;
		}
		if (strcmp(cidade, "SAIR") == 0){
			casos.qtdcidades=contacidades;
			break;
		}
		else{

			strcpy(casos.cidades[contacidades].nome, cidade);
			system("cls");
			slowprint("Digite a historia dessa cidade:", DELAY);
			printf("\n\n>>>");
			fflush(stdin);
			gets_s(casos.cidades[contacidades].historia);
			_strupr(casos.cidades[contacidades].historia);
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
	for (i = 0; i < 10; i++){
		//pego os suspeitos do arquivo
		fread(&suspeitos[i], sizeof(tipoSuspeitos), 1, arquivoSuspeitos);
	}

	do{
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
	slowprint("Confirme os dados", DELAY);

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