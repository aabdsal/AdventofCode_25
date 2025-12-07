// Optimitzat per a que soles busque les columnes que hi ha "haces" en lloc de revisar tota la fila sencera

#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <set>

using namespace std;

int main(void)
{
    int divisiones = 0;
    set<int> colHaces;
    set<int> colHacesSiguientes;

    vector<string> lineas;
    string linea;
    ifstream archivo("input.txt");

    if (!archivo.is_open())
    {
        cerr << "Error al abrir el archivo." << endl;
        return 1;
    }
    while (getline(archivo, linea))
    {
        lineas.push_back(linea);
    }
    archivo.close();

    for (int i = 0; i < lineas[0].size(); i++)
    {
        if (lineas[0][i] == 'S')
        {
            colHaces.insert(i); // Insertamos la columna del haz inicial
        }
    }
    for (int fila = 1; fila < lineas.size(); fila++)
    {
        colHacesSiguientes.clear(); // Limpiamos para la siguiente fila

        for (int col : colHaces)
        {
            if (lineas[fila][col] == '^')
            {
                ++divisiones; // Y aumentamos el contador de divisiones, porque efectivamente se divide, no sabemos en cuántas, pero se divide
                // Ahora agregamos los posibles nuevos haces si estan dentro del rango, si no estan no los agregamos
                if (((col - 1) >= 0)) // Izquierda
                {
                    colHacesSiguientes.insert(col - 1);
                }
                if (((col + 1) < lineas[fila].size())) // Derecha
                {
                    colHacesSiguientes.insert(col + 1);
                }
            }
            else
            {
                colHacesSiguientes.insert(col); // El haz continúa recto
            }
        }
        colHaces = colHacesSiguientes;
    }
    cout << divisiones << endl;
    return 0;
}