/*
Versao do compilador: 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.9)

Descricao: O trabalho eh uma releitura do jogo River Raid, para Atari,
feito em linguagem C, e executado diretamente no terminal.
JOAO PEDRO ASSUNCAO COUTINHO
github.com/joaocout
*/

#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifndef _WIN32
    #include <termios.h>
    #include <fcntl.h>
    int kbhit(){
        struct termios oldt, newt;
        int ch, oldf;
        tcgetattr(STDIN_FILENO,&oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        ch = getchar();    
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
    if( ch != EOF){
        ungetc(ch,stdin);
        return 1;
    }
    return 0;
    }
    int getch(void) {
        int ch;
        struct termios oldt;
        struct termios newt;
        tcgetattr(STDIN_FILENO,&oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#else
    #include <conio.h>
#endif

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define RAND() (rand()%100 + 1)

char tabuleiro[10][135];
int altura=10, largura=135;
int probX=21, probF=7;
int velocidade=50000;
int combustivel, pontos;
/*flags que auxiliam no fechamento correto do jogo*/
int sair;/*eh 1 caso o usuario opte por sair e 0 caso contrario*/
int fim;/*eh 1 caso ocorra gameover e 0 caso contrario*/

int usleep();
void mainmenu();
void jogo();

/*mostra uma tela de gameover*/
void gameover(int x){/*x eh o motivo do fim de jogo*//*1 eh batida*//*0 eh falta de combustivel*/
    if(!fim){
        system(CLEAR);
        int c=0; /*recebe o inteiro correspondente a tecla apertada*/
        printf("\n");
        printf("  ___ ____ ___ _  ___   ___ _  _____ ____"); printf("\n");
        printf(" / _ `/ _ `/  ' \\/ -_) / _ \\ |/ / -_) __/"); printf("\n");
        printf(" \\_, /\\_,_/_/_/_/\\__/  \\___/___/\\__/_/   "); printf("\n");
        printf("/___/                                    "); printf("\n");
        printf("\n");
        if(x==0)
            printf("Seu combustivel acabou!\n");
        else if(x==1)
            printf("Voce bateu!\n");
        printf("Voce fez %d pontos.\n", pontos);
        printf("Pressione ESPACO para voltar a tela inicial.\n");
        while(c!=32){
            c = getch();
                if(c==32){
                    fim=1;
                    jogo();
                }
        }
    }
    system(CLEAR);
}

/*mostra o atual tabuleiro na tela*/
void print_tabuleiro(){
    int linha, coluna;/*dimensoes da matriz*/
    for(linha=0; linha<altura; linha++){
        for(coluna=0; coluna<largura; coluna++){
            if(tabuleiro[linha][coluna]=='X')
                printf("\e[1;31m" "%c" "\e[1;0m", tabuleiro[linha][coluna]);/*printando x com cor vermelha*/
            else if(tabuleiro[linha][coluna]=='F')
                printf("\e[1;34m" "%c" "\e[1;0m", tabuleiro[linha][coluna]);/*printando f com cor azul*/
            else
                printf("%c", tabuleiro[linha][coluna]);
        }
        printf("\n");
    }
}

/*configura o tabuleiro para sua forma inicial*/
void set_tabuleiro(int posicaonave){
    int linha, coluna; /*dimensoes da matriz*/
    for(linha=0; linha<altura; linha++){
        for(coluna=0; coluna<largura; coluna++){
            if(linha==0)
                tabuleiro[linha][coluna]='#';
            if(linha==9)
                tabuleiro[linha][coluna]='#';
            if(linha!=0 && linha!=9)
                tabuleiro[linha][coluna]=' ';
        }
    }
    tabuleiro[posicaonave][0]='+';
}

/*faz com que os tiros ou 'f' ou 'x' que chegaram ao final do mapa, desaparecam*/
void fimdemapa(){
    int linha, coluna; /*dimensoes do tabuleiro*/
    for(linha=1; linha<(altura-1); linha++){
        for(coluna=0; coluna<largura; coluna=coluna+(largura-1)){
            if(tabuleiro[linha][coluna]=='>')/*remove os '>' do fim do mapa*/
                tabuleiro[linha][coluna]=' ';
            if(tabuleiro[linha][coluna]=='X' || tabuleiro[linha][coluna]=='F')/*remove os 'F' e os 'X' do fim do mapa*/
                tabuleiro[linha][coluna]=' ';
        }
    }
}

/*move os elementos do tabuleiro*/
/*e eh responsavel pelas colisoes horizontais*/
void movertabuleiro(){
    int linha, coluna; /*dimensoes da matriz*/
    for(linha=1; linha<(altura-1); linha++){ /*exclui-se a primeira e a ultima linhas pois elas delimitam o mapa*/
        for(coluna=(largura-2); coluna>0; coluna--){
            /*colisao entre '>' e 'X'*/
            if(tabuleiro[linha][coluna]=='>' && tabuleiro[linha][coluna+1]=='X'){
                tabuleiro[linha][coluna]=' ';
                tabuleiro[linha][coluna+1]=' ';
                pontos+=50;
            }
            else if(tabuleiro[linha][coluna]=='>' && tabuleiro[linha][coluna+2]=='X'){
                tabuleiro[linha][coluna]=' ';
                tabuleiro[linha][coluna+2]=' ';
                pontos+=50;
            }
            /*colisao entre '>' e 'F'*/
            else if(tabuleiro[linha][coluna]=='>' && tabuleiro[linha][coluna+1]=='F'){
                tabuleiro[linha][coluna]=' ';
                tabuleiro[linha][coluna+1]=' ';
            }
            else if(tabuleiro[linha][coluna]=='>' && tabuleiro[linha][coluna+2]=='F'){
                tabuleiro[linha][coluna]=' ';
                tabuleiro[linha][coluna+2]=' ';
            }
            /*movimento normal dos '>'*/
            else if(tabuleiro[linha][coluna]=='>'){
                tabuleiro[linha][coluna]=' ';
                tabuleiro[linha][coluna+1]='>';
            }
        }
    }
    for(linha=1; linha<(altura-1); linha++){
        for(coluna=1; coluna<largura; coluna++){
            /*colisao entre '+' e 'X'*/
            if(tabuleiro[linha][coluna]=='X' && tabuleiro[linha][coluna-1]=='+')
                gameover(1);
            /*colisao entre '+' e 'F'*/
            else if(tabuleiro[linha][coluna]=='F' && tabuleiro[linha][coluna-1]=='+'){
                tabuleiro[linha][coluna]=' ';
                combustivel+=40;
            }
            /*movimento normal dos 'X'*/
            else if(tabuleiro[linha][coluna]=='X'){
                tabuleiro[linha][coluna]=' ';
                tabuleiro[linha][coluna-1]='X';
            }
            /*movimento normal dos 'F'*/
            else if(tabuleiro[linha][coluna]=='F'){
                tabuleiro[linha][coluna]=' ';
                tabuleiro[linha][coluna-1]='F';
            }
        }
    }
}

/*responsavel por gerar 'F' ou 'X' no tabuleiro*/
void spawn() {
    int coluna=(largura-1); /*os mobs sempre aparecem na ultima coluna*/
    int r = (rand()%(altura-2))+1; /*decisao pseudorandomica da linha que o x ou f aparecera*/
    int x = RAND(); /*decisao pseudorancomica se x ou f aparecera*/
    if(x<=probF){
        if(tabuleiro[r][coluna-1]==' ') /*para que nao acontecam x e f juntos (xf ou fx)*/
            tabuleiro[r][coluna]='F';
    }
    else if(x>probF && x<=(probF+probX)){
        if(tabuleiro[r][coluna-1]==' ')/*para que nao acontecam x e f juntos (xf ou fx)*/
            tabuleiro[r][coluna]='X';
    }
}

/*responsavel pelos tiros '>'*/
void atirar(int posicaonave){
    if(tabuleiro[posicaonave][1]=='F')/*caso exista um f na frente da nave*/
        tabuleiro[posicaonave][1]=' ';
    else if(tabuleiro[posicaonave][1]=='X'){/*caso exista um x na frente da nave*/
        tabuleiro[posicaonave][1]=' ';
        pontos+=50;
    }
    else
        tabuleiro[posicaonave][1]='>';
}

/*responsavel por mover o '+' verticalmente*/
/* e responsavel pelas colisoes verticais entre o '+' o os mobs 'F' e 'X'*/
void movernave(int c, int *posicaonave){
    if((*posicaonave)>1){
        if(c==119 || c==87){/*w pressionado*/ /*movimento para cima*/
            if(tabuleiro[(*posicaonave)-1][0]=='F'){/*encontro vertical entre '+' e 'F'*/
                tabuleiro[(*posicaonave)-1][0]='+';
                tabuleiro[(*posicaonave)][0]=' ';
                combustivel+=40;
            }
            else if(tabuleiro[(*posicaonave)-1][0]=='X')/*encontro vertical entre '+' e 'X'*/
                gameover(1);
            else{/*movimento normal do '+'*/
                tabuleiro[(*posicaonave)-1][0]='+';
                tabuleiro[(*posicaonave)][0]=' ';
            }
            (*posicaonave)--;
        }
    }
    if((*posicaonave)<8){
        if (c==115 || c==83){/*s pressionado*/ /*movimento para baixo*/
            if(tabuleiro[(*posicaonave)+1][0]=='F'){/*encontro vertical entre '+' e 'F'*/
                tabuleiro[(*posicaonave)+1][0]='+';
                tabuleiro[(*posicaonave)][0]=' ';
                combustivel+=40;
            }
            else if(tabuleiro[(*posicaonave)+1][0]=='X')/*encontro vertical entre '+' e X'*/
                gameover(1);
            else{/*movimento normal do '+'*/
                tabuleiro[(*posicaonave)+1][0]='+';
                tabuleiro[(*posicaonave)][0]=' ';
            }
            (*posicaonave)++;
        }
    }
}

/*inicia a execucao do jogo*/
void start(){
    int posicaonave=4;/*linha em que o '+' se encontra*/
    int moveu, atirou; /*valem 1 caso a nave se mexa ou atire, respectivamente, e 0 caso contrario*/
    int c;/*recebe o inteiro correspondente a tecla pressionada*/
    set_tabuleiro(posicaonave);
    while(combustivel>=0 && !sair){
        moveu=0; atirou=0;
        usleep(velocidade);
        system(CLEAR);
        printf("Combustivel: %d \tPontos: %d\n", combustivel, pontos);
        print_tabuleiro();
        if(kbhit()==1){
            c = getch();
            /*punicao com movimento aleatorio caso outra tecla seja presisonada*/
            if(c!=119 && c!=115 && c!=87 && c!=83 && c!=107 && c!=75){
                int x = RAND();
                if(x<=50)
                    c=119;
                else if(x>50)
                    c=115;
            }

            if((c==119 || c==87) && posicaonave>1){/*w pressionado*/
                movernave(c, &posicaonave);
                combustivel-=2;
                moveu=1;
            }
            else if((c==115 || c==83) && posicaonave<8){/*s pressionado*/
                movernave(c, &posicaonave);
                combustivel-=2;
                moveu=1;
            }
            else if(c==107 || c==75){/*k pressionado*//*tiro*/
                atirar(posicaonave);
                combustivel-=3;
                atirou=1;
            }
        }
        if(!moveu && !atirou)
            combustivel--;
        pontos++;
        movertabuleiro();
        fimdemapa();
        spawn();
        if(combustivel<=0)
            gameover(0);
    }
}

/*mostra as instrucoes*/
void instrucoes() {
    system(CLEAR);
    int c;/*recebe o inteiro correspondente a tecla pressionada*/
    printf("'+' eh o avatar do jogador;\n");
    printf("Utilize 'w' para mover-se para cima, e 's' para mover-se para baixo;\n");
    printf("Utilize 'k', para atirar;\n");
    printf("'X' representa os inimigos, 'F' representa o combustivel;\n");
    printf("Atirar nos 'X' soma 50 ao total de pontos;\n");
    printf("Recolher os 'F' soma 40 ao total de combustivel;\n");
    printf("Ficar parado gasta 1 em combustivel, mover-se gasta 2, e atirar, 3;\n");
    printf("Por padrao o jogador recebe 1 enquanto estiver vivo;\n");
    printf("O jogo acaba quando o avatar atinge algum 'X' ou quando o combustivel acaba;\n");
    printf("Caso o jogador aperte alguma tecla invalida, o avatar realizara um movimento aleatorio;\n");
    printf("A chance de vir um 'X' eh o triplo da chance de vir um 'F';\n");
    printf("\n");
    printf("Pressione ESPACO para retornar ao menu.\n");
    while(c!=32){
        c = getch();
        if(c==32)/*ESPACO pressionado*/
            mainmenu();
    }
}

/*mostra o menu do jogo*/
void mainmenu() {
    int c=0; /*recebe o numero escolhido pelo usuario*/
    system(CLEAR);
    printf("[1] - Jogar\n[4] - Instrucoes\n[5] - Sair\n");
    printf("\nEscolha uma opcao: ");
    while(c!=49 && c!=52 && c!=53){
        c = getch();
        switch(c){
            case 49:
                combustivel=400; pontos=0; fim=0; sair=0;
                start();
                break;
            case 52:
            	instrucoes();
                break;            
            case 53:
            	system(CLEAR);
                sair=1;/*impede que o jogo rode caso seja dada a instrucao de saida*/
                break;
            default:
            	system(CLEAR);
    			printf("[1] - Jogar\n[4] - Instrucoes\n[5] - Sair\n");
    			printf("\nEscolha uma opcao: ");
                break;
        }
    }
}

/*mostra uma tela de boas-vindas*/
void jogo () {
    int c=0; /*recebe o inteiro correspondente a tecla pressionada*/
    system(CLEAR);
    printf("\n");
    printf("       __                                            __    /\\ \\    "); printf("\n");
    printf(" _ __ /\\_\\  __  __     __   _ __       _ __    __   /\\_\\   \\_\\ \\   "); printf("\n");
    printf("/\\`'__\\/\\ \\/\\ \\/\\ \\  /'__`\\/\\`'__\\    /\\`'__\\/'__`\\ \\/\\ \\  /'_` \\  "); printf("\n");
    printf("\\ \\ \\/ \\ \\ \\ \\ \\_/ |/\\  __/\\ \\ \\/     \\ \\ \\//\\ \\L\\.\\_\\ \\ \\/\\ \\L\\ \\ "); printf("\n");
    printf(" \\ \\_\\  \\ \\_\\ \\___/ \\ \\____\\\\ \\_\\      \\ \\_\\\\ \\__/.\\_\\\\ \\_\\ \\___,_\\"); printf("\n");
    printf("  \\/_/   \\/_/\\/__/   \\/____/ \\/_/       \\/_/ \\/__/\\/_/ \\/_/\\/__,_ /"); printf("\n");
    printf("\n");
    printf("Pressione ESPACO para continuar.\n");
    while(c!=32){
        c = getch();
        if(c == 32)/*ESPACO pressionado*/
            mainmenu();
    }
}

int main (){
    srand(time(0));
    jogo();

return 0;
}
