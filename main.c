#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

/*STRUCTS*/
typedef struct {
    Vector2 posicao;
    Rectangle retJogador;
} Player;

typedef struct {
    Vector2 posicao;
    Rectangle retInimigo;
} Enemy;

int main(void){
    int screenWidth = 1000;
    int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Battle INF");

    //Inimigo
        Enemy inimigo;
        inimigo.posicao = (Vector2){(float)GetRandomValue(0,screenWidth), (float)GetRandomValue(40, screenHeight)};

    //Mouse
        Vector2 ponteiroMouse = {0.0f, 0.0f};

    /*CORES*/
        Color corTexto[3] = {LIGHTGRAY, LIGHTGRAY, LIGHTGRAY}; //Opções do menu

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        /*UPDATE*/
            ponteiroMouse = GetMousePosition(); //Atualizando o ponteiro do mouse

        /*Draw*/
            BeginDrawing();

                ClearBackground(BLACK);
                DrawText("Battle INF", 360, 80, 50, LIGHTGRAY);

                Rectangle novoJogo = {415, 220, 147, 30};
                DrawText("Novo jogo", 415, 220, 30, corTexto[0]);

                Rectangle continuar = {415, 280, 147, 30};
                DrawText("Continuar", 415, 280, 30, corTexto[1]);

                Rectangle sair = {455, 340, 60, 30};
                DrawText("Sair", 455, 340, 30, corTexto[2]);

                if (CheckCollisionPointRec(ponteiroMouse, novoJogo)) corTexto[0] = GOLD;
                else corTexto[0] = LIGHTGRAY;
                if (CheckCollisionPointRec(ponteiroMouse, continuar)) corTexto[1] = GOLD;
                else corTexto[1] = LIGHTGRAY;
                if (CheckCollisionPointRec(ponteiroMouse, sair)) corTexto[2] = GOLD;
                else corTexto[2] = LIGHTGRAY;

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){

                    SetRandomSeed(time(NULL)); //Seed

                    /*Retangulos*/
                    Rectangle tijolos1 = {65, 230, 210, 55};
                    Rectangle tijolos2 = {275, 230, 55, 257.5};
                    Rectangle tijolos3 = {65, 432, 210, 55};
                    Rectangle tijolos4 = {650, 40, 100, 55};
                    Rectangle tijolos5 = {590, 95, 60, 55};
                    Rectangle tijolos6 = {550, 150, 130, 55};
                    Rectangle tijolos7 = {750, 290, 55, 55};

                    /*VETORES E POSICIONAMENTO*/
                    /*------------------------*/
                    //Jogador
                        Player jogador;
                        jogador.posicao = (Vector2){(float)screenWidth/2, (float)screenHeight/2};
                        float x; //Guarda valor de x
                        float y; //Guarda valor de y

                    //Inimigo
                        Enemy inimigo;
                        inimigo.posicao = (Vector2){(float)GetRandomValue(0,screenWidth), (float)GetRandomValue(40, screenHeight)};

                    //Bordas
                        Rectangle retanguloHeader  = {0,0, screenWidth, 40};
                        Rectangle retanguloLeft = {0, 0, 0, screenHeight};
                        Rectangle retanguloRight = {screenWidth, 0, 0, screenHeight};
                        Rectangle retanguloBottom = {0, screenHeight, screenWidth, 0};

                    //Jogador
                        Image ImgJogador = LoadImage("recursos/jogador.png");
                        Texture2D texturaJogador = LoadTextureFromImage(ImgJogador);

                    //Inimigo
                        Image ImgInimigo = LoadImage("recursos/inimigo.png");
                        Texture2D texturaInimigo = LoadTextureFromImage(ImgInimigo);

                    //Tijolos
                        Image ImgTijolos = LoadImage("recursos/tijolos.png");
                        Texture2D texturaTijolos = LoadTextureFromImage(ImgTijolos);

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
                        if (CheckCollisionRecs(tijolos1, jogador.retJogador)
                            || CheckCollisionRecs(tijolos2, jogador.retJogador)
                            || CheckCollisionRecs(tijolos3, jogador.retJogador)
                            || CheckCollisionRecs(tijolos4, jogador.retJogador)
                            || CheckCollisionRecs(tijolos5, jogador.retJogador)
                            || CheckCollisionRecs(tijolos6, jogador.retJogador)
                            || CheckCollisionRecs(tijolos7, jogador.retJogador)
                            || CheckCollisionRecs(retanguloHeader, jogador.retJogador)
                            || CheckCollisionRecs(inimigo.retInimigo, jogador.retJogador)
                            || CheckCollisionRecs(retanguloLeft, jogador.retJogador)
                            || CheckCollisionRecs(retanguloRight, jogador.retJogador)
                            || CheckCollisionRecs(retanguloBottom, jogador.retJogador)){
                            jogador.posicao.x = x;
                            jogador.posicao.y = y;
                        }

                        BeginDrawing();
                            ClearBackground (LIGHTGRAY);

                            DrawRectangle (0,0, screenWidth, 40, DARKGRAY); //Listra cinza escuro
                            DrawText("Fase 1", 442, 7, 30, GOLD);

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
                        EndDrawing();
                    }

            }

            EndDrawing();
    }

    CloseWindow();

    return 0;
}
