#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define FALSE 0
#define TRUE 1
#define MAXTIROS 500
#define MAXINIMIGOS 6

int direita1;
int esquerda1;
int cima1;
int baixo1;
int randomMove;

Image ImgInimigo;
Texture2D texturaInimigo;

/*STRUCTS*/
typedef struct player{
    Vector2 posicao;
    Vector2 origin;
    Rectangle source;
    Rectangle retJogador;
    Rectangle retCollision;
    float rotacao;
    int vidas;
    int pontos;
} Player; //Jogador

typedef struct enemy{
    Vector2 posicao;
    Vector2 origin;
    Rectangle source;
    Rectangle retInimigo;
    Rectangle retCollision;
    float rotacao;
    Vector2 posicao_anterior;
} Enemy; //Inimigo

typedef struct energy{
    Vector2 posicao;
    Rectangle retEnergia;
    int ativacao;
} Energy;

/*Tiros do jogador*/
/*
typedef struct shootRight{
    Vector2 velocidade;
    Vector2 posicao;
    Color corTiro;
    int ativacao;
    int lifeSpawn;
    float raio;
    Rectangle retTiroDir;
} ShootRight; //Tiro Direita

typedef struct ShootLeft{
    Vector2 velocidade;
    Vector2 posicao;
    Color corTiro;
    int ativacao;
    int lifeSpawn;
    float raio;
    Rectangle retTiroEsq;
} ShootLeft; //Tiro Esquerda

typedef struct ShootUp{
    Vector2 velocidade;
    Vector2 posicao;
    Color corTiro;
    int ativacao;
    int lifeSpawn;
    float raio;
    Rectangle retTiroCima;
} ShootUp; //Tiro cima

typedef struct ShootDown{
    Vector2 velocidade;
    Vector2 posicao;
    Color corTiro;
    int ativacao;
    int lifeSpawn;
    float raio;
    Rectangle retTiroBaixo;
} ShootDown; //Tiro baixo

/*Tiros do inimigo*/
/*
typedef struct ShootRightI{
    Vector2 velocidade;
    Vector2 posicao;
    Color corTiro;
    int ativacao;
    int lifeSpawn;
    float raio;
    Rectangle retTiroDirI;
} ShootRightI; //Tiro direita

typedef struct ShootLeftI{
    Vector2 velocidade;
    Vector2 posicao;
    Color corTiro;
    int ativacao;
    int lifeSpawn;
    float raio;
    Rectangle retTiroEsqI;
} ShootLeftI; //Tiro esquerda

typedef struct ShootUpI{
    Vector2 velocidade;
    Vector2 posicao;
    Color corTiro;
    int ativacao;
    int lifeSpawn;
    float raio;
    Rectangle retTiroCimaI;
} ShootUpI; //Tiro cima

typedef struct ShootDownI{
    Vector2 velocidade;
    Vector2 posicao;
    Color corTiro;
    int ativacao;
    int lifeSpawn;
    float raio;
    Rectangle retTiroBaixoI;
} ShootDownI; //Tiro baixo
*/

typedef struct rectangles{
    //Fase 1
    Rectangle tamanho[7];
    //Bordas do jogo
    Rectangle tamanhoB[4];
} Rectangles; //Retangulos

/*FUNÇÕES*/
void menu (int screenWidth){
    DrawRectangle (0,0, screenWidth, 40, DARKGRAY); //Listra cinza escuro
    DrawText("Fase 1", 442, 7, 30, GOLD);
} //Menu da fase 1 - DESENHO

void blocosFase1 (Texture2D texturaTijolos){
    for (int i=65; i<=275; i += 30)
        DrawTexture(texturaTijolos, i, 230, LIGHTGRAY);
    for (float i=285.5; i<=432; i += 55.2)
        DrawTexture(texturaTijolos, 275, i, LIGHTGRAY);
    for (int i=275; i>=65; i-=30)
        DrawTexture(texturaTijolos, i, 432, LIGHTGRAY);
    for (int i=650; i<=750; i+=30)
        DrawTexture(texturaTijolos, i, 40, LIGHTGRAY);
    for (int i=590; i<=750; i+=30)
        DrawTexture(texturaTijolos, i, 95, LIGHTGRAY);
    for (int i=550; i<=680; i+=30)
        DrawTexture(texturaTijolos, i, 150, LIGHTGRAY);
    DrawTexture(texturaTijolos, 750, 290, LIGHTGRAY);
} //Blocos da fase 1 - DESENHO

/*COLISÕES*/
int collisionPlayerTij (Rectangles tijolos, Player jogador){
    int retorno;

    if (CheckCollisionRecs(tijolos.tamanho[0], jogador.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[1], jogador.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[2], jogador.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[3], jogador.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[4], jogador.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[5], jogador.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[6], jogador.retCollision))
        retorno = TRUE;
    else retorno = FALSE;

    return retorno;
} // Colisão do player com os blocos

int collisionPlayerBordas (Rectangles bordas, Player jogador){
    int retorno;

    if (CheckCollisionRecs(bordas.tamanhoB[0], jogador.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[1], jogador.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[2], jogador.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[3], jogador.retCollision))
        retorno = TRUE;
    else retorno = FALSE;

    return retorno;
} //Colisão do player com as bordas

int collisionInimigo (Rectangles tijolos, Rectangles bordas, Enemy inimigo, Player jogador){
    int retorno = 0;

    if (CheckCollisionRecs(tijolos.tamanho[0], inimigo.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[1], inimigo.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[2], inimigo.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[3], inimigo.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[4], inimigo.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[5], inimigo.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[6], inimigo.retCollision)
    || CheckCollisionRecs(inimigo.retCollision, jogador.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[0], inimigo.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[1], inimigo.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[2], inimigo.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[3], inimigo.retCollision))
        retorno = 1;

    return retorno;
} //Colisão do inimigo

int collisionTiroDir (Rectangles tijolos, Rectangles bordas, ShootRight *tiroDireita){
    int retorno = 0;

    if (CheckCollisionRecs(tijolos.tamanho[0], tiroDireita->retTiroDir)
    || CheckCollisionRecs(tijolos.tamanho[1], tiroDireita->retTiroDir)
    || CheckCollisionRecs(tijolos.tamanho[2], tiroDireita->retTiroDir)
    || CheckCollisionRecs(tijolos.tamanho[3], tiroDireita->retTiroDir)
    || CheckCollisionRecs(tijolos.tamanho[4], tiroDireita->retTiroDir)
    || CheckCollisionRecs(tijolos.tamanho[5], tiroDireita->retTiroDir)
    || CheckCollisionRecs(tijolos.tamanho[6], tiroDireita->retTiroDir)
    || CheckCollisionRecs(bordas.tamanhoB[0], tiroDireita->retTiroDir)
    || CheckCollisionRecs(bordas.tamanhoB[1], tiroDireita->retTiroDir)
    || CheckCollisionRecs(bordas.tamanhoB[2], tiroDireita->retTiroDir)
    || CheckCollisionRecs(bordas.tamanhoB[3], tiroDireita->retTiroDir))
        retorno = 1;

    return retorno;
}

int collisionTiroEsq (Rectangles tijolos, Rectangles bordas, ShootLeft *tiroEsquerda){
    int retorno;

    if (CheckCollisionRecs(tijolos.tamanho[0], tiroEsquerda->retTiroEsq)
    || CheckCollisionRecs(tijolos.tamanho[1], tiroEsquerda->retTiroEsq)
    || CheckCollisionRecs(tijolos.tamanho[2], tiroEsquerda->retTiroEsq)
    || CheckCollisionRecs(tijolos.tamanho[3], tiroEsquerda->retTiroEsq)
    || CheckCollisionRecs(tijolos.tamanho[4], tiroEsquerda->retTiroEsq)
    || CheckCollisionRecs(tijolos.tamanho[5], tiroEsquerda->retTiroEsq)
    || CheckCollisionRecs(tijolos.tamanho[6], tiroEsquerda->retTiroEsq)
    || CheckCollisionRecs(bordas.tamanhoB[0], tiroEsquerda->retTiroEsq)
    || CheckCollisionRecs(bordas.tamanhoB[1], tiroEsquerda->retTiroEsq)
    || CheckCollisionRecs(bordas.tamanhoB[2], tiroEsquerda->retTiroEsq)
    || CheckCollisionRecs(bordas.tamanhoB[3], tiroEsquerda->retTiroEsq))
        retorno = 1;

    return retorno;
}

int collisionTiroCima (Rectangles tijolos, Rectangles bordas, ShootUp *tiroCima){
    int retorno = 0;

    if (CheckCollisionRecs(tijolos.tamanho[0], tiroCima->retTiroCima)
    || CheckCollisionRecs(tijolos.tamanho[1], tiroCima->retTiroCima)
    || CheckCollisionRecs(tijolos.tamanho[2], tiroCima->retTiroCima)
    || CheckCollisionRecs(tijolos.tamanho[3], tiroCima->retTiroCima)
    || CheckCollisionRecs(tijolos.tamanho[4], tiroCima->retTiroCima)
    || CheckCollisionRecs(tijolos.tamanho[5], tiroCima->retTiroCima)
    || CheckCollisionRecs(tijolos.tamanho[6], tiroCima->retTiroCima)
    || CheckCollisionRecs(bordas.tamanhoB[0], tiroCima->retTiroCima)
    || CheckCollisionRecs(bordas.tamanhoB[1], tiroCima->retTiroCima)
    || CheckCollisionRecs(bordas.tamanhoB[2], tiroCima->retTiroCima)
    || CheckCollisionRecs(bordas.tamanhoB[3], tiroCima->retTiroCima))
        retorno = 1;

    return retorno;
}

int collisionTiroBaixo (Rectangles tijolos, Rectangles bordas, ShootDown *tiroBaixo){
    int retorno;

    if (CheckCollisionRecs(tijolos.tamanho[0], tiroBaixo->retTiroBaixo)
    || CheckCollisionRecs(tijolos.tamanho[1], tiroBaixo->retTiroBaixo)
    || CheckCollisionRecs(tijolos.tamanho[2], tiroBaixo->retTiroBaixo)
    || CheckCollisionRecs(tijolos.tamanho[3], tiroBaixo->retTiroBaixo)
    || CheckCollisionRecs(tijolos.tamanho[4], tiroBaixo->retTiroBaixo)
    || CheckCollisionRecs(tijolos.tamanho[5], tiroBaixo->retTiroBaixo)
    || CheckCollisionRecs(tijolos.tamanho[6], tiroBaixo->retTiroBaixo)
    || CheckCollisionRecs(bordas.tamanhoB[0], tiroBaixo->retTiroBaixo)
    || CheckCollisionRecs(bordas.tamanhoB[1], tiroBaixo->retTiroBaixo)
    || CheckCollisionRecs(bordas.tamanhoB[2], tiroBaixo->retTiroBaixo)
    || CheckCollisionRecs(bordas.tamanhoB[3], tiroBaixo->retTiroBaixo))
        retorno = 1;

    return retorno;
}

void atualizaInimigo (Enemy *inimigo){
        if (randomMove == 1 || randomMove == 2 || randomMove == 3 || randomMove == 4){
            inimigo->posicao.x += 3.0f;
            inimigo->rotacao = 90;
            inimigo->retCollision = (Rectangle){inimigo->posicao.x - 22, inimigo->posicao.y - 19, 44, 38};
            direita1 = TRUE;
            esquerda1 = FALSE;
            cima1 = FALSE;
            baixo1 = FALSE;
        }
        if (randomMove == 5 || randomMove == 6 || randomMove == 7 || randomMove == 8){
            inimigo->posicao.x -= 3.0f;
            inimigo->rotacao = -90;
            inimigo->retCollision = (Rectangle){inimigo->posicao.x -22, inimigo->posicao.y - 19, 44, 38};
            direita1 = FALSE;
            esquerda1 = TRUE;
            cima1 = FALSE;
            baixo1 = FALSE;
        }
        if (randomMove == 9 || randomMove == 10 || randomMove == 11 || randomMove == 12){
            inimigo->posicao.y -= 3.0f;
            inimigo->rotacao = 0;
            inimigo->retCollision = (Rectangle){inimigo->posicao.x -19, inimigo->posicao.y -22, 38, 44};
            direita1 = FALSE;
            esquerda1 = FALSE;
            cima1 = TRUE;
            baixo1 = FALSE;
        }
        if (randomMove == 13 || randomMove == 14 || randomMove == 15 || randomMove == 16){
            inimigo->posicao.y += 3.0f;
            inimigo->rotacao = 180;
            inimigo->retCollision = (Rectangle){inimigo->posicao.x -19, inimigo->posicao.y -22, 38, 44};
            direita1 = FALSE;
            esquerda1 = FALSE;
            cima1 = FALSE;
            baixo1 = TRUE;
        }
}

void desenhoInimigo (Enemy *inimigo, Texture2D texturaInimigo){
    DrawTexturePro(texturaInimigo, inimigo->source, inimigo->retInimigo, inimigo->origin, inimigo->rotacao, LIGHTGRAY);
}

void tiroInimigo (){

}

int main(void){
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Battle INF");
    //Mouse
    Vector2 ponteiroMouse = {0.0f, 0.0f};
    //Cores
    Color corTexto[4] = {LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY}; //Opções do menu

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        /*UPDATE*/
        ponteiroMouse = GetMousePosition(); //Atualizando o ponteiro do mouse
        /*DRAW*/
        BeginDrawing();

            ClearBackground(BLACK);
            DrawText("Battle INF", 360, 80, 50, LIGHTGRAY);

            Rectangle novoJogo = {415, 220, 147, 30};
            DrawText("Novo jogo", 415, 220, 30, corTexto[0]);

            Rectangle continuar = {415, 280, 147, 30};
            DrawText("Continuar", 415, 280, 30, corTexto[1]);

            Rectangle ranking = {425, 340, 123, 30};
            DrawText("Ranking", 425, 340, 30, corTexto[2]);

            Rectangle sair = {455, 400, 60, 30};
            DrawText("Sair", 455, 400, 30, corTexto[3]);

            if (CheckCollisionPointRec(ponteiroMouse, novoJogo)) corTexto[0] = GOLD;
            else corTexto[0] = LIGHTGRAY; //Mudança de cor
            if (CheckCollisionPointRec(ponteiroMouse, continuar)) corTexto[1] = GOLD;
            else corTexto[1] = LIGHTGRAY; //Mudança de cor
            if (CheckCollisionPointRec(ponteiroMouse, ranking)) corTexto[2] = GOLD;
            else corTexto[2] = LIGHTGRAY; //Mudança de cor
            if (CheckCollisionPointRec(ponteiroMouse, sair)) corTexto[3] = GOLD;
            else corTexto[3] = LIGHTGRAY; //Mudança de cor

            if (CheckCollisionPointRec(ponteiroMouse, novoJogo) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                int tempo = 0;
                int nroInimigos = 1;
                //Cores
                Color corShield[3] = {WHITE, WHITE, WHITE};

                /*IMPORTANTE TEXTURAS*/
                Image ImgJogador = LoadImage("recursos/jogador.png");
                Texture2D texturaJogador = LoadTextureFromImage(ImgJogador); //Jogador

                ImgInimigo = LoadImage("recursos/inimigo.png");
                texturaInimigo = LoadTextureFromImage(ImgInimigo); //Inimigo

                Image ImgTijolos = LoadImage("recursos/tijolos.png");
                Texture2D texturaTijolos = LoadTextureFromImage(ImgTijolos); //Tijolos
                Image ImgShield = LoadImage("recursos/shield1.png");
                Texture2D texturaShield = LoadTextureFromImage(ImgShield); //Shield
                Image ImgEnergy = LoadImage("recursos/energy_drop1.png");
                Texture2D texturaEnergia = LoadTextureFromImage(ImgEnergy); //Energia

                /*RETANGULOS DA FASE 1*/
                //Tijolos
                Rectangles tijolos;
                tijolos.tamanho[0] = (Rectangle){65, 230, 210, 55};
                tijolos.tamanho[1] = (Rectangle){275, 230, 55, 257.5};
                tijolos.tamanho[2] = (Rectangle){65, 432, 210, 55};
                tijolos.tamanho[3] = (Rectangle){650, 40, 145, 55};
                tijolos.tamanho[4] = (Rectangle){590, 95, 206, 55};
                tijolos.tamanho[5] = (Rectangle){550, 150, 173.2, 55};
                tijolos.tamanho[6] = (Rectangle){750, 290, 55, 55};

                //Bordas
                Rectangles bordas;
                bordas.tamanhoB[0] = (Rectangle){0,0, screenWidth, 40}; //Header
                bordas.tamanhoB[1] = (Rectangle){0, 0, 0, screenHeight}; //Esquerda
                bordas.tamanhoB[2] = (Rectangle){screenWidth - 1, 0, 0, screenHeight}; //Direita
                bordas.tamanhoB[3] = (Rectangle){0, screenHeight, screenWidth, 0}; //Baixo

                /*VETORES E POSICIONAMENTO*/
                //Jogador
                Player jogador;
                jogador.posicao = (Vector2){(float)screenWidth/2, (float)screenHeight/2};
                jogador.origin = (Vector2){16.5, 21.5};
                jogador.source = (Rectangle){(float)screenWidth/2 + 28, (float)screenHeight/2 + 1, 33, 43};
                jogador.vidas = 3;
                jogador.rotacao = 0.0;
                jogador.pontos = 0;
                float x;
                float y;
                    /*OBS: variáveis referentes aos tiros - ativação*/
                    int direita;
                    int esquerda;
                    int cima;
                    int baixo;
                /*
                Enemy inimigo[MAXINIMIGOS];
                for (int i=0; i<MAXINIMIGOS; i++){
                    inimigo[i].posicao = (Vector2){GetRandomValue(60, 950), GetRandomValue(80, 760)}; //MUDAR
                    inimigo[i].origin = (Vector2){19, 22};
                    inimigo[i].source = (Rectangle){(float)screenWidth/2 + 32, (float)screenHeight/2 + 8, 38, 44};
                    inimigo[i].retCollision = (Rectangle){inimigo[i].posicao.x  -19, inimigo[i].posicao.y -22, 38, 44};
                    inimigo[i].rotacao = 0.0;
                }
                float x1[MAXINIMIGOS];
                float y1[MAXINIMIGOS];
                int direita1[MAXINIMIGOS];
                int esquerda1[MAXINIMIGOS];
                int cima1[MAXINIMIGOS];
                int baixo1[MAXINIMIGOS];
                */
                //Inimigo
                Enemy inimigo;
                inimigo.posicao = (Vector2){GetRandomValue(60, 950), GetRandomValue(80, 760)}; //MUDAR
                inimigo.origin = (Vector2){19, 22};
                inimigo.source = (Rectangle){(float)screenWidth/2 + 32, (float)screenHeight/2 + 8, 38, 44};
                inimigo.retCollision = (Rectangle){inimigo.posicao.x  -19, inimigo.posicao.y -22, 38, 44};
                inimigo.rotacao = 0.0;
                float x1;
                float y1;
                    /*OBS: variáveis referentes à movimentação aleatória do inímigo*/
                    int direita1;
                    int esquerda1;
                    int cima1;
                    int baixo1;


                /*----------------------------------- TIROS DO JOGADOR --------------------------------------*/
                //Tiro pra direita
                ShootRight tiroDireita[MAXTIROS] = {0};
                for (int i=0; i<MAXTIROS; i++){
                    tiroDireita[i].posicao = (Vector2){jogador.posicao.x + 30, jogador.posicao.y - 16.5};
                    tiroDireita[i].velocidade.x = 4;
                    tiroDireita[i].raio = 4;
                    tiroDireita[i].ativacao = FALSE;
                    tiroDireita[i].corTiro = DARKGRAY;
                    tiroDireita[i].lifeSpawn = 0;
                }
                //Tiro pra esquerda
                ShootLeft tiroEsquerda[MAXTIROS] = {0};
                for (int i=0; i<MAXTIROS; i++){
                    tiroEsquerda[i].posicao = (Vector2){jogador.posicao.x + 30, jogador.posicao.y - 16.5};
                    tiroEsquerda[i].velocidade.x = 4;
                    tiroEsquerda[i].raio = 4;
                    tiroEsquerda[i].ativacao = FALSE;
                    tiroEsquerda[i].corTiro = DARKGRAY;
                    tiroEsquerda[i].lifeSpawn = 0;
                }
                //Tiro pra cima
                ShootUp tiroCima[MAXTIROS] = {0};
                for (int i=0; i<MAXTIROS; i++){
                    tiroCima[i].posicao = (Vector2){jogador.posicao.x + 30, jogador.posicao.y - 16.5};
                    tiroCima[i].velocidade.x = 4;
                    tiroCima[i].raio = 4;
                    tiroCima[i].ativacao = FALSE;
                    tiroCima[i].corTiro = DARKGRAY;
                    tiroCima[i].lifeSpawn = 0;
                }
                //Tiro pra baixo
                ShootDown tiroBaixo[MAXTIROS] = {0};
                for (int i = 0; i<MAXTIROS; i++){
                    tiroBaixo[i].posicao = (Vector2){jogador.posicao.x + 30, jogador.posicao.y - 16.5};
                    tiroBaixo[i].velocidade.x = 4;
                    tiroBaixo[i].raio = 4;
                    tiroBaixo[i].ativacao = FALSE;
                    tiroBaixo[i].corTiro = DARKGRAY;
                    tiroBaixo[i].lifeSpawn = 0;
                }

                /*----------------------------------- TIROS DO INIMIGO --------------------------------------*/
                /*
                //Tiro pra direita
                ShootRightI tiroDireitaI[MAXTIROS] = {0};
                for (int i = 0; i<MAXTIROS; i++){
                    for (int j=0; j<MAXINIMIGOS; j++){
                        tiroDireitaI[i].posicao = (Vector2){inimigo[j].posicao.x + 30, inimigo[j].posicao.y - 16.5};
                        tiroDireitaI[i].velocidade.x = 4;
                        tiroDireitaI[i].raio = 4;
                        tiroDireitaI[i].ativacao = FALSE;
                        tiroDireitaI[i].corTiro = DARKGRAY;
                        tiroDireitaI[i].lifeSpawn = 0;
                    }
                }
                //Tiro pra esquerda
                ShootLeftI tiroEsquerdaI[MAXTIROS] = {0};
                for (int i=0; i<MAXTIROS; i++){
                    for (int j=0; j<MAXINIMIGOS; j++){
                        tiroEsquerdaI[i].posicao = (Vector2){inimigo[j].posicao.x + 30, inimigo[j].posicao.y - 16.5};
                        tiroEsquerdaI[i].velocidade.x = 4;
                        tiroEsquerdaI[i].raio = 4;
                        tiroEsquerdaI[i].ativacao = FALSE;
                        tiroEsquerdaI[i].corTiro = DARKGRAY;
                        tiroEsquerdaI[i].lifeSpawn = 0;
                    }
                }
                //Tiro pra cima
                ShootUpI tiroCimaI[MAXTIROS] = {0};
                for (int i=0; i<MAXTIROS; i++){
                    for (int j=0; j<MAXINIMIGOS; j++){
                        tiroCimaI[i].posicao = (Vector2){inimigo[j].posicao.x + 11, inimigo[j].posicao.y - 16.5};
                        tiroCimaI[i].velocidade.y = 4;
                        tiroCimaI[i].raio = 4;
                        tiroCimaI[i].ativacao = FALSE;
                        tiroCimaI[i].corTiro = DARKGRAY;
                        tiroCimaI[i].lifeSpawn = 0;
                    }
                }
                //Tiro pra baixo
                ShootDownI tiroBaixoI[MAXTIROS] = {0};
                for (int i=0; i<MAXTIROS; i++){
                    for (int j=0; j<MAXINIMIGOS; j++){
                        tiroBaixoI[i].posicao = (Vector2){inimigo[j].posicao.x + 11, inimigo[j].posicao.y - 16.5};
                        tiroBaixoI[i].velocidade.y = 4;
                        tiroBaixoI[i].raio = 4;
                        tiroBaixoI[i].ativacao = FALSE;
                        tiroBaixoI[i].corTiro = DARKGRAY;
                        tiroBaixoI[i].lifeSpawn = 0;
                    }
                }
                */

                ShootRightI tiroDireitaI[MAXTIROS] = {0};
                for (int i = 0; i<MAXTIROS; i++){
                    tiroDireitaI[i].posicao = (Vector2){inimigo.posicao.x + 30, inimigo.posicao.y - 16.5};
                    tiroDireitaI[i].velocidade.x = 4;
                    tiroDireitaI[i].raio = 4;
                    tiroDireitaI[i].ativacao = FALSE;
                    tiroDireitaI[i].corTiro = DARKGRAY;
                    tiroDireitaI[i].lifeSpawn = 0;
                }
                //Tiro pra esquerda
                ShootLeftI tiroEsquerdaI[MAXTIROS] = {0};
                for (int i=0; i<MAXTIROS; i++){
                    tiroEsquerdaI[i].posicao = (Vector2){inimigo.posicao.x + 30, inimigo.posicao.y - 16.5};
                    tiroEsquerdaI[i].velocidade.x = 4;
                    tiroEsquerdaI[i].raio = 4;
                    tiroEsquerdaI[i].ativacao = FALSE;
                    tiroEsquerdaI[i].corTiro = DARKGRAY;
                    tiroEsquerdaI[i].lifeSpawn = 0;
                }
                //Tiro pra cima
                ShootUpI tiroCimaI[MAXTIROS] = {0};
                for (int i=0; i<MAXTIROS; i++){
                    tiroCimaI[i].posicao = (Vector2){inimigo.posicao.x + 11, inimigo.posicao.y - 16.5};
                    tiroCimaI[i].velocidade.y = 4;
                    tiroCimaI[i].raio = 4;
                    tiroCimaI[i].ativacao = FALSE;
                    tiroCimaI[i].corTiro = DARKGRAY;
                    tiroCimaI[i].lifeSpawn = 0;
                }
                //Tiro pra baixo
                ShootDownI tiroBaixoI[MAXTIROS] = {0};
                for (int i=0; i<MAXTIROS; i++){
                    tiroBaixoI[i].posicao = (Vector2){inimigo.posicao.x + 11, inimigo.posicao.y - 16.5};
                    tiroBaixoI[i].velocidade.y = 4;
                    tiroBaixoI[i].raio = 4;
                    tiroBaixoI[i].ativacao = FALSE;
                    tiroBaixoI[i].corTiro = DARKGRAY;
                    tiroBaixoI[i].lifeSpawn = 0;
                }
                /*-------------------------------------------------------------------------------------------*/

                while (!WindowShouldClose()){
                    SetRandomSeed(time(NULL));
                    int tiroInimigo;
                    /*
                    for (int i=0; i<MAXINIMIGOS; i++){
                        randomMove[i] = GetRandomValue(1,16);
                        tiroInimigo[i] = GetRandomValue(0,1);
                    }
                    */

                    randomMove = GetRandomValue(1,16);
                    tiroInimigo = GetRandomValue(0,16);

                    //Guarda valores de posição
                    x = jogador.posicao.x;
                    y = jogador.posicao.y;

                    x1 = inimigo.posicao.x;
                    y1 = inimigo.posicao.y;

                    jogador.retJogador = (Rectangle){jogador.posicao.x + 5, jogador.posicao.y + 16.5, 33, 44};

                    //Movimentação do jogador
                    if (IsKeyDown(KEY_RIGHT)){
                        jogador.posicao.x += 3.0f;
                        jogador.rotacao = 90;
                        jogador.retCollision = (Rectangle){jogador.posicao.x - 23, jogador.posicao.y - 16.5, 43, 33};
                        direita = TRUE;
                        esquerda = FALSE;
                        cima = FALSE;
                        baixo = FALSE;
                    }
                    if (IsKeyDown(KEY_LEFT)){
                        jogador.posicao.x -= 3.0f;
                        jogador.rotacao = -90;
                        jogador.retCollision = (Rectangle){jogador.posicao.x - 23, jogador.posicao.y - 16.5, 43, 33};
                        direita = FALSE;
                        esquerda = TRUE;
                        cima = FALSE;
                        baixo = FALSE;
                    }
                    if (IsKeyDown(KEY_UP)){
                        jogador.posicao.y -= 3.0f;
                        jogador.rotacao = 0;
                        jogador.retCollision = (Rectangle){jogador.posicao.x - 16.5, jogador.posicao.y - 21.5, 33, 43};
                        direita = FALSE;
                        esquerda = FALSE;
                        cima = TRUE;
                        baixo = FALSE;
                    }
                    if (IsKeyDown(KEY_DOWN)){
                        jogador.posicao.y += 3.0f;
                        jogador.rotacao = 180;
                        jogador.retCollision = (Rectangle){jogador.posicao.x - 16.5, jogador.posicao.y - 21.5, 33, 43};
                        direita = FALSE;
                        esquerda = FALSE;
                        cima = FALSE;
                        baixo = TRUE;
                    }

                    /*MOVIMENTAÇÃO ALEATÓRIA DO TANQUE INIMIGO*/
                    inimigo.posicao_anterior = inimigo.posicao;

                    atualizaInimigo(&inimigo);

                    if (collisionInimigo(tijolos, bordas, inimigo, jogador))
                        inimigo.posicao = inimigo.posicao_anterior;


                    /*
                    for (int i=0; i<MAXINIMIGOS; i++){
                        if (randomMove[i] == 1 || randomMove[i] == 2 || randomMove[i] == 3 || randomMove[i] == 4){
                            inimigo[i].posicao.x += 3.0f;
                            inimigo[i].rotacao = 90;
                            inimigo[i].retCollision = (Rectangle){inimigo[i].posicao.x - 22, inimigo[i].posicao.y - 19, 44, 38};
                            direita1[i] = TRUE;
                            esquerda1[i] = FALSE;
                            cima1[i] = FALSE;
                            baixo1[i] = FALSE;
                        }
                        if (randomMove[i] == 5 || randomMove[i] == 6 || randomMove[i] == 7 || randomMove[i] == 8){
                            inimigo[i].posicao.x -= 3.0f;
                            inimigo[i].rotacao = -90;
                            inimigo[i].retCollision = (Rectangle){inimigo[i].posicao.x -22, inimigo[i].posicao.y - 19, 44, 38};
                            direita1[i] = FALSE;
                            esquerda1[i] = TRUE;
                            cima1[i] = FALSE;
                            baixo1[i] = FALSE;
                        }
                        if (randomMove[i] == 9 || randomMove[i] == 10 || randomMove[i] == 11 || randomMove[i] == 12){
                            inimigo[i].posicao.y -= 3.0f;
                            inimigo[i].rotacao = 0;
                            inimigo[i].retCollision = (Rectangle){inimigo[i].posicao.x -19, inimigo[i].posicao.y -22, 38, 44};
                            direita1[i] = FALSE;
                            esquerda1[i] = FALSE;
                            cima1[i] = TRUE;
                            baixo1[i] = FALSE;
                        }
                        if (randomMove[i] == 13 || randomMove[i] == 14 || randomMove[i] == 15 || randomMove[i] == 16){
                            inimigo[i].posicao.y += 3.0f;
                            inimigo[i].rotacao = 180;
                            inimigo[i].retCollision = (Rectangle){inimigo[i].posicao.x -19, inimigo[i].posicao.y -22, 38, 44};
                            direita1[i] = FALSE;
                            esquerda1[i] = FALSE;
                            cima1[i] = FALSE;
                            baixo1[i] = TRUE;
                        }
                    }
                    */

                    /*---------------------------------- TIROS DO JOGADOR -------------------------------------*/
                    //Tiro pra direita
                    for (int i=0; i<MAXTIROS; i++){
                        if (IsKeyPressed(KEY_SPACE) && direita == TRUE){
                            if (!tiroDireita[i].ativacao){
                                tiroDireita[i].posicao = (Vector2){jogador.posicao.x, jogador.posicao.y};
                                tiroDireita[i].ativacao = TRUE;
                            }
                        }
                    }
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroDireita[i].ativacao){
                            tiroDireita[i].posicao.x += 4;
                            tiroDireita[i].lifeSpawn++;

                            if (tiroDireita[i].posicao.x > 1000)
                                tiroDireita[i].ativacao = FALSE;
                            if (tiroDireita[i].ativacao){
                                DrawCircleV(tiroDireita[i].posicao, tiroDireita[i].raio, DARKGRAY);
                                tiroDireita[i].retTiroDir = (Rectangle){tiroDireita[i].posicao.x, tiroDireita[i].posicao.y, 4, 4};
                            }
                        }
                    }
                    //Tiro pra esquerda
                    for (int i=0; i<MAXTIROS; i++){
                        if (IsKeyPressed(KEY_SPACE) && esquerda == TRUE){
                            if (!tiroEsquerda[i].ativacao){
                                tiroEsquerda[i].posicao = (Vector2){jogador.posicao.x, jogador.posicao.y};
                                tiroEsquerda[i].ativacao = TRUE;
                            }
                        }
                    }
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroEsquerda[i].ativacao){
                            tiroEsquerda[i].posicao.x = tiroEsquerda[i].posicao.x - 4;
                            tiroEsquerda[i].lifeSpawn++;

                            if (tiroEsquerda[i].posicao.x < 0)
                                tiroEsquerda[i].ativacao = FALSE;
                            if (tiroEsquerda[i].ativacao){
                                DrawCircleV(tiroEsquerda[i].posicao, tiroEsquerda[i].raio, DARKGRAY);
                                tiroEsquerda[i].retTiroEsq = (Rectangle){tiroEsquerda[i].posicao.x, tiroEsquerda[i].posicao.y, 4, 4};
                            }
                        }
                    }
                    //Tiro pra cima
                    for (int i=0; i<MAXTIROS; i++){
                        if (IsKeyPressed(KEY_SPACE) && cima == TRUE){
                            if (!tiroCima[i].ativacao){
                                tiroCima[i].posicao = (Vector2){jogador.posicao.x, jogador.posicao.y};
                                tiroCima[i].ativacao = TRUE;
                            }
                        }
                    }
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroCima[i].ativacao){
                            tiroCima[i].posicao.y -= 4;
                            tiroCima[i].lifeSpawn++;

                            if (tiroCima[i].posicao.y < 40)
                                tiroCima[i].ativacao = FALSE;
                            if (tiroCima[i].ativacao){
                                DrawCircleV(tiroCima[i].posicao, tiroCima[i].raio, DARKGRAY);
                                tiroCima[i].retTiroCima = (Rectangle){tiroCima[i].posicao.x, tiroCima[i].posicao.y, 4, 4};
                            }
                        }
                    }
                    //Tiro pra baixo
                    for (int i=0; i<MAXTIROS; i++){
                        if (IsKeyPressed(KEY_SPACE) && baixo == TRUE){
                            if (!tiroBaixo[i].ativacao){
                                tiroBaixo[i].posicao = (Vector2){jogador.posicao.x, jogador.posicao.y};
                                tiroBaixo[i].ativacao = TRUE;
                            }
                        }
                    }
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroBaixo[i].ativacao){
                            tiroBaixo[i].posicao.y += 4;
                            tiroBaixo[i].lifeSpawn++;

                            if (tiroBaixo[i].posicao.y > 800)
                                tiroBaixo[i].ativacao = FALSE;
                            if (tiroBaixo[i].ativacao){
                                DrawCircleV(tiroBaixo[i].posicao, tiroBaixo[i].raio, DARKGRAY);
                                tiroBaixo[i].retTiroBaixo = (Rectangle){tiroBaixo[i].posicao.x, tiroBaixo[i].posicao.y, 4, 4};
                            }
                        }
                    }

                    /*----------------------------------- TIROS DO INIMIGO --------------------------------------*/
                    //Tiro pra direita
                    /*
                    for (int i=0; i<MAXTIROS; i++){
                        for (int j=0; j<MAXINIMIGOS; j++){
                            if (tiroInimigo[j] == 0 && direita1[j] == TRUE){
                                if (!tiroDireitaI[i].ativacao){
                                    tiroDireitaI[i].posicao = (Vector2){inimigo[j].posicao.x, inimigo[j].posicao.y};
                                    tiroDireitaI[i].ativacao = TRUE;
                                }
                            }
                        }
                    }
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroDireitaI[i].ativacao){
                            tiroDireitaI[i].posicao.x += 4;
                            tiroDireitaI[i].lifeSpawn++;

                            if (tiroDireitaI[i].posicao.x > 1000)
                                tiroDireitaI[i].ativacao = FALSE;
                            if (tiroDireitaI[i].ativacao){
                                DrawCircleV(tiroDireitaI[i].posicao, tiroDireitaI[i].raio, DARKGRAY);
                                tiroDireitaI[i].retTiroDirI = (Rectangle){tiroDireitaI[i].posicao.x, tiroDireitaI[i].posicao.y, 4, 4};
                            }
                        }
                    }
                    //Tiro pra esquerda
                    for (int i=0; i<MAXTIROS; i++){
                        for (int j=0; j<MAXINIMIGOS; j++){
                            if (tiroInimigo[j] == 0 && esquerda1[j] == TRUE){
                                if (!tiroEsquerdaI[i].ativacao){
                                    tiroEsquerdaI[i].posicao = (Vector2){inimigo[j].posicao.x, inimigo[j].posicao.y};
                                    tiroEsquerdaI[i].ativacao = TRUE;
                                }
                            }
                        }
                    }
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroEsquerdaI[i].ativacao){
                            tiroEsquerdaI[i].posicao.x = tiroEsquerdaI[i].posicao.x - 4;
                            tiroEsquerdaI[i].lifeSpawn++;

                            if (tiroEsquerdaI[i].posicao.x < 0)
                                tiroEsquerdaI[i].ativacao = FALSE;
                            if (tiroEsquerdaI[i].ativacao){
                                DrawCircleV(tiroEsquerdaI[i].posicao, tiroEsquerdaI[i].raio, DARKGRAY);
                                tiroEsquerdaI[i].retTiroEsqI = (Rectangle){tiroEsquerdaI[i].posicao.x, tiroEsquerdaI[i].posicao.y, 4, 4};
                            }
                        }
                    }

                    //Tiro pra cima
                    for (int i=0; i<MAXTIROS; i++){
                        for (int j=0; j<MAXINIMIGOS; j++){
                            if (tiroInimigo[j] == 0 && cima1[j] == TRUE){
                                if (!tiroCimaI[i].ativacao){
                                    tiroCimaI[i].posicao = (Vector2){inimigo[j].posicao.x, inimigo[j].posicao.y};
                                    tiroCimaI[i].ativacao = TRUE;
                                }
                            }
                        }
                    }
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroCimaI[i].ativacao){
                            tiroCimaI[i].posicao.y -= 4;
                            tiroCimaI[i].lifeSpawn++;

                            if (tiroCimaI[i].posicao.y < 40)
                                tiroCimaI[i].ativacao = FALSE;
                            if (tiroCimaI[i].ativacao){
                                DrawCircleV(tiroCimaI[i].posicao, tiroCimaI[i].raio, DARKGRAY);
                                tiroCimaI[i].retTiroCimaI = (Rectangle){tiroCimaI[i].posicao.x, tiroCimaI[i].posicao.y, 4, 4};
                            }
                        }
                    }
                    //Tiro pra baixo
                    for (int i=0; i<MAXTIROS; i++){
                        for (int j=0; j<MAXINIMIGOS; j++){
                            if (tiroInimigo[j] == 0 && baixo1[j] == TRUE){
                                if (!tiroBaixoI[i].ativacao){
                                    tiroBaixoI[i].posicao = (Vector2){inimigo[j].posicao.x, inimigo[j].posicao.y};
                                    tiroBaixoI[i].ativacao = TRUE;
                                }
                            }
                        }
                    }
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroBaixoI[i].ativacao){
                            tiroBaixoI[i].posicao.y += 4;
                            tiroBaixoI[i].lifeSpawn++;

                            if (tiroBaixoI[i].posicao.y > 800)
                                tiroBaixoI[i].ativacao = FALSE;
                            if (tiroBaixoI[i].ativacao){
                                DrawCircleV(tiroBaixoI[i].posicao, tiroBaixoI[i].raio, DARKGRAY);
                                tiroBaixoI[i].retTiroBaixoI = (Rectangle){tiroBaixoI[i].posicao.x, tiroBaixoI[i].posicao.y, 4, 4};
                            }
                        }
                    }
                    */
                    //Tiro pra direita
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroInimigo == 0 && direita1 == TRUE){
                            if (!tiroDireitaI[i].ativacao){
                                tiroDireitaI[i].posicao = (Vector2){inimigo.posicao.x, inimigo.posicao.y};
                                tiroDireitaI[i].ativacao = TRUE;
                            }
                        }
                    }
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroDireitaI[i].ativacao){
                            tiroDireitaI[i].posicao.x += 4;
                            tiroDireitaI[i].lifeSpawn++;

                            if (tiroDireitaI[i].posicao.x > 1000)
                                tiroDireitaI[i].ativacao = FALSE;
                            if (tiroDireitaI[i].ativacao){
                                DrawCircleV(tiroDireitaI[i].posicao, tiroDireitaI[i].raio, DARKGRAY);
                                tiroDireitaI[i].retTiroDirI = (Rectangle){tiroDireitaI[i].posicao.x, tiroDireitaI[i].posicao.y, 4, 4};
                            }
                        }
                    }
                    //Tiro pra esquerda
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroInimigo == 0 && esquerda1 == TRUE){
                            if (!tiroEsquerdaI[i].ativacao){
                                tiroEsquerdaI[i].posicao = (Vector2){inimigo.posicao.x, inimigo.posicao.y};
                                tiroEsquerdaI[i].ativacao = TRUE;
                            }
                        }
                    }
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroEsquerdaI[i].ativacao){
                            tiroEsquerdaI[i].posicao.x = tiroEsquerdaI[i].posicao.x - 4;
                            tiroEsquerdaI[i].lifeSpawn++;

                            if (tiroEsquerdaI[i].posicao.x < 0)
                                tiroEsquerdaI[i].ativacao = FALSE;
                            if (tiroEsquerdaI[i].ativacao){
                                DrawCircleV(tiroEsquerdaI[i].posicao, tiroEsquerdaI[i].raio, DARKGRAY);
                                tiroEsquerdaI[i].retTiroEsqI = (Rectangle){tiroEsquerdaI[i].posicao.x, tiroEsquerdaI[i].posicao.y, 4, 4};
                            }
                        }
                    }
                    //Tiro pra cima
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroInimigo == 0 && cima1 == TRUE){
                            if (!tiroCimaI[i].ativacao){
                                tiroCimaI[i].posicao = (Vector2){inimigo.posicao.x, inimigo.posicao.y};
                                tiroCimaI[i].ativacao = TRUE;
                            }
                        }
                    }
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroCimaI[i].ativacao){
                            tiroCimaI[i].posicao.y -= 4;
                            tiroCimaI[i].lifeSpawn++;

                            if (tiroCimaI[i].posicao.y < 40)
                                tiroCimaI[i].ativacao = FALSE;
                            if (tiroCimaI[i].ativacao){
                                DrawCircleV(tiroCimaI[i].posicao, tiroCimaI[i].raio, DARKGRAY);
                                tiroCimaI[i].retTiroCimaI = (Rectangle){tiroCimaI[i].posicao.x, tiroCimaI[i].posicao.y, 4, 4};
                            }
                        }
                    }
                    //Tiro pra baixo
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroInimigo == 0 && baixo1 == TRUE){
                            if (!tiroBaixoI[i].ativacao){
                                tiroBaixoI[i].posicao = (Vector2){inimigo.posicao.x, inimigo.posicao.y};
                                tiroBaixoI[i].ativacao = TRUE;
                            }
                        }
                    }
                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroBaixoI[i].ativacao){
                            tiroBaixoI[i].posicao.y += 4;
                            tiroBaixoI[i].lifeSpawn++;

                            if (tiroBaixoI[i].posicao.y > 800)
                                tiroBaixoI[i].ativacao = FALSE;
                            if (tiroBaixoI[i].ativacao){
                                DrawCircleV(tiroBaixoI[i].posicao, tiroBaixoI[i].raio, DARKGRAY);
                                tiroBaixoI[i].retTiroBaixoI = (Rectangle){tiroBaixoI[i].posicao.x, tiroBaixoI[i].posicao.y, 4, 4};
                            }
                        }
                    }

                    /*-------------------------------------------------------------------------------------------*/

                    //Retangulos do jogador e do inimigo
                    jogador.retJogador = (Rectangle){jogador.posicao.x, jogador.posicao.y, 33, 43};
                    inimigo.retInimigo = (Rectangle){inimigo.posicao.x, inimigo.posicao.y, 38, 44};


                    /*CHECKCOLLISION*/
                    int colPlayerTij; //Player
                    int colPlayerBordas; //Player
                    int colTiroDir; //Player
                    int colTiroEsq; //Player
                    int colTiroCima; //Player
                    int colTiroBaixo;
                    colPlayerTij = collisionPlayerTij(tijolos, jogador);
                    colPlayerBordas = collisionPlayerBordas(bordas, jogador);
                    colTiroDir = collisionTiroDir(tijolos, bordas, &tiroDireita);
                    colTiroEsq = collisionTiroEsq(tijolos, bordas, &tiroEsquerda);
                    colTiroCima = collisionTiroCima(tijolos, bordas, &tiroCima);
                    colTiroBaixo = collisionTiroBaixo(tijolos, bordas, &tiroBaixo);
                    /*
                    for (int i=0; i<MAXINIMIGOS; i++){
                        if (colPlayerTij == TRUE || CheckCollisionRecs(inimigo[i].retCollision, jogador.retCollision) || colPlayerBordas == TRUE){
                            jogador.posicao.x = x;
                            jogador.posicao.y = y;
                        } //Colisão do Jogador
                    }
                    for (int i=0; i<MAXINIMIGOS; i++){
                        if (colInimigo[i] == 1){
                            inimigo[i].posicao.x = x1[i];
                            inimigo[i].posicao.y = y1[i];
                        } //Colisão do inimigo
                    }

                    if (colTiroDir == 1){
                        for (int i=0; i<MAXTIROS; i++)
                            tiroDireita[i].ativacao = FALSE;
                    } //Tiro direita do jogador

                    for (int j=0; j<MAXINIMIGOS; j++){
                        if (CheckCollisionRecs(inimigo[j].retCollision, tiroDireita->retTiroDir)){
                            for (int i=0; i<MAXTIROS; i++){
                                tiroDireita[i].ativacao = FALSE;
                                tiroDireita[i].retTiroDir = (Rectangle){jogador.posicao.x, jogador.posicao.y, 0, 0};
                            }
                            jogador.pontos += 800;
                        } //Tiro pra direita: colisão com o tanque inimigo; soma pontuação
                    }

                    if (colTiroEsq == 1){
                        for (int i=0; i<MAXTIROS; i++)
                            tiroEsquerda[i].ativacao = FALSE;
                    } //Tiro pra esquerda do jogador

                    for (int j=0; j<MAXINIMIGOS; j++){
                        if (CheckCollisionRecs(inimigo[j].retCollision, tiroEsquerda->retTiroEsq)){
                            for (int i=0; i<MAXTIROS; i++){
                                tiroEsquerda[i].ativacao = FALSE;
                                tiroEsquerda[i].retTiroEsq = (Rectangle){jogador.posicao.x, jogador.posicao.y, 0, 0};
                            }
                            jogador.pontos += 800;
                        } // Tiro pra esquerda: colisão com o tanque inimigo; soma pontuação
                    }

                    if (colTiroCima == 1){
                        for (int i=0; i<MAXTIROS; i++)
                            tiroCima[i].ativacao = FALSE;
                    } //Tiro pra cima do jogador

                    for (int j=0; j<MAXINIMIGOS; j++){
                        if (CheckCollisionRecs(inimigo[j].retCollision, tiroCima->retTiroCima)){
                            for (int i=0; i<MAXTIROS; i++){
                                tiroCima[i].ativacao = FALSE;
                                tiroCima[i].retTiroCima = (Rectangle){jogador.posicao.x, jogador.posicao.y, 0, 0};
                            }
                            jogador.pontos += 800;
                        } //Tiro pra cima: colisão com tanque inimigo; soma pontuação
                    }

                    if (colTiroBaixo == 1){
                        for (int i=0; i<MAXTIROS; i++)
                            tiroBaixo[i].ativacao = FALSE;
                    } //Tiro pra baixo do jogador

                    for (int j=0; j<MAXINIMIGOS; j++){
                        if (CheckCollisionRecs(inimigo[j].retCollision, tiroBaixo->retTiroBaixo)){
                            for (int i=0; i<MAXTIROS; i++){
                                tiroBaixo[i].ativacao = FALSE;
                                tiroBaixo[i].retTiroBaixo = (Rectangle){jogador.posicao.x, jogador.posicao.y, 0, 0};
                            }
                            jogador.pontos += 800;
                        } //Tiro pra baixo: colisão com tanque inimgo; soma pontuação
                    }

                    //Vidas
                    if (CheckCollisionRecs(tiroDireitaI->retTiroDirI, jogador.retCollision)
                    || CheckCollisionRecs(tiroEsquerdaI->retTiroEsqI, jogador.retCollision)
                    || CheckCollisionRecs(tiroCimaI->retTiroCimaI, jogador.retCollision)
                    || CheckCollisionRecs(tiroBaixoI->retTiroBaixoI, jogador.retCollision))
                        jogador.vidas = jogador.vidas - 1;


                    //Apaga os escudos
                    if (jogador.vidas == 2) corShield[2] = BLANK;
                    if (jogador.vidas == 1) corShield[1] = BLANK;
                    if (jogador.vidas == 0) corShield[0] = BLANK;
                    */
                    if (colPlayerTij == TRUE || CheckCollisionRecs(inimigo.retCollision, jogador.retCollision) || colPlayerBordas == TRUE){
                        jogador.posicao.x = x;
                        jogador.posicao.y = y;
                    } //Colisão do Jogador

                    if (colTiroDir == 1){
                        for (int i=0; i<MAXTIROS; i++)
                            tiroDireita[i].ativacao = FALSE;
                    } //Tiro direita do jogador

                    if (CheckCollisionRecs(inimigo.retCollision, tiroDireita->retTiroDir)){
                        for (int i=0; i<MAXTIROS; i++){
                            tiroDireita[i].ativacao = FALSE;
                            tiroDireita[i].retTiroDir = (Rectangle){jogador.posicao.x, jogador.posicao.y, 0, 0};
                        }
                        jogador.pontos += 800;
                    } //Tiro pra direita: colisão com o tanque inimigo; soma pontuação

                    if (colTiroEsq == 1){
                        for (int i=0; i<MAXTIROS; i++)
                            tiroEsquerda[i].ativacao = FALSE;
                    } //Tiro pra esquerda do jogador

                    if (CheckCollisionRecs(inimigo.retCollision, tiroEsquerda->retTiroEsq)){
                        for (int i=0; i<MAXTIROS; i++){
                            tiroEsquerda[i].ativacao = FALSE;
                            tiroEsquerda[i].retTiroEsq = (Rectangle){jogador.posicao.x, jogador.posicao.y, 0, 0};
                        }
                        jogador.pontos += 800;
                    } // Tiro pra esquerda: colisão com o tanque inimigo; soma pontuação

                    if (colTiroCima == 1){
                        for (int i=0; i<MAXTIROS; i++)
                            tiroCima[i].ativacao = FALSE;
                    } //Tiro pra cima do jogador

                    if (CheckCollisionRecs(inimigo.retCollision, tiroCima->retTiroCima)){
                        for (int i=0; i<MAXTIROS; i++){
                            tiroCima[i].ativacao = FALSE;
                            tiroCima[i].retTiroCima = (Rectangle){jogador.posicao.x, jogador.posicao.y, 0, 0};
                        }
                        jogador.pontos += 800;
                    } //Tiro pra cima: colisão com tanque inimigo; soma pontuação

                    if (colTiroBaixo == 1){
                        for (int i=0; i<MAXTIROS; i++)
                            tiroBaixo[i].ativacao = FALSE;
                    } //Tiro pra baixo do jogador

                    if (CheckCollisionRecs(inimigo.retCollision, tiroBaixo->retTiroBaixo)){
                        for (int i=0; i<MAXTIROS; i++){
                            tiroBaixo[i].ativacao = FALSE;
                            tiroBaixo[i].retTiroBaixo = (Rectangle){jogador.posicao.x, jogador.posicao.y, 0, 0};
                        }
                        jogador.pontos += 800;
                    } //Tiro pra baixo: colisão com tanque inimgo; soma pontuação

                    //Vidas
                    if (CheckCollisionRecs(tiroDireitaI->retTiroDirI, jogador.retCollision)
                    || CheckCollisionRecs(tiroEsquerdaI->retTiroEsqI, jogador.retCollision)
                    || CheckCollisionRecs(tiroCimaI->retTiroCimaI, jogador.retCollision)
                    || CheckCollisionRecs(tiroBaixoI->retTiroBaixoI, jogador.retCollision))
                        jogador.vidas = jogador.vidas - 1;


                    //Apaga os escudos
                    if (jogador.vidas == 2) corShield[2] = BLANK;
                    if (jogador.vidas == 1) corShield[1] = BLANK;
                    if (jogador.vidas == 0) corShield[0] = BLANK;

                    BeginDrawing();

                        ClearBackground (LIGHTGRAY);

                        menu(screenWidth); // Desenhando menu
                        DrawText(TextFormat("Pontos: %d", jogador.pontos), 850, 12, 20, GOLD);
                        DrawTexture(texturaShield, 10, 5, corShield[0]);
                        DrawTexture(texturaShield, 50, 5, corShield[1]);
                        DrawTexture(texturaShield, 90, 5, corShield[2]);

                        DrawTexturePro(texturaJogador, jogador.source, jogador.retJogador, jogador.origin, jogador.rotacao, LIGHTGRAY);
                        //DrawTexturePro(texturaInimigo, inimigo.source, inimigo.retInimigo, inimigo.origin, inimigo.rotacao, LIGHTGRAY);
                        desenhoInimigo(&inimigo, texturaInimigo);

                        DrawRectangleRec(tiroDireita->retTiroDir, BLACK);
                        DrawRectangleRec(tiroEsquerda->retTiroEsq, BLACK);
                        DrawRectangleRec(tiroCima->retTiroCima, BLACK);
                        DrawRectangleRec(tiroBaixo->retTiroBaixo, BLACK);

                        DrawRectangleRec(tiroDireitaI->retTiroDirI, BLACK);
                        DrawRectangleRec(tiroEsquerdaI->retTiroEsqI, BLACK);
                        DrawRectangleRec(tiroCimaI->retTiroCimaI, BLACK);
                        DrawRectangleRec(tiroBaixoI->retTiroBaixoI, BLACK);

                        blocosFase1 (texturaTijolos); //Chamada da função que carrega os blocos

                    EndDrawing();
                }

            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
