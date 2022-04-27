#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

/*----- DEFINI��ES -----*/
#define FALSE 0
#define TRUE 1
#define MAXTIROS 10

/*----- GLOBAIS -----*/
int randomMove; //MOVIMENTA��O RAND�MICA DO INIMIGO

Image ImgInimigo;
Texture2D texturaInimigo; //TEXTURA DO INIMIGO

Image ImgEnergy;
Texture2D texturaEnergia; //TEXTURA DA ENERGIA

/*----- STRUCTS -----*/
typedef struct shoot{
    Vector2 posicao;
    Color cor;
    int ativacao;
    float raio;
    float sentido;
    float velocidade;
} Shoot; //TIRO

typedef struct player{
    Vector2 posicao;
    Vector2 origin;
    Vector2 posicao_anterior;
    Rectangle source;
    Rectangle retJogador;
    Rectangle retCollision;
    Shoot tiro;
    int vidas;
    int pontos;
    float rotacao;
    float sentido;
    int kills;
} Player; //JOGADOR

typedef struct enemy{
    Vector2 posicao;
    Vector2 origin;
    Vector2 posicao_anterior;
    Rectangle source;
    Rectangle retInimigo;
    Rectangle retCollision;
    Shoot tiro;
    float sentido;
    float rotacao;
    int ativacao;
} Enemy; //INIMIGO

typedef struct energy{
    Vector2 posicao;
    Rectangle retEnergia;
    int ativacao;
} Energy; //ENERGIA

typedef struct rectangles{
    Rectangle tamanho[7];
    Rectangle tamanho2[5];
    Rectangle tamanhoB[4];
} Rectangles; //RET�NGULOS

/*===========================*/

/*-----FUN��ES-----*/
void menu (int screenWidth){
    DrawRectangle (0,0, screenWidth, 40, DARKGRAY); //LISTRA CINZA ESCURO
    DrawText("Fase 1", 442, 7, 30, GOLD);
} //MENU DA FASE 1 - DESENHO

void menu2 (int screenWidth){
    DrawRectangle (0,0, screenWidth, 40, DARKGRAY); //LISTRA CINZA ESCURO
    DrawText("Fase 2", 442, 7, 30, GOLD);
} //MENU DA FASE 2 - DESENHO

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
} //BLOCOS DA FASE 1 - DESENHO

void blocosFase2 (Texture2D texturaTijolos){
    for (int j=230; j<=400; j+=55){
        for (int i=65; i<=275; i += 30)
            DrawTexture(texturaTijolos, i, j, LIGHTGRAY);
    }
    for (int i=300; i<=650; i+=30)
        DrawTexture(texturaTijolos, i, 110, LIGHTGRAY);
    for (int i=650; i<=800; i+=30)
        DrawTexture(texturaTijolos, i, 300, LIGHTGRAY);
    for (int i=700; i<=850; i+=30)
        DrawTexture(texturaTijolos, i, 355, LIGHTGRAY);
    for (int i=600; i<=800; i+=30)
        DrawTexture(texturaTijolos, i, 410, LIGHTGRAY);
} //BLOCOS DA FASE 2 - DESENHO

void iniciaRets1 (Rectangles *tijolos){
    tijolos->tamanho[0] = (Rectangle){65, 230, 210, 55};
    tijolos->tamanho[1] = (Rectangle){275, 230, 55, 257.5};
    tijolos->tamanho[2] = (Rectangle){65, 432, 210, 55};
    tijolos->tamanho[3] = (Rectangle){650, 40, 145, 55};
    tijolos->tamanho[4] = (Rectangle){590, 95, 206, 55};
    tijolos->tamanho[5] = (Rectangle){550, 150, 173.2, 55};
    tijolos->tamanho[6] = (Rectangle){750, 290, 55, 55};
} //INICIALIZA��O DOS RET�NGULOS DA FASE 1

void iniciaRets2 (Rectangles *tijolos2){
    tijolos2->tamanho2[0] = (Rectangle){60, 229.5, 272, 223};
    tijolos2->tamanho2[1] = (Rectangle){298, 108, 388, 57.5};
    tijolos2->tamanho2[2] = (Rectangle){650, 298, 207, 57.5};
    tijolos2->tamanho2[3] = (Rectangle){698, 355, 208.5, 55};
    tijolos2->tamanho2[4] = (Rectangle){598, 408, 238, 57.5};
} //INICIALIZA��O DOS RET�NGULOS DA FASE 2

void iniciaBordas (Rectangles *bordas, int screenWidth, int screenHeight){
    bordas->tamanhoB[0] = (Rectangle){0,0, screenWidth, 40}; //Header
    bordas->tamanhoB[1] = (Rectangle){0, 0, 1, screenHeight}; //Esquerda
    bordas->tamanhoB[2] = (Rectangle){screenWidth - 1, 0, 1, screenHeight}; //Direita
    bordas->tamanhoB[3] = (Rectangle){0, screenHeight, screenWidth, 1}; //Baixo
} //INICIALIZA��O DAS BORDAS

void inicializaJogador (Player *jogador, int screenWidth, int screenHeight){
    jogador->posicao = (Vector2){(float)screenWidth/2, (float)screenHeight/2};
    jogador->origin = (Vector2){16.5, 21.5};
    jogador->source = (Rectangle){(float)screenWidth/2 + 28, (float)screenHeight/2 + 1, 33, 43};
    jogador->vidas = 3;
    jogador->rotacao = 0.0;
    jogador->pontos = 0;
    jogador->sentido = M_PI;
    jogador->kills = 0;
} //INICIALIZA��O DA MOVIMENTA��O DO JOGADOR

void inicializaInimigo (Enemy *inimigo, int screenWidth, int screenHeight){
    inimigo->origin = (Vector2){19, 22};
    inimigo->source = (Rectangle){(float)screenWidth/2 + 32, (float)screenHeight/2 + 8, 38, 44};
    inimigo->retCollision = (Rectangle){inimigo->posicao.x  -19, inimigo->posicao.y -22, 38, 44};
    inimigo->sentido = 0.0;
    inimigo->ativacao = FALSE;
} //INICIALIZA��O DA MOVIMENTA��O DO INIMIGO

void inicializaTiroJ (Player *jogador){
    jogador->tiro.posicao = (Vector2){jogador->posicao.x, jogador->posicao.y};
    jogador->tiro.raio = 2.7;
    jogador->tiro.sentido = 0.0;
    jogador->tiro.cor = BLACK;
    jogador->tiro.velocidade = 5.7;
    jogador->tiro.ativacao = FALSE;
} ////INICIALIZA��O DO TIRO DO JOGADOR

/*COLIS�ES*/
int collisionPlayer (Rectangles tijolos, Rectangles bordas, Player jogador, Enemy inimigo){
    int retorno;

    if (CheckCollisionRecs(tijolos.tamanho[0], jogador.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[1], jogador.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[2], jogador.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[3], jogador.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[4], jogador.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[5], jogador.retCollision)
    || CheckCollisionRecs(tijolos.tamanho[6], jogador.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[0], jogador.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[1], jogador.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[2], jogador.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[3], jogador.retCollision)
    || CheckCollisionRecs(inimigo.retCollision, jogador.retCollision))
        retorno = TRUE;
    else retorno = FALSE;

    return retorno;
} // COLIS�O DO PLAYER DA FASE 1

int collisionPlayer2 (Rectangles tijolos2, Rectangles bordas, Player jogador, Enemy inimigo){
    int retorno;

    if (CheckCollisionRecs(tijolos2.tamanho2[0], jogador.retCollision)
    || CheckCollisionRecs(tijolos2.tamanho2[1], jogador.retCollision)
    || CheckCollisionRecs(tijolos2.tamanho2[2], jogador.retCollision)
    || CheckCollisionRecs(tijolos2.tamanho2[3], jogador.retCollision)
    || CheckCollisionRecs(tijolos2.tamanho2[4], jogador.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[0], jogador.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[1], jogador.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[2], jogador.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[3], jogador.retCollision)
    || CheckCollisionRecs(inimigo.retCollision, jogador.retCollision))
        retorno = TRUE;
    else retorno = FALSE;

    return retorno;
} // COLIS�O DO PLAYER DA FASE 2

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
} //COLIS�O DO INIMIGO DA FASE 1

int collisionInimigo2 (Rectangles tijolos2, Rectangles bordas, Enemy inimigo, Player jogador){
    int retorno = 0;

    if (CheckCollisionRecs(tijolos2.tamanho2[0], inimigo.retCollision)
    || CheckCollisionRecs(tijolos2.tamanho2[1], inimigo.retCollision)
    || CheckCollisionRecs(tijolos2.tamanho2[2], inimigo.retCollision)
    || CheckCollisionRecs(tijolos2.tamanho2[3], inimigo.retCollision)
    || CheckCollisionRecs(tijolos2.tamanho2[4], inimigo.retCollision)
    || CheckCollisionRecs(tijolos2.tamanho2[5], inimigo.retCollision)
    || CheckCollisionRecs(tijolos2.tamanho2[6], inimigo.retCollision)
    || CheckCollisionRecs(inimigo.retCollision, jogador.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[0], inimigo.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[1], inimigo.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[2], inimigo.retCollision)
    || CheckCollisionRecs(bordas.tamanhoB[3], inimigo.retCollision))
        retorno = 1;

    return retorno;
} //COLIS�O DO INIMIGO DA FASE 1

int collisionTiro (Rectangles tijolos, Rectangles bordas, Shoot *tiro){
    int retorno = 0;

    if (CheckCollisionPointRec(tiro->posicao, tijolos.tamanho[0])
    || CheckCollisionPointRec(tiro->posicao, tijolos.tamanho[1])
    || CheckCollisionPointRec(tiro->posicao, tijolos.tamanho[2])
    || CheckCollisionPointRec(tiro->posicao, tijolos.tamanho[3])
    || CheckCollisionPointRec(tiro->posicao, tijolos.tamanho[4])
    || CheckCollisionPointRec(tiro->posicao, tijolos.tamanho[5])
    || CheckCollisionPointRec(tiro->posicao, tijolos.tamanho[6])
    || CheckCollisionPointRec(tiro->posicao, bordas.tamanhoB[0])
    || CheckCollisionPointRec(tiro->posicao, bordas.tamanhoB[1])
    || CheckCollisionPointRec(tiro->posicao, bordas.tamanhoB[2])
    || CheckCollisionPointRec(tiro->posicao, bordas.tamanhoB[3]))
        retorno = 1;

    return retorno;
} //COLIS�O DO TIRO COM OS TIJOLOS E COM AS BORDAS NA FASE 1

int collisionTiro2 (Rectangles tijolos2, Rectangles bordas, Shoot *tiro){
    int retorno = 0;

    if (CheckCollisionPointRec(tiro->posicao, tijolos2.tamanho2[0])
    || CheckCollisionPointRec(tiro->posicao, tijolos2.tamanho2[1])
    || CheckCollisionPointRec(tiro->posicao, tijolos2.tamanho2[2])
    || CheckCollisionPointRec(tiro->posicao, tijolos2.tamanho2[3])
    || CheckCollisionPointRec(tiro->posicao, tijolos2.tamanho2[4])
    || CheckCollisionPointRec(tiro->posicao, bordas.tamanhoB[0])
    || CheckCollisionPointRec(tiro->posicao, bordas.tamanhoB[1])
    || CheckCollisionPointRec(tiro->posicao, bordas.tamanhoB[2])
    || CheckCollisionPointRec(tiro->posicao, bordas.tamanhoB[3]))
        retorno = 1;

    return retorno;
} //COLIS�O DO TIRO COM OS TIJOLOS E COM AS BORDAS NA FASE 2

int collisionVidas (Shoot *tiro, Player jogador){
    int retorno = 0;

    if (CheckCollisionPointRec(tiro->posicao, jogador.retCollision))
        retorno = 1;

    return retorno;
} //COLIS�O PARA PERDER VIDAS

int collisionPontos (Shoot *tiro, Enemy inimigo){
    int retorno = 0;

    if (CheckCollisionPointRec(tiro->posicao, inimigo.retCollision))
        retorno = 1;

    return retorno;
} //COLIS�O PARA MATAR INIMIGO E GANHAR PONTOS

void atualizaInimigo (Enemy *inimigo){
    if (randomMove >= 1 && randomMove <= 4){
        inimigo->posicao.x += 3.0f;
        inimigo->sentido = 90;
        inimigo->retCollision = (Rectangle){inimigo->posicao.x - 22, inimigo->posicao.y - 19, 44, 38};
    }
    if (randomMove >= 5 && randomMove <= 8){
        inimigo->posicao.x -= 3.0f;
        inimigo->sentido = -90;
        inimigo->retCollision = (Rectangle){inimigo->posicao.x -22, inimigo->posicao.y - 19, 44, 38};
    }
    if (randomMove >=9 && randomMove <= 12){
        inimigo->posicao.y -= 3.0f;
        inimigo->sentido = 0;
        inimigo->retCollision = (Rectangle){inimigo->posicao.x -19, inimigo->posicao.y -22, 38, 44};
        inimigo->rotacao = 0;
    }
    if (randomMove >= 13 && randomMove <= 16){
        inimigo->posicao.y += 3.0f;
        inimigo->sentido = 180;
        inimigo->retCollision = (Rectangle){inimigo->posicao.x -19, inimigo->posicao.y -22, 38, 44};
    }
} //MOVIMENTA��O ALEAT�RIA DO INIMIGO

void desenhoInimigo (Enemy *inimigo, Texture2D texturaInimigo, Color corInimigo){
    DrawTexturePro(texturaInimigo, inimigo->source, inimigo->retInimigo, inimigo->origin, inimigo->sentido, corInimigo);
} //DESENHO DO INIMIGO

void tanqueAtirar (Shoot *tiro, float sentido, Vector2 posicao, float multiplicador){
    tiro->ativacao = TRUE;
    tiro->sentido = sentido;
    tiro->posicao = posicao;
    tiro->velocidade = 5.7*multiplicador;
    tiro->cor = BLACK;
    tiro->raio = 2.7;
} //INICIALIZA��O DO TIRO DO TANQUE

void atualizaTiro (Shoot *tiro){
    tiro->posicao.x += tiro->velocidade*sin(tiro->sentido);
    tiro->posicao.y += tiro->velocidade*cos(tiro->sentido);
} //ATUALIZA��O DA POSI��O DO TIRO

void desenhaTiro (Shoot *tiro){
    DrawCircleV(tiro->posicao, tiro->raio, tiro->cor);
} //DESENHA O TIRO

void desenhaEnergia (Texture2D texturaEnergia, Energy energia){
    DrawTexture(texturaEnergia, energia.posicao.x, energia.posicao.y, LIGHTGRAY);
} //DESENHA A ENERGIA

int collisionEnergia (Player jogador, Energy energia){
    int retorno = 0;

    if (CheckCollisionRecs(jogador.retCollision, energia.retEnergia))
        retorno = 1;

    return retorno;
} //TESTA A COLIS�O DA ENERGIA COM O PLAYER

double spawnInimigo (double tempoInimigo, Enemy *inimigo, Rectangles tijolos, Rectangles bordas, Player jogador){
    if (GetTime() > tempoInimigo + 5 && inimigo->ativacao == FALSE){
        tempoInimigo = GetTime();
        inimigo->ativacao = TRUE;
        do {
            inimigo->posicao = (Vector2){GetRandomValue(60, 950), GetRandomValue(80, 760)};
            inimigo->retCollision = (Rectangle){inimigo->posicao.x  -19, inimigo->posicao.y -22, 38, 44};
        } while (collisionInimigo(tijolos, bordas, *inimigo, jogador) == 1);
    }

    if (inimigo->ativacao == TRUE)
        tempoInimigo = GetTime();

    return tempoInimigo;
} //SPAWNA O INIMIGO A CADA 5s

int main(void){
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Battle INF");
    //MOUSE
    Vector2 ponteiroMouse = {0.0f, 0.0f};
    //CORES
    Color corTexto[4] = {LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY}; //Op��es do menu

    SetTargetFPS(60); //FPS

    while (!WindowShouldClose()){
        /*----- UPDATE -----*/
        ponteiroMouse = GetMousePosition(); //ATUALIZANDO O PONTEIRO DO MOUSE

        /*----- DRAW ------*/
        BeginDrawing(); //MENU INICIAL

            ClearBackground(BLACK);
            DrawText("Battle INF", 360, 80, 50, LIGHTGRAY);

            Rectangle novoJogo = {415, 220, 147, 30};
            DrawText("Novo jogo", 415, 220, 30, corTexto[0]);

            Rectangle continuar = {415, 280, 147, 30};
            DrawText("Continuar", 415, 280, 30, corTexto[1]);

            Rectangle ranking = {425, 340, 123, 30};
            DrawText("Ranking", 425, 340, 30, corTexto[2]);

            Rectangle mapas = {450, 400, 60, 30};
            DrawText("Sair", 450, 400, 30, corTexto[3]);

            if (CheckCollisionPointRec(ponteiroMouse, novoJogo)) corTexto[0] = GOLD;
            else corTexto[0] = LIGHTGRAY; //MUDAN�A DE COR AO PASSAR O MOUSE POR CIMA
            if (CheckCollisionPointRec(ponteiroMouse, continuar)) corTexto[1] = GOLD;
            else corTexto[1] = LIGHTGRAY; //MUDAN�A DE COR AO PASSAR O MOUSE POR CIMA
            if (CheckCollisionPointRec(ponteiroMouse, ranking)) corTexto[2] = GOLD;
            else corTexto[2] = LIGHTGRAY; //MUDAN�A DE COR AO PASSAR O MOUSE POR CIMA
            if (CheckCollisionPointRec(ponteiroMouse, mapas)) corTexto[3] = GOLD;
            else corTexto[3] = LIGHTGRAY; //MUDAN�A DE COR AO PASSAR O MOUSE POR CIMA

            if (CheckCollisionPointRec(ponteiroMouse, novoJogo) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){ //CLICAR EM NOVO JOGO E CHAMAR A FASE 1
                //CORES
                Color corShield[3] = {WHITE, WHITE, WHITE};

                //TEXTURAS
                Image ImgJogador = LoadImage("recursos/jogador.png");
                Texture2D texturaJogador = LoadTextureFromImage(ImgJogador); //JOGADOR
                ImgInimigo = LoadImage("recursos/inimigo.png");
                texturaInimigo = LoadTextureFromImage(ImgInimigo); //INIMIGO
                Image ImgTijolos = LoadImage("recursos/tijolos.png");
                Texture2D texturaTijolos = LoadTextureFromImage(ImgTijolos); //TIJOLOS
                Image ImgShield = LoadImage("recursos/shield1.png");
                Texture2D texturaShield = LoadTextureFromImage(ImgShield); //SHIELD
                ImgEnergy = LoadImage("recursos/energy_drop1.png");
                texturaEnergia = LoadTextureFromImage(ImgEnergy); //ENERGIA

                //RET�NGULOS DA FASE 1 - INICIALIZA��O
                Rectangles tijolos;
                iniciaRets1 (&tijolos); //TIJOLOS

                Rectangles bordas;
                iniciaBordas(&bordas, screenWidth, screenHeight); //BORDAS

                //POSICIONAMENTO - INICIALIZA��O
                Player jogador;
                inicializaJogador(&jogador, screenWidth, screenHeight); //JOGADOR

                double tempoInimigo = GetTime();
                Enemy inimigo;
                inicializaInimigo(&inimigo, screenWidth, screenHeight); //INIMIGO

                inicializaTiroJ(&jogador); //TIRO DO JOGADOR

                Energy energia;
                energia.ativacao = FALSE;
                float multiplicador = 1; //ENERGIA

                while (!WindowShouldClose() && jogador.vidas != 0 && jogador.kills != 1){
                    SetRandomSeed(time(NULL)); //SEED RAND�MICA

                    int posXenergia = GetRandomValue(60, 940);
                    int posYenergia = GetRandomValue(60, 740);
                    int tiroInimigo;
                    randomMove = GetRandomValue(1,16); //VARI�VEIS QUE ASSUMEM VALORES RAND�MICOS

                    //CORES
                    Color corInimigo = LIGHTGRAY;

                    /*----- GERA��O DA ENERGIA -----*/
                    if (energia.ativacao == FALSE){
                        if (!GetRandomValue(0,16)){
                            energia.ativacao = TRUE;
                            energia.posicao = (Vector2){posXenergia, posYenergia};
                            energia.retEnergia = (Rectangle){posXenergia, posYenergia, 26, 31};
                            multiplicador = 1;
                        }
                    }
                    if (collisionEnergia(jogador, energia)){
                        energia.ativacao = FALSE;
                        energia.retEnergia = (Rectangle){posXenergia, posYenergia, 0, 0};
                        multiplicador = 1.5;
                    }

                    /*===========================*/

                    /*------ MOVIMENTA��O DO JOGADOR -----*/
                    jogador.posicao_anterior = jogador.posicao;
                    jogador.retJogador = (Rectangle){jogador.posicao.x + 5, jogador.posicao.y + 16.5, 33, 44}; //GUARDA POSI��O

                    if (IsKeyDown(KEY_RIGHT)){
                        jogador.posicao.x += 3.0f*multiplicador;
                        jogador.rotacao = 90;
                        jogador.retCollision = (Rectangle){jogador.posicao.x - 23, jogador.posicao.y - 16.5, 43, 33};
                        jogador.sentido = M_PI/2;
                    }
                    if (IsKeyDown(KEY_LEFT)){
                        jogador.posicao.x -= 3.0f*multiplicador;
                        jogador.rotacao = -90;
                        jogador.retCollision = (Rectangle){jogador.posicao.x - 23, jogador.posicao.y - 16.5, 43, 33};
                        jogador.sentido = (-1*M_PI)/2;
                    }
                    if (IsKeyDown(KEY_UP)){
                        jogador.posicao.y -= 3.0f*multiplicador;
                        jogador.rotacao = 0;
                        jogador.retCollision = (Rectangle){jogador.posicao.x - 16.5, jogador.posicao.y - 21.5, 33, 43};
                        jogador.sentido = M_PI;
                    }
                    if (IsKeyDown(KEY_DOWN)){
                        jogador.posicao.y += 3.0f*multiplicador;
                        jogador.rotacao = 180;
                        jogador.retCollision = (Rectangle){jogador.posicao.x - 16.5, jogador.posicao.y - 21.5, 33, 43};
                        jogador.sentido = 0;
                    }

                    if (collisionPlayer(tijolos, bordas, jogador, inimigo)) //COLIS�O DO PLAYER
                        jogador.posicao = jogador.posicao_anterior;

                    jogador.retJogador = (Rectangle){jogador.posicao.x, jogador.posicao.y, 33, 43}; //RET�NGULO DO JOGADOR

                    /*===========================*/

                    /*----- MOVIMENTA��O DO TANQUE INIMIGO -----*/
                    inimigo.posicao_anterior = inimigo.posicao;
                    inimigo.retInimigo = (Rectangle){inimigo.posicao.x, inimigo.posicao.y, 38, 44}; //GUARDA POSI��O

                    atualizaInimigo(&inimigo);

                    if (collisionInimigo(tijolos, bordas, inimigo, jogador))
                        inimigo.posicao = inimigo.posicao_anterior;

                    if (!inimigo.ativacao){
                        corInimigo = BLANK;
                        inimigo.retCollision = (Rectangle){-50, -50, 0, 0};
                    }

                    /*===========================*/

                    /*----- TIROS DO JOGADOR -----*/
                    if (!jogador.tiro.ativacao)
                        if (IsKeyPressed(KEY_SPACE))
                            tanqueAtirar(&jogador.tiro, jogador.sentido, jogador.posicao, multiplicador);

                    if (jogador.tiro.ativacao)
                        atualizaTiro(&(jogador.tiro));

                    if (collisionTiro(tijolos, bordas, &jogador.tiro))
                        jogador.tiro.ativacao = FALSE;

                    if (collisionVidas(&inimigo.tiro, jogador)) //PERDER VIDAS
                        jogador.vidas--;

                    if (collisionPontos(&jogador.tiro, inimigo)){
                        jogador.pontos += 800;
                        jogador.tiro.ativacao = FALSE;
                        jogador.tiro.posicao = (Vector2){-1, -1};
                        inimigo.ativacao = FALSE;
                        inimigo.retCollision = (Rectangle){inimigo.posicao.x, inimigo.posicao.y, 0, 0};
                        jogador.kills++;
                    }

                    if (jogador.tiro.posicao.x > 1000 || jogador.tiro.posicao.x < 0 || jogador.tiro.posicao.y < 40 || jogador.tiro.posicao.y > 800)
                            jogador.tiro.ativacao = FALSE;

                    /*===========================*/

                    /*----- TIROS DO INIMIGO -----*/
                    if (!inimigo.tiro.ativacao)
                        if (!GetRandomValue(0,16))
                            tanqueAtirar(&inimigo.tiro, inimigo.sentido, inimigo.posicao, multiplicador);
                    if (inimigo.tiro.ativacao)
                        atualizaTiro(&(inimigo.tiro));
                    if (collisionTiro (tijolos, bordas, &inimigo.tiro))
                        inimigo.tiro.ativacao = FALSE;

                    /*===========================*/

                    //APAGA OS ESCUDOS
                    if (jogador.vidas == 2) corShield[2] = BLANK;
                    if (jogador.vidas == 1) corShield[1] = BLANK;
                    if (jogador.vidas == 0) corShield[0] = BLANK;

                    /*===========================*/

                    /*----- DESENHOS FASE 1 -----*/
                    BeginDrawing();

                        ClearBackground (LIGHTGRAY);

                        menu(screenWidth);
                        DrawText(TextFormat("Pontos: %d", jogador.pontos), 850, 12, 20, GOLD);
                        DrawTexture(texturaShield, 10, 5, corShield[0]);
                        DrawTexture(texturaShield, 50, 5, corShield[1]);
                        DrawTexture(texturaShield, 90, 5, corShield[2]);

                        DrawTexturePro(texturaJogador, jogador.source, jogador.retJogador, jogador.origin, jogador.rotacao, LIGHTGRAY); //Desenho jogador

                        desenhoInimigo(&inimigo, texturaInimigo, corInimigo); //Desenho inimigo
                        tempoInimigo = spawnInimigo(tempoInimigo, &inimigo, tijolos, bordas, jogador);

                        if (inimigo.tiro.ativacao)
                            desenhaTiro(&inimigo.tiro); //DESENHO TIRO DO INIMIGO

                        if (jogador.tiro.ativacao)
                            desenhaTiro(&jogador.tiro); //DESENHO TIRO DO JOGADOR

                        if (energia.ativacao == TRUE)
                            desenhaEnergia(texturaEnergia, energia); //DESENHO ENERGIA

                        blocosFase1 (texturaTijolos); //DESENHO OBST�CULOS

                    EndDrawing();
                }

                /*----- FASE 2 -----*/
                if (jogador.kills == 1){
                    //POSICIONAMENTO - INICIALIZA��O
                    inicializaJogador(&jogador, screenWidth, screenHeight); //INICIALIZA��O DO JOGADOR

                    inicializaInimigo(&inimigo, screenWidth, screenHeight); //INICIALIZA��O DO INIMIGO
                    double tempoInimigo2 = GetTime();

                    inicializaTiroJ(&jogador); //INICIALIZA��O DO TIRO DO JOGADOR

                    Energy energia;
                    energia.ativacao = FALSE;
                    float multiplicador = 1; //ENERGIA

                    //RET�NGULOS  DOS TIJOLOS
                    Rectangles tijolos2;
                    iniciaRets2(&tijolos2);

                    while (!WindowShouldClose()){
                        SetRandomSeed(time(NULL)); //SEED RAND�MICA
                        int posXenergia = GetRandomValue(60, 940);
                        int posYenergia = GetRandomValue(60, 740);
                        int tiroInimigo;
                        randomMove = GetRandomValue(1,16);

                        //CORES
                        Color corInimigo = LIGHTGRAY;

                        /*----- GERA��O DA ENERGIA -----*/
                        if (energia.ativacao == FALSE){
                            if (!GetRandomValue(0,16)){
                                energia.ativacao = TRUE;
                                energia.posicao = (Vector2){posXenergia, posYenergia};
                                energia.retEnergia = (Rectangle){posXenergia, posYenergia, 26, 31};
                                multiplicador = 1;
                            }
                        }
                        if (collisionEnergia(jogador, energia)){
                            energia.ativacao = FALSE;
                            energia.retEnergia = (Rectangle){posXenergia, posYenergia, 0, 0};
                            multiplicador = 1.5;
                        }

                        /*===========================*/

                        /*----- MOVIMENTA��O DO JOGADOR -----*/
                        jogador.posicao_anterior = jogador.posicao;
                        jogador.retJogador = (Rectangle){jogador.posicao.x + 5, jogador.posicao.y + 16.5, 33, 44}; //GUARDA POSI��O

                        if (IsKeyDown(KEY_RIGHT)){
                            jogador.posicao.x += 3.0f*multiplicador;
                            jogador.rotacao = 90;
                            jogador.retCollision = (Rectangle){jogador.posicao.x - 18, jogador.posicao.y, 43, 33};
                            jogador.sentido = M_PI/2;
                        }
                        if (IsKeyDown(KEY_LEFT)){
                            jogador.posicao.x -= 3.0f*multiplicador;
                            jogador.rotacao = -90;
                            jogador.retCollision = (Rectangle){jogador.posicao.x - 16, jogador.posicao.y, 43, 33};
                            jogador.sentido = (-1*M_PI)/2;
                        }
                        if (IsKeyDown(KEY_UP)){
                            jogador.posicao.y -= 3.0f*multiplicador;
                            jogador.rotacao = 0;
                            jogador.retCollision = (Rectangle){jogador.posicao.x - 12, jogador.posicao.y -5, 33, 43};
                            jogador.sentido = M_PI;
                        }
                        if (IsKeyDown(KEY_DOWN)){
                            jogador.posicao.y += 3.0f*multiplicador;
                            jogador.rotacao = 180;
                            jogador.retCollision = (Rectangle){jogador.posicao.x - 12, jogador.posicao.y - 5, 33, 43};
                            jogador.sentido = 0;
                        }

                        if (collisionPlayer2(tijolos2, bordas, jogador, inimigo) == TRUE) //COLIS�O DO JOGADOR
                            jogador.posicao = jogador.posicao_anterior;

                        /*===========================*/

                        /*----- MOVIMENTA��O DO INIMIGO -----*/
                        inimigo.posicao_anterior = inimigo.posicao;
                        inimigo.retInimigo = (Rectangle){inimigo.posicao.x, inimigo.posicao.y, 38, 44}; //GUARDA POSI��O

                        atualizaInimigo(&inimigo);
                        if (collisionInimigo2(tijolos2, bordas, inimigo, jogador) == TRUE)
                            inimigo.posicao = inimigo.posicao_anterior;

                        /*------ TIROS DO JOGADOR ------*/
                        //int colTiro2 = collisionTiro2(tijolos, bordas, &(jogador.tiro));

                        if (!jogador.tiro.ativacao)
                            if (IsKeyPressed(KEY_SPACE))
                                tanqueAtirar(&jogador.tiro, jogador.sentido, jogador.posicao, multiplicador);

                        if (jogador.tiro.ativacao)
                            atualizaTiro(&(jogador.tiro));

                        if (collisionTiro2(tijolos, bordas, &(jogador.tiro)) == TRUE)
                            jogador.tiro.ativacao = FALSE;

                        if (collisionVidas(&inimigo.tiro, jogador)) //PERDER VIDAS
                            jogador.vidas--;

                        if (collisionPontos(&jogador.tiro, inimigo)){
                            jogador.pontos += 800;
                            jogador.tiro.ativacao = FALSE;
                            jogador.tiro.posicao = (Vector2){-1, -1};
                            inimigo.ativacao = FALSE;
                            inimigo.retCollision = (Rectangle){inimigo.posicao.x, inimigo.posicao.y, 0, 0};
                            jogador.kills++;
                        }

                        if (jogador.tiro.posicao.x > 1000 || jogador.tiro.posicao.x < 0 || jogador.tiro.posicao.y < 40 || jogador.tiro.posicao.y > 800)
                                jogador.tiro.ativacao = FALSE;


                        /*----- DESENHO DA FASE 2 -----*/
                        BeginDrawing();

                            ClearBackground (LIGHTGRAY);
                            menu2(screenWidth);
                            DrawText(TextFormat("Pontos: %d", jogador.pontos), 850, 12, 20, GOLD);
                            DrawTexture(texturaShield, 10, 5, corShield[0]);
                            DrawTexture(texturaShield, 50, 5, corShield[1]);
                            DrawTexture(texturaShield, 90, 5, corShield[2]);

                            blocosFase2(texturaTijolos);

                            DrawTexturePro(texturaJogador, jogador.source, jogador.retJogador, jogador.origin, jogador.rotacao, LIGHTGRAY); //Desenho jogador

                            desenhoInimigo(&inimigo, texturaInimigo, corInimigo); //DESENHO INIMIGO
                            tempoInimigo2 = spawnInimigo(tempoInimigo, &inimigo, tijolos2, bordas, jogador);

                            if (jogador.tiro.ativacao == TRUE)
                                desenhaTiro(&jogador.tiro); //DESENHO DO TIRO DO JOGADOR

                        EndDrawing();

                    }

                }

            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
