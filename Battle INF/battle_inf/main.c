 #include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define FALSE 0
#define TRUE 1
#define MAXTIROS 3

/*STRUCTS*/
typedef struct player{
    Vector2 posicao;
    Vector2 origin;
    Rectangle source;
    Rectangle retJogador;
    Rectangle retCollision;
    float rotacao;
    int vidas;
} Player; //Jogador

typedef struct enemy{
    Vector2 posicao;
    Vector2 origin;
    Rectangle source;
    Rectangle retInimigo;
    Rectangle retCollision;
    float rotacao;
} Enemy; //Inimigo

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


typedef struct rectangles{
    //Fase 1
    Rectangle tamanho[7];
    //Bordas do jogo
    Rectangle tamanhoB[4];
} Rectangles; //Retangulos

/*-----------------------*/

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
                SetRandomSeed(time(NULL));

                //Cores
                Color corShield[3] = {WHITE, WHITE, WHITE};

                /*IMPORTANTE TEXTURAS*/
                Image ImgJogador = LoadImage("recursos/jogador.png");
                Texture2D texturaJogador = LoadTextureFromImage(ImgJogador); //Jogador

                Image ImgInimigo = LoadImage("recursos/inimigo.png");
                Texture2D texturaInimigo = LoadTextureFromImage(ImgInimigo); //Inimigo

                Image ImgTijolos = LoadImage("recursos/tijolos.png");
                Texture2D texturaTijolos = LoadTextureFromImage(ImgTijolos); //Tijolos

                Image ImgShield = LoadImage("recursos/shield1.png");
                Texture2D texturaShield = LoadTextureFromImage(ImgShield); //Shield

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
                bordas.tamanhoB[2] = (Rectangle){screenWidth, 0, 0, screenHeight}; //Direita
                bordas.tamanhoB[3] = (Rectangle){0, screenHeight, screenWidth, 0}; //Baixo


                /*VETORES E POSICIONAMENTO*/
                //Jogador
                Player jogador;
                jogador.posicao = (Vector2){(float)screenWidth/2, (float)screenHeight/2};
                jogador.origin = (Vector2){16.5, 21.5};
                jogador.source = (Rectangle){(float)screenWidth/2 + 28, (float)screenHeight/2 + 1, 33, 43};
                jogador.vidas = 3;
                jogador.rotacao = 0.0;
                float x;
                float y;
                    /*OBS: variáveis referentes aos tiros - ativação*/
                    int direita;
                    int esquerda;
                    int cima;
                    int baixo;

                //Inimigo
                Enemy inimigo;
                inimigo.posicao = (Vector2){(float)GetRandomValue(0,screenWidth), (float)GetRandomValue(40, screenHeight)};
                inimigo.origin = (Vector2){19, 22};
                inimigo.source = (Rectangle){(float)screenWidth/2 + 32, (float)screenHeight/2 + 8, 38, 44};
                inimigo.retCollision = (Rectangle){inimigo.posicao.x  -19, inimigo.posicao.y -22, 38, 44};
                inimigo.rotacao = 0.0;

                /*-------------------------------------------------------------------------------------------------------*/

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

                /*-----------------------------------------------------------------------------------------------------*/

                while (!WindowShouldClose()){
                    /*UPDATE*/
                    x = jogador.posicao.x;
                    y = jogador.posicao.y;

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

                    /*LÓGICA PARA ROTACAO DO TANQUE INIMIGO*/
                    if (IsKeyDown(KEY_D)){
                        inimigo.posicao.x += 3.0f;
                        inimigo.rotacao = 90;
                        inimigo.retCollision = (Rectangle){inimigo.posicao.x - 22, inimigo.posicao.y - 19, 44, 38};
                    }
                    if (IsKeyDown(KEY_A)){
                        inimigo.posicao.x -= 3.0f;
                        inimigo.rotacao = -90;
                        inimigo.retCollision = (Rectangle){inimigo.posicao.x -22, inimigo.posicao.y - 19, 44, 38};
                    }
                    if (IsKeyDown(KEY_W)){
                        inimigo.posicao.y -= 3.0f;
                        inimigo.rotacao = 0;
                        inimigo.retCollision = (Rectangle){inimigo.posicao.x -19, inimigo.posicao.y -22, 38, 44};
                    }
                    if (IsKeyDown(KEY_S)){
                        inimigo.posicao.y += 3.0f;
                        inimigo.rotacao = 180;
                        inimigo.retCollision = (Rectangle){inimigo.posicao.x -19, inimigo.posicao.y -22, 38, 44};
                    }

                    /*TIROS*/
                    //Tiro pra direita
                    for (int i=0; i<MAXTIROS; i++){
                        if (IsKeyPressed(KEY_SPACE) && direita == TRUE){
                            if (!tiroDireita[i].ativacao){
                                tiroDireita[i].posicao = (Vector2){jogador.posicao.x + 30, jogador.posicao.y};
                                tiroDireita[i].ativacao = TRUE;
                            }
                        }
                    }

                    for (int i=0; i<MAXTIROS; i++){
                        if (tiroDireita[i].ativacao){
                            tiroDireita[i].posicao.x += 4;
                            tiroDireita[i].lifeSpawn++;

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
                            tiroEsquerda[i].posicao.x -= 4;
                            tiroEsquerda[i].lifeSpawn++;

                            if (tiroEsquerda[i].posicao.x < 0) tiroEsquerda[i].ativacao = FALSE;
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

                            if (tiroBaixo[i].ativacao){
                                DrawCircleV(tiroBaixo[i].posicao, tiroBaixo[i].raio, DARKGRAY);
                                tiroBaixo[i].retTiroBaixo = (Rectangle){tiroBaixo[i].posicao.x, tiroBaixo[i].posicao.y, 4, 4};
                            }
                        }
                    }

                    /*----------------------------------------------------------------*/

                    //Retangulos do jogador e do inimigo
                    jogador.retJogador = (Rectangle){jogador.posicao.x, jogador.posicao.y, 33, 43};
                    inimigo.retInimigo = (Rectangle){inimigo.posicao.x, inimigo.posicao.y, 38, 44};

                    /*CHECKCOLLISION*/
                    int colPlayerTij; //Player
                    int colPlayerBordas; //Player
                    colPlayerTij = collisionPlayerTij(tijolos, jogador);
                    colPlayerBordas = collisionPlayerBordas(bordas, jogador);

                    if (colPlayerTij == TRUE || CheckCollisionRecs(inimigo.retCollision, jogador.retCollision) || colPlayerBordas == TRUE){
                        jogador.posicao.x = x;
                        jogador.posicao.y = y;
                    } //Jogador

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
                        || CheckCollisionRecs(bordas.tamanhoB[3], tiroDireita->retTiroDir)){
                            for (int i=0; i<MAXTIROS; i++)
                                tiroDireita[i].ativacao = FALSE;
                        } //Tiro pra Direita

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
                        || CheckCollisionRecs(bordas.tamanhoB[3], tiroEsquerda->retTiroEsq)){
                            for (int i=0; i<MAXTIROS; i++)
                                tiroEsquerda[i].ativacao = FALSE;
                        } //Tiro pra Esquerda

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
                        || CheckCollisionRecs(bordas.tamanhoB[3], tiroCima->retTiroCima)){
                            for (int i=0; i<MAXTIROS; i++)
                                tiroCima[i].ativacao = FALSE;
                        } //Tiro pra cima

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
                        || CheckCollisionRecs(bordas.tamanhoB[3], tiroBaixo->retTiroBaixo)){
                            for (int i=0; i<MAXTIROS; i++)
                                tiroBaixo[i].ativacao = FALSE;
                        } //Tiro pra baixo

                    /*LÓGICA PARA COLISÃO DO INIMIGO - INCOMPLETA*/
                    /*
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
                        || CheckCollisionRecs(bordas.tamanhoB[3], inimigo.retCollision)){
                        inimigo.posicao.x = (float)GetRandomValue(0,screenWidth);
                        inimigo.posicao.y = (float)GetRandomValue(40, screenHeight);
                    } // Inimigo
                    */

                    //Vidas
                    if (CheckCollisionRecs(inimigo.retInimigo, jogador.retJogador)) jogador.vidas = jogador.vidas - 1;
                    if (jogador.vidas == 2) corShield[2] = BLANK;
                    if (jogador.vidas == 1) corShield[1] = BLANK;
                    if (jogador.vidas == 0) corShield[0] = BLANK;

                    BeginDrawing();

                        ClearBackground (LIGHTGRAY);

                        menu(screenWidth); // Desenhando menu
                        DrawTexture(texturaShield, 10, 5, corShield[0]);
                        DrawTexture(texturaShield, 50, 5, corShield[1]);
                        DrawTexture(texturaShield, 90, 5, corShield[2]);

                        DrawTexturePro(texturaJogador, jogador.source, jogador.retJogador, jogador.origin, jogador.rotacao, LIGHTGRAY);
                        DrawTexturePro(texturaInimigo, inimigo.source, inimigo.retInimigo, inimigo.origin, inimigo.rotacao, LIGHTGRAY);

                        DrawRectangleRec(tiroDireita->retTiroDir, BLACK);
                        DrawRectangleRec(tiroEsquerda->retTiroEsq, BLACK);
                        DrawRectangleRec(tiroCima->retTiroCima, BLACK);
                        DrawRectangleRec(tiroBaixo->retTiroBaixo, BLACK);

                        blocosFase1 (texturaTijolos); //Chamada da função que carrega os blocos

                    EndDrawing();
                }

            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
