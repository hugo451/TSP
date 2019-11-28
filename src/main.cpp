#include "readData.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include <algorithm>
#include <stdlib.h>

using namespace std;

double ** matrizAdj; // matriz de adjacencia
int dimension; // quantidade total de vertices

typedef struct
{
    int custototal;
    int custo_insercao = 10000000;
    int indices[2] = {0, 0};
    vector<int> solucao;
}t_swap;

typedef struct
{
    int custototal;
    int custo_insercao = 10000000;
    int indices[2] = {0, 0};
    vector<int> solucao;
}t_insertion;

typedef struct
{
    int custototal;
    int custo_insercao = 10000000;
    int indices[2] = {0, 0};
    vector<int> solucao;
}t_or_2_opt;

typedef struct
{
    int custototal;
    int custo_insercao = 10000000;
    int indices[2] = {0, 0};
    vector<int> solucao;
}t_or_3_opt;

typedef struct
{
    int custototal;
    int custo_insercao = 10000000;
    int indices[2] = {0, 0};
    vector<int> solucao;
}t_2_opt;

typedef struct
{
    int custototal = 1000000;
    vector<int> solucao;
}t_solucao;

void greed_algorithim(vector<int> &caminho, vector<int> &candidatos)
{
    int i = caminho[0], melhor, indice, x;

    while(!candidatos.empty())
    {
    	melhor = matrizAdj[i][candidatos[0]];
        x = 0;
        for(int j : candidatos)
        {
            if(melhor >= matrizAdj[i][j] && matrizAdj[i][j] != 0)
            {
                melhor = matrizAdj[i][j];
                indice = x;
            }
            x++;
        }
        i = candidatos[indice];
        caminho.push_back(candidatos[indice]);
        candidatos.erase(candidatos.begin() + indice);
    }
}

void constrution(vector<int> &caminho, vector<int> &candidatos, double alpha)
{
    int referencia = caminho[0], b, melhor;

    if(alpha == 0 || int(alpha*candidatos.size()) == 0)
    {
        greed_algorithim(caminho, candidatos);
    }
    else if(alpha == 1)
    {
        while (!candidatos.empty())
        {
            int indice = rand() % candidatos.size();
            caminho.push_back(candidatos[indice]);
            candidatos.erase(candidatos.begin() + indice);
        }
    }
    else
    {
        while(!candidatos.empty())
        {
            vector<int> resi;
            vector<int> restrita;   
            for(b = 0; b < int(alpha*candidatos.size()) + 1; b++)
            {
                melhor = matrizAdj[referencia][candidatos[0]];
                for (int k : candidatos)
                {
                    if(melhor >= matrizAdj[referencia][k] && matrizAdj[referencia][k] != 0)
                    {
                        melhor = matrizAdj[referencia][k];
                    }
                }
                referencia = candidatos[b];
                restrita.push_back(candidatos[b]);
                resi.push_back(b);
            }
            int indice = rand() % restrita.size();
            caminho.push_back(restrita[indice]);
            candidatos.erase(candidatos.begin() + resi[indice]);
        }
    }
    
}

int calcula_custo(vector<int> &caminho)
{
    int c, s = matrizAdj[caminho[dimension - 1]][caminho[0]];

    for(c = 1; c < dimension; c++)
    {
        s = s + matrizAdj[caminho[c]][caminho[c-1]];
    }

    return s;
}

void custo_swap(vector<int> &caminho, t_swap *melhor)
{
    int i, j, custo;

    for(i = 1; i < dimension - 2; i++)
    {
        for(j = i+1; j < dimension - 1; j++)
        {
            if(i == j - 1)
            {
                custo = matrizAdj[caminho[j]][caminho[i-1]] + matrizAdj[caminho[i]][caminho[j+1]] - matrizAdj[caminho[i]][caminho[i-1]] - matrizAdj[caminho[j]][caminho[j+1]];
            }
            else
            {
                custo = matrizAdj[caminho[j]][caminho[i-1]] + matrizAdj[caminho[j]][caminho[i+1]] + matrizAdj[caminho[i]][caminho[j-1]] + matrizAdj[caminho[i]][caminho[j+1]] - matrizAdj[caminho[j]][caminho[j-1]] - matrizAdj[caminho[j]][caminho[j+1]] - matrizAdj[caminho[i]][caminho[i-1]] - matrizAdj[caminho[i]][caminho[i+1]];
            }
            if(melhor->custo_insercao > custo)
            {
                melhor->indices[0] = i;
                melhor->indices[1] = j;

                melhor->custo_insercao = custo;
            }
        }
    }
    for(i = 1; i < dimension - 1; i++)
    {
        if(i == dimension - 2)
        {
            custo = matrizAdj[caminho[dimension - 3]][caminho[dimension - 1]] + matrizAdj[caminho[dimension - 2]][caminho[0]] - matrizAdj[caminho[dimension - 1]][caminho[0]] - matrizAdj[caminho[dimension - 2]][caminho[dimension - 3]];
        }
        else
        {
            custo = matrizAdj[caminho[i]][caminho[dimension - 2]] + matrizAdj[caminho[dimension - 1]][caminho[i - 1]] + matrizAdj[caminho[dimension - 1]][caminho[i + 1]] + matrizAdj[caminho[i]][caminho[0]] - matrizAdj[caminho[i]][caminho[i - 1]] - matrizAdj[caminho[i]][caminho[i + 1]] - matrizAdj[caminho[dimension - 1]][caminho[dimension - 2]] - matrizAdj[caminho[dimension - 1]][caminho[0]];
        }

        if(melhor->custo_insercao > custo)
        {
            melhor->indices[0] = i;
            melhor->indices[1] = j;

            melhor->custo_insercao = custo;
        }
    }
}

void faz_swap(vector<int> &caminho, t_swap *melhor)
{
    iter_swap(caminho.begin() + melhor->indices[0], caminho.begin() + melhor->indices[1]);
}

void custo_insertion(vector<int> &caminho, t_insertion *melhor)
{
    int i, j, custo;

    for(i = 1; i < dimension - 2; i++)
    {
        for(j = 1; j < dimension - 1; j++)
        {
            if(j == i - 1 || j == i || j == i + 1)
            {
                continue;
            }
            else
            {
                if(i < j)
                {
                    custo = matrizAdj[caminho[i-1]][caminho[i+1]] + matrizAdj[caminho[i]][caminho[j]] + matrizAdj[caminho[i]][caminho[j+1]] - matrizAdj[caminho[i]][caminho[i-1]] - matrizAdj[caminho[i]][caminho[i+1]] - matrizAdj[caminho[j]][caminho[j+1]];
                }
                else
                {
                    custo = matrizAdj[caminho[i-1]][caminho[i+1]] + matrizAdj[caminho[i]][caminho[j]] + matrizAdj[caminho[i]][caminho[j-1]] - matrizAdj[caminho[i]][caminho[i-1]] - matrizAdj[caminho[i]][caminho[i+1]] - matrizAdj[caminho[j]][caminho[j-1]];
                }
                

                if(melhor->custo_insercao > custo)
                {
                    melhor->indices[0] = i;
                    melhor->indices[1] = j;

                    melhor->custo_insercao = custo;
                }
            }
            
        }
    }
    for(i = 1; i < dimension - 2; i++)
    {
        custo = matrizAdj[caminho[i-1]][caminho[i+1]] + matrizAdj[caminho[dimension-1]][caminho[i]] + matrizAdj[caminho[i]][caminho[0]] - matrizAdj[caminho[i]][caminho[i-1]] - matrizAdj[caminho[i]][caminho[i+1]] - matrizAdj[caminho[dimension-1]][caminho[0]];

        if(melhor->custo_insercao > custo)
        {
            melhor->indices[0] = i;
            melhor->indices[1] = j;

            melhor->custo_insercao = custo;
        }
    }

}

void faz_insertion(vector<int> &caminho, t_insertion *melhor)
{
    if(melhor->indices[0] < melhor->indices[1])
    {
        caminho.insert(caminho.begin() + melhor->indices[1] + 1, caminho[melhor->indices[0]]);
        caminho.erase(caminho.begin() + melhor->indices[0]);
    }
    else
    {
        caminho.insert(caminho.begin() + melhor->indices[1], caminho[melhor->indices[0]]);
        caminho.erase(caminho.begin() + melhor->indices[0] + 1);
    }
    
}

void custo_or_2_opt(vector<int> &caminho, t_or_2_opt *melhor)
{
    int i, j, custo;

    for(i = 1; i < dimension - 2; i++)
    {
        for(j = 1; j < dimension - 1; j++)
        {
            if(i == j || j == i+1)
            {
                continue;
            }
            else
            {
                if(i < j)
                {
                    custo = matrizAdj[caminho[i-1]][caminho[i+2]] + matrizAdj[caminho[j]][caminho[i]] + matrizAdj[caminho[j+1]][caminho[i+1]] - matrizAdj[caminho[i-1]][caminho[i]] - matrizAdj[caminho[i+1]][caminho[i+2]] - matrizAdj[caminho[j]][caminho[j+1]];
                }
                else
                {
                    custo = matrizAdj[caminho[j-1]][caminho[i]] + matrizAdj[caminho[j]][caminho[i+1]] + matrizAdj[caminho[i-1]][caminho[i+2]] - matrizAdj[caminho[j-1]][caminho[j]] - matrizAdj[caminho[i-1]][caminho[i]] - matrizAdj[caminho[i+1]][caminho[i+2]];
                }
                
                if(melhor->custo_insercao > custo)
                {
                    melhor->indices[0] = i;
                    melhor->indices[1] = j;

                    melhor->custo_insercao = custo;
                }
            }
            
        }
    }
    for (i = 1; i < dimension - 2; i++)
    {
        custo = matrizAdj[caminho[i-1]][caminho[i+2]] + matrizAdj[caminho[dimension-1]][caminho[i]] + matrizAdj[caminho[i+1]][caminho[0]] - matrizAdj[caminho[i]][caminho[i-1]] - matrizAdj[caminho[i+2]][caminho[i+1]] - matrizAdj[caminho[dimension-1]][caminho[0]];

        if(melhor->custo_insercao > custo)
        {
            melhor->indices[0] = i;
            melhor->indices[1] = j;

            melhor->custo_insercao = custo;
        }
    }
    
}

void faz_or_2_opt(vector<int> &caminho, t_or_2_opt *melhor)
{
    if(melhor->indices[0] < melhor->indices[1])
    {
        caminho.insert(caminho.begin() + melhor->indices[1] + 1, caminho[melhor->indices[0]]);
        caminho.insert(caminho.begin() + melhor->indices[1] + 2, caminho[melhor->indices[0] + 1]);
        caminho.erase(caminho.begin() + melhor->indices[0]);
        caminho.erase(caminho.begin() + melhor->indices[0]);
    }
    else
    {
        caminho.insert(caminho.begin() + melhor->indices[1], caminho[melhor->indices[0]]);
        caminho.insert(caminho.begin() + melhor->indices[1] + 1, caminho[melhor->indices[0] + 2]);
        caminho.erase(caminho.begin() + (melhor->indices[0] + 2));
        caminho.erase(caminho.begin() + (melhor->indices[0] + 2));
    }
}

void custo_or_3_opt(vector<int> &caminho, t_or_3_opt *melhor)
{
    int i, j, custo;

    for(i = 1; i < dimension - 3; i++)
    {
        for(j = 1; j < dimension - 1; j++)
        {
            if(i == j || j == i+1 || j == i+2)
            {
                continue;
            }
            else
            {
                if(i < j)
                {
                    custo = matrizAdj[caminho[i-1]][caminho[i+3]] + matrizAdj[caminho[j]][caminho[i]] + matrizAdj[caminho[j+1]][caminho[i+2]] - matrizAdj[caminho[i-1]][caminho[i]] - matrizAdj[caminho[i+3]][caminho[i+2]] - matrizAdj[caminho[j]][caminho[j+1]];
                }
                else
                {
                    custo = matrizAdj[caminho[j-1]][caminho[i]] + matrizAdj[caminho[j]][caminho[i+2]] + matrizAdj[caminho[i-1]][caminho[i+3]] - matrizAdj[caminho[j-1]][caminho[j]] - matrizAdj[caminho[i-1]][caminho[i]] - matrizAdj[caminho[i+3]][caminho[i+2]];
                }
                
                if(melhor->custo_insercao > custo)
                {
                    melhor->indices[0] = i;
                    melhor->indices[1] = j;

                    melhor->custo_insercao = custo;
                }
            }
            
        }
    }
    for (i = 1; i < dimension - 3; i++)
    {
        custo = matrizAdj[caminho[i-1]][caminho[i+3]] + matrizAdj[caminho[dimension-1]][caminho[i]] + matrizAdj[caminho[i+2]][caminho[0]] - matrizAdj[caminho[i]][caminho[i-1]] - matrizAdj[caminho[i+2]][caminho[i+3]] - matrizAdj[caminho[dimension-1]][caminho[0]];

        if(melhor->custo_insercao > custo)
        {
            melhor->indices[0] = i;
            melhor->indices[1] = j;

            melhor->custo_insercao = custo;
        }
    }
}

void faz_or_3_opt(vector<int> &caminho, t_or_3_opt *melhor)
{
    if(melhor->indices[0] < melhor->indices[1])
    {
        caminho.insert(caminho.begin() + melhor->indices[1] + 1, caminho[melhor->indices[0]]);
        caminho.insert(caminho.begin() + melhor->indices[1] + 2, caminho[melhor->indices[0] + 1]);
        caminho.insert(caminho.begin() + melhor->indices[1] + 3, caminho[melhor->indices[0] + 2]);
        caminho.erase(caminho.begin() + melhor->indices[0]);
        caminho.erase(caminho.begin() + melhor->indices[0]);
        caminho.erase(caminho.begin() + melhor->indices[0]);
    }
    else
    {
        caminho.insert(caminho.begin() + melhor->indices[1], caminho[melhor->indices[0]]);
        caminho.insert(caminho.begin() + melhor->indices[1] + 1, caminho[melhor->indices[0] + 2]);
        caminho.insert(caminho.begin() + melhor->indices[1] + 2, caminho[melhor->indices[0] + 4]);
        caminho.erase(caminho.begin() + melhor->indices[0] + 3);
        caminho.erase(caminho.begin() + melhor->indices[0] + 3);
        caminho.erase(caminho.begin() + melhor->indices[0] + 3);
    }
}

void custo_2_opt(vector<int> &caminho, t_2_opt *melhor)
{
    int i, j, custo;

    for(i = 1; i < dimension - 3; i++)
    {
        for(j = i+2; j < dimension - 1; j++)
        {
            custo = matrizAdj[caminho[i]][caminho[j+1]] + matrizAdj[caminho[j]][caminho[i-1]] - matrizAdj[caminho[i]][caminho[i-1]] - matrizAdj[caminho[j]][caminho[j+1]];

            if(melhor->custo_insercao > custo)
            {
                melhor->indices[0] = i;
                melhor->indices[1] = j;

                melhor->custo_insercao = custo;    {
                melhor->indices[0] = i;
                melhor->indices[1] = j;

                melhor->custo_insercao = custo;
            }
            }
        }
    }
}

void faz_2_opt(vector<int> &caminho, t_2_opt *melhor)
{
    reverse(caminho.begin() + melhor->indices[0], caminho.begin() + melhor->indices[1] + 1);
}

void RVND(vector<int> &caminho, t_swap *melhor1, t_insertion *melhor2, t_or_2_opt *melhor3, t_or_3_opt *melhor4, t_2_opt *melhor5) 
{
    int escolha;

    vector<int> vizinhanca = {1, 2, 3, 4, 5};

    while (!vizinhanca.empty())
    {
        escolha = rand()%vizinhanca.size();
        switch (vizinhanca[escolha])
        {
            case 1:
                custo_swap(caminho, melhor1);
                if(melhor1->custo_insercao < 0)
                {
                    faz_swap(caminho, melhor1);
                    melhor1->custo_insercao = 0;
                }
                else
                {
                    vizinhanca.erase(vizinhanca.begin() + escolha);
                }
                break;
            case 2:
                custo_insertion(caminho, melhor2);
                if(melhor2->custo_insercao < 0)
                {
                    faz_insertion(caminho, melhor2);
                    melhor2->custo_insercao = 0;
                }
                else
                {
                    vizinhanca.erase(vizinhanca.begin() + escolha);
                }
                break;
            case 3:
                custo_or_2_opt(caminho, melhor3);
                if(melhor3->custo_insercao < 0)
                {
                    faz_or_2_opt(caminho, melhor3);
                    melhor3->custo_insercao = 0;
                }
                else
                {
                    vizinhanca.erase(vizinhanca.begin() + escolha);
                }
                break;
            case 4:
                custo_or_3_opt(caminho, melhor4);
                if(melhor4->custo_insercao < 0)
                {
                    faz_or_3_opt(caminho, melhor4);
                    melhor4->custo_insercao = 0;
                }
                else
                {
                    vizinhanca.erase(vizinhanca.begin() + escolha);
                }
                break;
            case 5:
                custo_2_opt(caminho, melhor5);
                if(melhor5->custo_insercao < 0)
                {
                    faz_2_opt(caminho, melhor5);
                    melhor5->custo_insercao = 0;
                }
                else
                {
                    vizinhanca.erase(vizinhanca.begin() + escolha);
                }
                break;
        }
    }
}

void perturb(vector<int> &caminho, vector<int> &solucao)
{
    int i;
    double num;

    num = rand()%(int(solucao.size()/10))  + 2;

    for(i = 1; i <= num; i++)
    {
        solucao.push_back(solucao[1]);
        solucao.erase(solucao.begin() + 1);
    }
    caminho = solucao;
}

void printData();


int main(int argc, char** argv) {

    clock_t tempo_i;
    clock_t tempo_e;

    srand(time(NULL));

    readData(argc, argv, &dimension, &matrizAdj);
        
    vector<int> caminho = {1};
    vector<int> candidatos = {};

    int i;

    for(i = 2; i <= dimension; i++)
    {
        candidatos.push_back(i);
    }

        t_swap melhor1;
        t_insertion melhor2;
        t_or_2_opt melhor3;
        t_or_3_opt melhor4;
        t_2_opt melhor5;
        t_solucao solucao, solucaocopia, solucao_otima;
        
        int imax = 50, iils, iterils, custototal;

        if(dimension < 150)
        {
            iils = dimension;
        }
        else
        {
            iils = int(dimension/2);
        }

        double alpha;

        tempo_i = clock();
        
        for (int i = 0; i < imax; i++)
        {
            alpha = (rand()%101)/100.0;
            constrution(caminho, candidatos, alpha);
            solucao.solucao = caminho;
            solucao.custototal = calcula_custo(solucao.solucao);
            solucaocopia.solucao = caminho;
            iterils = 0;
            while (iterils < iils)
            {
                RVND(caminho,  &melhor1, &melhor2, &melhor3, &melhor4, &melhor5);
                custototal = calcula_custo(caminho);
                if(solucao.custototal > custototal)
                {
                    solucao.solucao = caminho;
                    solucao.custototal = custototal;
                    solucaocopia.solucao = caminho;
                    iterils = 0;
                }
                perturb(caminho, solucaocopia.solucao);
                iterils++;
            }
            if (solucao.custototal < solucao_otima.custototal)
            {
                solucao_otima.custototal = solucao.custototal;
                solucao_otima.solucao = solucao.solucao;
            }
        }

        tempo_e = clock();

        cout << "COST: " << solucao_otima.custototal << endl;
        cout << "TIME: " << (tempo_e - tempo_i) / 1000000.00 << endl;

    
  return 0;
}

void printData() {
  cout << "dimension: " << dimension << endl;
  for (size_t i = 1; i <= dimension; i++) {
    for (size_t j = 1; j <= dimension; j++) {
      cout << matrizAdj[i][j] << " ";
    }
    cout << endl;
  }
}
