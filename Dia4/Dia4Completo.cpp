// Como para la parte 2 se necesita hacer la parte 1, aqui estan implementadas ambas partes con Grafos, aunque para la primera parte no es necesario ni siquiera usar grafos
// Y casi que ni eficiente, ya que seria como un "overkill" usar grafos para la priemra parte.

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <fstream>
#include <memory> // Para poder usar shared_ptr y que un nodo pueda tener punteros a otros nodos

using namespace std;

class Posicion // Como coordenada en la matriz
{
private:
    int fila;    // Aso seria com la "x"
    int columna; // Aso seria com la "y"

public:
    Posicion(int f = 0, int c = 0) : fila(f), columna(c) {}

    int getFila() const { return fila; }
    int getColumna() const { return columna; }

    bool operator==(const Posicion &otra) const
    {
        return fila == (otra.fila) && (columna == otra.columna);
    }

    void mostrar() const // Ho vaig fer per a enterarme de algo quan feia el debug, per a vore on tenia els errors
    {
        cout << "(" << fila << "," << columna << ")";
    }
};

struct HashPosicion // Hash personalizada per a posicions, pero no esta implementada del tot, es com un acolxat per al unordered_map
{
    size_t operator()(const Posicion &p) const
    {
        // Combinamos los hashes de fila y columna
        return hash<int>()(p.getFila()) ^ (hash<int>()(p.getColumna()) << 1);
    }
};

class Nodo // Asi comença lo bo, clase Nodo de la que estara compost el grafo
{
private:
    Posicion posicion;
    int grado; // Cantidad de vecinos activos (no eliminados)
    bool eliminado;
    vector<shared_ptr<Nodo>> vecinos; // Usamos shared_ptr para compartir nodos, que un nodo pueda tener punteros a otros nodos

public:
    // Constructor
    Nodo(const Posicion &pos) : posicion(pos), grado(0), eliminado(false) {}

    // Getters
    Posicion getPosicion() const { return posicion; }
    int getGrado() const { return grado; }
    bool estaEliminado() const { return eliminado; }

    // Setters
    void setGrado(int g) { grado = g; }

    void agregarVecino(shared_ptr<Nodo> vecino) // Como unir los nodos (agregar un vecino) y se le aumenta el grado, usado mas adelante para comprobar los vecinos
    {
        vecinos.push_back(vecino);
        grado++;
    }

    bool eliminar() // Quan es comproba que te mes de 4 veïns, s'elimina i es redueix el grau dels veïns, per mantindreu tot actualitzat sense rerecórrer tot el grafo ni la matriu
    {
        if (eliminado)
            return false; // Ya estaba eliminado

        eliminado = true;
        grado = 0; // Un nodo eliminado no tiene grado

        // Reducir el grado de todos los vecinos
        for (auto &vecino : vecinos)
        {
            if (!vecino->estaEliminado())
            {
                vecino->reducirGrado();
            }
        }

        return true;
    }

    void reducirGrado() // Asi es reduix el grau, emprleat en la fució eliminar (anterior)
    {
        if (!eliminado)
        {
            grado--;
        }
    }

    const vector<shared_ptr<Nodo>> &getVecinos() const
    {
        return vecinos;
    }

    bool esAccesible() const // Comprovar si es accesible (grau < 4 i no eliminat)
    {
        return !eliminado && grado < 4;
    }

    void mostrar() const // Mostrar info del nodo per a poder deuguear i vore on comença tot a anar malament
    {
        posicion.mostrar();
        cout << " [Grado: " << grado << ", Eliminado: " << (eliminado ? "Sí" : "No") << "]";
    }
};

class GrafoRollos // I asi ja comencen els Grafos complets
{
private:
    unordered_map<Posicion, shared_ptr<Nodo>, HashPosicion> nodos;
    int filas;
    int columnas;

public:
    GrafoRollos(int f = 0, int c = 0) : filas(f), columnas(c) {}

    void setDimensiones(int f, int c)
    {
        filas = f;
        columnas = c;
    }

    shared_ptr<Nodo> agregarNodo(const Posicion &pos)
    {
        if (nodos.find(pos) == nodos.end())
        {
            nodos[pos] = make_shared<Nodo>(pos);
        }
        return nodos[pos];
    }

    shared_ptr<Nodo> obtenerNodo(const Posicion &pos) const
    {
        auto it = nodos.find(pos);
        return (it != nodos.end()) ? it->second : nullptr;
    }

    const unordered_map<Posicion, shared_ptr<Nodo>, HashPosicion> &getNodos() const
    {
        return nodos;
    }

    void construirDesdeMatriz(const vector<string> &matriz)
    {
        filas = matriz.size();
        columnas = matriz[0].size();

        // Primera pasada: crear todos los nodos
        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                if (matriz[i][j] == '@')
                {
                    Posicion pos(i, j);
                    agregarNodo(pos);
                }
            }
        }

        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                if (matriz[i][j] == '@')
                {
                    Posicion posActual(i, j);
                    auto nodoActual = obtenerNodo(posActual);

                    for (int di = -1; di <= 1; di++) // Mirem a les 8 posicions veïnes
                    {
                        for (int dj = -1; dj <= 1; dj++)
                        {
                            if (di == 0 && dj == 0)
                                continue;
                            // Evitar duplicados: solo procesar cuando (di,dj) > (0,0) lexicográficamente
                            if (di < 0 || (di == 0 && dj < 0))
                                continue;

                            int ni = i + di;
                            int nj = j + dj;

                            if (ni >= 0 && ni < filas && nj >= 0 && nj < columnas)
                            {
                                if (matriz[ni][nj] == '@')
                                {
                                    Posicion posVecino(ni, nj);
                                    auto nodoVecino = obtenerNodo(posVecino);

                                    // Conectar ambos nodos (grafo no dirigido)
                                    nodoActual->agregarVecino(nodoVecino);
                                    nodoVecino->agregarVecino(nodoActual);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    int contarAccesiblesIniciales() const // Implementacio per a la part 1, ho faig perque per a la part 2 es necesita la 1 i com no em costaba res, pero fer grafos per a solucionar la part 1 es absurd
    {
        int contador = 0;
        for (const auto &par : nodos)
        {
            if (par.second->esAccesible())
            {
                contador++;
            }
        }
        return contador;
    }

    int eliminarEnCascada() // Esta es la part 2 en la que anem eliminant els Nodos en cascada que siguen accesibles (mes de 4 veins conectats ("@"))
    {
        int totalEliminados = 0;
        queue<shared_ptr<Nodo>> cola;

        for (const auto &par : nodos)
        {
            auto nodo = par.second;
            if (nodo->esAccesible())
            {
                cola.push(nodo);
            }
        }

        while (!cola.empty())
        {
            auto nodoActual = cola.front();
            cola.pop();

            if (nodoActual->estaEliminado() || !nodoActual->esAccesible())
            {
                continue;
            }

            if (nodoActual->eliminar())
            {
                totalEliminados++;

                for (auto vecino : nodoActual->getVecinos())
                {
                    if (!vecino->estaEliminado() && vecino->esAccesible())
                    {
                        cola.push(vecino);
                    }
                }
            }
        }

        return totalEliminados;
    }

    void mostrarEstado() const // Funció per a mostrar l'estat del grafo, per a debuguejar i no liarme
    {
        cout << "=== ESTADO DEL GRAFO ===\n";
        cout << "Nodos totales: " << nodos.size() << "\n";

        int activos = 0, eliminados = 0, accesibles = 0;
        for (const auto &par : nodos)
        {
            auto nodo = par.second;
            if (nodo->estaEliminado())
            {
                eliminados++;
            }
            else
            {
                activos++;
                if (nodo->esAccesible())
                {
                    accesibles++;
                }
            }
        }

        cout << "Nodos activos: " << activos << "\n";
        cout << "Nodos eliminados: " << eliminados << "\n";
        cout << "Nodos accesibles: " << accesibles << "\n";

        cout << "\nDetalle de nodos:\n";
        for (const auto &par : nodos)
        {
            cout << "  ";
            par.second->mostrar();
            cout << "\n";
        }
        cout << "======================\n";
    }
};

int main(void)
{
    ifstream archivo("input.txt");
    if (!archivo.is_open())
    {
        cerr << "Error: No se pudo abrir el archivo input.txt\n";
        return 1;
    }

    vector<string> matriz;
    string linea;

    while (getline(archivo, linea))
    {
        matriz.push_back(linea);
    }
    archivo.close();

    GrafoRollos grafo;
    grafo.construirDesdeMatriz(matriz);

    int parte1 = grafo.contarAccesiblesIniciales();
    cout << "Parte 1: " << parte1 << endl;

    GrafoRollos grafoParaParte2;
    grafoParaParte2.construirDesdeMatriz(matriz);

    int parte2 = grafoParaParte2.eliminarEnCascada();
    cout << "Parte 2: " << parte2 << endl;

    return 0;
}