#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define FALSE 0
#define TRUE 1
#define MAX_SHOOTS 3

//Structs
typedef struct player{
    Vector2 posicao;
    Vector2 origin;
    Rectangle source;
    Rectangle retJogador;
    float rotacao;
    int vidas;
} Player; //Jogador

typedef struct enemy{
    Vector2 posicao;
    Rectangle retInimigo;
} Enemy; //Inimigo

typedef struct shoot{
    Vector2 velocidade;
    Vector2 posicao;
    Color corTiro;
    int ativacao;
    int lifeSpawn;
    float raio;
} Shoot; //Tiro

typedef struct rectangles{
    //Fase 1
    Rectangle tamanho[7];
    //Bordas do jogo
    Rectangle tamanhoB[4];
} Rectangles; //Retangulos

//Funções
//Menu das fases
void menu (int screenWidth){
    DrawRectangle (0,0, screenWidth, 40, DARKGRAY); //Listra cinza escuro
    DrawText("Fase 1", 442, 7, 30, GOLD);
}

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
}

int main(void){
    int screenWidth = 1000;
    int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Battle INF");

    //Mouse
        Vector2 ponteiroMouse = {0.0f, 0.0f};

    //Cores
        Color corTexto[4] = {LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY}; //Opções do menu

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        //Update
        ponteiroMouse = GetMousePosition(); //Atualizando o ponteiro do mouse

        //Draw
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
            else corTexto[0] = LIGHTGRAY;
            if (CheckCollisionPointRec(ponteiroMouse, continuar)) corTexto[1] = GOLD;
            else corTexto[1] = LIGHTGRAY;
            if (CheckCollisionPointRec(ponteiroMouse, ranking)) corTexto[2] = GOLD;
            else corTexto[2] = LIGHTGRAY;
            if (CheckCollisionPointRec(ponteiroMouse, sair)) corTexto[3] = GOLD;
            else corTexto[3] = LIGHTGRAY;

            /*------------------------------------------*/

            if (CheckCollisionPointRec(ponteiroMouse, novoJogo) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                SetRandomSeed(time(NULL));

                //Retangulos da fase 1
                Rectangles tijolos;
                tijolos.tamanho[0] = (Rectangle){65, 230, 210, 55};
                tijolos.tamanho[1] = (Rectangle){275, 230, 55, 257.5};
                tijolos.tamanho[2] = (Rectangle){65, 432, 210, 55};
                tijolos.tamanho[3] = (Rectangle){650, 40, 145, 55};
                tijolos.tamanho[4] = (Rectangle){590, 95, 206, 55};
                tijolos.tamanho[5] = (Rectangle){550, 150, 173.2, 55};
                tijolos.tamanho[6] = (Rectangle){750, 290, 55, 55};

                //Cores
                Color corShield[3] = {WHITE, WHITE, WHITE};

                /*Vetores e posicionamento*/
                //Jogador
                Player jogador;
                jogador.posicao = (Vector2){(float)screenWidth/2, (float)screenHeight/2};
                jogador.origin = (Vector2){16.5, 21.5};
                jogador.source = (Rectangle){(float)screenWidth/2 + 28, (float)screenHeight/2 + 1, 34, 44};
                jogador.vidas = 3;
                jogador.rotacao = 0.0;
                float x;
                float y;

                //Inimigo
                Enemy inimigo;
                inimigo.posicao = (Vector2){(float)GetRandomValue(0,screenWidth), (float)GetRandomValue(40, screenHeight)};

                //Bordas
                Rectangles bordas;
                bordas.tamanhoB[0] = (Rectangle){0,0, screenWidth, 40}; //Header
                bordas.tamanhoB[1] = (Rectangle){0, 0, 0, screenHeight}; //Esquerda
                bordas.tamanhoB[2] = (Rectangle){screenWidth, 0, 0, screenHeight}; //Direita
                bordas.tamanhoB[3] = (Rectangle){0, screenHeight, screenWidth, 0}; //Baixo

                //Tiro
                Shoot tiro[MAX_SHOOTS] = {0};
                int tiroCima;

                //Texturas
                //Jogador
                Image ImgJogador = LoadImage("recursos/jogador.png");
                Texture2D texturaJogador = LoadTextureFromImage(ImgJogador);

                //Inimigo
                Image ImgInimigo = LoadImage("recursos/inimigo.png");
                Texture2D texturaInimigo = LoadTextureFromImage(ImgInimigo);

                //Tijolos
                Image ImgTijolos = LoadImage("recursos/tijolos.png");
                Texture2D texturaTijolos = LoadTextureFromImage(ImgTijolos);

                //Shield
                Image ImgShield = LoadImage("recursos/shield1.png");
                Texture2D texturaShield = LoadTextureFromImage(ImgShield);

                while (!WindowShouldClose()){
                    /*Update*/
                    x = jogador.posicao.x;
                    y = jogador.posicao.y;

                    //Movimentação do jogador
                    if (IsKeyDown(KEY_RIGHT)){
                        jogador.posicao.x += 3.0f;
                        jogador.rotacao = 90;
                        jogador.retJogador = (Rectangle){jogador.posicao.x - 23, jogador.posicao.y - 16.5, 44, 33};
                    }
                    if (IsKeyDown(KEY_LEFT)){
                        jogador.posicao.x -= 3.0f;
                        jogador.rotacao = -90;
                        jogador.retJogador = (Rectangle){jogador.posicao.x - 23, jogador.posicao.y - 16.5, 44, 33};
                    }
                    if (IsKeyDown(KEY_UP)){
                        jogador.posicao.y -= 3.0f;
                        jogador.rotacao = 0;
                        jogador.retJogador = (Rectangle){jogador.posicao.x - 16.5, jogador.posicao.y - 21.5, 33, 44};
                    }
                    if (IsKeyDown(KEY_DOWN)){
                        jogador.posicao.y += 3.0f;
                        jogador.rotacao = 180;
                        jogador.retJogador = (Rectangle){jogador.posicao.x - 16.5, jogador.posicao.y - 21.5, 33, 44};
                    }

                    //Retangulos do jogador e do inimigo
                    jogador.retJogador = (Rectangle){jogador.posicao.x, jogador.posicao.y, 33, 44};
                    inimigo.retInimigo = (Rectangle){inimigo.posicao.x, inimigo.posicao.y, 38, 44};

                    //Check Collision
                    //Jogador
                    if (CheckCollisionRecs(tijolos.tamanho[0], jogador.retJogador)
                        || CheckCollisionRecs(tijolos.tamanho[1], jogador.retJogador)
                        || CheckCollisionRecs(tijolos.tamanho[2], jogador.retJogador)
                        || CheckCollisionRecs(tijolos.tamanho[3], jogador.retJogador)
                        || CheckCollisionRecs(tijolos.tamanho[4], jogador.retJogador)
                        || CheckCollisionRecs(tijolos.tamanho[5], jogador.retJogador)
                        || CheckCollisionRecs(tijolos.tamanho[6], jogador.retJogador)
                        || CheckCollisionRecs(inimigo.retInimigo, jogador.retJogador)
                        || CheckCollisionRecs(bordas.tamanhoB[0], jogador.retJogador)
                        || CheckCollisionRecs(bordas.tamanhoB[1], jogador.retJogador)
                        || CheckCollisionRecs(bordas.tamanhoB[2], jogador.retJogador)
                        || CheckCollisionRecs(bordas.tamanhoB[3], jogador.retJogador)){
                        jogador.posicao.x = x;
                        jogador.posicao.y = y;
                    }

                    //Inimigo
                    if (CheckCollisionRecs(tijolos.tamanho[0], inimigo.retInimigo)
                        || CheckCollisionRecs(tijolos.tamanho[1], inimigo.retInimigo)
                        || CheckCollisionRecs(tijolos.tamanho[2], inimigo.retInimigo)
                        || CheckCollisionRecs(tijolos.tamanho[3], inimigo.retInimigo)
                        || CheckCollisionRecs(tijolos.tamanho[4], inimigo.retInimigo)
                        || CheckCollisionRecs(tijolos.tamanho[5], inimigo.retInimigo)
                        || CheckCollisionRecs(tijolos.tamanho[6], inimigo.retInimigo)
                        || CheckCollisionRecs(inimigo.retInimigo, jogador.retJogador)
                        || CheckCollisionRecs(bordas.tamanhoB[0], inimigo.retInimigo)
                        || CheckCollisionRecs(bordas.tamanhoB[1], inimigo.retInimigo)
                        || CheckCollisionRecs(bordas.tamanhoB[2], inimigo.retInimigo)
                        || CheckCollisionRecs(bordas.tamanhoB[3], inimigo.retInimigo)){
                        inimigo.posicao.x = (float)GetRandomValue(0,screenWidth);
                        inimigo.posicao.y = (float)GetRandomValue(40, screenHeight);
                    }

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
                        DrawTexture(texturaInimigo, inimigo.posicao.x, inimigo.posicao.y, LIGHTGRAY);

                        blocosFase1 (texturaTijolos);



                    EndDrawing();
                }

            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
