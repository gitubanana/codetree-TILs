#include <iostream>
#include <cstring>
#include <vector>
#define CURMAP map[curIdx]
#define NEXTMAP map[nextIdx]

enum e_space
{
    EMPTY = 0,
    OFFICE
};

struct t_pos
{
    int y, x;
};

struct t_wind : public t_pos
{
    int dir;
};

const int MAX_SIZE = 20;
const int dy[] = {0, -1, 0, 1};
const int dx[] = {-1, 0, 1, 0};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int size, wantTemp;
int curIdx, nextIdx;
int map[2][MAX_SIZE][MAX_SIZE];
bool visited[MAX_SIZE][MAX_SIZE];
bool isWall[MAX_SIZE][MAX_SIZE][dirSize];
std::vector<t_pos> offices;
std::vector<t_wind> coolers;

inline bool inRange(const t_pos &pos)
{
    return ((0 <= pos.y && pos.y < size)
            && (0 <= pos.x && pos.x < size));
}

inline bool allCool(void)
{
    for (const t_pos &pos : offices)
    {
        if (CURMAP[pos.y][pos.x] < wantTemp)
        {
            return (false);
        }
    }

    return (true);
}

void    mixCoolAir(void)
{
    nextIdx = curIdx ^ 1;

    memset(NEXTMAP, 0, sizeof(NEXTMAP));
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            int &curSpace = CURMAP[y][x];
            int &nextSpace = NEXTMAP[y][x];

            for (int dir = 0; dir < dirSize; ++dir)
            {
                if (isWall[y][x][dir])
                    continue ;

                t_pos cmp = {
                    y + dy[dir],
                    x + dx[dir]
                };

                if (!inRange(cmp))
                    continue ;

                int &cmpSpace = CURMAP[cmp.y][cmp.x];
                int diff = curSpace - cmpSpace;
                if (diff < 4)
                    continue ;

                int plus = diff / 4;

                NEXTMAP[cmp.y][cmp.x] += plus;
                nextSpace -= plus;
            }
            nextSpace += curSpace;
        }
    }

    curIdx = nextIdx;
}

void    decreaseTemp(void)
{
    for (int y = 0; y < size; ++y)
    {
        int step = (y == 0 || y == size - 1) ? 1 : size - 1;

        for (int x = 0; x < size; x += step)
        {
            int &curSpace = CURMAP[y][x];
            if (curSpace == EMPTY)
                continue ;

            --curSpace;
        }
    }
}

void    dfs(const t_pos &cur, int coolerDir, int plus=5)
{
    static std::vector<int> dirsBydir[dirSize][dirSize - 1] = {
        {{0}, {1, 0}, {3, 0}},
        {{1}, {0, 1}, {2, 1}},
        {{2}, {1, 2}, {3, 2}},
        {{3}, {0, 3}, {2, 3}}
    };

    visited[cur.y][cur.x] = true;
    CURMAP[cur.y][cur.x] += plus;

    if (plus == 1)
        return ;

    for (const std::vector<int> &dirs : dirsBydir[coolerDir])
    {
        bool canGo = true;
        t_pos next = cur;

        for (const int &dir : dirs)
        {
            if (isWall[next.y][next.x][dir])
            {
                canGo = false;
                break ;
            }

            next.y += dy[dir];
            next.x += dx[dir];

            if (!inRange(next))
            {
                canGo = false;
                break ;
            }
        }

        if (canGo && !visited[next.y][next.x])
        {
            dfs(next, coolerDir, plus - 1);
        }
    }
}

void    workCoolers(void)
{
    for (const t_wind &cooler : coolers)
    {
        memset(visited, false, sizeof(visited));
        dfs(
            {
                cooler.y + dy[cooler.dir],
                cooler.x + dx[cooler.dir]
            },
            cooler.dir
        );
    }
}

int main(void)
{
    std::cin.tie(0)->sync_with_stdio(0);

    int wallCnt;

    std::cin >> size >> wallCnt >> wantTemp;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            int &curSpace = CURMAP[y][x];

            std::cin >> curSpace;
            switch (curSpace)
            {
                case EMPTY:
                    break;
                case OFFICE:
                    offices.push_back({y, x});
                    curSpace = EMPTY;
                    break;
                default: // coolers with dir
                    coolers.push_back({y, x, curSpace - 2});
                    curSpace = EMPTY;
                    break;
            }
        }
    }

    while (wallCnt--)
    {
        int y, x, type;
        std::cin >> y >> x >> type;

        int dir = (type == 0);
        --y, --x;

        isWall[y][x][dir] = true;
        isWall[y + dy[dir]][x + dx[dir]][dir + 2] = true;
    }

    int minute = 0;
    while (!allCool())
    {
        if (minute == 100)
        {
            minute = -1;
            break ;
        }

        workCoolers();
        mixCoolAir();
        decreaseTemp();

        ++minute;
    }

    std::cout << minute << '\n';
    return (0);
}