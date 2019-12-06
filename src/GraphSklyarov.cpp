//#include "GraphSklyarov.h"
#include "../headers/TrigPolySklyarov.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int is_file_empty = 0; // з її допомогою буде видалятися попередній результат роботи програми в текстовому файлі при забереження у файл

// Реалізація зв'язаного списку
void add_val(List_ **vals, int val)
{
    List_ *v = new List_;
    List_ *last = *vals;
    v->val = val;
    v->next_val = NULL;
    if (*vals == NULL)
    {
        *vals = v;
    }
    else
    {
        while (last->next_val != NULL)
        {
            last = last->next_val;
        }
        last->next_val = v;
    }
}

bool isempty(List_ **vals)
{
    if (*vals == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void delete_first(List_ **vals)
{
    List_ *v = NULL;
    List_ *v1 = *vals;
    v1 = v1->next_val;
    while (v1 != NULL)
    {
        add_val(&v, v1->val);
        v1 = v1->next_val;
    }
    (*vals) = v;
}


// Метода класу list_of_edges
list_of_edges::list_of_edges(int v, int e)
{
    edg = e;
    vert = v;
    amount_of_values = 0; // кількість введених ребер 0, потім при виклику функції add_edge збільшуємо на 1
    lst = new int*[edg];  // створюємо двовимірний масив, він складається з sizee єлементів, кожен з яких - це масив з трьх елементів: from, to, weight
    for (int i = 0; i < edg; i++)
    {
        lst[i] = new int[3];
    }
}

list_of_edges::~list_of_edges()
{
    for (int i = 0; i < 3; i++)
    {
        delete[] lst[i];
    }
    delete[] lst;
}

void list_of_edges::amount_of_values_()
{
    amount_of_values++;
}

int list_of_edges::get_e()
{
    return edg;
}

int list_of_edges::get_v()
{
    return vert;
}

void list_of_edges::add_edge(int from, int to, int weight)
{
    lst[amount_of_values][0] = from;
    lst[amount_of_values][1] = to;
    lst[amount_of_values][2] = weight;
    amount_of_values_();
}

void list_of_edges::write_to_file(int res)
{
    ofstream file("res.txt", ios::app);
    if (is_file_empty == 0)
    {
        file.close();
        file.open("res.txt");
    }
    file << res << "\n";
    file.close();
    is_file_empty = 1;
}

int list_of_edges::components_of_graph(int v, int e)
{
    // використано алгоритм пошуку в ширину в графі

    int counter = 0; // кількість звязаних компонент графа
    int *used = new int[v]; // створюємо масив вершин графа; спочатку всі вершини відмічені нулями, якщо ми цю вершину відвідали, то помічаємо її 1
    for (int i = 0; i < v; i++)
    {
        used[i] = 0;
    }
    int s; // номер вершини
    List_ *q = NULL; // створюємо чергу, в якій будуть міститися вершини
    int k = 0; // якщо к = 0, то не всі вершини відмічені, якщо к = 1, то цикл завершується
    while (k != 1)
    {
        for (int i = 0; i < v; i++) // цей цикл кожного разу буде перевіряти чи всі вершини помічені одиницею
        {                 // якщо всі помічені, значить мі пройшлися по всім вершинам, інакше додаємо в чергу найближчу за номером не помічену вершину
            if (used[i] != 1)       // на самому початку ми додаємо в чергу першу вершину і одразу помічаємо її одиницею
            {
                counter++;
                s = i + 1;
                used[i] = 1;
                add_val(&q, s);
                k = 0;
                break;
            }
            else
            {
                k = 1;
            }
        }
        // далі йде сам алгоритм пошуку в ширину
        // він полягає в тому, що ми беремо якусь вершину (спочатку це перша вершина), далі за домопогою списку ребер графа
        // ми проходимось по всім сусіднім вершинам даної, якщо ми в них не були, то помічаємо їх 1 і додаємо в чергу q

        while(!isempty(&q))
        {
            int vv = q->val;
            delete_first(&q);
            for (int i = 0; i < v; i++)
            {
                if (i >= e)
                {
                    if (vv < v && used[vv - 1] != 1)
                    {
                        add_val(&q, vv);
                    }
                    break;
                }
                if (lst[i][0] == vv && used[lst[i][1] - 1] != 1)
                {
                    used[lst[i][1] - 1] = 1;
                    add_val(&q, lst[i][1]);
                }
                if (lst[i][1] == vv && used[lst[i][0] - 1] != 1)
                {
                    used[lst[i][0] - 1] = 1;
                    add_val(&q, lst[i][0]);
                }
            }
        }
    }
    cout << "Кількість зв'язних компонент графа: " << counter << endl;
    // Одразу записуємо в файл
    write_to_file(counter);
}

list_of_edges read_graph()
{
    int v, e, from, to, weight;
    int i = 1;
    ifstream file("graph.txt");
    if (!file.is_open())
    {
        cout << "cannot open file\n";
    }
    while (!file.eof())
    {
        if (i == 1)
        {
            file >> e;
        }
        else if (i == 2)
        {
            file >> v;
        }
        else
        {
            break;
        }
        i++;
    }
    i = 1;
    list_of_edges a(v, e);
    while(!file.eof())
    {
        if (i >= 2)
        {
            string str;
            getline(file, str, '\n');
            from = str[0] - '0';
            to = str[2] - '0';
            weight = str[4] - '0';
            a.add_edge(from, to, weight);
        }
        else
        {
            string str;
            getline(file, str, '\n');
        }
        i++;
    }
    return a;
}

