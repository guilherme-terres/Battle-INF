#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

//Fase 1
            /*
            ClearBackground (LIGHTGRAY);
            DrawRectangle (0,0, screenWidth, 40, DARKGRAY);
            DrawText("Fase 1", 442, 7, 30, GOLD);
            DrawTexture(texturaJogador, x, y, LIGHTGRAY);

            for (int i=65; i<=275; i += 30)
                DrawTexture(texturaTijolos, i, 230, LIGHTGRAY);
            for (float i=285.5; i<=432; i += 55.2)
                DrawTexture(texturaTijolos, 275, i, LIGHTGRAY);
            for (int i=275; i>=65; i-=30)
                DrawTexture(texturaTijolos, i, 432, LIGHTGRAY);
            */

int main(void){
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Battle INF");
    Vector2 ponteiroMouse = {0.0f, 0.0f};
    Vector2 player_position = {(float)screenWidth/2, (float)screenHeight/2};

    /*Variáveis de cor do menu inicial*/
    Color corTexto1 = LIGHTGRAY;
    Color corTexto2 = LIGHTGRAY;
    Color corTexto3 = LIGHTGRAY;

    float rotation = 0.0f;

    /*Imagens*/
    Image jogador = LoadImage("recursos/jogador.png");
    Texture2D texturaJogador = LoadTextureFromImage(jogador);

    float x = screenWidth/2;
    float y = screenHeight/2;

    Image tijolos = LoadImage("recursos/tijolos.png");
    Texture2D texturaTijolos = LoadTextureFromImage(tijolos);

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        //Update

        ponteiroMouse = GetMousePosition();

        if (IsKeyDown(KEY_RIGHT)){
            x += 3.0f;
        }
        if (IsKeyDown(KEY_LEFT)) {
            x -= 3.0f;
        }
        if (IsKeyDown(KEY_UP)){
            y -= 3.0f;
        }
        if (IsKeyDown(KEY_DOWN)){
            y += 3.0f;
        }

        //Draw
        BeginDrawing();
            ClearBackground(BLACK);

            /*Menu Inicial*/

            DrawText("Battle INF", 360, 80, 50, LIGHTGRAY);

            Rectangle novoJogo = {415, 220, 147, 30};
            DrawText("Novo jogo", 415, 220, 30, corTexto1);
            /*-------------------------------------------*/

            Rectangle continuar = {415, 280, 147, 30,};
            DrawText("Continuar", 415, 280, 30, corTexto2);
            /*-------------------------------------------*/

            Rectangle sair = {455, 340, 60, 30};
            DrawText("Sair", 455, 340, 30, corTexto3);

            if (CheckCollisionPointRec(ponteiroMouse, novoJogo)) corTexto1 = GOLD;
            else corTexto1 = LIGHTGRAY;

            if (CheckCollisionPointRec(ponteiroMouse, continuar)) corTexto2 = GOLD;
            else corTexto2 = LIGHTGRAY;

            if (CheckCollisionPointRec(ponteiroMouse, sair)) corTexto3 = GOLD;
            else corTexto3 = LIGHTGRAY;

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
