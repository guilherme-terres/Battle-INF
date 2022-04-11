#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define FALSE 0
#define TRUE 1

//Structs
typedef struct {
    Vector2 posicao;
    Rectangle retJogador;
    int vidas;
} Player; //Jogador

typedef struct {
    Vector2 posicao;
    Rectangle retInimigo;
} Enemy; //Inimigo

typedef struct {
    Vector2 posicao;
    Vector2 velocidade;
} Shoot; //Tiro

typedef struct {
    //Fase 1
    Rectangle tamanho[7];
    //Bordas do jogo
    Rectangle tamanhoB[4];

} Rectangles; //Retangulos


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

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                SetRandomSeed(time(NULL));

                //Retangulos da fase 1
                Rectangles tijolos;
                tijolos.tamanho[0] = (Rectangle){65, 230, 210, 55};
                tijolos.tamanho[1] = (Rectangle){275, 230, 55, 257.5};
                tijolos.tamanho[2] = (Rectangle){65, 432, 210, 55};
                tijolos.tamanho[3] = (Rectangle){650, 40, 100, 55};
                tijolos.tamanho[4] = (Rectangle){590, 95, 60, 55};
                tijolos.tamanho[5] = (Rectangle){550, 150, 130, 55};
                tijolos.tamanho[6] = (Rectangle){750, 290, 55, 55};

                //Cores
                Color corShield[3] = {WHITE, WHITE, WHITE};

                /*Vetores e posicionamento*/
                //Jogador
                Player jogador;
                jogador.posicao = (Vector2){(float)screenWidth/2, (float)screenHeight/2};
                jogador.vidas = 3;
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

                /*Texturas*/
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

                    if (IsKeyDown(KEY_RIGHT)) jogador.posicao.x += 3.0f;
                    if (IsKeyDown(KEY_LEFT)) jogador.posicao.x -= 3.0f;
                    if (IsKeyDown(KEY_UP)) jogador.posicao.y -= 3.0f;
                    if (IsKeyDown(KEY_DOWN)) jogador.posicao.y += 3.0f;

                    //Retangulos do jogador e do inimigo
                    jogador.retJogador = (Rectangle){jogador.posicao.x, jogador.posicao.y, 33, 43};
                    inimigo.retInimigo = (Rectangle){inimigo.posicao.x, inimigo.posicao.y, 38, 44};

                    //Check Collision
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

                    BeginDrawing();
                        ClearBackground (LIGHTGRAY);

                        DrawRectangle (0,0, screenWidth, 40, DARKGRAY); //Listra cinza escuro
                        DrawText("Fase 1", 442, 7, 30, GOLD);
                        DrawTexture(texturaShield, 10, 5, corShield[0]);
                        DrawTexture(texturaShield, 50, 5, corShield[1]);
                        DrawTexture(texturaShield, 90, 5, corShield[2]);

                        DrawTexture(texturaJogador, jogador.posicao.x, jogador.posicao.y, LIGHTGRAY);
                        DrawTexture(texturaInimigo, inimigo.posicao.x, inimigo.posicao.y, LIGHTGRAY);

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

                    if (CheckCollisionRecs(inimigo.retInimigo, jogador.retJogador)){
                        jogador.vidas = jogador.vidas - 1;
                    }
                    if (jogador.vidas == 2) corShield[2] = BLANK;
                    if (jogador.vidas == 1) corShield[1] = BLANK;
                    if (jogador.vidas == 0) corShield[0] = BLANK;

                    EndDrawing();
                }

            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
