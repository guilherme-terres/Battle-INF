#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

/*----- DEFINIÇÕES -----*/
#define FALSE 0
#define TRUE 1
#define MAXTIROS 10
#define MAXDIGITOS 9

/*----- GLOBAIS -----*/
int randomMove; //MOVIMENTAÇÃO RANDÔMICA DO INIMIGO

Image ImgInimigo;
Texture2D texturaInimigo; //TEXTURA DO INIMIGO

Image ImgEnergy;
Texture2D texturaEnergia; //TEXTURA DA ENERGIA

Image ImgShield;
Texture2D texturaShield;

/*----- STRUCTS -----*/
typedef struct usuario {
    char nick[MAXDIGITOS];
    int pontos;
} Usuario;

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
} Rectangles; //RETÂNGULOS


/*-----FUNÇÕES-----*/
void menuInicial (Color *corTexto){
    DrawText("Battle INF", 360, 80, 50, LIGHTGRAY);
    DrawText("Novo jogo", 415, 220, 30, corTexto[0]);
    DrawText("Continuar", 415, 280, 30, corTexto[1]);
    DrawText("Ranking", 428, 340, 30, corTexto[2]);
    DrawText("Sair", 455, 400, 30, corTexto[3]);
}

void menu (int screenWidth, Texture2D texturaShield, Color *corShield){
    DrawRectangle (0,0, screenWidth, 40, DARKGRAY); //LISTRA CINZA ESCURO
    DrawText("Fase 1", 442, 7, 30, GOLD);
    DrawTexture(texturaShield, 10, 5, corShield[0]);
    DrawTexture(texturaShield, 50, 5, corShield[1]);
    DrawTexture(texturaShield, 90, 5, corShield[2]);
} //MENU DA FASE 1 - DESENHO

void menu2 (int screenWidth, Texture2D texturaShield, Color *corShield){
    DrawRectangle (0,0, screenWidth, 40, DARKGRAY); //LISTRA CINZA ESCURO
    DrawText("Fase 2", 442, 7, 30, GOLD);
    DrawTexture(texturaShield, 10, 5, corShield[0]);
    DrawTexture(texturaShield, 50, 5, corShield[1]);
    DrawTexture(texturaShield, 90, 5, corShield[2]);
} //MENU DA FASE 2 - DESENHO

void formHeader (int screenWidth){
    DrawRectangle (0,0, screenWidth, 40, DARKGRAY); //LISTRA CINZA ESCURO
    DrawText("PARABENS! VOCE PASSOU DE FASE!", 300, 12, 20, GOLD);
} //CABEÇALHO DO FORMULÁRIO DE INSERÇÃO DE NOME

void formFooter (int screenWidth){
    DrawRectangle (0, 560, screenWidth, 40, DARKGRAY); //LISTRA CINZA ESCURO
    DrawText("CLIQUE ENTER PARA PROSSEGUIR!", 310, 570, 20, GOLD);
} //FOOTER DO FORMULÁRIO DE INSERÇÃO DE NOME

void headerRanking(int screenWidth){
    DrawRectangle (0,0, screenWidth, 40, DARKGRAY); //LISTRA CINZA ESCURO
    DrawText("RANKING", 440, 12, 20, GOLD);
} //CABEÇALHO DO RANKING

void footerRanking(int screenWidth){
    DrawRectangle (0, 560, screenWidth, 40, DARKGRAY); //LISTRA CINZA ESCURO
    DrawText("FECHA A JANELA PARA VOLTAR O MENU!", 270, 570, 20, GOLD);
} //FOOTER DA PÁGINA DE RANKING

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

void desenhaNome (int screenWidth, Rectangle textBox, int contadorLetras, Usuario *usuario){
    formHeader(screenWidth); //CABEÇALHO

    DrawText("INSIRA SEU NOME", 390, 190, 20, DARKGRAY);
    DrawRectangleRec(textBox, DARKGRAY);
    DrawText(TextFormat("CARACTERES: %d/%d", contadorLetras, MAXDIGITOS), 385, 300, 20, DARKGRAY); //GRÁFICO
    DrawText(usuario->nick, (int)textBox.x + 5, (int)textBox.y + 8, 20, GOLD); //ESCREVER OS CARACTERES INSERIDOS

    formFooter(screenWidth);
} //DESENHA FORMULÁRIO

void iniciaRets1 (Rectangles *tijolos){
    tijolos->tamanho[0] = (Rectangle){65, 230, 210, 55};
    tijolos->tamanho[1] = (Rectangle){275, 230, 55, 257.5};
    tijolos->tamanho[2] = (Rectangle){65, 432, 210, 55};
    tijolos->tamanho[3] = (Rectangle){650, 40, 145, 55};
    tijolos->tamanho[4] = (Rectangle){590, 95, 206, 55};
    tijolos->tamanho[5] = (Rectangle){550, 150, 173.2, 55};
    tijolos->tamanho[6] = (Rectangle){750, 290, 55, 55};
} //INICIALIZAÇÃO DOS RETÂNGULOS DA FASE 1

void iniciaRets2 (Rectangles *tijolos2){
    tijolos2->tamanho2[0] = (Rectangle){63, 230, 270, 220};
    tijolos2->tamanho2[1] = (Rectangle){298.5, 110, 388, 55};
    tijolos2->tamanho2[2] = (Rectangle){650, 299, 206.5, 56};
    tijolos2->tamanho2[3] = (Rectangle){699, 355, 208.5, 55};
    tijolos2->tamanho2[4] = (Rectangle){599, 408, 238, 57.5};
} //INICIALIZAÇÃO DOS RETÂNGULOS DA FASE 2

void iniciaBordas (Rectangles *bordas, int screenWidth, int screenHeight){
    bordas->tamanhoB[0] = (Rectangle){0,0, screenWidth, 40}; //Header
    bordas->tamanhoB[1] = (Rectangle){0, 0, 1, screenHeight}; //Esquerda
    bordas->tamanhoB[2] = (Rectangle){screenWidth - 1, 0, 1, screenHeight}; //Direita
    bordas->tamanhoB[3] = (Rectangle){0, screenHeight, screenWidth, 1}; //Baixo
} //INICIALIZAÇÃO DAS BORDAS

void inicializaJogador (Player *jogador, int screenWidth, int screenHeight){
    jogador->posicao = (Vector2){(float)screenWidth/2, (float)screenHeight/2};
    jogador->origin = (Vector2){16.5, 21.5};
    jogador->source = (Rectangle){(float)screenWidth/2 + 28, (float)screenHeight/2 + 1, 33, 43};
    jogador->vidas = 3;
    jogador->rotacao = 0.0;
    jogador->pontos = 0;
    jogador->sentido = M_PI;
    jogador->kills = 0;
} //INICIALIZAÇÃO DA MOVIMENTAÇÃO DO JOGADOR

void inicializaInimigo (Enemy *inimigo, int screenWidth, int screenHeight){
    inimigo->origin = (Vector2){19, 22};
    inimigo->source = (Rectangle){(float)screenWidth/2 + 32, (float)screenHeight/2 + 8, 38, 44};
    inimigo->retCollision = (Rectangle){inimigo->posicao.x  -19, inimigo->posicao.y -22, 38, 44};
    inimigo->sentido = 0.0;
    inimigo->rotacao = 0.0;
    inimigo->ativacao = FALSE;
} //INICIALIZAÇÃO DA MOVIMENTAÇÃO DO INIMIGO

void inicializaTiroJ (Player *jogador){
    jogador->tiro.posicao = (Vector2){jogador->posicao.x, jogador->posicao.y};
    jogador->tiro.raio = 2.7;
    jogador->tiro.sentido = 0.0;
    jogador->tiro.cor = BLACK;
    jogador->tiro.velocidade = 5.7;
    jogador->tiro.ativacao = FALSE;
} //INICIALIZAÇÃO DO TIRO DO JOGADOR

void inicializaTiroI (Enemy *inimigo){
    inimigo->tiro.posicao = (Vector2){inimigo->posicao.x, inimigo->posicao.y};
    inimigo->tiro.raio = 2.7;
    inimigo->tiro.sentido = 0.0;
    inimigo->tiro.cor = BLACK;
    inimigo->tiro.velocidade = 5.7;
    inimigo->tiro.ativacao = FALSE;
} //INICIALIZAÇÃO DO TIRO DO JOGADOR

/*----- COLISÕES -----*/
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
} // COLISÃO DO PLAYER DA FASE 1

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
} // COLISÃO DO PLAYER DA FASE 2

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
} //COLISÃO DO INIMIGO DA FASE 1

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
} //COLISÃO DO INIMIGO DA FASE 1

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
} //COLISÃO DO TIRO COM OS TIJOLOS E COM AS BORDAS NA FASE 1

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
} //COLISÃO DO TIRO COM OS TIJOLOS E COM AS BORDAS NA FASE 2

int collisionVidas (Shoot *tiro, Player jogador){
    int retorno = 0;

    if (CheckCollisionPointRec(tiro->posicao, jogador.retCollision))
        retorno = 1;

    return retorno;
} //COLISÃO PARA PERDER VIDAS

int collisionPontos (Shoot *tiro, Enemy inimigo){
    int retorno = 0;

    if (CheckCollisionPointRec(tiro->posicao, inimigo.retCollision))
        retorno = 1;

    return retorno;
} //COLISÃO PARA MATAR INIMIGO E GANHAR PONTOS

int colEnergiaBlocos1 (Energy *energia, Rectangles tijolos, Rectangles bordas){
    int retorno = 0;

    if (CheckCollisionRecs(energia->retEnergia, tijolos.tamanho[0])
    || CheckCollisionRecs(energia->retEnergia, tijolos.tamanho[1])
    || CheckCollisionRecs(energia->retEnergia, tijolos.tamanho[2])
    || CheckCollisionRecs(energia->retEnergia, tijolos.tamanho[3])
    || CheckCollisionRecs(energia->retEnergia, tijolos.tamanho[4])
    || CheckCollisionRecs(energia->retEnergia, tijolos.tamanho[5])
    || CheckCollisionRecs(energia->retEnergia, tijolos.tamanho[6])
    || CheckCollisionRecs(energia->retEnergia, bordas.tamanhoB[0])
    || CheckCollisionRecs(energia->retEnergia, bordas.tamanhoB[1])
    || CheckCollisionRecs(energia->retEnergia, bordas.tamanhoB[2])
    || CheckCollisionRecs(energia->retEnergia, bordas.tamanhoB[3]))
        retorno = 1;

    return retorno;
} //COLISÃO DA ENERGIA COM OS BLOCOS DA FASE 1

int colEnergiaBlocos2 (Energy *energia, Rectangles tijolos2, Rectangles bordas){
    int retorno = 0;

    if (CheckCollisionRecs(energia->retEnergia, tijolos2.tamanho2[0])
    || CheckCollisionRecs(energia->retEnergia, tijolos2.tamanho2[1])
    || CheckCollisionRecs(energia->retEnergia, tijolos2.tamanho2[2])
    || CheckCollisionRecs(energia->retEnergia, tijolos2.tamanho2[3])
    || CheckCollisionRecs(energia->retEnergia, tijolos2.tamanho2[4])
    || CheckCollisionRecs(energia->retEnergia, bordas.tamanhoB[0])
    || CheckCollisionRecs(energia->retEnergia, bordas.tamanhoB[1])
    || CheckCollisionRecs(energia->retEnergia, bordas.tamanhoB[2])
    || CheckCollisionRecs(energia->retEnergia, bordas.tamanhoB[3]))
        retorno = 1;

    return retorno;
} //COLISÃO DA ENERGIA COM OS BLOCOS DA FASE 2

int collisionTexto (Rectangle textBox){
    int retorno = FALSE;

    if (CheckCollisionPointRec(GetMousePosition(), textBox))
        retorno = TRUE;

    return retorno;
} //COLISÃO COM A CAIXA DE TEXTO

void collisionMenu (Vector2 ponteiroMouse, Rectangle novoJogo, Rectangle continuar, Rectangle ranking, Rectangle mapas, Color *corTexto){
    if (CheckCollisionPointRec(ponteiroMouse, novoJogo)) corTexto[0] = GOLD;
    else corTexto[0] = LIGHTGRAY; //MUDANÇA DE COR AO PASSAR O MOUSE POR CIMA
    if (CheckCollisionPointRec(ponteiroMouse, continuar)) corTexto[1] = GOLD;
    else corTexto[1] = LIGHTGRAY; //MUDANÇA DE COR AO PASSAR O MOUSE POR CIMA
    if (CheckCollisionPointRec(ponteiroMouse, ranking)) corTexto[2] = GOLD;
    else corTexto[2] = LIGHTGRAY; //MUDANÇA DE COR AO PASSAR O MOUSE POR CIMA
    if (CheckCollisionPointRec(ponteiroMouse, mapas)) corTexto[3] = GOLD;
    else corTexto[3] = LIGHTGRAY; //MUDANÇA DE COR AO PASSAR O MOUSE POR CIMA
}

void atualizaInimigo (Enemy *inimigo){
    if (randomMove >= 1 && randomMove <= 4){
        inimigo->posicao.x += 3.0f;
        inimigo->rotacao = 90;
        inimigo->sentido = M_PI/2;
        inimigo->retCollision = (Rectangle){inimigo->posicao.x - 22, inimigo->posicao.y - 19, 44, 38};
    }
    if (randomMove >= 5 && randomMove <= 8){
        inimigo->posicao.x -= 3.0f;
        inimigo->rotacao = -90;
        inimigo->sentido = (-1)*M_PI/2;
        inimigo->retCollision = (Rectangle){inimigo->posicao.x -22, inimigo->posicao.y - 19, 44, 38};
    }
    if (randomMove >=9 && randomMove <= 12){
        inimigo->posicao.y -= 3.0f;
        inimigo->rotacao = 0;
        inimigo->sentido = 0;
        inimigo->retCollision = (Rectangle){inimigo->posicao.x -19, inimigo->posicao.y -22, 38, 44};
    }
    if (randomMove >= 13 && randomMove <= 16){
        inimigo->posicao.y += 3.0f;
        inimigo->rotacao = 180;
        inimigo->sentido = M_PI;
        inimigo->retCollision = (Rectangle){inimigo->posicao.x -19, inimigo->posicao.y -22, 38, 44};
    }
} //MOVIMENTAÇÃO ALEATÓRIA DO INIMIGO

void desenhoInimigo (Enemy *inimigo, Texture2D texturaInimigo, Color corInimigo){
    DrawTexturePro(texturaInimigo, inimigo->source, inimigo->retInimigo, inimigo->origin, inimigo->rotacao, corInimigo);
} //DESENHO DO INIMIGO

void tanqueAtirar (Shoot *tiro, float sentido, Vector2 posicao, float multiplicador){
    tiro->ativacao = TRUE;
    tiro->sentido = sentido;
    tiro->posicao = posicao;
    tiro->velocidade = 5.7*multiplicador;
    tiro->cor = BLACK;
    tiro->raio = 2.7;
} //INICIALIZAÇÃO DO TIRO DO TANQUE

void tanqueAtirar2 (Shoot *tiro, float sentido, Vector2 posicao, float multiplicador){
    tiro->ativacao = TRUE;
    tiro->sentido = sentido;
    tiro->posicao = (Vector2){posicao.x + 5, posicao.y + 16};
    tiro->velocidade = 5.7*multiplicador;
    tiro->cor = BLACK;
    tiro->raio = 2.7;
} //INICIALIZAÇÃO DO TIRO DO TANQUE

void atualizaTiro (Shoot *tiro){
    tiro->posicao.x += tiro->velocidade*sin(tiro->sentido);
    tiro->posicao.y += tiro->velocidade*cos(tiro->sentido);
} //ATUALIZAÇÃO DA POSIÇÃO DO TIRO

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
} //TESTA A COLISÃO DA ENERGIA COM O PLAYER

/*
void gerarEnergia1 (Energy *energia, Rectangles tijolos, Rectangles bordas, float multiplicador){
    energia->ativacao = TRUE;
    multiplicador = 1;
    do {
        energia->posicao = (Vector2){GetRandomValue(60,940), GetRandomValue(60,740)};
        energia->retEnergia = (Rectangle){energia->posicao.x, energia->posicao.y, 26, 31};
    } while (colEnergiaBlocos1(&energia, tijolos, bordas) == 1);
}
*/

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

double spawnInimigo2 (double tempoInimigo, Enemy *inimigo, Rectangles tijolos2, Rectangles bordas, Player jogador){
    if (GetTime() > tempoInimigo + 5 && inimigo->ativacao == FALSE){
        tempoInimigo = GetTime();
        inimigo->ativacao = TRUE;
        do {
            inimigo->posicao = (Vector2){GetRandomValue(60, 950), GetRandomValue(80, 760)};
            inimigo->retCollision = (Rectangle){inimigo->posicao.x  -19, inimigo->posicao.y -22, 38, 44};
        } while (collisionInimigo2(tijolos2, bordas, *inimigo, jogador) == 1);
    }

    if (inimigo->ativacao == TRUE)
        tempoInimigo = GetTime();

    return tempoInimigo;
} //SPAWNA O INIMIGO A CADA 5s


int main(void){
    const int screenWidth = 1000;
    const int screenHeight = 600;
    FILE *highscores;

    highscores = fopen("highscores.bin", "ab+"); //ABERTURA DO ARQUIVO HIGHSCORES.BIN
    Usuario usuario;

    InitWindow(screenWidth, screenHeight, "Battle INF"); //INICIALIZAÇÃO DA JANELA
    Vector2 ponteiroMouse = {0.0f, 0.0f}; //MOUSE
    Color corTexto[4] = {LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY}; //COR DAS OPÇÕES DO MENU

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        /*----- UPDATE -----*/
        ponteiroMouse = GetMousePosition(); //ATUALIZANDO O PONTEIRO DO MOUSE

        /*----- DESENHO DO MENU INICIAL ------*/
        BeginDrawing();

            ClearBackground(BLACK);
            menuInicial(&corTexto);
            //RETÂNGULOS
            Rectangle novoJogo = {415, 220, 147, 30};
            Rectangle continuar = {415, 280, 147, 30};
            Rectangle ranking = {428, 340, 123, 30};
            Rectangle mapas = {455, 400, 60, 30};

            collisionMenu(ponteiroMouse, novoJogo, continuar, ranking, mapas, &corTexto); //FUNÇÃO PARA MUDAR A COR DO TEXTO

            if (CheckCollisionPointRec(ponteiroMouse, novoJogo) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){ //CLICAR EM NOVO JOGO E CHAMAR A FASE 1
                Color corShield[3] = {WHITE, WHITE, WHITE}; //CORES

                //TEXTURAS
                Image ImgJogador = LoadImage("recursos/jogador.png");
                Texture2D texturaJogador = LoadTextureFromImage(ImgJogador); //JOGADOR
                ImgInimigo = LoadImage("recursos/inimigo.png");
                texturaInimigo = LoadTextureFromImage(ImgInimigo); //INIMIGO
                Image ImgTijolos = LoadImage("recursos/tijolos.png");
                Texture2D texturaTijolos = LoadTextureFromImage(ImgTijolos); //TIJOLOS
                ImgShield = LoadImage("recursos/shield1.png");
                texturaShield = LoadTextureFromImage(ImgShield); //SHIELD
                ImgEnergy = LoadImage("recursos/energy_drop1.png");
                texturaEnergia = LoadTextureFromImage(ImgEnergy); //ENERGIA

                //RETÂNGULOS DA FASE 1 - INICIALIZAÇÃO
                Rectangles tijolos;
                iniciaRets1 (&tijolos); //TIJOLOS

                Rectangles bordas;
                iniciaBordas(&bordas, screenWidth, screenHeight); //BORDAS

                //POSICIONAMENTO - INICIALIZAÇÃO
                Player jogador;
                inicializaJogador(&jogador, screenWidth, screenHeight); //JOGADOR
                inicializaTiroJ(&jogador); //TIRO DO JOGADOR

                double tempoInimigo = GetTime();
                Enemy inimigo;
                inicializaInimigo(&inimigo, screenWidth, screenHeight); //INIMIGO
                inicializaTiroI(&inimigo); //TIRO DO INIMIGO

                Energy energia;
                energia.ativacao = FALSE;
                float multiplicador = 1; //ENERGIA


                while (!WindowShouldClose() && jogador.vidas != 0 && jogador.kills != 1){
                    SetRandomSeed(time(NULL));
                    int posXenergia = GetRandomValue(60,940); //POSIÇÃO X DA ENERGIA
                    int posYenergia = GetRandomValue(60,740); //POSIÇÃO Y DA ENERGIA
                    int tiroInimigo;
                    randomMove = GetRandomValue(1,16); //MOVIMENTAÇÃO ALEATÓRIA DO INIMIGO
                    Color corInimigo = LIGHTGRAY; //COR DO INIMIGO


                    /*----- GERAÇÃO DA ENERGIA -----*/

                    if (energia.ativacao == FALSE){
                        if (!GetRandomValue(0,16)){
                            //gerarEnergia1(&energia, tijolos, bordas, multiplicador);
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


                    /*------ MOVIMENTAÇÃO DO JOGADOR -----*/
                    jogador.posicao_anterior = jogador.posicao;
                    jogador.retJogador = (Rectangle){jogador.posicao.x + 5, jogador.posicao.y + 16.5, 33, 44}; //GUARDA POSIÇÃO

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

                    if (collisionPlayer(tijolos, bordas, jogador, inimigo)) //COLISÃO DO PLAYER
                        jogador.posicao = jogador.posicao_anterior;

                    jogador.retJogador = (Rectangle){jogador.posicao.x, jogador.posicao.y, 33, 43}; //RETÂNGULO DO JOGADOR


                    /*----- MOVIMENTAÇÃO DO TANQUE INIMIGO -----*/
                    inimigo.posicao_anterior = inimigo.posicao;
                    inimigo.retInimigo = (Rectangle){inimigo.posicao.x, inimigo.posicao.y, 38, 44}; //GUARDA POSIÇÃO
                    atualizaInimigo(&inimigo); //ATUALIZA A POSIÇÃO DO INIMIGO DE FORMA ALEATÓRIA

                    if (collisionInimigo(tijolos, bordas, inimigo, jogador))
                        inimigo.posicao = inimigo.posicao_anterior;
                    if (!inimigo.ativacao){
                        corInimigo = BLANK;
                        inimigo.retCollision = (Rectangle){-50, -50, 0, 0};
                    }


                    /*----- TIROS DO JOGADOR -----*/
                    if (!jogador.tiro.ativacao)
                        if (IsKeyPressed(KEY_SPACE)) //CASO ESPAÇO SEJA PRESSIONADO
                            tanqueAtirar(&jogador.tiro, jogador.sentido, jogador.posicao, multiplicador);
                    if (jogador.tiro.ativacao) //ATUALIZAÇÃO DA POSIÇÃO DO TIRO
                        atualizaTiro(&(jogador.tiro));
                    if (collisionTiro(tijolos, bordas, &jogador.tiro)) //COLISÃO DO TIRO COM AS PAREDES
                        jogador.tiro.ativacao = FALSE;
                    if (collisionVidas(&inimigo.tiro, jogador)) //PERDER VIDAS
                        jogador.vidas--;
                    if (collisionPontos(&jogador.tiro, inimigo)){ //PONTOS E DESAPARECIMENTO DO INIMIGO
                        jogador.pontos += 800;
                        jogador.tiro.ativacao = FALSE;
                        jogador.tiro.posicao = (Vector2){-1, -1};
                        inimigo.ativacao = FALSE;
                        inimigo.retCollision = (Rectangle){inimigo.posicao.x, inimigo.posicao.y, 0, 0};
                        jogador.kills++;
                    }
                    if (jogador.tiro.posicao.x > 1000 || jogador.tiro.posicao.x < 0 || jogador.tiro.posicao.y < 40 || jogador.tiro.posicao.y > 800)
                            jogador.tiro.ativacao = FALSE;


                    /*----- TIROS DO INIMIGO -----*/
                    if (!inimigo.tiro.ativacao)
                        if (!GetRandomValue(0,1)) //CASO O VALOR ALEATÓRIO SEJA ZERO (0)
                            tanqueAtirar(&inimigo.tiro, inimigo.sentido, inimigo.posicao, multiplicador);
                    if (inimigo.tiro.ativacao) //ATUALIZA A POSIÇÃO DO TIRO DO INIMIGO
                        atualizaTiro(&(inimigo.tiro));
                    if (collisionTiro (tijolos, bordas, &inimigo.tiro))
                        inimigo.tiro.ativacao = FALSE;

                    //APAGA OS ESCUDOS
                    if (jogador.vidas == 2) corShield[2] = BLANK;
                    if (jogador.vidas == 1) corShield[1] = BLANK;
                    if (jogador.vidas == 0) corShield[0] = BLANK;


                    /*----- DESENHOS FASE 1 -----*/
                    BeginDrawing();

                        ClearBackground (LIGHTGRAY);

                        menu(screenWidth, texturaShield, &corShield);
                        DrawText(TextFormat("Pontos: %d", jogador.pontos), 850, 12, 20, GOLD);
                        DrawTexturePro(texturaJogador, jogador.source, jogador.retJogador, jogador.origin, jogador.rotacao, LIGHTGRAY); //DESENHO JOGADOR
                        desenhoInimigo(&inimigo, texturaInimigo, corInimigo); //DESENHO INIMIGO
                        tempoInimigo = spawnInimigo(tempoInimigo, &inimigo, tijolos, bordas, jogador);

                        if (inimigo.tiro.ativacao)
                            desenhaTiro(&inimigo.tiro); //DESENHO TIRO DO INIMIGO

                        if (jogador.tiro.ativacao)
                            desenhaTiro(&jogador.tiro); //DESENHO TIRO DO JOGADOR

                        if (energia.ativacao == TRUE)
                            desenhaEnergia(texturaEnergia, energia); //DESENHO ENERGIA

                        blocosFase1 (texturaTijolos); //DESENHO OBSTÁCULOS

                    EndDrawing();
                }


                /*----- FASE 2 -----*/
                if (jogador.kills == 1){
                    //POSICIONAMENTO - INICIALIZAÇÃO
                    inicializaJogador(&jogador, screenWidth, screenHeight); //INICIALIZAÇÃO DO JOGADOR
                    inicializaTiroJ(&jogador); //INICIALIZAÇÃO DO TIRO DO JOGADOR

                    double tempoInimigo2 = GetTime();
                    inicializaInimigo(&inimigo, screenWidth, screenHeight); //INICIALIZAÇÃO DO INIMIGO
                    inicializaTiroI(&inimigo);

                    energia.ativacao = FALSE;
                    float multiplicador = 1; //ENERGIA

                    Rectangles tijolos2;
                    iniciaRets2(&tijolos2); //RETÂNGULOS  DOS TIJOLOS

                    while (!WindowShouldClose()){
                        SetRandomSeed(time(NULL)); //SEED RANDÔMICA
                        int posXenergia = GetRandomValue(60, 940);
                        int posYenergia = GetRandomValue(60, 740);
                        int tiroInimigo;
                        randomMove = GetRandomValue(1,16);

                        //CORES
                        Color corInimigo = LIGHTGRAY;


                        /*----- GERAÇÃO DA ENERGIA -----*/
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


                        /*----- MOVIMENTAÇÃO DO JOGADOR -----*/
                        jogador.posicao_anterior = jogador.posicao;
                        jogador.retJogador = (Rectangle){jogador.posicao.x + 5, jogador.posicao.y + 16.5, 33, 44}; //GUARDA POSIÇÃO

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

                        if (collisionPlayer2(tijolos2, bordas, jogador, inimigo) == TRUE) //COLISÃO DO JOGADOR
                            jogador.posicao = jogador.posicao_anterior;


                        /*----- MOVIMENTAÇÃO DO INIMIGO -----*/
                        inimigo.posicao_anterior = inimigo.posicao;
                        inimigo.retInimigo = (Rectangle){inimigo.posicao.x, inimigo.posicao.y, 38, 44}; //GUARDA POSIÇÃO

                        atualizaInimigo(&inimigo);
                        if (collisionInimigo2(tijolos2, bordas, inimigo, jogador) == TRUE)
                            inimigo.posicao = inimigo.posicao_anterior;


                        /*------ TIROS DO JOGADOR ------*/
                        if (!jogador.tiro.ativacao)
                            if (IsKeyPressed(KEY_SPACE))
                                tanqueAtirar2(&jogador.tiro, jogador.sentido, jogador.posicao, multiplicador);

                        if (jogador.tiro.ativacao)
                            atualizaTiro(&(jogador.tiro));

                        if (collisionTiro2(tijolos2, bordas, &(jogador.tiro)) == TRUE)
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


                        /*----- TIROS DO INIMIGO -----*/
                        if (!inimigo.tiro.ativacao){
                            if (!GetRandomValue(0,8))
                                tanqueAtirar(&inimigo.tiro, inimigo.sentido, inimigo.posicao, multiplicador);
                        }
                        if (inimigo.tiro.ativacao)
                            atualizaTiro(&(inimigo.tiro));
                        if (collisionTiro (tijolos, bordas, &inimigo.tiro))
                            inimigo.tiro.ativacao = FALSE;

                        /*----- DESENHO DA FASE 2 -----*/
                        BeginDrawing();

                            ClearBackground (LIGHTGRAY);

                            menu2(screenWidth, texturaShield, &corShield);
                            DrawText(TextFormat("Pontos: %d", jogador.pontos), 850, 12, 20, GOLD);
                            DrawTexturePro(texturaJogador, jogador.source, jogador.retJogador, jogador.origin, jogador.rotacao, LIGHTGRAY); //DESENHO JOGADOR
                            desenhoInimigo(&inimigo, texturaInimigo, corInimigo); //DESENHO INIMIGO
                            tempoInimigo2 = spawnInimigo2(tempoInimigo, &inimigo, tijolos2, bordas, jogador);

                            if (jogador.tiro.ativacao)
                                desenhaTiro(&jogador.tiro); //DESENHO DO TIRO DO JOGADOR

                            if (energia.ativacao)
                                desenhaEnergia(texturaEnergia, energia); //DESENHA ENERGIA

                            blocosFase2(texturaTijolos);

                        EndDrawing();

                    }

                }

            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
