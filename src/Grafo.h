
//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <iostream>
#include <vector>
#include <map>  // Para usar std::map


using namespace std;
class Grafo {
private:

    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No*> lista_adj;
    int findIndex(const vector<No*>& lista, char id);

public:
    Grafo(int ordem, bool in_direcionado, bool in_ponderado_aresta, bool in_ponderado_vertice, vector<No*> lista_adj);
    ~Grafo();
    Grafo(const string &caminho);

    void calcular_raio_diametro_centro_periferia();
    vector<int> dijkstra_distancias(char origem);
    void imprimirDistancias(char origem);
    int getIndiceNo(char id);
    No* getNoById(char id);
    void adicionaNo(char id, int peso);
    void adicionaAresta(char id_origem, char id_alvo, int peso);

    vector<No*> getListaAdj();
    int getOrdem();
    vector<char> fecho_transitivo_direto(int id_no); // a
    vector<char> fecho_transitivo_indireto(int id_no); // b
    vector<char> caminho_minimo_dijkstra(int id_no_a, int id_no_b); // c
    vector<char> caminho_minimo_floyd(int id_no, int id_no_b); // d
    Grafo* arvore_geradora_minima_prim(vector<char> ids_nos); // e
    Grafo* arvore_geradora_minima_kruskal(vector<char> ids_nos); // f
    Grafo* arvore_caminhamento_profundidade(int id_no); // g
    int raio(); // h 1
    int diametro(); // h 2
    vector<char> centro(); // h 3
    vector<char> periferia(); // h 4
    vector<char> vertices_de_articulacao(); // i
};



#endif //GRAFO_H
