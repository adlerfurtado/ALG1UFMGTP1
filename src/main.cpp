#include <iostream>
#include <cstdio>
#include <cmath>

#include "abrigos.hpp"
#include "pessoas.hpp"

using namespace std;

const int MAX_ABRIGOS = 10000; // Número máximo de abrigos, ajusta se for necessário

// Função para verificar se dois abrigos estão conectados
bool estaoConectados(const Abrigos& a, const Abrigos& b) {
    long long dx = (long long)a.getX() - b.getX();
    long long dy = (long long)a.getY() - b.getY();
    long long somaRaios = (long long)a.getRaio() + b.getRaio();
    return dx * dx + dy * dy <= somaRaios * somaRaios;
}

// Função para verificar se uma pessoa está dentro de um abrigo
bool pessoaDentroDoAbrigo(const Pessoas& p, const Abrigos& a) {
    long long dx = (long long)p.getX() - a.getX();
    long long dy = (long long)p.getY() - a.getY();
    long long raio = (long long)a.getRaio();
    return dx * dx + dy * dy <= raio * raio;
}

int bfsMaisDistante(int** grafo, int* grau, int numAbrigos, int inicio, int& maisDistante) {
    bool* visitado = new bool[numAbrigos];
    int* distancia = new int[numAbrigos];

    for (int i = 0; i < numAbrigos; ++i) {
        visitado[i] = false;
        distancia[i] = -1;
    }

    int* fila = new int[numAbrigos];
    int ini = 0, fim = 0;

    fila[fim++] = inicio;
    visitado[inicio] = true;
    distancia[inicio] = 0;

    int maxDist = 0;
    maisDistante = inicio;

    while (ini < fim) {
        int atual = fila[ini++];

        for (int i = 0; i < grau[atual]; ++i) {
            int viz = grafo[atual][i];
            if (!visitado[viz]) {
                visitado[viz] = true;
                distancia[viz] = distancia[atual] + 1;
                fila[fim++] = viz;

                if (distancia[viz] > maxDist) {
                    maxDist = distancia[viz];
                    maisDistante = viz;
                }
            }
        }
    }

    delete[] visitado;
    delete[] distancia;
    delete[] fila;

    return maxDist;
}

int calcularDistanciaMaxima(int** grafo, int* grau, int numAbrigos) {
    bool* visitadoGlobal = new bool[numAbrigos];
    for (int i = 0; i < numAbrigos; ++i) {
        visitadoGlobal[i] = false;
    }

    int maiorDiametro = 0;

    for (int i = 0; i < numAbrigos; ++i) {
        if (!visitadoGlobal[i]) {
            int pontoDistante;

            // Primeira BFS para encontrar o ponto mais distante da componente
            bfsMaisDistante(grafo, grau, numAbrigos, i, pontoDistante);

            // Segunda BFS para calcular o diâmetro da componente
            int diametro = bfsMaisDistante(grafo, grau, numAbrigos, pontoDistante, pontoDistante);

            if (diametro > maiorDiametro) {
                maiorDiametro = diametro;
            }

            // Marca todos os vértices da componente como visitados
            int* fila = new int[numAbrigos];
            int ini = 0, fim = 0;
            fila[fim++] = i;
            visitadoGlobal[i] = true;

            while (ini < fim) {
                int atual = fila[ini++];
                for (int j = 0; j < grau[atual]; ++j) {
                    int viz = grafo[atual][j];
                    if (!visitadoGlobal[viz]) {
                        visitadoGlobal[viz] = true;
                        fila[fim++] = viz;
                    }
                }
            }

            delete[] fila;
        }
    }

    delete[] visitadoGlobal;
    return maiorDiametro;
}

// Função auxiliar para encontrar abrigos críticos (DFS)
void encontrarCriticosDFS(int u, int pai, int& tempo,
    int** grafo, int* grau,
    bool* visitado,
    int* tempoDescoberta,
    int* menorTempo,
    bool* ehCritico) {

    visitado[u] = true;
    tempoDescoberta[u] = menorTempo[u] = tempo++;
    int filhos = 0;

    for (int i = 0; i < grau[u]; ++i) {
        int v = grafo[u][i];
        if (!visitado[v]) {
            filhos++;
            encontrarCriticosDFS(v, u, tempo, grafo, grau, visitado, tempoDescoberta, menorTempo, ehCritico);
            menorTempo[u] = (menorTempo[u] < menorTempo[v]) ? menorTempo[u] : menorTempo[v];

            if (pai != -1 && menorTempo[v] >= tempoDescoberta[u]) {
                ehCritico[u] = true;
            }
        } else if (v != pai) {
            menorTempo[u] = (menorTempo[u] < tempoDescoberta[v]) ? menorTempo[u] : tempoDescoberta[v];
        }
    }

    if (pai == -1 && filhos > 1) {
        ehCritico[u] = true;
    }
}

// Função para encontrar o menor caminho entre Ana e Bruno
int encontrarCaminhoEntreAnaEBruno(int** grafo, int* grau, int numAbrigos, int* abrigosAna, int qtdAna, int* abrigosBruno, int qtdBruno) {
    bool* visitado = new bool[numAbrigos];
    int* distancia = new int[numAbrigos];

    // Inicializa as variáveis
    for (int i = 0; i < numAbrigos; ++i) {
        visitado[i] = false;
        distancia[i] = -1;
    }

    // Fila com array simples
    int* fila = new int[numAbrigos];
    int ini = 0, fim = 0;

    // Começa a BFS a partir de todos os abrigos onde Ana pode estar
    for (int i = 0; i < qtdAna; ++i) {
        int abrigo = abrigosAna[i];
        fila[fim++] = abrigo;
        visitado[abrigo] = true;
        distancia[abrigo] = 0;
    }

    while (ini < fim) {
        int atual = fila[ini++];

        // Verifica se Bruno pode estar neste abrigo
        for (int i = 0; i < qtdBruno; ++i) {
            if (abrigosBruno[i] == atual) {
                int resultado = distancia[atual];
                delete[] fila;
                delete[] visitado;
                delete[] distancia;
                return resultado; // Retorna a distância quando encontrar
            }
        }

        // Expande os vizinhos
        for (int i = 0; i < grau[atual]; ++i) {
            int vizinho = grafo[atual][i];
            if (!visitado[vizinho]) {
                visitado[vizinho] = true;
                distancia[vizinho] = distancia[atual] + 1;
                fila[fim++] = vizinho;
            }
        }
    }

    // Libera memória
    delete[] fila;
    delete[] visitado;
    delete[] distancia;
    return -1; // Caso Bruno não seja alcançável
}

int main() {
    int coordenadaX = 0, coordenadaY = 0, numAbrigos = 0, raio = 0;

    // Lê coordenadas da Ana
    scanf("%d %d", &coordenadaX, &coordenadaY);
    Pessoas Ana(coordenadaX, coordenadaY);

    // Lê coordenadas do Bruno
    scanf("%d %d", &coordenadaX, &coordenadaY);
    Pessoas Bruno(coordenadaX, coordenadaY);

    // Lê número de abrigos
    scanf("%d", &numAbrigos);
    Abrigos* abrigos = new Abrigos[numAbrigos];

    // Lê as informações de cada abrigo
    for (int i = 0; i < numAbrigos; ++i) {
        if (scanf("%d %d %d", &raio, &coordenadaX, &coordenadaY) == 3) {
            abrigos[i] = Abrigos(raio, coordenadaX, coordenadaY);
        } else {
            cerr << "Erro ao ler dados do abrigo " << i << endl;
            delete[] abrigos;
            return 1;
        }
    }

    // Cria o grafo de abrigos
    int** grafo = new int*[numAbrigos];
    int* grau = new int[numAbrigos];

    for (int i = 0; i < numAbrigos; ++i) {
        grafo[i] = new int[numAbrigos]; // Aloca espaço máximo por simplicidade
        grau[i] = 0;
    }

    // Conecta os abrigos no grafo
    for (int i = 0; i < numAbrigos; ++i) {
        for (int j = i + 1; j < numAbrigos; ++j) {
            if (estaoConectados(abrigos[i], abrigos[j])) {
                grafo[i][grau[i]++] = j;
                grafo[j][grau[j]++] = i;
            }
        }
    }

    // Parte 1: Identifica os abrigos de Ana e Bruno
    int* abrigosAna = new int[numAbrigos];
    int* abrigosBruno = new int[numAbrigos];
    int qtdAna = 0, qtdBruno = 0;

    for (int i = 0; i < numAbrigos; ++i) {
        if (pessoaDentroDoAbrigo(Ana, abrigos[i])) {
            abrigosAna[qtdAna++] = i;
        }
        if (pessoaDentroDoAbrigo(Bruno, abrigos[i])) {
            abrigosBruno[qtdBruno++] = i;
        }
    }

    // Busca o caminho mais curto entre Ana e Bruno
    int resultadoParte1 = encontrarCaminhoEntreAnaEBruno(grafo, grau, numAbrigos, abrigosAna, qtdAna, abrigosBruno, qtdBruno);
    cout << "Parte 1: " << resultadoParte1 << endl;
    
    // Parte 2: Calcula a distância máxima entre abrigos
    int resultadoParte2 = calcularDistanciaMaxima(grafo, grau, numAbrigos);
    cout << "Parte 2: " << resultadoParte2 << endl;

    // Parte 3: Encontra os abrigos críticos
    bool* visitado = new bool[numAbrigos];
    int* tempoDescoberta = new int[numAbrigos];
    int* menorTempo = new int[numAbrigos];
    bool* ehCritico = new bool[numAbrigos];

    for (int i = 0; i < numAbrigos; ++i) {
        visitado[i] = false;
        tempoDescoberta[i] = -1;
        menorTempo[i] = -1;
        ehCritico[i] = false;
    }

    int tempo = 0;
    for (int i = 0; i < numAbrigos; ++i) {
        if (!visitado[i]) {
            encontrarCriticosDFS(i, -1, tempo, grafo, grau, visitado, tempoDescoberta, menorTempo, ehCritico);
        }
    }

    // Coleta os índices dos abrigos críticos
    int* abrigosCriticos = new int[numAbrigos];
    int qtdCriticos = 0;

    for (int i = 0; i < numAbrigos; ++i) {
        if (ehCritico[i]) {
            abrigosCriticos[qtdCriticos++] = i + 1;
        }
    }

    cout << "Parte 3: " << qtdCriticos;
    for (int i = 0; i < qtdCriticos; ++i) {
        cout << " " << abrigosCriticos[i];
    }
    cout << endl;

    // Libera memória
    for (int i = 0; i < numAbrigos; ++i) {
        delete[] grafo[i];
    }
    delete[] grafo;
    delete[] grau;
    delete[] abrigos;
    delete[] visitado;
    delete[] tempoDescoberta;
    delete[] menorTempo;
    delete[] ehCritico;
    delete[] abrigosCriticos;
    delete[] abrigosAna;
    delete[] abrigosBruno;

    return 0;
}
