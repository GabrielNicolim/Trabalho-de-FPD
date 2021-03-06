/* 
	Trabalho de FPD 4 Bimestre 2020					
	
	Felipe Lima Estevanatto 06
	Gabriel Gomes Nicolim 08
	
			71/81A
			
		CTI - Bauru - 2020
		
	- Este projeto visa aplicar os conceitos aprendidos em FPD e TP ao longo de 2020.
	
	-Para rodar nas versões antigas do DEVC++ Orwell basta descomentar todas as linhas com uso do text_info(linha 30) e vActual(1947 e 1957)
	Versão 4.5 = Exclusão física adicionada, fix na função busca por ID e melhoria da consulta tipo

*/

#include <stdio.h>
#include <math.h>
#include <windows.h> //inclue a wincon.h e winuser.h
#include <conio.h>
#include <time.h>
#include <string.h>
#include <io.h> //para checagem se o arquivo existe
#include "functions\desenhos.h"

#define versao 4.5

//text_info vActual = {0, 0, 79, 24, WHITE, WHITE, C80, 160, 38, 1, 1}; // Define os limites para linha (38) e coluna (160)

// Funções conio.c

void textcolor(int newcolor);
void textbackground(int newcolor);
void gotoxy(int x, int y);
void cursor (int x);
void clreol(int x);

// Funções de construção do programa

void loading();
void inicio(); 
void borda(); 

// Desenhos presentes no arquivo desenhos.h (Todos com int ic, int il para escolher posição)
//	-Disquete, Logo, Lupa, CPU, GPU, RAM, Mobo, HDD	

// Funções de interação com o usuário 
void consulta_geral();
void consulta_tipo();
void consulta_nome();
void excluir_dados();
void gera_tabela(int li);  
void gera_tabela_vertical(int li); 
void completa_tabela(int linha);
void completa_tabela_vertical(int coluna, int linha); 
void tabela_tipos(int c, int l, int L);
int navegar_menu(int ini, int fim, int p); // Recebe inicio e fim do menu e a posição do cursor
void cadastro_visual();
void cadastro_recebimento();
void consulta_id();

// Menu 

void menu_ini();
void alteracao(); 
void sub_menu();
void info_de_sistema();
void sair();

//==============================================
// Funções de validação => Validações + Registro
//================================================

bool isID_cadastrado(int aux);
long valida_id(int L, int C, int qntapagar);
long valida_quantidade_recebimento(int L, int C);
char *valida_nome_recebimento(int L, int C);
char valida_tipo_recebimento(int L, int C);
float valida_preco_recebimento(int L, int C);

// mensagem alteraveis e de confirmação
void erro_apagar (int col, int lin, int tipo_erro, int apagar);
void voltando_menu(int linha,int coluna, int delay, bool menu);
bool confirmarSN(int L, int C,int confirmaTipo); 


// variáveis globais

int obtem_id_alteracao();
int random_menu; // Var que armazena numero aleatório ente 0 e 3 para gerar um menu diferente 

FILE *fp; //Ponteiro para arquivo


void abrir_arquivo()
{
	if((fp = fopen("estoque.bin", "ab+")) == NULL){
		system("cls"); borda();
		textcolor(cor_texto);
		gotoxy(65, 16); printf("Erro na abertura do arquivo!"); //caso haja um problema com o arquivo
		gotoxy(1, 33);
		exit(1);
	}
}

int abrir_arquivo_alterar()
{
	if((fp = fopen("estoque.bin", "rb+")) == NULL){
		cursor(0); 
		gotoxy(40, 16); printf("N%co foram encontrados dados, cadastre algo!", 198);
		getch();
		return 1;
	}
	else return 0;
}

struct estrutura
{
	long id = 1;
	char nome[194];  // espaço da tela de cadastro
	long quantidade;
	char tipo;
	double preco_unitario;
	bool excluido;
	
}produto;

//======================================================================
//							Começo do programa
//======================================================================

main()
{
	SetConsoleTitle("GoTech"); 		   // Define o nome do console
    system("mode con:cols=160 lines=37");  // Define o tamanho do console
    
    //======================================================================
	//Pega o tamanho do monitor e joga o programa pro meio do monitor
	//======================================================================
	
    DWORD dwWidth = GetSystemMetrics(SM_CXSCREEN);  // essas funções pegam o tamanho da tela em pixels (ex: 1920 x 1080)
	DWORD dwHeight = GetSystemMetrics(SM_CYSCREEN);
    
    HWND wh = GetConsoleWindow(); // precisa disso pro MoveWindow  (HWND é como é indentificada a janela do Windows "identificador de janelas" 
    MoveWindow(wh, dwWidth/8 , dwHeight/8 , 1300, 720, TRUE); // move a janela para uma cordenada determinada a setando num tamanho determinado de 20 em 20
    
    //======================================================================
    
	srand(time(NULL));
	random_menu = rand() % 3; // Escolhe o menu que será exibido ao usuário de maneira randomizada
	
    loading();
    
	inicio(); // Função de construção da tela inicial
}

void loading()
{
	borda(); 
	
	cursor(0);
	
	// Tela de loading com apresentação da versãodo programa
	
	textcolor(cor_texto);
	
	gotoxy(58, 10); printf("Aguarde, estamos preparando tudo para voc%c!",136);
	gotoxy(15, 30); printf("Copyright%c GoTech",184);
	gotoxy(139, 30); printf("Vers%co %.1f",198, versao);
	
	textcolor(RED); // Loading vermelho
	
	for(int i = 0; i <= 52; i++){
		gotoxy(52+i, 20);printf("%c", 219);
		Sleep(10);
	}
	
	textcolor(cor_texto);
	gotoxy(58, 10); printf("     Tudo pronto! Podemos iniciar...                            ");
	Sleep(1500);
	while (kbhit())  //Isso continua se vê que há um input do teclado em espera e, em caso afirmativo, chama getch() para descartar o caractere
    getch();
}

//
// Primeira página que o usuário tem contato 
//

void inicio() // Apresenta tela de inicio
{	
	while(true){
		borda(); 
	
		switch(random_menu){ // Menu com icone aleatorio	
			case 0:
				disquete(38, 6); // Disquete + logo
				logo(68, 8);
				break;
			case 1:				// cpu + logo
				cpu(38, 6); 	
				logo(68, 8);
				break;	
			case 2:				// logo
				logo(52, 8); 	
				break;
			case 3:				// Ram + logo
				mobo(103, 6); 		
				logo(40, 8);
				break;		
		}
		
		menu_ini();
	}
}

void menu_ini()
{	 
	textcolor(cor_texto);
	
	gotoxy(70, 19); printf("Cadastro");
	gotoxy(70, 21); printf("Pesquisa");
	gotoxy(70, 23); printf("Altera%c%co", 135,198); 
	gotoxy(70, 25); printf("Info de Sistema");
	gotoxy(70, 27); printf("Sair");
	gotoxy(150, 41); printf("Vers%co %.1f",198, versao);
	
	// Chama navegação com setas
	
	int escolha = navegar_menu(19, 27, 68);
	
	switch(escolha){	
		case 0:
			cadastro_visual();
			break;
		case 1:
			sub_menu();
			break;
		case 2:
			alteracao(); 
			break; 
		case 3:
			info_de_sistema();
			break;
		case 4:
			sair();
			break;
	}
		
	return; 
}

//
// Escolhas possiveis no primeiro menu 
//

void cadastro_visual()
{
	abrir_arquivo();
	
	cursor(1);
	
	do{
		system("cls");
		
		borda();
		
		tabela_tipos(58,37,1);
		
		textcolor(cor_destaque); textbackground(15);
		gotoxy(69, 4);  printf(" Cadastro de Produtos "); // Mensagem em destaque no meio da tela
		textbackground(cor_fundo);
			
		gotoxy(20, 31); printf("Digite 0 no campo de ID para retornar ao menu");
		gotoxy(20, 8); 	printf("Id do produto....: ");				
		gotoxy(20, 10); printf("Nome do Produto..: ");		
		gotoxy(20, 12); printf("Quantidade.......: ");			
		gotoxy(20, 14); printf("Tipo.............: ");					
		gotoxy(20, 16); printf("Pre%co Unit%crio...: ", 135, 160);		
			
		cadastro_recebimento(); // Recebe as respostas do usuário
				
	}while( confirmarSN(20,26,3) );
	
	fclose(fp);
	
	return; 
}

void cadastro_recebimento()
{
	textcolor(cor_texto);
	
	fflush( fp ); 
	cursor(1);
	
	int IDaux = 1;	
	do{
		IDaux = valida_id(39,8,100); 						
		if(IDaux == 0){ 
			inicio();  return;												
		}
		else{
			if(isID_cadastrado(IDaux)){
				erro_apagar( 39, 8, 1 , 70); //coluna, linha, tipo de erro: "[ERRO] ID já cadastrado" e quantidade a se apagar	
				IDaux = -1; 	//continua no loop
			}else break;
		}
	}while(IDaux == -1);
		
	produto.id = IDaux;												// Após validação toda copia para a struct
	strcpy(produto.nome,valida_nome_recebimento(39,10));			// Recebe nome e já copia para a struct
	produto.quantidade = valida_quantidade_recebimento(39,12);		// Recebe quantidade e já copia para a struct					
	produto.tipo = valida_tipo_recebimento(39,14);		 			// Recebe tipo e já copia para a struct
	produto.preco_unitario = valida_preco_recebimento(39,16);	 	// Recebe preco e já copia para a struct
	produto.excluido = false;						 				// Define como não excluido
	
	textcolor(cor_destaque);

	//Se quiser confirmar o cadastro:
   	if( confirmarSN(65,22,2) ){
   		
		if(fwrite(&produto, sizeof(produto), 1, fp) != 1){		
			textcolor(RED);
			gotoxy(79, 22); printf("Erro na escrita do arquivo!");
			textcolor(cor_texto);			
		}
		else{  //se tudo der certo
		
			fflush( fp ); //limpa buffer arquivo 
			
			borda();
			
			switch(random_menu){		
				case 0:
					ram(85, 8);
					break;
				case 1: 
					gpu(88, 8);
					break;
				case 2:
					disquete(85, 8);
					break;
			}
			cursor(0); 
			
			textcolor(cor_texto);
			gotoxy(20,11); printf("Dados salvos com sucesso!");	
			textcolor(cor_destaque);		
		}
		
		Sleep(1000);
		while (kbhit())  //Isso continua se vê que há um input do teclado em espera e, em caso afirmativo, chama getch() para descartar o caractere
    	getch();
		
		cursor(1);
			
		return; 
	}
}

// 
// Parte do recebimento de dados 
// 

long valida_id(int L, int C, int qntapagar)  // Recebe e valida id
{
	char id[80];
	long tam, aux;		// Armazena tamanho da string | Auxiliar 1 e 2 | int auxiliar que armazenará a id digitada
	bool valido, vazio;
	
	do{		
		vazio = true;  //loop principal
		valido = false;
		cursor(1);
		
		fflush(stdin);
				
		gotoxy(L, C); gets(id);
		
		tam = strlen(id);
		
		if(tam == 0) gotoxy(L, C); // Se nada foi digitado retorna a posição de inicio e pergunta novamente
		else if(id[0] == '0'){	
			// Se o primeiro digito de id for igual a 0 volta pro menu
			voltando_menu(72,35,1300,true);
			return 0; //if(valida_id_recebimento(int L, int C) == 0) inicio(); 
		} 
		else{
			if(tam > 6){  //se id > 999999
				vazio = true; 			
				erro_apagar( L, C, 0 , qntapagar); //coluna, linha, tipo de erro: "[ERRO] ID inválido" e quantidade a se apagar
			}
			else{		
				for(int i = 0; i < tam; i++){ // verifica caractere por caractere se é número			
					if(id[i] != '0' && id[i] != '1' && id[i] != '2' && id[i] != '3' && id[i] != '4' && id[i] != '5' && id[i] != '6' && id[i] != '7' && id[i] != '8' && id[i] != '9') 
					{
						erro_apagar( L, C, 0 , qntapagar); //coluna, linha, tipo de erro: "[ERRO] ID inválido" e quantidade a se apagar
						break;
					}else valido = true;
				} 
				if(valido){  // Se não for numerico ou for muito grande	= passou no teste							
				
					aux = atoi(id); //aux é int e id é char[10] então converte a string para int (tambem poderia ser aux = strtol (id,NULL,10); )
					vazio = false;
				}			
			}
		}
	}while(vazio);
					
	return aux;   // retorna o valor ID já verificado
}

bool isID_cadastrado(int aux){   //verifica se o ID mandado está presente no arquivo ou não
	
	rewind(fp); 

	//enquanto não chegar o final do arquivo E (produto.id for diferente de auxiliar OU (produto.id for igual auxiliar E for excluido) )
	while(fread(&produto, sizeof(produto), 1, fp) == 1){																
		if(produto.id == aux and !produto.excluido){				
			return true;
		}
	}	
	return false;	
}

char *valida_nome_recebimento(int L, int C) // Recebe e valida nome retirando espaços extra ( a     palavras -> a palavra)
{
	char nome[255];
	
	int tam; 		 // Armazena tamanho da string
	char *output = nome;
	char *from, *to;
	bool space= false;
	
	while(true){
				
		gotoxy(L, C); gets(nome);
			
		tam = strlen(nome);
		
		if(tam == 0) gotoxy(L, C); // Se nada for digitado
		if(tam > 150) erro_apagar(39,10,5,70); //[ERRO] Sem espaço no estoque!
		else{
			//magia negra para tirar espaços extras do nome
		    to=from=output;    //variáveis de ponteiro ficam td igual ao input
		    while(true){
		        if(space && *from == ' ' && to[-1] == ' ') //Se tiver marcado com espaço & from & to[-1]
		            ++from;
		        else{
		            if(*from==' ')space = true;		 //Se *from for espaço, space = true, else space = false
		            else space=false;
		
		            *to++ = *from++;   //seta um igual ao outro e soma 1 depois
					
		            if(!to[-1])break;
		        }
		    } 
		    break;
		}	
	}
	
	return output;             
}

long valida_quantidade_recebimento(int L, int C) // Recebe e valida quantidade
{
	char aux[50];
	
	int tam;
	bool vazio, invalido; // Auxiliares
	
	do{
		fflush(stdin);
		
		vazio = true;
		invalido = false;
		
		gotoxy(L, C); gets(aux);
		tam = strlen(aux); // Tamanho da string

		if(tam == 0){
			gotoxy(L, C);	// Se nada for digitado volta pro começo e pergunta novamente	
		} 				
		else {
			for(int i = 0; i < tam; i++){
				if(aux[i] != '0' && aux[i] != '1' && aux[i] != '2' && aux[i] != '3' && aux[i] != '4' && aux[i] != '5' && aux[i] != '6' && aux[i] != '7' && aux[i] != '8' && aux[i] != '9'){			 
					invalido = true;		// Se não é numérico 
					break;				// Se não é numérico 
				}
			}
			if(invalido == true){ // Erro	
				erro_apagar( L, C, 2 , 70); //coluna, linha e tipo de erro: "[ERRO] Quantidade Inválida" quantidade a se apagar
				continue;
			}
			if(strtol(aux, NULL, 10) > 9999999){	// Se é muito grande
				erro_apagar( L, C, 5, 70); 	//coluna, linha, tipo de erro: "[ERRO] Sem espaço no estoque" e quantidade a se apagar
				continue;
			} 	
			else{
				vazio = false;
				invalido = false;				
			}
		
		}
	}while(vazio);
			
	return strtol(aux, NULL, 10);  //retorna em long o valor
}

char valida_tipo_recebimento(int L, int C) // Recebe e valida tipo 
{
 	char tipo;
 	bool valido=false;
 	
 	do{	
 		
 		fflush(stdin);
 	
 		gotoxy(L, C); tipo = getche();
		
		tipo = toupper(tipo);
		
		// Verificações
		if(tipo == '\r' ) gotoxy(L,C); //se nada for digitado
		else if(tipo == 'P' || tipo == 'G' || tipo == 'C' || tipo == 'M' || tipo == 'F' || tipo == 'W' || tipo == 'A' || tipo == 'R' || tipo == 'O') valido=true;
		else{
			erro_apagar( L, C, 3 , 70); //coluna, linha, tipo de erro: "[ERRO] Tipo inválido" e quantidade a se apagar	
		}
		
	}while(!valido);
		
	return tipo; 
}

float valida_preco_recebimento(int L, int C) // Recebe preço e valida
{
	char aux[32];
	int tam;
	char* ptr; // Ponteiro pra segurar o lixo da string
	float num;
	bool numerico=false;
	
	do{	
		fflush(stdin);
		
		gotoxy(L, C); gets(aux);
		
		tam = strlen(aux); // Recebe tamanho da string	
		
		if(tam == 0) gotoxy(L, C); // Se nada for digitado
		else{
					
			for(int i = 0; i < tam; i++){
				if(aux[i] == ',') aux[i]='.';	
				if((aux[i] < '0' || aux[i] > '9') && aux[i] != '.') { // Verifica se é numérico	 
					// Erro					
					erro_apagar( L, C, 4 , 70 ); //coluna, linha , tipo de erro: "[ERRO] Valor inválido" equantidade a se apagar											
					break;
				}else numerico=true;
			}						
			if(numerico){ // Se valor for numérico
				numerico=false;
				num = strtod(aux, &ptr); // Converte para float

				if(num < 0)
				{
					erro_apagar( L, C, 4 ,70); //coluna, linha, tipo de erro: "[ERRO] Valor inválido" e quantidade a se apagar	
				}
				else if(num > 1000000)
				{
					erro_apagar( L, C, 6,70);  // coluna, linha, tipo de erro: "[ERRO] Não aceitamos objetos desse valor" e quantidade a se apagar
				}
				else numerico=true;
			}
		}
	}while(!numerico);
		
	return num; 
}

void erro_apagar(int col, int lin, int tipo_erro, int apagar)
{
	cursor(0);	
	textbackground(cor_fundo);
	gotoxy(col, lin); clreol(apagar);
	textbackground(12);
	gotoxy(col, lin); //seta o cursor no lugar para apresentar o erro
	switch(tipo_erro){
		case 0:
			printf("[ERRO] ID inv%clido",160);	//[ERRO] Id inválido
			break;
		case 1:
			printf("[ERRO] ID j%c cadastrado", 131); //[ERRO] Id já cadastrado
			break;
		case 2:
			printf("[ERRO] Quantidade inv%clida",160); //[ERRO] Quantidade inválida
			break;
		case 3:
			printf("[ERRO] Tipo inv%clido",160);	//[ERRO] Tipo inválido
			break;
		case 4:
			printf("[ERRO] Valor inv%clido",160); //[ERRO] Valor inválido
			break;
		case 5:
			printf("[ERRO] Sem espa%co no estoque!",135);	//[ERRO] Sem espaço no estoque!
			break;
		case 6:
			printf("[ERRO] N%co aceitamos objetos desse valor", 198);	//[ERRO] Não aceitamos objetos desse valor
			break;
		default:
			printf("[ERRO] Valor inv%clido",160); //[ERRO] Valor inválido
			break;
	}	
	Sleep(1000);
	while (kbhit())  //Isso continua se vê que há um input do teclado em espera e, em caso afirmativo, chama getch() para descartar o caractere
    getch();
	textbackground(cor_fundo);
	gotoxy(col, lin); clreol(apagar);
	cursor(1);	
	gotoxy(col, lin);	
}

void sub_menu() // Gera a parte visual e realiza a escolha da opção do submenu 
{
	int escolha;
	
	do{	
		borda();
		
		lupa(63, 4);
		
		// Menu 
		
		textcolor(cor_texto);		
		gotoxy(70, 19);  printf("Geral");
		gotoxy(70, 21);  printf("Busca por ID");
		gotoxy(70, 23);  printf("Busca por Tipo");
		gotoxy(70, 25);  printf("Busca por Nome"); 
		gotoxy(70, 27);  printf("Excluir Dados");
		gotoxy(70, 29);  printf("Retornar ao In%ccio", 161);
			
		escolha = navegar_menu(19, 29, 68);
		
		switch(escolha){		
			case 0:
				consulta_geral();
				break;
			case 1:
				consulta_id();
				break;
			case 2:
				consulta_tipo();
				break;
			case 3:
				consulta_nome(); 
				break;
			case 4:
				excluir_dados();
				break;
			case 5: 
				return; // Retorna ao menu inicial 
		} 
	}while(escolha != 5);
}

bool confirmarSN(int L, int C, int confirmaTipo){
	
	char confirmar;
	
	gotoxy(L, C); textcolor(cor_destaque);		
	switch(confirmaTipo){
		case 0:
		printf("Confirmar Altera%c%ces? (S/N): ", 135, 228);
		break;
		case 1:
		printf("Confirmar Exclus%co? (S/N): ", 198);
		L = L - 3;
		break;
		case 2:
		printf("Deseja salvar os dados? (S/N): ");
		L++;
		break;
		case 3:
		printf("Deseja realizar mais um cadastro? (S/N): ");
		L = L += 10;
		break;
	}			
	textcolor(cor_texto);
	
	gotoxy(L+30, C);
	do{
		confirmar = getche();
				
		if(confirmar != 's' && confirmar != 'S' && confirmar != 'n' && confirmar != 'N'){			
			erro_apagar(L+30,C,10,50);      //erro default do switch
		}
		else{
			if(confirmar == 's' || confirmar == 'S') return true;	// Confirmação
			else return false;
		}			
	}while(confirmar != 's' && confirmar != 'S' && confirmar != 'n' && confirmar != 'N');
	
	return 0;	
}

void alteracao()
{
	int id, escolha;

	do{
		borda(); 
		
		HDD(69, 4);
		
		textcolor(cor_texto);				
		gotoxy(70, 19); printf("Altera%c%co Geral", 135, 198);
		gotoxy(70, 21); printf("Alterar Nome");
		gotoxy(70, 23); printf("Alterar Quantidade");
		gotoxy(70, 25); printf("Alterar Tipo"); 
		gotoxy(70, 27); printf("Alterar Pre%co", 135);
		gotoxy(70, 29); printf("Retornar ao In%ccio", 161);
		
		
		escolha = navegar_menu(19, 29, 68); 
		
		if(escolha == 0){ // Se alteraçõa geral for escolhida		
			id = obtem_id_alteracao();
			if(id == 0) continue; // ignora o restante do if
			
			borda();
			
			tabela_tipos(58,37,1);
			
			textcolor(cor_destaque); textbackground(15);			
			gotoxy(76, 4); printf("Altera%c%co Geral", 135, 198); // Mensagem em destaque no meio da tela
			textcolor(cor_texto);    textbackground(cor_fundo);
						
			//GERA TABELA COM OS DADOS JÀ INCLUÍDOS
			//=================================================================================
			gotoxy(20,5);	printf("-Dados originais:");		
			gera_tabela_vertical(7);  //faz a tabela e já completa ela automaticamente
			//=================================================================================

			gotoxy(20,21);	printf("-Novos dados:");	
			textcolor(cor_destaque);
			
			gotoxy(20, 23); printf("Nome do Produto..: ");		
			gotoxy(20, 25); printf("Quantidade.......: ");			
			gotoxy(20, 27); printf("Tipo.............: ");					
			gotoxy(20, 29); printf("Pre%co Unit%crio...: ", 135, 160);
			textcolor(cor_texto);
			
			cursor(1);
							
			/// Alteração do nome /// Recebimento do novo nome ///
			
			char aux_nome[150]; // Armazena nome a ser alterado(97)		
			strcpy(aux_nome,valida_nome_recebimento(39,23));
			
			/// Alreração de Quantidade /// Recebimento de nova quantidade ///
					
			float aux_quantidade = valida_quantidade_recebimento(39,25);
						
			/// Alteração de tipo /// Recebimento de novo tipo ///
			
			char aux_tipo = valida_tipo_recebimento(39,27);

			/// Alteração de Preço /// Recebimento do novo preço ///
			
			float num = valida_preco_recebimento(39,29);
			
			cursor(1);
			
			if(confirmarSN(70,31,0)){
				abrir_arquivo_alterar();

				while(fread(&produto, sizeof(produto), 1, fp) == 1)	
				{
					if(produto.id == id and !produto.excluido)
					{
						int fposicao = ftell(fp); // guarda a posição do registro atual do arquivo 
						
						//Escreve as alterações na struct
						strcpy(produto.nome, aux_nome);
						produto.quantidade = aux_quantidade; // Retorna quantidade em int
						produto.tipo = aux_tipo;
						produto.preco_unitario = num; 
						
						if(fseek (fp, fposicao - (sizeof(produto)), SEEK_SET) != 0) 	//SEEK_SET indica o início do arquivo, funciona igual o rewind(fp); 
						{																// porém pode ser usado em verificações pois retorna algo
							gotoxy(60, 11);	printf("Houve um erro catastrofico voltando ao inicio do arquivo!");
							Sleep(1500);
							return;
						}
							
						if(fwrite(&produto, sizeof(produto), 1, fp) != 1)   //depois que colocou o "cursor" do leitor em cima da linha correta
						{													//usa-se o fwrite para salvar as alterações no arquivo
							textcolor(RED);
							gotoxy(79, 22); printf("Erro na escrita do arquivo!");
							textcolor(cor_texto);
						}
						break;
						
						fflush(fp);
					}
				}
			}
			
			fclose(fp);
			
			id = 0; 
		}
		else if(escolha == 1) // Alterar Nome
		{
			id = obtem_id_alteracao();	
			if(id == 0) continue; // ignora o restante do if
			
			borda();
			
			textcolor(cor_texto);
			
			//GERA TABELA COM OS DADOS JÀ INCLUÍDOS
			//=================================================================================
			gotoxy(20,5);	printf("-Dados originais:");		
			gera_tabela_vertical(7);  //faz a tabela e já completa ela automaticamente
			//=================================================================================

			gotoxy(20,21);	printf("-Novos dados:");	
			textcolor(cor_destaque);
			
			textbackground(15);
			gotoxy(73, 4); printf(" Altera%c%co de nome ", 135, 198); // Mensagem em destaque no meio da tela
			textbackground(cor_fundo);
			
			gotoxy(20, 25); printf("Nome do Produto..: ");		

			cursor(1);
			
			textcolor(cor_texto);
					
			/// Alteração do nome /// Recebimento do novo nome ///
			
			char aux_nome[150]; // Armazena nome						
			strcpy(aux_nome,valida_nome_recebimento(39,25));
			
			//CONFIRMAR PARA ESCREVER NO ARQUIVO
			if(confirmarSN(20,27,0)){  
				
				abrir_arquivo_alterar();

				while(fread(&produto, sizeof(produto), 1, fp) == 1){	
					if(produto.id == id and !produto.excluido){
						
						int fposicao = ftell(fp); // guarda a posição do registro atual do arquivo 
						
						// Alteração					
						strcpy(produto.nome, aux_nome);  //coloca o nome alterado no registro original
				
						if(fseek (fp, fposicao - (sizeof(produto)), SEEK_SET) != 0) 	//SEEK_SET indica o início do arquivo, funciona igual o rewind(fp); 
						{														// porém pode ser usado em verificações pois retorna algo
							gotoxy(20, 29);	printf("Houve um erro catastrofico voltando ao inicio do arquivo!");
							Sleep(1500);
							return;
						}
							
						if(fwrite(&produto, sizeof(produto), 1, fp) != 1)   //depois que colocou o "cursor" do leitor em cima da linha correta
						{													//usa-se o fwrite para salvar as alterações
							textcolor(RED);
							gotoxy(79, 22); printf("Erro na escrita do arquivo!");
							textcolor(cor_texto);
						}
						
						fflush(fp);						
						fclose(fp);
						break;							
					}
				}
			}
				
			id = 0; 
		}
		else if(escolha == 2) //Alterar Quantidade
		{
			id = obtem_id_alteracao();
			if(id == 0) continue; // ignora o restante do if
			
			borda();
			
			textcolor(cor_texto);
			
			//GERA TABELA COM OS DADOS JÀ INCLUÍDOS
			//=================================================================================
			gotoxy(20,5);	printf("-Dados originais:");		
			gera_tabela_vertical(7);  //faz a tabela e já completa ela automaticamente
			//=================================================================================
			
			gotoxy(20,23);	printf("-Novos dados:");	
			textcolor(cor_destaque);
			
			textbackground(15);
			gotoxy(71, 4); printf(" Altera%c%co de Quantidade ", 135, 198); // Mensagem em destaque no meio da tela
			textbackground(cor_fundo);
				
			gotoxy(20, 25); printf("Quantidade.......: ");			
			
			cursor(1);
			
			textcolor(cor_texto);
							
			long aux_quantidade = valida_quantidade_recebimento(39,25);
			
			//CONFIRMAR PARA ESCREVER NO ARQUIVO
			if(confirmarSN(20,27,0)){
				
				abrir_arquivo_alterar();

				while(fread(&produto, sizeof(produto), 1, fp) == 1){	
					if(produto.id == id and !produto.excluido){
						
						int fposicao = ftell(fp); // guarda a posição do registro atual do arquivo 
						
						// Alteração					
						produto.quantidade = aux_quantidade;  //coloca o nome alterado no registro original
				
						if(fseek (fp, fposicao - (sizeof(produto)), SEEK_SET) != 0) 	//SEEK_SET indica o início do arquivo, funciona igual o rewind(fp); 
						{														// porém pode ser usado em verificações pois retorna algo
							gotoxy(20, 29);	printf("Houve um erro catastrofico voltando ao inicio do arquivo!");
							Sleep(1500);
							return;
						}
							
						if(fwrite(&produto, sizeof(produto), 1, fp) != 1)   //depois que colocou o "cursor" do leitor em cima da linha correta
						{													//usa-se o fwrite para salvar as alterações
							textcolor(RED);
							gotoxy(79, 22); printf("Erro na escrita do arquivo!");
							textcolor(cor_texto);
						}
						
						fflush(fp);
						fclose(fp);						
						break;					
					}
				}
			}
						
			id = 0; 
		}
		else if(escolha == 3) //Alterar Tipo
		{
			id = obtem_id_alteracao();
			if(id == 0) continue; // ignora o restante do if
			
			borda();
			
			textcolor(cor_texto);
			
			//GERA TABELA COM OS DADOS JÀ INCLUÍDOS
			//=================================================================================
			gotoxy(20,5);	printf("-Dados originais:");		
			gera_tabela_vertical(7);  //faz a tabela e já completa ela automaticamente
			//=================================================================================
	     	
			gotoxy(20,23);	printf("-Novos dados:");	
			textcolor(cor_destaque);
			
			tabela_tipos(58,37,1);
			
			textbackground(15);	textcolor(cor_destaque);		
			gotoxy(73, 4); printf(" Altera%c%co de Tipo ", 135, 198); // Mensagem em destaque no meio da tela	
			textbackground(cor_fundo);
						
			gotoxy(20, 25); printf("Tipo.............: ");					
			textcolor(cor_texto);
			
			cursor(1);
			char aux_tipo = valida_tipo_recebimento(39,25);
			
			//CONFIRMAR PARA ESCREVER NO ARQUIVO
			if(confirmarSN(20,27,0)){
				
				abrir_arquivo_alterar();

				while(fread(&produto, sizeof(produto), 1, fp) == 1){	
					if(produto.id == id and !produto.excluido){
						
						int fposicao = ftell(fp); // guarda a posição do registro atual do arquivo 
						
						// Alteração					
						produto.tipo = aux_tipo;  //coloca o nome alterado no registro original
				
						if(fseek (fp, fposicao - (sizeof(produto)), SEEK_SET) != 0) 	//SEEK_SET indica o início do arquivo, funciona igual o rewind(fp); 
						{														// porém pode ser usado em verificações pois retorna algo
							gotoxy(20, 29);	printf("Houve um erro catastrofico voltando ao inicio do arquivo!");
							Sleep(1500);
							return;
						}
							
						if(fwrite(&produto, sizeof(produto), 1, fp) != 1)   //depois que colocou o "cursor" do leitor em cima da linha correta
						{													//usa-se o fwrite para salvar as alterações
							textcolor(RED);
							gotoxy(79, 22); printf("Erro na escrita do arquivo!");
							textcolor(cor_texto);
						}
						
						fflush(fp);
						fclose(fp);						
						break;					
					}
				}
			}
					
			id = 0; 
		}
		else if(escolha == 4) //Alterar Preço
		{
			id = obtem_id_alteracao();
			if(id == 0) continue; // ignora o restante do if
			
			borda();
			
			textcolor(cor_texto);
			
			//GERA TABELA COM OS DADOS JÀ INCLUÍDOS
			//=================================================================================
			gotoxy(20,5);	printf("-Dados originais:");		
			gera_tabela_vertical(7);  //faz a tabela e já completa ela automaticamente
			//=================================================================================
			
			gotoxy(20,23);	printf("-Novos dados:");	
			textcolor(cor_destaque);
			textbackground(15);
			gotoxy(75, 4); printf(" Altera%c%co de Pre%co ", 135, 198, 135); // Mensagem em destaque no meio da tela
			textbackground(cor_fundo);						
			gotoxy(20, 25); printf("Pre%co Unit%crio...: ", 135, 160);
			
			cursor(1);
			
			textcolor(cor_texto);		
			
			float num = valida_preco_recebimento(39,25);
			
			//CONFIRMAR PARA ESCREVER NO ARQUIVO
			if(confirmarSN(20,27,0)){
				
				abrir_arquivo_alterar();

				while(fread(&produto, sizeof(produto), 1, fp) == 1){	
					if(produto.id == id and !produto.excluido){
						
						int fposicao = ftell(fp); // guarda a posição do registro atual do arquivo 
						
						// Alteração					
						produto.preco_unitario = num;  //coloca o nome alterado no registro original
				
						if(fseek (fp, fposicao - (sizeof(produto)), SEEK_SET) != 0) 	//SEEK_SET indica o início do arquivo, funciona igual o rewind(fp); 
						{														// porém pode ser usado em verificações pois retorna algo
							gotoxy(20, 29);	printf("Houve um erro catastrofico voltando ao inicio do arquivo!");
							Sleep(1500);
							return;
						}
							
						if(fwrite(&produto, sizeof(produto), 1, fp) != 1)   //depois que colocou o "cursor" do leitor em cima da linha correta
						{													//usa-se o fwrite para salvar as alterações
							textcolor(RED);
							gotoxy(79, 22); printf("Erro na escrita do arquivo!");
							textcolor(cor_texto);
						}
						
						fflush(fp);
						fclose(fp);						
						break;					
					}
				}
			}
					
			id = 0; 
		}
		else if(escolha == 5) break; //sair do menu
		
	}while(id == 0);
		
	return;
}

int obtem_id_alteracao()
{		
	int aux;		// auxiliar que armazenará a id digitada
	bool encontrado;
	
	abrir_arquivo();
	
	do{
		borda();
		
		cursor(1);
		
		textcolor(cor_destaque);		
		gotoxy(71, 4); textbackground(15);  printf(" Recebimento de ID "); textbackground(cor_fundo);
		gotoxy(20, 8); 	printf("Id do produto (0 para sair): ");			
		textcolor(cor_texto);
			
		encontrado = false;  //loop principal

		rewind(fp);	
		
		aux = valida_id( 49, 8, 100);
			                                                                    
		if(aux == 0){ 
			return 0; // Retorna 0 para que a func de alteracao saia do loop 
		} 
		else{
			
			if(isID_cadastrado(aux)){
				textbackground(15); textcolor(cor_destaque); 
				gotoxy(72, 16); printf("  Item encontrado!  ");
				textbackground(cor_fundo);
				Sleep(800);
				encontrado = true;
				break;
			}
			else{
				textcolor(cor_destaque); 
				gotoxy(20, 30); printf("Pressione uma tecla para continuar...");
				textbackground(15);
				gotoxy(60, 16); printf(" [ Nenhum item com este ID foi encontrado ] ");
				textbackground(cor_fundo);
				getch();
				encontrado = false;
			}	
		}					
	}while(!encontrado);
	
	fclose(fp);
	
	return aux; 
}

void info_de_sistema() // Apresenta as informações do sistema
{
	borda();
	
	char input;
	
	do{		
		int L = 20, C = 7; // Controla o eixo x e y das informações
		
		// Apresenta as informações do sistema
		textcolor(cor_destaque); textbackground(15);
		gotoxy(73, 4);  printf(" Info do Sistema ");
		textbackground(cor_fundo);
		
		time_t t = time(NULL);
    	struct tm *tm = localtime(&t);
    	gotoxy(125, 4);printf("%s", asctime(tm));
		
		textcolor(cor_destaque);	
		gotoxy(L, C); 	 printf("Empresa: ");
		gotoxy(L, C+2);   printf("Nomes: ");
		gotoxy(L, C+4);  printf("N%cmeros: ",163);
		gotoxy(L, C+6);  printf("Turma: ");
		gotoxy(L, C+8);  printf("Ano: ");
		gotoxy(L, C+10);  printf("Sobre o software: ");
		
		textcolor(cor_texto);						
		gotoxy(L+9, C);	 printf("GoTech");
		gotoxy(L+7, C+2);   printf("Felipe Lima e Gabriel Nicolim");
		gotoxy(L+9, C+4);  printf("06 e 08");
		gotoxy(L+7, C+6);  printf("71A");
		gotoxy(L+5, C+8);  printf("2020");
		gotoxy(L, C+10);  printf("Sobre o software: ");
			
		textcolor(cor_texto);
		printf("Nosso software, que hoje se encontra na vers%co %.1f, se destina a simular um sistema de gerenciamento de",198, versao);
		gotoxy(20, 19); printf("estoque de uma loja de inform%ctica em C/C++.",160);
		gotoxy(20, 21); printf("Para a realiza%c%co desse projeto utilizamos fun%c%ces de cabe%calho da conio.c e conio.h e file da stdio.h.",135,198,135,228,135);
		gotoxy(20, 23); printf("Podem ser registrados perif%cricos e outras pe%cas de computador, que mais tarde podem ser alterados ou excluidos.",130,135);
			
		textcolor(cor_destaque);
		gotoxy(20, 30); printf("Pressione 0 para retornar ao menu"); 
		
		fflush(stdin); 
			
		input =	getch();
			
	}while(input != '0');
		
	voltando_menu(72,35,1500,true); // Apresenta mensagem Voltando pro menu... a baixo da borda
	
	return;
}

void sair() // Finaliza a execução do programa
{
	borda();
	
	textcolor(cor_texto);
	
	gotoxy(62, 17); printf("Obrigado por utilizar nosso programa!");
	
	fclose(fp);
	
	textcolor(cor_fundo); gotoxy(80, 34); // Esconde mensagem de encerramento 
	
	exit(1);
}

//
// Sub menu 
//

void consulta_geral()
{
	int cont_tuplas = - 1; // Contador de registros 
	
	char retornar = '0';
	
	abrir_arquivo();
	
	cursor(0); // Desliga o cursor
	
	bool vazio = true;  
	
	while(fread(&produto, sizeof(produto), 1, fp) == 1){// Verifica se o arquivo bin está vazio 
		if(!produto.excluido){		
			vazio = false;				//marca o arquivo como contendo dados e conta quantos tem
			cont_tuplas++;
		}
	}
	
	if(!vazio){	
	
		int contl = 1, limite, limiteAnte, pag, linha, pag_limite; // Variaveis Auxiliares
		
		pag = 1;
		
		pag_limite = ceil(cont_tuplas / 12);  
		
		gera_tabela(5);	// Gera borda e tabela inicial
		
		do{					
			limite = (12 * pag); // 12 linhas de dados por página (oq cabe na tabela)
			
			limiteAnte = (12 * (pag - 1));  // Limite da página anterior 
			
			textcolor(cor_texto);
			
			gotoxy(20, 31); printf("Pressione 0 para voltar ao menu de pesquisa");
			
			gotoxy(146, 4);	printf("%d", pag); // Número da página 
			
			contl= 1;    //reseta o contador de linha
			linha= 7;	//reseta a linha inicial(pmr da tabela) em q os dados começarão a ser colocados
			
			rewind(fp);
			
			while(fread(&produto, sizeof(produto), 1, fp) == 1){ // segue até o fim do arquivo
					
				if(contl > limiteAnte){ //se a linha atual for maior que o limite inferior:
				
					if(!produto.excluido){ // Só apresenta e vai para a próxima posição se o item não tiver sido excluido 			
						completa_tabela(linha);    //preenche a tabela
						linha += 2; 
					}
				}
				
				if(contl == limite) break; //se a linha atual for igual ao limite quebra
				else contl++; //adiciona mais uma linha ao contador
			}
			
			gotoxy(20,34); 
			
			fflush(stdin);
			
			retornar = getch();
			
			switch(retornar){	
				case char(77): // Se a seta direita for pressionada
					if(pag <= pag_limite) 
					{
						pag++; // Avança a página | Limita pag a 10 
						rewind(fp);		//seta a leitura do arquivo na posição inicial do arquivo ("1º linha e coluna")	
						gera_tabela(5);
					}	
					break;
					
				case char(75): // Se a seta da esquerda
					if(pag > 1) 				
					{
						pag--; // Volta a pagina
						rewind(fp);	
						gera_tabela(5);	
					} 			
					break;
			}			
		}while (retornar != '0'); 		
		fclose(fp);			
		voltando_menu(72,35,1500,false); // Apresenta mensagem Voltando... abaixo da borda
		
		return;    
	}
	else   //caso não há nenhuma tupla de dados
	{
		borda();
		textcolor(cor_destaque); 
		gotoxy(52, 16); printf("[ N%co h%c nenhum item registrado! Por favor registre algo ]", 198,160);
		gotoxy(62, 30); printf("Pressione qualquer tecla para voltar");
		getch();
		fclose(fp);	
		return;  // Retorna ao submenu
	}			
}

void consulta_id()   //consulta por id
{	
	
	long id_busca; // Armazena id digitado pelo usuário durante a busca 
	bool continua = true;
	
	textcolor(cor_texto);
	
	do{
		
		borda();
			
		textcolor(cor_destaque);
		gotoxy(73, 4); textbackground(15); printf(" Consulta por ID "); textbackground(cor_fundo);
		gotoxy(20, 7); printf("Digite o Id (0 para sair): ");
		textcolor(cor_texto);

		id_busca = valida_id(47, 7, 100); 
		
		if(id_busca == 0) return;
		else{
			abrir_arquivo(); cursor(0);
			
			if(isID_cadastrado(id_busca)){			
					
				rewind(fp);
				
				while(!feof(fp) && fread(&produto, sizeof(produto), 1, fp) ){
					
					if(produto.id == id_busca and !produto.excluido){				
						//GERA TABELA COM OS DADOS JÀ INCLUÍDOS
				   		//=================================================================================
				   		gera_tabela_vertical(11);
						//=================================================================================
						
							tabela_tipos(58,37,1);
					
							textcolor(cor_destaque);
							gotoxy(20,30);printf("Pressione uma tecla para continuar...");					
							
							fflush(stdin);
							getch();
						
						break;	//sai do loop
					}				
				};
			}
			else{
				textcolor(cor_destaque); 
				gotoxy(71, 18);printf("[ C%cdigo inexistente ]",162);
				gotoxy(20, 30);printf("Pressione uma tecla para redigitar...");
				
				getch();
			}
			fclose(fp); //fecha arquivo
		}
		
	}while(continua);
		
	return;
}

void consulta_tipo()
{	
	
	borda();
		
	textcolor(cor_destaque);textbackground(15);
	gotoxy(71, 4); printf(" Consulta por Tipo ");
	textbackground(cor_fundo);
			
	tabela_tipos(56,18,2);	
	gotoxy(20,7); printf("Digite tipo a ser pesquisado (0 para sair): ");

	int aux = valida_tipo_recebimento( 64, 7);
	if(aux == '0') return;
	
	int cont_tuplas = -1;
	
	abrir_arquivo();
	
	cursor(0); // Desliga o cursor
	
	bool vazio = true;
	
	while(fread(&produto, sizeof(produto), 1, fp) == 1){ // segue até o fim do arquivo contando quantos tipos escolhidos tem
		if(produto.tipo == aux && !produto.excluido){ 		
			vazio = false;
			cont_tuplas++;
		}
	}
	
	if(!vazio){	
		int contl = 1, limite, limiteAnte, pag, linha, pag_limite; // Variaveis Auxiliares		
		char retornar;
		
		pag = 1;
		
		pag_limite = ceil(cont_tuplas / 12);  
		
		gera_tabela(5);	
		
		do{
			
			limite = (12 * pag); // 12 linhas de dados por página (oq cabe na tabela)
			limiteAnte = (12 * (pag - 1));  // Limite da página anterior 
			
			textcolor(cor_texto);
			
			gotoxy(20, 31); printf("Pressione 0 para voltar a escolha do tipo");
			gotoxy(146, 4);	printf("%d", pag); // Número da página 
			gotoxy(146, 6); printf("%c", aux);
			
			contl= 1;    //reseta o contador de linha
			linha= 7;	//reseta a linha inicial(pmr da tabela) em q os dados começarão a ser colocados
			
			rewind(fp);
			
			while(fread(&produto, sizeof(produto), 1, fp) == 1) // segue até o fim do arquivo
			{			
				if(contl > limiteAnte){ //se a linha atual for maior que o limite inferior:				
					if(produto.tipo == aux && !produto.excluido){ // Só apresenta e vai para a próxima posição se o item não tiver sido excluido 					
						completa_tabela(linha);    //preenche a tabela
						linha += 2; 
					}
				}
				
				if(contl == limite) break; //se a linha atual for igual ao limite quebra
				else if(!produto.excluido and produto.tipo == aux) contl++; //adiciona mais uma linha ao contador
			}
			
			gotoxy(20,34); 
			
			fflush(stdin);
			
			retornar = getch();
			
			switch(retornar){
						
				case char(77): // Se a seta direita for pressionada
					if(pag <= pag_limite){ 
					
						pag++; // Avança a página | Limita pag a 10 
						rewind(fp);		//seta a leitura do arquivo na posição inicial do arquivo ("1º linha e coluna")	
						gera_tabela(5);
					}	
					break;
					
				case char(75): // Se a seta da esquerda
					if(pag > 1){ 				
					
						pag--; // Volta a pagina
						rewind(fp);	
						gera_tabela(5);	
					} 			
					break;
			}
			
		}while (retornar != '0');  
			
		// Se 0 for pressionado 
		voltando_menu(74,35,1500,false); // Apresenta mensagem Voltando... abaixo da borda
	}		
	else{
		borda();
		textcolor(cor_destaque); 
		cursor(0); 
		gotoxy(50, 16); printf("[ N%co h%c nenhum item desse tipo registrado! Por favor registre algo ]", 198,160);
		gotoxy(62, 30); printf("Pressione qualquer tecla para voltar");
		getch(); 
	}
	
	fclose(fp);	// fecha o arquivo
		
	return;
}

void consulta_nome()
{
	
	char aux[150], comp[80]; // Armazena nome  
	int tam, resultados, j;
	
	do{
	
		borda();
			
		cursor(1);
			
		textcolor(cor_destaque); 
		gotoxy(71, 4); textbackground(15); printf(" Consulta por Nome "); textbackground(cor_fundo);
		textcolor(cor_texto); gotoxy(20, 7); printf("Digite o Nome (0 para sair): ");
		
		strcpy(aux,valida_nome_recebimento(49,7));
		
		if(aux[0] == '0'){
			voltando_menu(74,35,1500,false); // Apresenta mensagem Voltando... abaixo da borda
			return;
		}	
		
		cursor(0);
		
		abrir_arquivo();
				
		resultados = 0;
		
		while(fread(&produto, sizeof(produto), 1, fp) == 1) // segue até o fim do arquivo
		{			
			strcpy(comp, produto.nome);
				
			if(strstr(strlwr(comp),strlwr(aux)) != NULL && !produto.excluido)	// Só apresenta e vai para a próxima posição se o item não tiver sido excluido
			{	
				resultados += 1;
			} 	
		}
		
		if(resultados == 0) 
		{
			textcolor(cor_destaque);
			gotoxy(69, 18); printf("[ Nome n%co encontrado ]", 198);			
		}
		else
		{
			if(resultados == 1)
			{
				rewind(fp);
				 
				while(fread(&produto, sizeof(produto), 1, fp) == 1) // segue até o fim do arquivo
				{			
					strcpy(comp, produto.nome);
						
					if(strstr(strlwr(comp),strlwr(aux)) != NULL && !produto.excluido)	// Só apresenta e vai para a próxima posição se o item não tiver sido excluido
					{	
						//GERA TABELA COM OS DADOS JÀ INCLUÍDOS
				   		//=================================================================================
				   		gera_tabela_vertical(11);
						//=================================================================================
						tabela_tipos(58,37,1);
						break;
					} 	
				}	
			}
			else
			{
				
				int cont_tuplas = resultados; // Contador de registros 
			
				char retornar = '0';
				
				cursor(0); // Desliga o cursor
		
				int contl = 1, limite, limiteAnte, pag, linha, pag_limite; // Variaveis Auxiliares
				
				pag = 1;
				
				pag_limite = ceil(cont_tuplas / 12);  
				
				gera_tabela(5);	// Gera borda e tabela inicial
				
				do{
							
					limite = (12 * pag); // 12 linhas de dados por página (oq cabe na tabela)
					
					limiteAnte = (12 * (pag - 1));  // Limite da página anterior 
					
					textcolor(cor_texto);
					
					gotoxy(20, 31); printf("Pressione 0 para voltar");
					
					gotoxy(146, 4);	printf("%d", pag); // Número da página 
					
					contl= 1;    //reseta o contador de linha
					linha= 7;	//reseta a linha inicial(pmr da tabela) em q os dados começarão a ser colocados
					
					rewind(fp);
					
					while(fread(&produto, sizeof(produto), 1, fp) == 1) // segue até o fim do arquivo
					{		
						strcpy(comp, produto.nome);
						
						if(contl > limiteAnte) //se a linha atual for maior que o limite inferior:
						{
							if(strstr(strlwr(comp), strlwr(aux)) != NULL && !produto.excluido) // Só apresenta e vai para a próxima posição se o item não tiver sido excluido 
							{
								completa_tabela(linha);    //preenche a tabela
								linha += 2; 
							}
						}
						
						if(contl == limite) break; //se a linha atual for igual ao limite quebra
						else if(strstr(strlwr(comp), strlwr(aux)) != NULL) contl++; //adiciona mais uma linha ao contador
					}
					
					gotoxy(20,34); 
					
					fflush(stdin);
					
					retornar = getch();
					
					switch(retornar){
									
						case char(77): // Se a seta direita for pressionada
							if(pag <= pag_limite) 
							{
								pag++; // Avança a página | Limita pag a 10 
								rewind(fp);		//seta a leitura do arquivo na posição inicial do arquivo ("1º linha e coluna")	
								gera_tabela(5);
							}	
							break;
							
						case char(75): // Se a seta da esquerda
							if(pag > 1) 				
							{
								pag--; // Volta a pagina
								rewind(fp);	
								gera_tabela(5);	
							} 			
							break;
					}
					
				}while (retornar != '0');  
				
				// Se 0 for pressionado 
				voltando_menu(75,35,1500,false); // Apresenta mensagem Voltando... abaixo da borda
				
				fclose(fp);	// fecha o arquivo
				
			}
		}
		
		if(resultados <= 1)
		{
			textcolor(cor_destaque);
		
			gotoxy(20, 30); printf("Pressione uma tecla para redigitar...");
			
			getch();	
		}				
	
	}while(aux[0] != '0');
	
	return;
	
}

void excluir_dados() //exclusao lógica (continua no binário)
{
	cursor(1); // Liga o cursor
	
	long fposicao;  //"linha" do struct no arquivo
	bool continuar = true;
	int IDaux = 1;
			
	do{
		borda();
	
		tabela_tipos(58,37,1); //apresenta a info doq as letras significam
		
		textcolor(cor_destaque); textbackground(15);
		gotoxy(71, 4); printf(" Exclus%co de dados ", 198);
		textbackground(cor_fundo);
		gotoxy(20,7); printf("Digite o c%cdigo do produto a ser excluido (0 para sair): ", 162);
		
		textcolor(cor_texto);
		
		//Vai receber a id digitada e já validar(se é numero) e devolver 0 se quiser voltar
		IDaux = valida_id(77, 7, 74); 
								
		if(IDaux == 0) return;
		else{
			
			abrir_arquivo_alterar();
			
			if(isID_cadastrado(IDaux)) // Se o ID for valido,numérico e encontrado
			{		
				rewind(fp);  //volta ao inicio do arquivo para mostrar o dado na tela
				
				while(!feof(fp)){
					fread(&produto, sizeof(produto), 1, fp);
					if(produto.id == IDaux and !produto.excluido){
									
						//GERA TABELA COM OS DADOS JÀ INCLUÍDOS
				   		//=================================================================================
				   		gera_tabela_vertical(11);
						//=================================================================================
						
						break;	//sai do loop pra n ter q fazer verificação extra (preguiça)
					}				
				};
				
			   	// exclusão é uma operação crítica, por isso, sempre será confirmada pelo usuário
			   	if(confirmarSN(20,24,1))
			   	{
			   		rewind(fp);
			   		fflush(fp);				   		
			   						   		
					//Cria o binário temporário para leitura e escrita
					FILE *fp_tmp;
					fp_tmp=fopen("tmp.bin", "wb+");
					if (!fp_tmp) {
						printf("Não foi possível abrir o arquivo temporário.");
						fclose(fp_tmp); 
					}
			   		
					int encontrado = 0;
			   		
			   		//Vai copiar tudo pra o novo arquivo temporário EXCETO o ID marcado
			   		while (fread(&produto,sizeof(struct estrutura),1,fp) != NULL) {
						if (produto.id == IDaux ){ 
							textcolor(cor_texto); textbackground(12);
							gotoxy(20,27);printf(" Cadastro exclu%cdo com sucesso!",161);
							gotoxy(20,28);printf(" %d registros encontrados e deletados!",++encontrado);
							textbackground(cor_fundo);
						}else {
							fwrite(&produto, sizeof(struct estrutura), 1, fp_tmp);
						}
					};
					
					fclose(fp);
					fclose(fp_tmp); 						 
					
					remove("estoque.bin");
					rename( "tmp.bin", "estoque.bin" );
					/*
					//checa se pode ter acesso e se o arquivo existe, retornando verificações de erro para debug( <io.h> )
					//if (_access_s("estoque.bin", 0) == 0)
						if (remove("estoque.bin") == 0)
					      printf("Arquivo original deletado com sucesso!    ");
					    else
					      perror("Não foi possivel deletar o aquivo!      ");
				    
				    //if (_access_s("tmp.bin", 0) == 0)
					    if ( rename( "tmp.bin", "estoque.bin" ) == 0 )
					    	puts ( "Arquivo renomeado com sucesso!" );
					    else
					    	perror( "Erro renomeando arquivo!" );
				    */	
					textcolor(cor_destaque);
			   	}
			   	else break; 	
						
			}
			else{
				cursor(0);
			
				textcolor(cor_destaque);
				gotoxy(68, 18);printf("[ C%cdigo n%co encontrado ]",162,198);
				
				textcolor(cor_destaque);	
			}
			
			fflush(fp);			// limpeza de buffers 
			fclose(fp);			// fechamento do arquivo
			
			gotoxy(20,30);printf("Pressione uma tecla para retornar...");	getch();
		}
	}while(continuar);
	
	return;
}


void tabela_tipos(int c, int l, int L)
{
    textcolor(cor_texto);
    
    gotoxy(c,l);    printf(" P - Perif%crico       G - Gpu      C - Cpu",130);
    gotoxy(c,l+L);    printf(" M - Mobo             F - Fonte    W - Cabos");
    gotoxy(c,l+L+L);    printf(" A - Armazenamento    R - Ram      O - Outros");
}

int navegar_menu(int ini, int fim, int p)
{
	cursor(0); // Desativa o cursor
	
	int aux = ini; // Recebe posição da seta
	int input;
	
	do{	
		gotoxy(p,aux); printf("%c", 62);	//posição inicial da seta
			
		fflush(stdin); 
		
		input = getch();
		
		gotoxy(p,aux); printf(" ");
		
		switch(input){		
			case 72: // Seta para cima
				aux -= 2;
				
				if(aux < ini) aux  = fim;

				gotoxy(p,aux);printf("%c", 62);
				break;
				
			case 80: // Seta para baixo
				aux+=2;
					
				if(aux > fim) aux = ini;
					
				gotoxy(p,aux);printf("%c", 62);
				break;	
			case 13:
				return (aux - ini)/2; // Retorna o valor da opção seleciona => inicia em 0
				break;	
		}
		
	}while(true);

}

void gera_tabela(int li)
{
	int ci = 20;

	borda();
	tabela_tipos(58,37,1); //apresenta a info doq as letras significam
	
	textcolor(cor_texto);
	gotoxy(ci,4);	printf ("+------------------------------------------------------------------------------------------------------------------------+");
	gotoxy(ci,5);	printf ("|    ID    |                                Nome                                |  Pre%co Unit%crio  | Quantidade |  Tipo  |",135, 160);
	gotoxy(ci,6);	printf ("|------------------------------------------------------------------------------------------------------------------------|");
	gotoxy(ci,7);	printf ("|          |                                                                    |                  |            |        |");
	gotoxy(ci,8);	printf ("|------------------------------------------------------------------------------------------------------------------------|");
	gotoxy(ci,9);	printf ("|          |                                                                    |                  |            |        |");
	gotoxy(ci,10);	printf ("|------------------------------------------------------------------------------------------------------------------------|");
	gotoxy(ci,11);	printf ("|          |                                                                    |                  |            |        |");
	gotoxy(ci,12);	printf ("|------------------------------------------------------------------------------------------------------------------------|");
	gotoxy(ci,13);	printf ("|          |                                                                    |                  |            |        |");
	gotoxy(ci,14);	printf ("|------------------------------------------------------------------------------------------------------------------------|");
	gotoxy(ci,15);	printf ("|          |                                                                    |                  |            |        |");
	gotoxy(ci,16);	printf ("|------------------------------------------------------------------------------------------------------------------------|");
	gotoxy(ci,17);	printf ("|          |                                                                    |                  |            |        |");
	gotoxy(ci,18);	printf ("|------------------------------------------------------------------------------------------------------------------------|");
	gotoxy(ci,19);	printf ("|          |                                                                    |                  |            |        |");
	gotoxy(ci,20);	printf ("|------------------------------------------------------------------------------------------------------------------------|");
	gotoxy(ci,21);	printf ("|          |                                                                    |                  |            |        |");
	gotoxy(ci,22);	printf ("|------------------------------------------------------------------------------------------------------------------------|");
	gotoxy(ci,23);	printf ("|          |                                                                    |                  |            |        |");
	gotoxy(ci,24);	printf ("|------------------------------------------------------------------------------------------------------------------------|");
	gotoxy(ci,25);	printf ("|          |                                                                    |                  |            |        |");
	gotoxy(ci,26);	printf ("|------------------------------------------------------------------------------------------------------------------------|");
	gotoxy(ci,27);	printf ("|          |                                                                    |                  |            |        |");
	gotoxy(ci,28);	printf ("|------------------------------------------------------------------------------------------------------------------------|");
	gotoxy(ci,29);	printf ("|          |                                                                    |                  |            |        |");
	gotoxy(ci,30);	printf ("+------------------------------------------------------------------------------------------------------------------------+");
	
}

void completa_tabela(int linha)  //função para colocar os dados na tabela
{ 
	if(!produto.excluido){
		
		int tam = strlen(produto.nome);
		
		if(tam > 66) tam = 66; // define o tamanho máximo a ser apresentado (66 espaços na tabela)
		
		gotoxy(22, linha); printf("%d", produto.id);
		gotoxy(33, linha); for(int i = 0; i < tam; i++) printf("%c", produto.nome[i]); 		
		gotoxy(102, linha); printf("%.2f", produto.preco_unitario);
		gotoxy(123, linha); printf("%d", produto.quantidade);
		gotoxy(136, linha); printf("%c", produto.tipo);	
	}
}

void gera_tabela_vertical(int li)
{
	if(!produto.excluido){
	
		int tam = strlen(produto.nome); 
	
		if(tam > 97){	
			
			int x = 0;
			
			gotoxy(20,li);		printf ("+------------------------------------------------------------------------------------------------------------------------+");
			gotoxy(20,li+1);	printf ("|  ID                |                                                                                                   |");
			gotoxy(20,li+2);	printf ("+------------------------------------------------------------------------------------------------------------------------+");
			gotoxy(20,li+3);	printf ("|  Nome              |                                                                                                   |");
			gotoxy(20,li+4);	printf ("|                    |                                                                                                   |");
			gotoxy(20,li+5);	printf ("+------------------------------------------------------------------------------------------------------------------------+");
			gotoxy(20,li+6);	printf ("|  Pre%co Unit%crio    |                                                                                                   |", 135, 160);
			gotoxy(20,li+7);	printf ("+------------------------------------------------------------------------------------------------------------------------+");
			gotoxy(20,li+8);	printf ("|  Quantidade        |                                                                                                   |");
			gotoxy(20,li+9);	printf ("+------------------------------------------------------------------------------------------------------------------------+");
			gotoxy(20,li+10);	printf ("|  Tipo              |                                                                                                   |");
			gotoxy(20,li+11);	printf ("+------------------------------------------------------------------------------------------------------------------------+");
		
			gotoxy(43, li+1); printf("%d", produto.id);
			gotoxy(43, li+3); for(int i = 0; i < tam; i++){
				if(i > 97 && x == 0){
					gotoxy(43, li+4);
					x = 1; 
				} 
				printf("%c", produto.nome[i]);
			} 		
			gotoxy(43, li+6); printf("%.2f", produto.preco_unitario);
			gotoxy(43, li+8); printf("%d", produto.quantidade);
			gotoxy(43, li+10); printf("%c", produto.tipo);
		}
		else{
		
			gotoxy(20,li);		printf ("+------------------------------------------------------------------------------------------------------------------------+");
			gotoxy(20,li+1);	printf ("|  ID                |                                                                                                   |");
			gotoxy(20,li+2);	printf ("+------------------------------------------------------------------------------------------------------------------------+");
			gotoxy(20,li+3);	printf ("|  Nome              |                                                                                                   |");
			gotoxy(20,li+4);	printf ("+------------------------------------------------------------------------------------------------------------------------+");
			gotoxy(20,li+5);	printf ("|  Pre%co Unit%crio    |                                                                                                   |", 135, 160);
			gotoxy(20,li+6);	printf ("+------------------------------------------------------------------------------------------------------------------------+");
			gotoxy(20,li+7);	printf ("|  Quantidade        |                                                                                                   |");
			gotoxy(20,li+8);	printf ("+------------------------------------------------------------------------------------------------------------------------+");
			gotoxy(20,li+9);	printf ("|  Tipo              |                                                                                                   |");
			gotoxy(20,li+10);	printf ("+------------------------------------------------------------------------------------------------------------------------+");
		
			gotoxy(43, li+1); printf("%d", produto.id);
			gotoxy(43, li+3); for(int i = 0; i < tam; i++) printf("%c", produto.nome[i]); 		
			gotoxy(43, li+5); printf("%.2f", produto.preco_unitario);
			gotoxy(43, li+7); printf("%d", produto.quantidade);
			gotoxy(43, li+9); printf("%c", produto.tipo);	
			
		}
	}	
	
}

void textcolor(int newcolor) // Define a cor do texto (vActual está aqui)
{
   CONSOLE_SCREEN_BUFFER_INFO csbi;

   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
      (csbi.wAttributes & 0xf0) | newcolor);
   //vActual.attribute = (csbi.wAttributes & 0xf0) | newcolor;
}

void textbackground(int newcolor) // Define a cor do fundo (vActual está aqui)
{
   CONSOLE_SCREEN_BUFFER_INFO csbi;

   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
      (csbi.wAttributes & 0x0f) | (newcolor << 4));
   //vActual.attribute = (csbi.wAttributes & 0x0f) | (newcolor << 4);
}

void gotoxy(int x, int y)	// Move o cursor para a coluna e linha desejada
{
  COORD c;
  c.X = x - 1;
  c.Y = y - 1;
  SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void cursor (int x) { 	// Define se o cursor ira aparecer sim(1) ou não(0)
	switch (x) {
		case 0: {
			CONSOLE_CURSOR_INFO cursor = {1, FALSE};
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
			break;
		}
		case 1: {
			CONSOLE_CURSOR_INFO cursor = {1, TRUE};
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
			break;
		}
	}
}

void clreol(int x)  //função customizada e mais versátil para o programa do clreol da conio.h
{
	for(int i=0; i < x; i++)
        printf(" ");         //preenche com "vazio/em branco" X espaços escolhidos
	for(int i=0; i < x; i++)
        printf("\b");		//volta X vezes o cursor para trás para a posição original após limpar a linha
}

void voltando_menu(int linha,int coluna,int delay, bool menu){  //Apresenta a mensagem de voltar ao menu, espera e vai pro menu
	cursor(0);
	textbackground(12);
	textcolor(cor_texto);
	gotoxy(linha, coluna);
	if(menu){
		printf("Voltando ao menu...");
	}else{
		printf("Voltando...");
	}		 	
	Sleep(delay);
} 
//12/12/2020 = 4600 linhas ->  02/05/2021 = 2003 linhas