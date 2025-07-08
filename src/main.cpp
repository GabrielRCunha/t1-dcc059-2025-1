#include <iostream>
#include "Gerenciador.h"
#include "Grafo.h"
#include "Aresta.h"

using namespace std;
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Uso: ./main <nome_do_arquivo>\n";
        return 1;
    }

    cout << "Leitura do arquivo: " << argv[1] << endl;

    Grafo *grafo = new Grafo(argv[1]);

    Gerenciador::comandos(grafo);

    return 0;
}
