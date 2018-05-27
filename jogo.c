#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>

#ifndef _WIN32
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

#define RAND ((rand()%100 + 1))

char tabuleiro[10][135];
int altura=10, largura=135;
int probX=45, probF=40;
int velocidade=50000;
int combustivel=400, pontos=0;

int usleep();
void mainmenu();
void jogo();

/*mostra uma tela de gameover*/
void gameover(int x){
    system(CLEAR);
    int c=0; /*recebe o inteiro correspondente a tecla apertada*/
    printf("\n");
    printf("  ___ ____ ___ _  ___   ___ _  _____ ____"); printf("\n");
    printf(" / _ `/ _ `/  ' \\/ -_) / _ \\ |/ / -_) __/"); printf("\n");
    printf(" \\_, /\\_,_/_/_/_/\\__/  \\___/___/\\__/_/   "); printf("\n");
    printf("/___/                                    "); printf("\n");
    printf("\n");
    if(x==0){
        printf("Seu combustível acabou!!\n");
        printf("Pressione ENTER para voltar à tela inicial\n");
    }
    else if(x==1){
        printf("Você bateu!!\n");
        printf("Pressione ENTER para voltar à tela inicial\n");
    }
    while(c!=10){
        c = getch();
        if(c==10){
            jogo();
        }
    }
}

/*mostra o atual tabuleiro na tela*/
void print_tabuleiro(){
    int linha, coluna;/*dimensoes da matriz*/
    for(linha=0; linha<altura; linha++){
        for(coluna=0; coluna<largura; coluna++){
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

/*faz com que os tiros ou 'f' ou 'x' que chegaram ao final do mapa, desapareçam*/
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
/*e é responsavel pelas colisoes*/
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

/*responsável por gerar 'F' ou 'X' no tabuleiro*/
void spawn() {
    int coluna=(largura-1); /*os mobs sempre aparecem na ultima coluna*/
    if(RAND<=probF)
        tabuleiro[(rand()%(altura-2))+1][coluna]='F';
    else if(RAND>probF && RAND<=(probF+probX))
        tabuleiro[(rand()%(altura-2))+1][coluna]='X';
}

/*responsável pelos tiros '>'*/
void atirar(int posicaonave){
    tabuleiro[posicaonave][1]='>';
}

/*responsável por mover o '+' verticalmente*/
void movernave(int c, int *posicaonave){
    if((*posicaonave)>1){
        if(c==119){
            tabuleiro[(*posicaonave)-1][0]='+';
            tabuleiro[(*posicaonave)][0]=' ';
            (*posicaonave)--;
        }
    }
    if((*posicaonave)<8){
        if (c==115){
            tabuleiro[(*posicaonave)+1][0]='+';
            tabuleiro[(*posicaonave)][0]=' ';
            (*posicaonave)++;
        }
    }
}

/*inicia a execução do jogo*/
void start(){
    int posicaonave=4;/*linha em que o '+' se encontra*/
    set_tabuleiro(posicaonave);
    int moveu, atirou; /*valem 1 caso a nave se mexa ou atire, respectivamente, e 0 caso contrario*/
    int c;/*recebe o inteiro correspondente a tecla pressionada*/
    while(combustivel>=0){
        moveu=0; atirou=0;
        usleep(velocidade);
        system(CLEAR);
        printf("Combustível: %d\tPontos: %d\n", combustivel, pontos);
        print_tabuleiro();
        if(kbhit()==1){
            c = getch();
            if(c==119 || c==115){
                movernave(c, &posicaonave);
                combustivel-=2;
                moveu=1;
            }
            else if(c==107){
                atirar(posicaonave);
                combustivel-=4;
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
    printf("'+' é o avatar do jogador;\n");
    printf("Utilize 'w' para mover-se para cima, e 's' para mover-se para baixo;\n");
    printf("'X' representa os inimigos, 'F' representa o combustível;\n");
    printf("Utilize 'k', para atirar\n");
    printf("Ficar parado gasta 1 em combustível, mover-se gasta 2, e atirar, 4;\n");
    printf("O jogo acaba quando o avatar atinge algum 'X' ou quando o combustível acaba;\n");
    printf("\n");
    printf("Pressione ENTER para retornar ao menu:\n");
    while(c!=10){
        c = getch();
        if(c==10)
            mainmenu();
    }
}

/*mostra o menu do jogo*/
void mainmenu() {
    int c=0; /*recebe o inteiro correspondente a tecla pressionada*/
    system(CLEAR);
    printf("(J)ogar\n(C)onfiguracoes\n(R)anking\n(I)nstrucoes\n(S)air\n");
    while(c!=106 && c!=105){
        c = getch();
        switch(c){
            case 106:
                start();
                break;
            case 105:
                instrucoes();
                break;
            default:
                break;
        }
    }
}

/*mostra uma tela de boas-vindas*/
void jogo () {
    combustivel=400; pontos=0;
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
    printf("Pressione ENTER para continuar:\n");
    while(c!=10){
        c = getch();
        if(c == 10)
            mainmenu();
    }
}

int main (){
    srand(time(0));
    jogo();

return 0;
}