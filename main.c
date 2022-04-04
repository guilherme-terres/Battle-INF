#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

//Menu inicial
/*
    ClearBackground(BLACK);

    DrawText("Battle INF", 360, 80, 50, LIGHTGRAY);

    Rectangle novoJogo = {415, 220, 147, 30};
    DrawText("Novo jogo", 415, 220, 30, corTexto1);

    Rectangle continuar = {415, 280, 147, 30,};
    DrawText("Continuar", 415, 280, 30, corTexto2);

    Rectangle sair = {455, 340, 60, 30};
    DrawText("Sair", 455, 340, 30, corTexto3);

    if (CheckCollisionPointRec(ponteiroMouse, novoJogo)) corTexto1 = GOLD;
    else corTexto1 = LIGHTGRAY;

    if (CheckCollisionPointRec(ponteiroMouse, continuar)) corTexto2 = GOLD;
    else corTexto2 = LIGHTGRAY;

    if (CheckCollisionPointRec(ponteiroMouse, sair)) corTexto3 = GOLD;
    else corTexto3 = LIGHTGRAY;
*/

int main(void){
    /*LARGURA E ALTURA DA TELA*/
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Battle INF");

    /*VETORES E POSICIONAMENTO*/
    Vector2 ponteiroMouse = {0.0f, 0.0f}; //Vetor Ponteiro do Mouse
    Vector2 player_position = {(float)screenWidth/2, (float)screenHeight/2}; //Posição do jogador
    float x = player_position.x; //Guardando a componente X da posição do jogador
    float y = player_position.y; //Guardando a componente Y da posição do jogador

    /*Variáveis de cor*/
    Color corTexto1 = LIGHTGRAY; //Menu inicial
    Color corTexto2 = LIGHTGRAY; //Menu inicial
    Color corTexto3 = LIGHTGRAY; //Menu inicial

    /*Imagens e Texturas*/
    //Jogador
    Image jogador = LoadImage("recursos/jogador.png");
    Texture2D texturaJogador = LoadTextureFromImage(jogador);

    //Tijolos
    Image tijolos = LoadImage("recursos/tijolos.png");
    Texture2D texturaTijolos = LoadTextureFromImage(tijolos);

    SetTargetFPS(60); //Setando FPS

    while (!WindowShouldClose()){
        //Update
        ponteiroMouse = GetMousePosition(); //Atualizando o ponteiro do mouse

        if (IsKeyDown(KEY_RIGHT)) player_position.x += 3.0f; //Alterando a posição com as teclas
        if (IsKeyDown(KEY_LEFT)) player_position.x -= 3.0f; //Alterando a posição com as teclas
        if (IsKeyDown(KEY_UP)) player_position.y -= 3.0f; //Alterando a posição com as teclas
        if (IsKeyDown(KEY_DOWN)) player_position.y += 3.0f; //Alterando a posição com as teclas

        //Draw
        BeginDrawing();

            ClearBackground (LIGHTGRAY);

            DrawRectangle (0,0, screenWidth, 40, DARKGRAY); //Listra cinza escuro
            DrawText("Fase 1", 442, 7, 30, GOLD);

            Rectangle retanguloJogador = {player_position.x, player_position.y, 42, 47};
            DrawTexture(texturaJogador, player_position.x, player_position.y, LIGHTGRAY);

            Rectangle tijolos1 = {65, 230, 180, 55};
            for (int i=65; i<=275; i += 30)
                DrawTexture(texturaTijolos, i, 230, LIGHTGRAY);

            Rectangle tijolos2 = {245, 230, 180, 55};
            for (float i=285.5; i<=432; i += 55.2)
                DrawTexture(texturaTijolos, 275, i, LIGHTGRAY);

            Rectangle tijolos3 = {65, 432, 180, 55};
            for (int i=275; i>=65; i-=30)
                DrawTexture(texturaTijolos, i, 432, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
