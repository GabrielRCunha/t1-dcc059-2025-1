
#include "Gerenciador.h"
#include "Grafo.h"
#include <fstream>


void Gerenciador::comandos(Grafo* grafo) {
    cout<<"Digite uma das opcoes abaixo e pressione enter:"<<endl<<endl;
    cout<<"(a) Fecho transitivo direto de um no;"<<endl;
    cout<<"(b) Fecho transitivo indireto de um no;"<<endl;
    cout<<"(c) Caminho minimo (Djikstra);"<<endl;
    cout<<"(d) Caminho minimo (Floyd);"<<endl;
    cout<<"(e) Arvore Geradora Minima (Algoritmo de Prim);"<<endl;
    cout<<"(f) Arvore Geradora Minima (Algoritmo de Kruskal);"<<endl;
    cout<<"(g) Arvore de caminhamento em profundidade;"<<endl;
    cout<<"(h) Raio, diametro, centro e periferia do grafo;"<<endl;
    cout<<"(0) Sair;"<<endl<<endl;

    char resp;
    cin >> resp;
    switch (resp) {
        case 'a': {

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
            cout << "Fecho transitivo direto de " << id_no << ": ";
            for (int i = 0; i < (int)fecho_transitivo_direto.size(); i++) //Imprime o Fecho Transitivo Direto do vértice escolhido
            {
                cout << fecho_transitivo_direto[i];
                if (i < (int)fecho_transitivo_direto.size() - 1)
                cout << ",";
            }
            cout << endl << endl;

            if(pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                ofstream out("fecho_trans_dir.txt");
                if (!out.is_open())
                {
                    cout << "ERRO!" << endl;
                }
                else
                {
                    out << "Fecho transitivo direto do no " << id_no << ": ";
                    if (fecho_transitivo_direto.empty())
                    {
                        out << "Nao ha caminho para " << id_no << ".";
                    }
                    else
                    {
                        for (size_t i = 0; i < fecho_transitivo_direto.size(); ++i)
                        {
                            out << fecho_transitivo_direto[i];
                            if (i != fecho_transitivo_direto.size() - 1)
                                out << ",";
                        }
                    }
                    out << endl;
                    out.close();
                    cout << "Fecho transitivo direto salvo em fecho_trans_dir.txt" << endl;
                }
            }

            break;
        }

        case 'b': {

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
            cout << "Fecho transitivo indireto de "<< id_no << ": ";
            for (int i = 0; i < (int)fecho_transitivo_indireto.size(); i++) //Imprime o Fecho Transitivo Indireto do vértice escolhido
            {
                cout << fecho_transitivo_indireto[i];
                if (i < (int)fecho_transitivo_indireto.size() - 1)
                cout << ",";
            }
            cout << endl;

            if(pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                ofstream out("fecho_trans_indir.txt");
                if (!out.is_open())
                {
                    cout << "ERRO!" << endl;
                }
                else
                {
                    out << "Fecho transitivo indireto do no " << id_no << ": ";
                    if (fecho_transitivo_indireto.empty())
                    {
                        out << "Nao ha caminho para " << id_no << ".";
                    }
                    else
                    {
                        for (size_t i = 0; i < fecho_transitivo_indireto.size(); ++i)
                        {
                            out << fecho_transitivo_indireto[i];
                            if (i != fecho_transitivo_indireto.size() - 1)
                                out << ",";
                        }
                    }
                    out << endl;
                    out.close();
                    cout << "Fecho transitivo indireto salvo em fecho_trans_indir.txt" << endl;
                }
            }

            break;
        }

       case 'c': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1, id_no_2);

            cout << "Caminho minimo de " << id_no_1 << " ate " << id_no_2 << " (Dijkstra): ";
            if (caminho_minimo_dijkstra.empty()) //Verifica se há caminho entre os vértices escolhidos
            {
                cout << "Nao ha caminho entre os nos." << endl;
            }
            else
            {
                for (int i = 0; i < (int)caminho_minimo_dijkstra.size(); i++) //Imprime o caminho mínimo entre os vértices escolhidos, segundo o algoritmo de Dijkstra
                {
                    cout << caminho_minimo_dijkstra[i];
                    if (i < (int)caminho_minimo_dijkstra.size() - 1)
                     cout << ",";
                }
                cout << endl;
            }

            if (pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                ofstream outFile("caminho_minimo_dijkstra.txt");
                if (outFile.is_open())
                {
                    if (caminho_minimo_dijkstra.empty())
                    {
                        outFile << "Nao ha caminho entre " << id_no_1 << " e " << id_no_2 << "." << endl;
                    }
                    else
                    {
                        for (size_t i = 0; i < caminho_minimo_dijkstra.size(); ++i)
                        {
                            outFile << caminho_minimo_dijkstra[i] << (i == caminho_minimo_dijkstra.size() - 1 ? "" : ",");
                        }
                        outFile << endl;
                    }
                    cout << "Caminho minimo salvo em caminho_minimo_dijkstra.txt" << endl;
                    outFile.close();
                }
                else
                {
                    cout << "ERRO!" << endl;
                }
            }

            break;
}


        case 'd': {

char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1, id_no_2);

            cout << "Caminho minimo entre " << id_no_1 << " e " << id_no_2 << ": ";
            if (caminho_minimo_floyd.empty())
            {
                cout << "Nao existe caminho entre os nos." << endl;
            }
            else
            {
                for (char c : caminho_minimo_floyd)
                {
                    cout << c << " ";
                }
                cout << endl;

                cout << endl;
            }

            if (pergunta_imprimir_arquivo("caminho_minimo_floyd.txt"))
            {
                ofstream outFile("caminho_minimo_floyd.txt");
                if (outFile.is_open())
                {
                    if (caminho_minimo_floyd.empty())
                    {
                        outFile << "Nao existe caminho entre " << id_no_1 << " e " << id_no_2 << "." << endl;
                    }
                    else
                    {
                        for (size_t i = 0; i < caminho_minimo_floyd.size(); ++i)
                        {
                            outFile << caminho_minimo_floyd[i] << (i == caminho_minimo_floyd.size() - 1 ? "" : " -> ");
                        }
                        outFile << endl;
                    }
                    cout << "Caminho minimo salvo em caminho_minimo_floyd.txt" << endl;
                    outFile.close();
                }
                else
                {
                    cout << "Erro ao abrir o arquivo para escrita." << endl;
                }
            }

            break;
        }
        case 'e': {

    int tam;
    cout << "Digite o tamanho do subconjunto: ";
    cin >> tam;

    if (tam > 0 && tam <= grafo->getOrdem()) {

        vector<char> ids = get_conjunto_ids(grafo, tam);
        Grafo* arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);

        if (arvore_geradora_minima_prim == nullptr) {
            cout << "Erro: Subconjunto inválido ou grafo desconexo." << endl << endl;
        } else {
            cout << "Arvore Geradora Minima (Prim) no subgrafo induzido pelos vertices selecionados:" << endl;
            for (No* no : arvore_geradora_minima_prim->getListaAdj()) {
                cout << "No " << no->getId() << ": ";
                vector<Aresta*> arestas = no->getArestas();
                for (int i = 0; i < (int)arestas.size(); i++) {
                    cout << "(" << arestas[i]->getIdOrigem() << " -> " << arestas[i]->getIdAlvo()
                         << ", peso: " << arestas[i]->getPeso() << ")";
                    if (i < (int)arestas.size() - 1) cout << ", ";
                }
                cout << endl;
            }
            cout << endl;
        }

        if (pergunta_imprimir_arquivo("agm_prim.txt")) {
            cout << "Metodo de impressao em arquivo nao implementado" << endl;
        }

        delete arvore_geradora_minima_prim;

    } else {
        cout << "Valor invalido" << endl;
    }

    break;
        }

        case 'f': {

            int tam;
            cout<<"Digite o tamanho do subconjunto: ";
            cin>>tam;

            if(tam > 0 && tam <= grafo->getOrdem()) {

                vector<char> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
                cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

                if(pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                    cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
                }

                delete arvore_geradora_minima_kruskal;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'g': {

            char id_no = get_id_entrada();
            Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }

            delete arvore_caminhamento_profundidade;
            break;
        }

        case 'h': {
            grafo->calcular_raio_diametro_centro_periferia();
            break;
        }
        case '0': {
            exit(0);
        }
        default: {
            cout<<"Opção inválida"<<endl;
        }
    }

    comandos(grafo);

}

char Gerenciador::get_id_entrada() {
    cout<<"Digite o id de um no: ";
    char id;
    cin>>id;
    cout<<endl;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam) {
    vector<char> ids = {};
    while((int)ids.size() < tam) {
        char id_no =get_id_entrada();
        bool existe = false;
        for(No* no: grafo->getListaAdj()){
            if(no->getId() == id_no){
                existe = true;
                break;
            }
        }

        if(!existe){
            cout<<"Vertice nao existe"<<endl;
        }else{
            bool repetido = find(ids.begin(), ids.end(),id_no) != ids.end();
            if(repetido) {
                    cout<<"Valor repetido"<<endl;
            }else {
                ids.push_back(id_no);
            }
        }

    }

    return ids;
}


bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo) {

    cout<<"Imprimir em arquivo externo? ("<<nome_arquivo<<")"<<endl;
    cout<<"(1) Sim;"<<endl;
    cout<<"(2) Nao."<<endl;
    int resp;
    cin>>resp;
    cout<<endl;

    switch (resp) {
        case 1:
            return true;
        case 2:
            return false;
        default:
            cout<<"Resposta invalida"<<endl;
            return pergunta_imprimir_arquivo(nome_arquivo);
    }
}