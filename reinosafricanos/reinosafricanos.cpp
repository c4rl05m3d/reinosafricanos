#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "print.h"
#include <iostream>
#include <vector>
using namespace std;
using namespace olc;

struct v2 {
    float x;
    float y;

    float distance() {
        return sqrt(x * x + y * y);
    }

    v2(int a, int b) {
        x = a;
        y = b;
    }

    v2(float a, float b) {
        x = a;
        y = b;
    }

    v2() {

    }

    v2 makeunit() {
        v2 temp;
        float d = distance();
        temp.x = x / d;
        temp.y = y / d;
        return temp;
    }

    v2 operator -(v2 other) {
        v2 temp;
        temp.x = x - other.x;
        temp.y = y - other.y;
        return temp;
    }

    v2 operator +(v2 other) {
        v2 temp;
        temp.x = x + other.x;
        temp.y = y + other.y;
        return temp;
    }

    v2 operator /(float other) {
        v2 temp;
        temp.x = x / other;
        temp.y = y / other;
        return temp;
    }

    v2 operator *(float other) {
        v2 temp;
        temp.x = x * other;
        temp.y = y * other;
        return temp;
    }

    void operator +=(v2 other) {
        x += other.x;
        y += other.y;
    }

    void operator -=(v2 other) {
        x -= other.x;
        y -= other.y;
    }
};

bool mousedentro(v2 pos, v2 end, v2 mouse) {
    if (end.x >= mouse.x and mouse.x >= pos.x and end.y >= mouse.y and mouse.y >= pos.y) {
        return true;
    }
    else { return false; }
}

struct carta {
    bool virada;
    Pixel cor;
    int numero;
    string reino;
    string fato;
    v2 pos;
    bool mouse;
    carta(bool v, Pixel c, int n, string r , string f, v2 p, bool m){
        virada = v;
        cor = c;
        numero = n;
        reino = r;
        fato = f;
        pos = p;
        mouse = m;
    }
};

struct botao {
    bool apertado;
    Pixel borda;
    Pixel preenchimento;
    string texto;
    bool mouse;
    vector<v2> vertices;
    int numero;
    botao(bool a, Pixel b, Pixel p, string t, bool m, vector<v2> v, int n) {
        apertado = a;
        borda = b;
        preenchimento = p;
        texto = t;
        mouse = m;
        vertices = v;
        numero = n;
    }
};


void drawall(vector<botao>& botoes, vector<carta> cartas, PixelGameEngine* engine) {
    for (botao b : botoes) {
        if (b.apertado == false) {
            float vartam = (b.mouse == true) ? 1.1 : 1;
            int tamx = b.vertices[1].x - b.vertices[0].x;
            int tamy = b.vertices[2].y - b.vertices[0].y;
            float varpos = (b.mouse == true) ? tamx / 10 : 0;
            engine->FillRect(b.vertices[0].x - varpos, b.vertices[0].y - varpos, tamx * vartam + varpos, tamy * vartam + varpos, b.preenchimento);
            engine->DrawRect(b.vertices[0].x - 1 - varpos, b.vertices[0].y - 1 - varpos, (tamx + 2) * vartam + varpos, (tamy + 2) * vartam + varpos, b.borda);
            if (b.numero == 0) {
                engine->DrawString(68 , 1000 , b.texto, WHITE, 2 * vartam);
            }
            else {
                engine->DrawString(b.vertices[0].x + 20, b.vertices[0].y + 15, b.texto, WHITE, 2 * vartam);
            }
        }
        else {
            int num = b.numero;
            for (carta c : cartas) {
                if (c.numero == num) {
                    engine->DrawString(c.pos.x + 20, b.vertices[0].y + 20, c.reino, WHITE, 2);
                }
            }
        }
    }
    for (carta c : cartas) {
        int posx = c.pos.x;
        int posy = c.pos.y;
        Pixel cor = (c.virada == false) ? c.cor : WHITE;
        engine->FillRect(posx, posy, 340, 350, cor);
        engine->DrawRect(posx - 1, posy - 1, 342, 352, WHITE);
        if (c.virada == false) {
            int posx2 = (c.numero == 10) ? posx - 15 : posx;
            engine->DrawString(posx2 + 145, posy + 150, to_string(c.numero), WHITE, 6);
        }
        else {
            engine->DrawString(posx + 5, posy + 10, c.fato, BLACK, 2);
        }

    }
}

void mostrardestacada(int numero, vector<carta> cartas, PixelGameEngine* engine) {
    vector<string> fatosampliados = {
        "O rei era chamado de mansa, que\n\nsignifica rei dos reis, soberano.\n\nEle controlava todo o império, que\n\nchegou a ter cerca de quatrocentas\n\ncidades e vilas, organizadas em\n\nprovíncias comandadas por um\n\nadministrador submetido ao\n\npoder do mansa.",
        "No século IX, o islamismo se\n\nespalhou no reino e foi adotado\n\nprincipalmente pelos membros da\n\ncorte, pelos intelectuais\n\ne pelas pessoas letradas.",
        "Por meio de guerras conquistou\n\nmuitos territorios. Contudo,\n\nentrou em declinio no seculo XVI.",
        "Tinha como principais atividades\n\neconômicas a pesca e o comércio\n\nlocal. Marinheiros muito habilidosos\n\ndominavam a navegação no rio Níger,\n\no principal da região do Sahel.",
        "Surgiu na África Ocidental no\n\nséculo III e cujo apogeu se deu\n\nentre os séculos IX e X.\n\nSua capital, Kumbi Saleh, chegou a\n\nter entre 15 mil e 20 mil habitantes"
    };
    for (carta c : cartas) {
        if (c.numero == numero) {
            engine->FillRect(200, 200, 1520, 660, WHITE);
            engine->DrawRect(199, 199, 1522, 662, GREY);
            engine->DrawString(220, 220, fatosampliados[numero - 1], BLACK, 5);
        }
    }
}

struct table : olc::PixelGameEngine {
    vector<botao> botoes;
    vector<carta> cartas;
    bool OnUserCreate() {
        botoes.push_back(botao(false, WHITE, RED, "Sair", false, { {50, 980}, {150, 980}, {50, 1030}, {150, 1030} }, 0));
        cartas.push_back(carta(false, DARK_RED, 1, "   REINO DO MALI", "O rei era chamado de\n\nmansa, que significa\n\nrei dos reis,\n\nsoberano. Ele\n\ncontrolava todo o\n\nimperio, que chegou\n\na ter cerca de\n\nquatrocentas cidades\n\ne vilas, organizadas\n\nem provincias...", { 50, 50 }, false));
        cartas.push_back(carta(false, DARK_GREEN, 2, "   REINO DE GANA", "No seculo IX, o\n\nislamismo se\n\nespalhou no reino e\n\nfoi adotado\n\nprincipalmente pelos\n\nmembros da corte,\n\npelos intelectuais e\n\npelas pessoas\n\nletradas.", { 420, 50 }, false));
        cartas.push_back(carta(false, DARK_RED, 3, "  REINO DE SONGAI", "Por meio de guerras\n\nconquistou muitos\n\nterritorios. Contudo,\n\nentrou em declinio\n\nno seculo XVI.", { 790, 50 }, false));
        cartas.push_back(carta(false, DARK_GREEN, 4, "  REINO DE SONGAI", "Tinha como\n\nprincipais atividades\n\neconomicas a pesca\n\ne o comercio local.\n\nMarinheiros muito\n\nhabilidosos dominavam\n\na navegação no rio\n\nNíger, o principal\n\nda região do Sahel.", { 1160, 50 }, false));
        cartas.push_back(carta(false, DARK_RED, 5, "   REINO DE GANA", "Surgiu na Africa\n\nOcidental no seculo\n\nIII e cujo apogeu se\n\ndeu entre os seculos\n\nIX e X. Sua capital,\n\nKumbi Saleh, chegou\n\na ter entre 15 mil e\n\n20 mil habitantes", { 1530, 50 }, false));
        cartas.push_back(carta(false, DARK_GREEN, 6, "   REINO DO MALI", "Houve intensa\n\nexpansao da cultura\n\narabe islamica no\n\nreino. Cidades como\n\nDjenne e Tombuctu\n\ntornaram-se grandes\n\ncentros culturais.", { 50, 520 }, false));
        cartas.push_back(carta(false, DARK_RED, 7, "  REINO DE SONGAI", "Apos a destituicao\n\ndo imperador pelo\n\nproprio filho, a\n\nchegada dos europeus\n\ne a invasao de\n\nTombuctu pelo Reino\n\nde Marrocos, o Reino\n\nentrou em crise.", { 420, 520 }, false));
        cartas.push_back(carta(false, DARK_GREEN, 8, "   REINO DO MALI", "Segundo a tradicao\n\noral, no seculo XIII,\n\no guerreiro Sundiata\n\nKeita, conhecido como\n\nrei Leao, conquistou\n\nterritorios dando\n\norigem ao reino que\n\nera composto\n\nprincipalmente dos\n\npovos mandingas.", { 790, 520 }, false));
        cartas.push_back(carta(false, DARK_RED, 9, "   REINO DE GANA", "A base da economia\n\nera o ouro, pois\n\nhavia muitas jazidas\n\ndo precioso metal na\n\nregiao. Por causa\n\ndisso, ficou\n\nconhecido como\n\nImperio do Ouro.", { 1160, 520 }, false));
        cartas.push_back(carta(false, DARK_GREEN, 10, "  REINO DE SONGAI", "Organizou-se em um\n\ngrande Estado\n\ncentralizado,\n\ncomandado pelo\n\nimperador, e seguia\n\nrigidamente as\n\nregras do Alcorao.", { 1530, 520 }, false));
        return true;
    }
    bool destaque = false;
    int destacada;
    v2 botaosair1 = { 50, 980 };
    v2 botaosair2 = { 150, 1030 };
    bool OnUserUpdate(float dt) {
        Clear(BLACK);
        v2 mousepos = { GetMouseX(), GetMouseY() };
        if (destaque == false) {
            drawall(botoes, cartas, this);
            for (botao& b : botoes) {
                if (mousedentro(b.vertices[0], b.vertices[3], mousepos)) { b.mouse = true; }
                else { b.mouse = false; }
            }
            if (GetMouse(0).bPressed) {
                for (carta& c : cartas) {
                    if (mousedentro({c.pos.x, c.pos.y}, {c.pos.x + 370, c.pos.y + 350}, mousepos) and c.virada == false) {
                        c.virada = true;
                        v2 pos = { c.pos.x + 90, c.pos.y + 360 };
                        vector<v2> vert = { pos, {pos.x + 150, pos.y}, {pos.x, pos.y + 50}, {pos.x + 150, pos.y + 50} };
                        botoes.push_back(botao(false, WHITE, GREY, "Revelar", false, vert, c.numero));
                    }
                    else if (mousedentro({c.pos.x, c.pos.y}, {c.pos.x + 370, c.pos.y + 350}, mousepos) and c.virada) {
                        destaque = true;
                        destacada = c.numero;
                    }
                }
                for (botao& b : botoes) {
                    if (mousedentro(b.vertices[0], b.vertices[3], mousepos) and b.numero > 0) { b.apertado = true; }
                }
            }
            if (mousedentro(botaosair1, botaosair2, mousepos) and GetMouse(0).bPressed) { exit(0); }
        }
        else {
            mostrardestacada(destacada, cartas, this);

            if (GetMouse(0).bPressed and mousedentro({ 200, 200 }, { 880, 1720 }, mousepos) == false) {
                destaque = false;
            }
        }
        return true;
    }
        
};


int main()
{
    table cartas;
    cartas.Construct(1920, 1080, 1, 1, true);
    cartas.Start();
}
