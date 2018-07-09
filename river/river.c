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
#include <string.h>

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
typedef struct{
    char player[11];
    int score;
}TipoPlayer;

TipoPlayer topscores[10];
char nomeplayer[11]; /*auxilia a gravar o nome do player que esta atualmente jogando*/

typedef struct{
    int posicao;
    int municao;
}TipoInimigoT;

TipoInimigoT inimigosT[200];/*decalarando com o max de inimigos que um grid pode ter ao mesmo tempo*/
int numeroT=0; /*forma de atribuir um numero a cada T*/

typedef struct{
    int posicao;
    int vida;
}TipoInimigoO;

TipoInimigoO inimigosO[200]; /*declarando com o max de O que um grid pode ter ao mesmo tempo*/
int numeroO=0; /*forma de atribuir um numero a cada O*/

char tabuleiro[200][200]; /*declarado com o tamanho maximo que o grid pode ter*/
int altura, largura;
int probX, probF, probO, probT;
int velocidade;
int municaoT, vidaO;
int combustivel, pontos;
/*flags que auxiliam no fechamento correto do jogo*/
int sair;/*eh 1 caso o usuario opte por sair e 0 caso contrario*/
int fim;/*eh 1 caso ocorra gameover e 0 caso contrario*/
int ranked=0; /*controla se o modo rankeado esta ativado*/

int usleep();
void mainmenu();
void jogo();

void bsort() {
    int i, j;
    for(i=0; i<10; i++){
        for(j=0; j<10-i-1; j++){
            if(topscores[j].score<topscores[j+1].score){
                int aux = topscores[j].score;
                topscores[j].score = topscores[j+1].score;
                topscores[j+1].score = aux;
                char temp[11];
                strcpy(temp, topscores[j].player);
                strcpy(topscores[j].player, topscores[j+1].player);
                strcpy(topscores[j+1].player, temp);
            }
        }
    }
}

void registermatch(){
    FILE* fd;
    fd = fopen("ranking.bin", "rb");
    if(fd==NULL){
        fd = fopen("ranking.bin", "wb");
        int i;
        for(i=0; i<10; i++){
            if(i==0){
                topscores[i].score=0;
                topscores[i].player[0] = '\0';
            }
        }
        topscores[i].score=pontos;
        strcpy(topscores[i].player, nomeplayer);
        fwrite(&topscores, sizeof(topscores), 1, fd);
        fclose(fd);
    }
    else{
        fclose(fd);
        if(pontos>topscores[9].score){
            topscores[9].score=pontos;
            strcpy(topscores[9].player, nomeplayer);
            bsort();
            fd = fopen("ranking.bin", "wb");
            fwrite(&topscores, sizeof(topscores), 1, fd);
            fclose(fd);
        }
    }
}

/*mostra uma tela de gameover*/
void gameover(int x){/*x eh o motivo do fim de jogo*//*1 eh batida*//*0 eh falta de combustivel*/
    if(!fim){
        if(ranked)
            registermatch();
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
            if(linha==altura-1)
                tabuleiro[linha][coluna]='#';
            if(linha!=0 && linha!=altura-1)
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
            if(tabuleiro[linha][coluna]=='X' || tabuleiro[linha][coluna]=='F' || tabuleiro[linha][coluna] == 'O' || tabuleiro[linha][coluna]=='T')/*remove os 'F' e os 'X' do fim do mapa*/
                tabuleiro[linha][coluna]=' ';
        }
    }
}

/*chamado quando um O eh destruido*/
void destruirO(){
    int linha, coluna;
    for(linha=1; linha<altura-1; linha++){
        for(coluna=0; coluna<largura; coluna++){
            if(tabuleiro[linha][coluna]=='X'){
                tabuleiro[linha][coluna] = ' ';
                pontos+=50;
            }
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
            /*colisao entre '>' e 'O'*/
            else if(tabuleiro[linha][coluna]=='>' && tabuleiro[linha][coluna+1]=='O'){
                tabuleiro[linha][coluna]=' ';
                int j;
                for(j=0; j<=numeroO; j++){
                    if(inimigosO[j].posicao==coluna+1 && inimigosO[j].vida>0)
                        inimigosO[j].vida--;
                    else if(inimigosO[j].vida==0 && inimigosO[j].posicao==coluna+1){
                        destruirO();
                        tabuleiro[linha][coluna]=' ';
                        tabuleiro[linha][coluna+1]=' ';
                    }
                }
            }
            else if(tabuleiro[linha][coluna]=='>' && tabuleiro[linha][coluna+2]=='O'){
                tabuleiro[linha][coluna]=' ';
                int j;
                for(j=0; j<=numeroO; j++){
                    if(inimigosO[j].posicao==coluna+2 && inimigosO[j].vida>0)
                        inimigosO[j].vida--;
                    else if(inimigosO[j].vida==0 && inimigosO[j].posicao==coluna+2){
                        destruirO();
                        tabuleiro[linha][coluna]=' ';
                        tabuleiro[linha][coluna+2]=' ';
                    }
                }
            }
            /*colisao entre '>' e 'T'*/
            else if(tabuleiro[linha][coluna]=='>' && tabuleiro[linha][coluna+1]=='T'){
                tabuleiro[linha][coluna]=' ';
                tabuleiro[linha][coluna+1]=' ';
            }
            else if(tabuleiro[linha][coluna]=='>' && tabuleiro[linha][coluna+2]=='T'){
                tabuleiro[linha][coluna]=' ';
                tabuleiro[linha][coluna+2]=' ';
            }
            /*colisao entre '>' e '<'*/
            else if(tabuleiro[linha][coluna]=='>' && tabuleiro[linha][coluna+1]=='<'){
                tabuleiro[linha][coluna]=' ';
                tabuleiro[linha][coluna+1]=' ';
            }
            else if(tabuleiro[linha][coluna]=='>' && tabuleiro[linha][coluna+2]=='<'){
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
            /*colisao entre + e O*/
            else if (tabuleiro[linha][coluna]=='O' && tabuleiro[linha][coluna-1]=='+'){
                tabuleiro[linha][coluna]=' ';
            }
             /*colisao entre '+' e 'T'*/
            if(tabuleiro[linha][coluna]=='T' && tabuleiro[linha][coluna-1]=='+')
                gameover(1);
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
            /*movimento normal dos O*/
            else if(tabuleiro[linha][coluna]=='O'){
                tabuleiro[linha][coluna]=' ';
                tabuleiro[linha][coluna-1]='O';
                int j;
                for(j=0; j<=numeroO; j++){
                    if(inimigosO[j].posicao==coluna){
                        inimigosO[j].posicao--;
                    }
                }
            }
            /*movimento normal dos T*/
            else if(tabuleiro[linha][coluna]=='T'){
                tabuleiro[linha][coluna]=' ';
                tabuleiro[linha][coluna-1]='T';
            }
            /*moviemtno normal dos <*/
            else if(tabuleiro[linha][coluna]=='<'){
                tabuleiro[linha][coluna]=' ';
                tabuleiro[linha][coluna-2]='<';
            }
        }
    }
}

/*responsavel por gerar os mobs no tabuleiro*/
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
    else if(x>(probF+probX) && x<= (probF+probX+probO)){
        if(tabuleiro[r][coluna-1]==' '){
            tabuleiro[r][coluna]='O';
            inimigosO[numeroO].posicao=coluna;/*atribuindo posicao e vida ao O*/
            inimigosO[numeroO].vida=vidaO;
            numeroO++;
        }
        if(numeroO==200){
            numeroO=0;
        }
    }
    else if(x>(probF+probX+probO) && x<=(probF+probX+probO+probT)){
        if(tabuleiro[r][coluna-1]==' '){
            tabuleiro[r][coluna]='T';
            inimigosT[numeroT].posicao=coluna;/*atribuindo vida e posicao ao T*/
            inimigosT[numeroT].posicao=municaoT;
            numeroT++;
        }
        if(numeroT==200){
            numeroT=0;
        }
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
    /*else if(tabulerio[posicaonave][1]=='O'){}*/
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
            else if(tabuleiro[(*posicaonave)-1][0]=='O'){
                tabuleiro[(*posicaonave)-1][0]='+';
                tabuleiro[(*posicaonave)][0]=' ';
            }
            else{/*movimento normal do '+'*/
                tabuleiro[(*posicaonave)-1][0]='+';
                tabuleiro[(*posicaonave)][0]=' ';
            }
            (*posicaonave)--;
        }
    }
    if((*posicaonave)<altura-2){
        if (c==115 || c==83){/*s pressionado*/ /*movimento para baixo*/
            if(tabuleiro[(*posicaonave)+1][0]=='F'){/*encontro vertical entre '+' e 'F'*/
                tabuleiro[(*posicaonave)+1][0]='+';
                tabuleiro[(*posicaonave)][0]=' ';
                combustivel+=40;
            }
            else if(tabuleiro[(*posicaonave)+1][0]=='X')/*encontro vertical entre '+' e X'*/
                gameover(1);
            else if(tabuleiro[(*posicaonave)+1][0]=='O'){
                tabuleiro[(*posicaonave)+1][0]='+';
                tabuleiro[(*posicaonave)][0]=' ';
            }
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
    int posicaonave=(altura/2);/*linha em que o '+' se encontra*/
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
        spawn();
        movertabuleiro();
        fimdemapa();
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
    printf("Os T sao inimigos que atiram uma quantidade determinada de projeteis, devem ser evitados, e nao dao pontos ao jogador;\n");
    printf("Os O tem uma quantidade predeterminada de vida, e caso sejam destruidos, destroem tambem todos os X;\n");
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
void configuracoes(){
    int temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8, temp9;
    int c = 0;
    FILE* fd;
    fd = fopen("config.txt", "r");
    if(fd == NULL){
        system(CLEAR);
        printf("Erro, arquivo de configuracao ausente\n");
        fd = fopen("config.txt", "w");
        fprintf(fd, "10 135 25 10 1 4 10 5 60000");
        fclose(fd);
        printf("Um novo arquivo foi criado com as configs padrao de rankeada\n");
        fd = fopen("config.txt", "r");
        printf("Pressione ESPACO para continuar: ");
        c = getch();
        while(c!=32){
            c = getch();
        }
    }
    fscanf(fd, "%d %d %d %d %d %d %d %d %d", &temp1, &temp2, &temp3, &temp4, &temp5, &temp6, &temp7, &temp8, &temp9);
    fclose(fd);
    system(CLEAR);
    printf("[1] - Tabuleiro\n[2] - NPCs\n[3] - Modo Rankeado\n[4] - Voltar\n");
    printf("\n");
    printf("Escolha uma opcao:\n");
    while(c!=49 && c!=50 && c!=51 && c!=52){
        c = getch();
        switch(c){
            case 49:/*configs do tabuleiro*/
                system(CLEAR);
                printf("Escolha a altura (ENTRE 5 E 20): ");
                scanf("%d", &temp1);
                while(temp1<5 || temp1>20){
                    system(CLEAR);
                    printf("Altura invalida, digite outra entre 5 e 20: ");
                    scanf("%d", &temp1);
                }
                system(CLEAR);
                printf("Escolha a largura (ENTRE 100 E 200): ");
                scanf("%d", &temp2);
                while(temp2<100 || temp2>200){
                    system(CLEAR);
                    printf("Largura invalida, digite outra entre 100 e 200: ");
                    scanf("%d", &temp2);
                }
                system(CLEAR);
                printf("Escolha a velocidade (ENTRE 10000 E 100000): ");
                scanf("%d", &temp9);
                while(temp9<10000 || temp9>100000){
                    system(CLEAR);
                    printf("Velocidade invalida, digite outra entre 10000 e 100000: ");
                    scanf("%d", &temp9);
                }
                fd = fopen("config.txt", "w");
                fprintf(fd, "%d %d %d %d %d %d %d %d %d", temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8, temp9);
                fclose(fd);
                configuracoes();
                break;
            case 50: /*configs dos npcs*/
                system(CLEAR);
                printf("Escolha a propabilidade dos X (ENTRE 0 E 100): ");
                scanf("%d", &temp3);
                while(temp3<0 || temp3>100){
                    system(CLEAR);
                    printf("Valor invalido, digite outro entre 0 e 100: ");
                    scanf("%d", &temp3);
                }
                system(CLEAR);
                printf("Escolha a propabilidade dos F (ENTRE 0 E 100): ");
                scanf("%d", &temp4);
                while(temp4<0 || temp4>100){
                    system(CLEAR);
                    printf("Valor invalido, digite outro entre 0 e 100: ");
                    scanf("%d", &temp4);
                }
                system(CLEAR);
                printf("Escolha a propabilidade dos O (ENTRE 0 E 100): ");
                scanf("%d", &temp5);
                while(temp5<0 || temp5>100){
                    system(CLEAR);
                    printf("Valor invalido, digite outro entre 0 e 100: ");
                    scanf("%d", &temp5);
                }
                system(CLEAR);
                printf("Escolha a propabilidade dos T (ENTRE 0 E 100): ");
                scanf("%d", &temp6);
                while(temp6<0 || temp6>100){
                    system(CLEAR);
                    printf("Valor invalido, digite outro entre 0 e 100: ");
                    scanf("%d", &temp6);
                }
                system(CLEAR);
                printf("Escolha a vida do O: (ENTRE 1 e 20): ");
                scanf("%d", &temp7);
                while(temp7<1 || temp7>20){
                    system(CLEAR);
                    printf("Valor invalido, digite outro entre 1 e 20: ");
                    scanf("%d", &temp7);
                }
                system(CLEAR);
                printf("Escolha a a municao do T: (ENTRE 1 e 10): ");
                scanf("%d", &temp8);
                while(temp8<1 || temp8>10){
                    system(CLEAR);
                    printf("Valor invalido, digite outro entre 1 e 10: ");
                    scanf("%d", &temp8);
                }
                fd = fopen("config.txt", "w");
                fprintf(fd, "%d %d %d %d %d %d %d %d %d", temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8, temp9);
                fclose(fd);
                configuracoes();
                break;
            case 51: /*ranked*/
                system(CLEAR);
                if(ranked==1){
                    printf("Modo Rankeado desativado\n\n");
                    ranked=0;
                }
                else if(ranked==0){
                    printf("Modo Rankeado ativado\n\n");
                    ranked=1;
                }
                printf("Pressione ESPACO para voltar a tela de configuracoes: ");
                int aux = getch();
                while(aux!=32){
                    aux = getch();
                }
                configuracoes();
                break;
            case 52: /*voltar*/
                mainmenu();
                break;
            default:
                system(CLEAR);
                printf("[1] - Tabuleiro\n[2] - NPCs\n[3] - Modo Rankeado\n[4] - Voltar\n");
                printf("\n");
                printf("Escolha uma opcao:\n");
                break;
        }
    }
}

void printranking() {
    int i;
    FILE* fd;
    fd = fopen("ranking.bin", "rb");
    if(fd == NULL){
        system(CLEAR);
        printf("Sem entradas no ranking\n");
    }
    else{
        system(CLEAR);
        fd = fopen("ranking.bin", "rb");
        fread(&topscores, sizeof(topscores), 1, fd);
        fclose(fd);
        for(i=0; i<10; i++){
            if(topscores[i].score>0)
                printf("%d - %s %d\n", i+1, topscores[i].player, topscores[i].score);
        }
    }
    printf("\n");
    printf("Pressione ESPACO para voltar ao menu: \n");
    int c = getch();
    while(c!=32){
        c = getch();
    }
    mainmenu();
}

/*mostra o menu do jogo*/
void mainmenu() {
    int c=0; /*recebe o numero escolhido pelo usuario*/
    system(CLEAR);
    printf("[1] - Jogar\n[2] - Configuracoes\n[3] - Ranking\n[4] - Instrucoes\n[5] - Sair\n");
    printf("\n");
    printf("Escolha uma opcao:\n");
    while(c!=49 && c!=52 && c!=53 && !sair){
        c = getch();
        switch(c){
            case 49:
            combustivel=400; pontos=0; fim=0; sair=0;
                if(ranked==1){
                    altura=10; largura=135; probX=25; probF=10; probO=1; probT=4; vidaO=10; municaoT=5; velocidade = 60000;
                    system(CLEAR);
                    printf("Digite seu nome (ENTRE 1 E 10 CARACTERES):\n");
                    scanf("%s", nomeplayer);
                    while(strlen(nomeplayer)<1 || strlen(nomeplayer)>10){
                        system(CLEAR);
                        printf("Nome invalido, digite um que possua entre 1 e 10 caracteres:\n");
                        scanf("%s", nomeplayer);
                    }
                    start();
                }
                else if(ranked==0){
                    FILE* fd = fopen("config.txt", "r");
                    if(fd == NULL){
                        system(CLEAR);
                        printf("Erro, arquivo de configuracao ausente\n");
                        fd = fopen("config.txt", "w");
                        fprintf(fd, "10 135 25 10 1 4 10 5 60000");
                        fclose(fd);
                        printf("Um novo arquivo foi criado com as configs padrao de rankeada\n");
                        fd = fopen("config.txt", "r");
                        printf("Pressione ESPACO para continuar: ");
                        c = getch();
                        while(c!=32){
                            c = getch();
                        }
                    }
                    fscanf(fd, "%d %d %d %d %d %d %d %d %d", &altura, &largura, &probX, &probF, &probO, &probT, &vidaO, &municaoT, &velocidade);
                    fclose(fd);
                    start();
                }
                break;
            case 50:
                configuracoes();
                break;
            case 51:
                printranking();
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
                printf("[1] - Jogar\n[2] - Configuracoes\n[3] - Ranking\n[4] - Instrucoes\n[5] - Sair\n");
                printf("\n");
                printf("Escolha uma opcao:\n");
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
