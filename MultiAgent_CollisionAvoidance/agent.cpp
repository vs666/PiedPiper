#include <bits/stdc++.h>
// #include "agent.h"
using namespace std;

#define FREE 0
#define AGENT 1
#define OBSTACLE 2

struct position
{
    int x;
    int y;
};

struct scope
{
    int ar[5][5];
    position pos;
};

// typedef struct position
// {
//     int x;
//     int y;
// } position;

// typedef struct scope
// {
//     int ar[5][5];
//     position pos;

// } scope;

typedef struct position position;
typedef struct scope scope;

// scope *make_scope(int x, int y)
// {
//     scope *r = (scope *)malloc(sizeof(scope));
//     r->pos.x = x;
//     r->pos.y = y;
//     return r;
// }

class Agent
{
public:
    position pos_t;
    Agent(int x, int y)
    {
        pos_t.x = x;
        pos_t.y = y;
    }
    void move(scope *sc, position destination)
    {
        // move logic here
        // cout << "MOVE MATRIX : " << endl;
        // for (int x = 0; x < 5; x++)
        // {
        //     for (int y = 0; y < 5; y++)
        //     {
        //         cout << sc->ar[x][y] << " ";
        //     }
        //     cout << endl;
        // }
        if (!moveRule3(destination, sc))
        {
            moveRule4(destination, sc);
        }
    }
    void tick()
    {
        t_elapsed++;
        t_elapsed %= t_switch;
        if (t_elapsed == 0)
        {
            HP_NW = !HP_NW;
        }
    }

private:
    bool HP_NW = true;
    int t_switch = 2;
    int t_elapsed = 0;
    float distance_e(position p1, position p2)
    {
        return sqrt(abs(p1.x - p2.x) * abs(p1.x - p2.x) + abs(p1.y - p2.y) * abs(p1.y - p2.y));
    }
    int distance_c(position p1, position p2)
    {
        return min(abs(p1.x - p2.x), abs(p1.y - p2.y));
    }

    int distance_c(position p1, int a, int b)
    {
        return min(abs(p1.x - a), abs(p1.y - b));
    }

    int distance_c(int a, int b, int c, int d)
    {
        return min(abs(c - a), abs(d - b));
    }
    bool checkRule1(position pos_t1, scope *sc)
    {
        int tx = pos_t1.x - pos_t.x + 2;
        int ty = pos_t1.y - pos_t.y + 2;
        if (sc->ar[tx][ty] != FREE)
        {
            return false;
        }
        return true;
    }

    bool checkRule2(position pos_t1, scope *sc)
    {
        // for North Western priority region
        if (HP_NW)
        {
            for (int x = 0; x <= 2; x++)
            {
                for (int y = 0; y < 5; y++)
                {
                    if (x == 2 && y == 2)
                    {
                        break;
                    }
                    else if (sc->ar[x][y] == AGENT)
                    { // value of HP_NW is synchronized to avoid deadlock
                        if (distance_c(x, y, pos_t1.x + 2 - pos_t.x, pos_t1.y + 2 - pos_t.y) <= 1)
                        {
                            return false;
                        }
                    }
                }
            }
        }
        // otherwise
        else
        {
            if (HP_NW)
            {
                for (int x = 2; x < 5; x++)
                {
                    for (int y = 0; y < 5; y++)
                    {
                        if (x == 2 && y <= 2)
                        {
                            continue;
                        }
                        else if (sc->ar[x][y] == AGENT)
                        { // value of HP_NW is synchronized to avoid deadlock
                            if (distance_c(x, y, pos_t1.x + 2 - pos_t.x, pos_t1.y + 2 - pos_t.y) <= 1)
                            {
                                return false;
                            }
                        }
                    }
                }
            }
        }
        return true;
    }

    bool moveRule3(position destination, scope *sc)
    {
        position pos_t1;
        pos_t1.x = pos_t.x;
        pos_t1.y = pos_t.y;
        if (abs(pos_t.x - destination.x) < abs(pos_t.y - destination.y))
        {
            pos_t1.y += (destination.y - pos_t.y > 0) ? 1 : -1;
        }
        else if (abs(pos_t.x - destination.x) > abs(pos_t.y - destination.y))
        {
            pos_t1.x += (destination.x - pos_t.x > 0) ? 1 : -1;
        }
        else
        {
            pos_t1.x += (destination.x - pos_t.x > 0) ? 1 : -1;
            pos_t1.y += (destination.y - pos_t.y > 0) ? 1 : -1;
        }
        if (checkRule1(pos_t1, sc) && checkRule2(pos_t1, sc))
        {
            pos_t.x = pos_t1.x;
            pos_t.y = pos_t1.y;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool moveRule4(position destination, scope *sc)
    {
        for (int x = -1; x < 2; x++)
        {
            for (int y = -1; y < 2; y++)
            {
                position pt1;
                pt1.x = pos_t.x + x;
                pt1.y = pos_t.y + y;
                if (checkRule1(pt1, sc) && checkRule2(pt1, sc) && distance_e(pt1, destination) < distance_e(pos_t, destination))
                {
                    pos_t.x = pt1.x;
                    pos_t.y = pt1.y;
                    return true;
                }
            }
        }
        return false; // not moved ( not necessarily deadlock )
    }
};

int main()
{
    int matrix[7][7] = {
        {1, 0, 0, 0, 0, 0, 2}, //0
        {0, 0, 2, 2, 0, 0, 0}, //1
        {0, 0, 2, 2, 0, 2, 0}, //2
        {0, 0, 0, 0, 0, 0, 0}, //3
        {2, 2, 2, 0, 0, 2, 2}, //4
        {0, 0, 0, 0, 0, 2, 2}, //5
        {0, 0, 2, 2, 0, 0, 0}  //6
    };
    position destination;
    destination.x = 6;
    destination.y = 6;
    Agent *ag = new Agent(0, 0);
    // print the matrix after each iteration
    int max_iter = 36;
    bool over = false;
    cout << "INITIAL CONDITION " << endl;
    for (int x = 0; x < 7; x++)
    {
        for (int y = 0; y < 7; y++)
        {
            cout << ((matrix[x][y] == 0) ? " " : (matrix[x][y] == 1 ? "V" : "X")) << " ";
        }
        cout << endl;
    }
    while (max_iter && !over)
    {
        if (ag->pos_t.x == destination.x && ag->pos_t.y == destination.y)
        {
            over = true;
            break;
        }
        cout << "ITER " << 36 - max_iter + 1 << endl;
        matrix[ag->pos_t.x][ag->pos_t.y] = 0;
        scope *sc = (scope *)malloc(sizeof(scope));
        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                int ix = ag->pos_t.x - 2 + x;
                int iy = ag->pos_t.y - 2 + y;
                if (ix < 0 || iy < 0 || ix > 6 || iy > 6)
                {
                    sc->ar[x][y] = 2;
                }
                else
                {
                    sc->ar[x][y] = matrix[ix][iy];
                }
            }
        }
        sc->pos.x = ag->pos_t.x;
        sc->pos.y = ag->pos_t.y;
        ag->move(sc, destination);
        ag->tick();
        free(sc);
        matrix[ag->pos_t.x][ag->pos_t.y] = 1;
        max_iter--;
        for (int x = 0; x < 7; x++)
        {
            for (int y = 0; y < 7; y++)
            {
                cout << ((matrix[x][y] == 0) ? " " : (matrix[x][y] == 1 ? "V" : "X")) << " ";
            }
            cout << endl;
        }
    }
}