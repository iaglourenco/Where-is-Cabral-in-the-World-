#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Onde no mundo esta o dinheiro do Brasil?


//adm
int loginadm(char user[], int password);  //retorna 1 se o login foi efetuado, 0 se não. 
void admconfig();
int addcaso();		// retorna 1 se a inserção foi um sucesso, 0 se não.

void cripto(int pass,int action); //funcao de criptografia de senhas, parametros ->> senha = senha a ser criptografada
																//   ->> action = acao que sera feita, ou seja,
     														    //       1 para criptografar 0 para descriptografar



void main(){

	// Abertura da referencia dos arquivos
	FILE *casos = fopen("casos.txt", "r+");
	if (!casos)
		printf("ERRO: Arquivo de casos nao encontrado!!\n");
	FILE *usuarios = fopen("users.txt", "r+");
	if (!usuarios)
		printf("ERRO: Arquivo de usuarios nao encontrado");

	char usuario[50];
	
	printf("Where is Sergio Cabral in the World?\n\n");
	printf("Detetive no teclado, digite seu usuario\n\n");
	gets(usuario);
}

int loginadm(char user[], int password){			//retorna 1 se o login foi efetuado, 0 se não.
	/*FUNCIONAMENTO DOS ARQUIVOS DE LOGIN 

	       <usuario:senha>
		Ex.

			<iago:1234>
	*/


	FILE *admdata = fopen("data.txt", "r+");
	if (!admdata)
		printf("ERRO: Arquivos do adm inacessiveis!!\n");

	int tam = strlen(user);
	char c,usuario[50];
	int i;


	while (!feof(admdata))			//ate o final do arquivo
	{

		c = fgetc(admdata);

		if (c == '<')
		{
			c = fgetc(admdata);
		}
		else
		{

			while (i < tam)
			{
				usuario[i] = c;
				c = fgetc(admdata);
				i++;

			}

			if (strcmp( usuario , user ) != 0 ){
				return 0;
			}



		}


	}

	




}