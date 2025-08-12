#include "Grafo.h"
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <stack>
#include <ctime>
#include <math.h>


Grafo::Grafo(int ordem, bool in_direcionado, bool in_ponderado_aresta, bool in_ponderado_vertice, vector<No*> lista_adj) {
    this->ordem = ordem;
    this->in_direcionado = in_direcionado;
    this->in_ponderado_aresta = in_ponderado_aresta;
    this->in_ponderado_vertice = in_ponderado_vertice;
    this->lista_adj = lista_adj;
}

Grafo::Grafo(const string &caminho)
{

    this->ordem = 0;
    ifstream arquivo(caminho);
    string linha;

    if (!arquivo.is_open())
    {
        cerr << "Nao foi possivel abrir o arquivo: " << caminho << endl;
        return;
    }

    cout << "Iniciando leitura do grafo a partir do arquivo: " << caminho << endl;

    if (getline(arquivo, linha))
    {
        stringstream ss(linha);
        int eh_direcionado, pondera_arestas, pondera_vertices;
        ss >> eh_direcionado >> pondera_arestas >> pondera_vertices;

        this->in_direcionado = (eh_direcionado == 1);
        this->in_ponderado_aresta = (pondera_arestas == 1);
        this->in_ponderado_vertice = (pondera_vertices == 1);
    }

    int total_vertices = 0;
    if (getline(arquivo, linha))
    {
        stringstream ss(linha);
        ss >> total_vertices;
    }

    for (int i = 0; i < total_vertices; ++i)
    {
        if (getline(arquivo, linha))
        {
            stringstream ss(linha);
            char identificador;
            int peso_vertice = 0;

            ss >> identificador;
            if (this->in_ponderado_vertice)
            {
                ss >> peso_vertice;
            }

            this->adicionaNo(identificador, peso_vertice);
        }
    }

    while (getline(arquivo, linha))
    {
        stringstream ss(linha);
        char origem_id, destino_id;
        int peso_aresta = 0;

        ss >> origem_id >> destino_id;

        if (this->in_ponderado_aresta)
        {
            ss >> peso_aresta;
        }

        this->adicionaAresta(origem_id, destino_id, peso_aresta);
    }

    arquivo.close();

    cout << "Leitura do grafo concluida!" << endl;
}

Grafo::~Grafo()
{
    for (No *no : lista_adj)
    delete no;
}

vector<No*> Grafo::getListaAdj()
{
    return lista_adj;
}

int Grafo::getOrdem()
{
    return ordem;
}

void Grafo::adicionaNo(char id, int peso)
{
    No* no = new No(id, peso, {});
    lista_adj.push_back(no);
    this->ordem++;
}

void Grafo::adicionaAresta(char id_origem, char id_alvo, int peso)
{
    No* no_origem = nullptr;
    No* no_destino = nullptr;

    for (int i = 0; i < lista_adj.size(); ++i)
    {
        No* no = lista_adj[i];

        if (no->getId() == id_origem)
        {
            no_origem = no;
        }

        if (no->getId() == id_alvo)
        {
            no_destino = no;
        }

        if (no_origem && no_destino)
        {
            break;
        }
    }

    if (no_origem != nullptr)
    {
        Aresta* nova_aresta = new Aresta(id_origem, id_alvo, peso);
        no_origem->adicionarAresta(nova_aresta);
    }

    if (!in_direcionado && no_origem != nullptr && no_destino != nullptr)
    {
        Aresta* aresta_inversa = new Aresta(id_alvo, id_alvo, peso);
        no_destino->adicionarAresta(aresta_inversa);
    }
}

vector<char> Grafo::fecho_transitivo_direto(int id_no) {
    vector<char> visitados; //Armazena nós já visitados para evitar repetição.
    vector<char> pilha; //Pilha para busca em profundidade

    pilha.push_back((char)id_no);

    while (!pilha.empty())
    {
        char id_atual = pilha.back(); //Pega o último elemento da fila
        pilha.pop_back(); //Remove da pilha

        No* atual = 0;
        for (int i = 0; i < lista_adj.size(); i++)
        {
            if (lista_adj[i]->getId() == id_atual)
            {
                atual = lista_adj[i];
                break;
            }
        }

        if (atual == 0) continue; //Verifica se encontrou o nó

        vector<Aresta*> arestas = atual->getArestas();

        for (int i = 0; i < arestas.size(); i++) //Percorre as arestas do nó atual
        {
            char id_alvo = arestas[i]->getIdAlvo();

            bool ja_visitado = false; //Evita repetição
            for (int j = 0; j < visitados.size(); j++)
            {
                if (visitados[j] == id_alvo)
                {
                    ja_visitado = true;
                    break;
                }
            }

            if (!ja_visitado)
            {
                visitados.push_back(id_alvo); //Marcado como visitado
                pilha.push_back(id_alvo); //Explorar esse nó depois
            }
        }
    }

    return visitados; //Retorna todos os nós alcançáveis a partir de id_no
}

vector<char> Grafo::fecho_transitivo_indireto(int id_no) {
    vector<char> resultado; //Armazena os nós que alcançam o id_no

    for (int i = 0; i < lista_adj.size(); i++) //Percorre os nós do grafo
    {
        char id_atual = lista_adj[i]->getId();

        if (id_atual == id_no) continue; //Evita a busca de caminho de um nó para ele mesmo

        vector<char> visitados; //Armazena nós já visitados para evitar repetição.
        vector<char> pilha; //Pilha para busca em profundidade
        pilha.push_back(id_atual); //Começa busca pelo nó

        while (!pilha.empty()) //Busca em profundidade
        {
            char topo = pilha.back();
            pilha.pop_back();

            bool ja_visitado = false;
            for (int v = 0; v < visitados.size(); v++) //Verifica se já foi visitado
            {
                if (visitados[v] == topo)
                {
                    ja_visitado = true;
                    break;
                }
            }
            if (ja_visitado) continue;

            visitados.push_back(topo);

            if (topo == id_no)
            {
                resultado.push_back(id_atual); //Adiciona id_atual ao resultado
                break; //Caminho encontrado, logo, não é necessário continuar a busca
            }

            No* no_topo = 0;
            for (int j = 0; j < lista_adj.size(); j++) //Continua a busca em profundidade a partir das arestas do topo
            {
                if (lista_adj[j]->getId() == topo)
                {
                    no_topo = lista_adj[j];
                    break;
                }
            }

            if (no_topo != 0)
            {
                vector<Aresta*> arestas = no_topo->getArestas();
                for (int k = 0; k < arestas.size(); k++)
                {
                    char id_destino = arestas[k]->getIdAlvo();
                    pilha.push_back(id_destino);
                }
            }
        }
    }

    return resultado; //Retorna nós com caminho até id_no
}

vector<char> Grafo::caminho_minimo_dijkstra(int id_no_a, int id_no_b) {
    const int infinito = 1000000; //Representa um valor "infinito"
    vector<char> vertices; //Guarda ids dos nós
 
    for (int i = 0; i < lista_adj.size(); i++)
    {
        vertices.push_back(lista_adj[i]->getId());
    }

    int dist[256]; //Distância mínima até cada nó
    bool visitado[256] = {false}; //Verifica nós já visitados
    char anterior[256]; //Guarda o anterior a cada nó visitado

    for (int i = 0; i < 256; i++)
    {
        dist[i] = infinito;
        anterior[i] = '\0';
    }

    dist[(int)id_no_a] = 0; //Distância até o nó origem é 0

    while (true) //loop Dijkstra
    {
        char atual = '\0';
        int menor_dist = infinito;

        for (int i = 0; i < vertices.size(); i++) //Seleciona nó com menor distância ainda não visitado
        {
            char id = vertices[i];
            if (!visitado[(int)id] && dist[(int)id] < menor_dist)
            {
                menor_dist = dist[(int)id];
                atual = id;
            }
        }

        if (atual == '\0') break; //Não há mais nós acessíveis
        if (atual == id_no_b) break; //Atingiu nó alvo

        visitado[(int)atual] = true;

        No* no_atual = nullptr;
        for (int i = 0; i < lista_adj.size(); i++) //Atualiza distância dos vizinhos
        {
            if (lista_adj[i]->getId() == atual)
            {
                no_atual = lista_adj[i];
                break;
            }
        }

        if (no_atual == nullptr) continue;

        vector<Aresta*> arestas = no_atual->getArestas();
        for (int j = 0; j < arestas.size(); j++) //Atualiza se encontrar melhor caminho
        {
            char vizinho = arestas[j]->getIdAlvo();
            int peso = arestas[j]->getPeso();

            if (dist[(int)vizinho] > dist[(int)atual] + peso)
            {
                dist[(int)vizinho] = dist[(int)atual] + peso;
                anterior[(int)vizinho] = atual; //Guarda anterior
            }
        }
    }

    vector<char> caminho;
    char atual = id_no_b;

    while (atual != '\0') //Reconstrução do caminho final
    {
        caminho.insert(caminho.begin(), atual);
        atual = anterior[(int)atual];
    }

    if (caminho.size() == 0 || caminho[0] != id_no_a) //Caso não haja caminho válido
    {
        return {};
    }

    return caminho;
}

vector<char> Grafo::caminho_minimo_floyd(int id_no, int id_no_b) {
    const int INF = 1000000;
    int n = ordem;

    // Mapeamento dos IDs para índices 0..n-1
    vector<char> vertices;
    for (auto no : lista_adj) {
        vertices.push_back(no->getId());
    }

    // Criar matriz de distâncias e matriz de próximos para reconstrução
    vector<vector<int>> dist(n, vector<int>(n, INF));
    vector<vector<int>> next(n, vector<int>(n, -1));

    // Preencher distâncias iniciais
    for (int i = 0; i < n; i++) {
        dist[i][i] = 0;
    }

    // Preencher as distâncias entre vértices adjacentes
    for (int i = 0; i < n; i++) {
        No* no = lista_adj[i];
        for (auto aresta : no->getArestas()) {
            // encontrar índice do nó alvo
            char alvo = aresta->getIdAlvo();
            int j = -1;
            for (int k = 0; k < n; k++) {
                if (vertices[k] == alvo) {
                    j = k;
                    break;
                }
            }
            if (j != -1) {
                dist[i][j] = aresta->getPeso();
                next[i][j] = j;
            }
        }
    }

    // Floyd-Warshall
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] == INF || dist[k][j] == INF) continue;
                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    // Encontrar índices dos vértices de entrada
    int start = -1, end = -1;
    for (int i = 0; i < n; i++) {
        if (vertices[i] == id_no) start = i;
        if (vertices[i] == id_no_b) end = i;
    }

    if (start == -1 || end == -1) {
        // Vértices não encontrados
        return {};
    }

    if (next[start][end] == -1) {
        // Não há caminho
        return {};
    }

    // Reconstruir caminho
    vector<char> caminho;
    int u = start;
    while (u != end) {
        caminho.push_back(vertices[u]);
        u = next[u][end];
        if (u == -1) return {}; // não há caminho
    }
    caminho.push_back(vertices[end]);

    return caminho;
}

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    int n = (int)ids_nos.size();
    if (n == 0) return nullptr;

    // Construir subgrafo induzido: só nós e arestas que ligam nós em ids_nos
    vector<No*> sub_lista_adj;

    // Mapa para saber se um vértice pertence ao subconjunto
    set<char> conjunto(ids_nos.begin(), ids_nos.end());

    for (char id : ids_nos) {
        int idx = findIndex(lista_adj, id);
        if (idx == -1) {
            // Se algum vértice não existir, retornar nullptr
            return nullptr;
        }
        No* no_orig = lista_adj[idx];
        vector<Aresta*> arestas_filtradas;

        // Filtrar só as arestas que apontam para vértices do subconjunto
        for (Aresta* a : no_orig->getArestas()) {
            if (conjunto.count(a->getIdAlvo()) > 0) {
                arestas_filtradas.push_back(new Aresta(a->getIdOrigem(), a->getIdAlvo(), a->getPeso()));
            }
        }

        // Criar novo nó para o subgrafo com as arestas filtradas
        No* novo_no = new No(no_orig->getId(), no_orig->getPeso(), arestas_filtradas);
        sub_lista_adj.push_back(novo_no);
    }

    // Agora aplicar Prim no subgrafo
    // Vetores para controle
    vector<bool> in_tree(n, false);
    vector<int> key(n, 1000000); // valor infinito
    vector<int> parent(n, -1);

    // Começar do primeiro vértice do subconjunto
    key[0] = 0;

    for (int count = 0; count < n - 1; count++) {
        // Escolher vértice com menor key ainda fora da árvore
        int u = -1;
        int min_key = 1000000;
        for (int v = 0; v < n; v++) {
            if (!in_tree[v] && key[v] < min_key) {
                min_key = key[v];
                u = v;
            }
        }

        if (u == -1) break; // desconexo

        in_tree[u] = true;

        // Atualizar valores key e parent para os vizinhos de u
        No* no_u = sub_lista_adj[u];
        for (Aresta* a : no_u->getArestas()) {
            char alvo = a->getIdAlvo();
            int peso = a->getPeso();
            int v = -1;
            for (int i = 0; i < n; i++) {
                if (sub_lista_adj[i]->getId() == alvo) {
                    v = i;
                    break;
                }
            }

            if (v != -1 && !in_tree[v] && peso < key[v]) {
                key[v] = peso;
                parent[v] = u;
            }
        }
    }

    // Construir o grafo AGM resultado com os vértices do subconjunto e arestas da AGM
    vector<No*> agm_lista_adj;

    // Criar nós sem arestas (vamos adicionar depois)
    for (int i = 0; i < n; i++) {
        No* novo_no = new No(sub_lista_adj[i]->getId(), sub_lista_adj[i]->getPeso(), {});
        agm_lista_adj.push_back(novo_no);
    }

    // Adicionar arestas da AGM: para cada v != 0, adicionar aresta parent[v] <-> v (bidirecional)
    for (int v = 1; v < n; v++) {
        int p = parent[v];
        if (p != -1) {
            char id_p = agm_lista_adj[p]->getId();
            char id_v = agm_lista_adj[v]->getId();
            int peso = key[v];

            // Como seu grafo original é direcionado, mas a AGM é geralmente não direcionada,
            // vamos adicionar as arestas nos dois sentidos

            Aresta* aresta1 = new Aresta(id_p, id_v, peso);
            Aresta* aresta2 = new Aresta(id_v, id_p, peso);

            agm_lista_adj[p]->adicionarAresta(aresta1);
            agm_lista_adj[v]->adicionarAresta(aresta2);
        }
    }

    // Liberar subgrafo temporário criado (arestas e nós)
    for (No* no : sub_lista_adj) {
        for (Aresta* a : no->getArestas()) {
            delete a;
        }
        delete no;

    }
    // Retornar novo grafo AGM
    return new Grafo(n, false, true, false, agm_lista_adj);
}

// Função auxiliar para encontrar índice de um vértice no vetor lista_adj
int Grafo::findIndex(const vector<No*>& lista, char id) {
    for (int i = 0; i < (int)lista.size(); i++) {
        if (lista[i]->getId() == id)
            return i;
    }
    return -1;
}

Grafo* Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    if (ids_nos.empty()) return nullptr;

    // 1. Coletar todas as arestas válidas entre os nós do subconjunto
    vector<tuple<char, char, int>> arestas; // (origem, destino, peso)
    
    for (char id : ids_nos) {
        No* no = getNoById(id);
        if (!no) continue;
        
        for (Aresta* a : no->getArestas()) {
            char destino = a->getIdAlvo();
            if (find(ids_nos.begin(), ids_nos.end(), destino) != ids_nos.end()) {
                arestas.emplace_back(id, destino, a->getPeso());
            }
        }
    }

    // 2. Ordenar arestas por peso
    sort(arestas.begin(), arestas.end(), 
        [](const auto& a, const auto& b) { return get<2>(a) < get<2>(b); });

    // 3. Union-Find (Disjoint Set)
    unordered_map<char, char> parent;
    for (char id : ids_nos) {
        parent[id] = id;
    }
    
    auto find = [&](char u) {
        while (parent[u] != u) {
            parent[u] = parent[parent[u]]; // Path compression
            u = parent[u];
        }
        return u;
    };

    // 4. Construir AGM
    vector<No*> nos_agm;
    unordered_map<char, No*> no_map;
    
    for (char id : ids_nos) {
        No* novo_no = new No(id, 0); // Corrigido de M0 para No
        nos_agm.push_back(novo_no);
        no_map[id] = novo_no;
    }

    for (const auto& aresta : arestas) { // Modificado para evitar problemas com structured binding
        char u = get<0>(aresta);
        char v = get<1>(aresta);
        int peso = get<2>(aresta);
        
        char root_u = find(u);
        char root_v = find(v);
        
        if (root_u != root_v) {
            parent[root_v] = root_u;
            no_map[u]->adicionarAresta(new Aresta(u, v, peso));
            no_map[v]->adicionarAresta(new Aresta(v, u, peso)); // Para grafo não direcionado
        }
    }

    return new Grafo(ids_nos.size(), false, true, false, nos_agm);
}

Grafo* Grafo::arvore_caminhamento_profundidade(int id_no) {
    No* no_inicial = getNoById(id_no);
    if (!no_inicial) return nullptr;

    // Estruturas para a busca
    unordered_map<char, bool> visitado;
    vector<No*> nos_arvore;
    unordered_map<char, No*> no_map;

    // Criar nó raiz
    No* raiz = new No(id_no, 0);
    nos_arvore.push_back(raiz);
    no_map[id_no] = raiz;
    visitado[id_no] = true;

    // Pilha para DFS: (nó atual, nó pai na árvore)
    stack<pair<char, char>> pilha;
    pilha.push({id_no, '\0'});

    while (!pilha.empty()) {
        char atual_id = pilha.top().first;
        char pai_id = pilha.top().second;
        pilha.pop();

        No* atual = getNoById(atual_id);
        if (!atual) continue;

        for (Aresta* a : atual->getArestas()) {
            char vizinho_id = a->getIdAlvo();
            
            if (!visitado[vizinho_id]) {
                // É uma aresta da árvore
                No* novo_no = new No(vizinho_id, 0);
                nos_arvore.push_back(novo_no);
                no_map[vizinho_id] = novo_no;
                no_map[atual_id]->adicionarAresta(new Aresta(atual_id, vizinho_id, a->getPeso()));
                visitado[vizinho_id] = true;
                pilha.push({vizinho_id, atual_id});
            }
            else if (vizinho_id != pai_id) {
                // É uma aresta de retorno (não adicionamos à árvore, mas poderíamos marcar)
                // Para destacar, poderíamos adicionar como uma aresta especial
                // Nesta implementação, estamos ignorando arestas de retorno
            }
        }
    }

    return new Grafo(nos_arvore.size(), false, true, false, nos_arvore);
}

int Grafo::getIndiceNo(char id) {
    for (int i = 0; i < (int)lista_adj.size(); i++) {
        if (lista_adj[i]->getId() == id)
            return i;
    }
    return -1;
}

No* Grafo::getNoById(char id) {
    for (int i = 0; i < (int)lista_adj.size(); i++) {
        if (lista_adj[i]->getId() == id)
            return lista_adj[i];
    }
    return nullptr;
}

vector<int> Grafo::dijkstra_distancias(char origem) {
    int n = lista_adj.size();
    vector<int> dist(n, 999999); // 999999 como "infinito"
    vector<bool> visitado(n, false);

    int origem_idx = getIndiceNo(origem);
    dist[origem_idx] = 0;

    for (int count = 0; count < n - 1; count++) {
        // Encontra o vértice com menor distância ainda não visitado
        int min_dist = 999999;
        int u = -1;

        for (int i = 0; i < n; i++) {
            if (!visitado[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }

        if (u == -1) break;

        visitado[u] = true;
        No* no_u = lista_adj[u];

        for (int j = 0; j < (int)no_u->getArestas().size(); j++) {
            Aresta* aresta = no_u->getArestas()[j]; 
            char destino = aresta->getIdAlvo();;
            int peso = aresta->getPeso();
            int v = getIndiceNo(destino);

            if (!visitado[v] && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
            }
        }
    }

    return dist;
}

void Grafo::imprimirDistancias(char origem) {
    vector<int> dist = dijkstra_distancias(origem);
    cout << "Distancias a partir do vertice " << origem << ":\n";
    for (int i = 0; i < (int)lista_adj.size(); i++) {
        cout << origem << " -> " << lista_adj[i]->getId() << " = ";
        if (dist[i] >= 999999)
            cout << "infinito\n";
        else
            cout << dist[i] << "\n";
    }
}

void Grafo::calcular_raio_diametro_centro_periferia() {
    int n = lista_adj.size();
    vector<int> excentricidades(n, 0); //Para armazenar a excentricidade de cada vértice

    //Calcula excentricidade de cada vértice
    for (int i = 0; i < n; i++) {
        char id_origem = lista_adj[i]->getId();
        vector<int> dist = dijkstra_distancias(id_origem);

        int maior_dist = -1;
        for (int j = 0; j < n; j++) {
            if (dist[j] < 999999 && dist[j] > maior_dist) {
                maior_dist = dist[j];
            }
        }
        excentricidades[i] = maior_dist;
    }

    //Calcula raio e diametro
    int raio = 999999;
    int diametro = -1;

    for (int i = 0; i < n; i++) {
        if (excentricidades[i] < raio) {
            raio = excentricidades[i];
        }
        if (excentricidades[i] > diametro) {
            diametro = excentricidades[i];
        }
    }

    //Encontra centro e periferia
    vector<char> centro;
    vector<char> periferia;

    for (int i = 0; i < n; i++) {
        if (excentricidades[i] == raio) {
            centro.push_back(lista_adj[i]->getId());
        }
        if (excentricidades[i] == diametro) {
            periferia.push_back(lista_adj[i]->getId());
        }
    }

    //Impressão
    cout << "Raio: " << raio << endl;
    cout << "Diametro: " << diametro << endl;

    cout << "Centro: ";
    for (int i = 0; i < (int)centro.size(); i++) {
        cout << centro[i];
    if (i < (int)centro.size() - 1)
    cout << ",";
    }
    cout << endl;

    cout << "Periferia: ";
    for (int i = 0; i < (int)periferia.size(); i++) {
        cout << periferia[i];
    if (i < (int)periferia.size() - 1)
    cout << ",";
    }
    cout << endl << endl;
}

vector<char> Grafo::guloso()
{
    vector<char> resultado;
    vector<char> vertices_restantes;

    // Preenchendo a lista com todos os vértices.
    for (No* no : lista_adj)
        vertices_restantes.push_back(no->getId());

    while (!vertices_restantes.empty())
    {
        // Escolhe o vértice de maior grau.
        char v = vertices_restantes.front();
        int maior_grau = -1;

        for (char u : vertices_restantes)
        {
            No* no_u = getNoById(u);
            int grau_u = no_u->getArestas().size();
            if (grau_u > maior_grau)
            {
                maior_grau = grau_u;
                v = u;
            }
        }

        resultado.push_back(v);

        // Remove v e todos os vizinhos.
        vector<char> novos_restantes;
        for (char u : vertices_restantes)
        {
            if (u == v) continue;

            // Condicional, se é vizinho de v.
            No* no_v = getNoById(v);
            bool ehVizinho = false;
            for (Aresta* a : no_v->getArestas())
            {
                if (a->getIdAlvo() == u)
                {
                    ehVizinho = true;
                    break;
                }
            }

            if (!ehVizinho)
                novos_restantes.push_back(u);
        }

        vertices_restantes = novos_restantes;
    }

    return resultado;
}

vector<char> Grafo::guloso_aleatorio(int iteracoes, float alfa)
{
    srand(time(NULL)); // semente aleatória

    vector<char> melhor_solucao;
    int melhor_tamanho = lista_adj.size() + 1; // número total de vértices + 1, como se fosse um infinito

    for (int it = 0; it < iteracoes; it++) // marca as iterações
    {
        vector<char> resultado; // solução de cada iteração
        vector<char> vertices_restantes; // vértices que ainda não foram cobertos

        for (No* no : lista_adj) // preenche com todos os vértices
            vertices_restantes.push_back(no->getId()); 

        while (!vertices_restantes.empty())
        {
            // Lista de candidatos: (vértice, grau)
            vector<pair<char, int>> candidatos;
            for (char u : vertices_restantes)
            {
                No* no_u = getNoById(u);
                int grau_u = no_u->getArestas().size();
                candidatos.push_back({u, grau_u});
            }

            // Ordena por grau decrescente
            sort(candidatos.begin(), candidatos.end(),
                 [](auto &a, auto &b) { return a.second > b.second; });

            // Define tamanho da LRC
            int tamanho_LRC = max(1, (int)ceil(alfa * candidatos.size()));

            // Escolhe aleatoriamente um vértice da LRC
            int indice_escolhido = rand() % tamanho_LRC;
            char v = candidatos[indice_escolhido].first;

            resultado.push_back(v);

            // Remove v e seus vizinhos
            vector<char> novos_restantes;
            No* no_v = getNoById(v);

            for (char u : vertices_restantes)
            {
                if (u == v) continue;

                bool ehVizinho = false;
                for (Aresta* a : no_v->getArestas())
                {
                    if (a->getIdAlvo() == u)
                    {
                        ehVizinho = true;
                        break;
                    }
                }

                if (!ehVizinho)
                    novos_restantes.push_back(u);
            }

            vertices_restantes = novos_restantes;
        }

        // Atualiza melhor solução
        if ((int)resultado.size() < melhor_tamanho)
        {
            melhor_solucao = resultado;
            melhor_tamanho = resultado.size();
        }
    }

    return melhor_solucao;
}


vector<char> Grafo::guloso_aleatorio_reativo(int iteracoes, vector<float> alfas, int bloco)
{
 srand(time(NULL));

    vector<char> melhor_solucao_global;
    int melhor_tamanho_global = lista_adj.size() + 1;

    // Probabilidades iniciais iguais para todos os alfas
    vector<float> prob(alfas.size(), 1.0 / alfas.size());

    // Soma dos custos médios para cada alfa
    vector<float> soma_resultados(alfas.size(), 0.0);
    vector<int> cont_resultados(alfas.size(), 0);

    int iter_bloco = 0;

    for (int it = 0; it < iteracoes; it++)
    {
        // Escolhe um alfa com base nas probabilidades
        float r = (float)rand() / RAND_MAX;
        float acum = 0.0;
        int idx_alfa = 0;

        for (size_t i = 0; i < prob.size(); i++)
        {
            acum += prob[i];
            if (r <= acum)
            {
                idx_alfa = i;
                break;
            }
        }

        float alfa_escolhido = alfas[idx_alfa];

        // Executa guloso aleatório com este alfa
        vector<char> resultado;
        vector<char> vertices_restantes;

        for (No* no : lista_adj)
            vertices_restantes.push_back(no->getId());

        while (!vertices_restantes.empty())
        {
            vector<pair<char, int>> candidatos;
            for (char u : vertices_restantes)
            {
                No* no_u = getNoById(u);
                int grau_u = no_u->getArestas().size();
                candidatos.push_back({u, grau_u});
            }

            sort(candidatos.begin(), candidatos.end(),
                 [](auto &a, auto &b) { return a.second > b.second; });

            int tamanho_LRC = max(1, (int)ceil(alfa_escolhido * candidatos.size()));
            int indice_escolhido = rand() % tamanho_LRC;
            char v = candidatos[indice_escolhido].first;

            resultado.push_back(v);

            vector<char> novos_restantes;
            No* no_v = getNoById(v);

            for (char u : vertices_restantes)
            {
                if (u == v) continue;

                bool ehVizinho = false;
                for (Aresta* a : no_v->getArestas())
                {
                    if (a->getIdAlvo() == u)
                    {
                        ehVizinho = true;
                        break;
                    }
                }

                if (!ehVizinho)
                    novos_restantes.push_back(u);
            }

            vertices_restantes = novos_restantes;
        }

        // Guarda estatísticas para este alfa
        soma_resultados[idx_alfa] += resultado.size();
        cont_resultados[idx_alfa]++;

        // Atualiza melhor solução global
        if ((int)resultado.size() < melhor_tamanho_global)
        {
            melhor_solucao_global = resultado;
            melhor_tamanho_global = resultado.size();
        }

        iter_bloco++;

        // Se terminou um bloco, ajusta probabilidades
        if (iter_bloco == bloco)
        {
            vector<float> qualidade(alfas.size(), 0.0);
            float soma_qualidade = 0.0;

            for (size_t i = 0; i < alfas.size(); i++)
            {
                if (cont_resultados[i] > 0)
                {
                    qualidade[i] = 1.0 / (soma_resultados[i] / cont_resultados[i]);
                    soma_qualidade += qualidade[i];
                }
            }

            for (size_t i = 0; i < prob.size(); i++)
            {
                if (soma_qualidade > 0)
                    prob[i] = qualidade[i] / soma_qualidade;
                else
                    prob[i] = 1.0 / alfas.size();
            }

            // Reinicia estatísticas
            fill(soma_resultados.begin(), soma_resultados.end(), 0.0);
            fill(cont_resultados.begin(), cont_resultados.end(), 0);
            iter_bloco = 0;
        }
    }

    return melhor_solucao_global;
}