// Programacio dinamica perque me sentia inspirat i en ganes de complicar-me el dia y la vida

#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

typedef long long ll;

using namespace std;

// Cada dia odie mes la programacio, 40 minuts i un dinar despues y me done conter de que el error estaba en que retornava un int
// en conter de un long long, aixo me limitaba el numero que me retornava y me donava be per a el cas de proba que fiquen en el AoC
// pero per al input gran no me donava be. Que dur.
ll posibilidades(const vector<string> &lineas, int fila, int col, vector<vector<ll>> &memo)
{
    if (fila >= lineas.size() - 1) // Cas base: Acabar les files o estar en la ultima fila
    {
        return 1;
    }
    if (memo[fila][col] != 0) // Cas Base: Per a no recalcular coses que ya tenim
    {
        return memo[fila][col];
    }
    if (lineas[fila + 1][col] == '^') // Igual que la part 1, pero ara guardant en memo y fent recursivitat
    {
        if (col - 1 >= 0)
        {
            memo[fila][col] += posibilidades(lineas, fila + 1, col - 1, memo); // Esquerra
        }
        if (col + 1 < lineas[fila].size())
        {
            memo[fila][col] += posibilidades(lineas, fila + 1, col + 1, memo); // Dreta
        }
    }
    else
    {
        memo[fila][col] += posibilidades(lineas, fila + 1, col, memo); // Segueix recte. Aso tambe se me havia olvidat ficar-ho y me donava sempre 0. Que dur.
    }
    return (memo[fila][col]);
}

int main(void)
{
    vector<string> lineas;
    string linea;
    ifstream archivo("input.txt"); // Sempre igual

    if (!archivo.is_open()) // Sempre igual
    {
        cerr << "Error al abrir el archivo." << endl;
        return 1;
    }
    while (getline(archivo, linea)) // Sempre igual
    {
        lineas.push_back(linea);
    }
    archivo.close();

    ll totalPosibilidades = 0;                                            // long long perque sino se fica especialet y no retorna lo que toca
    vector<vector<ll>> memo(lineas.size(), vector<ll>(lineas[0].size())); // Reutilitzat de altres problemes, sempre a ma
    for (int i = 0; i < lineas[0].size(); i++)
    {
        if (lineas[0][i] == 'S') // Cuan trobe la columna de inici, comença tot
        {
            totalPosibilidades += posibilidades(lineas, 0, i, memo); // Ale a resar y que tot vatja be, perque ni jo se si ho fara o se ficara especialet
        }
    }
    cout << totalPosibilidades << endl; // Lo que ixga bo sera, confie
    return 0;
}