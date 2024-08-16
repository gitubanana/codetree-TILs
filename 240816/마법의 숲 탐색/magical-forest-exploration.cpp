#include <iostream>
#include <cstring>

struct t_pos
{
    int y, x;
};

struct t_golem : public t_pos
{
    int exitDir;
};

const int MAX_SIZE = 70;
const int dy[] = {-1, 0, 1, 0};
const int dx[] = {0, 1, 0, -1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int rowSum, maxRow, id;
int ySize, xSize;
int golemId[MAX_SIZE][MAX_SIZE];
bool visited[MAX_SIZE][MAX_SIZE];

void    printMap(std::string str)
{
    std::cout << "=== " << str << " ===" << '\n';
    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            std::cout << golemId[y][x] << ' ';
        }
        std::cout << '\n';
    }
}

inline bool inRange(const t_pos &pos)
{
    return ((0 <= pos.y && pos.y < ySize)
            && (0 <= pos.x && pos.x < xSize));
}

inline bool isOkWithFourDir(const t_pos &pos)
{
    for (int dir = 0; dir < dirSize; ++dir)
    {
        t_pos next = {
            pos.y + dy[dir],
            pos.x + dx[dir]
        };

        if (!(0 <= next.x && next.x < xSize))
            return (false);

        if (next.y < 0)
            continue ;

        if (golemId[next.y][next.x])
            return (false);
    }
    return (true);
}

bool    moveDown(t_golem &pos)
{
    ++pos.y;

    if (isOkWithFourDir(pos))
        return (true);

    --pos.y;
    return (false);
}

void    dfs(const t_pos &cur)
{
    maxRow = std::max(maxRow, cur.y);
    visited[cur.y][cur.x] = true;

    int &curGolem = golemId[cur.y][cur.x];
    for (int dir = 0; dir < dirSize; ++dir)
    {
        t_pos next = {
            cur.y + dy[dir],
            cur.x + dx[dir]
        };

        if (!inRange(next)
            || !golemId[next.y][next.x]
            || visited[next.y][next.x])
            continue ;

        int &nextGolem = golemId[next.y][next.x];
        if (!(curGolem < 0 || nextGolem < 0 || curGolem == nextGolem))
            continue ;

        dfs(next);
    }
}

void    simulation(t_golem &pos)
{
    static const int yLimit = ySize - 2;

    bool canGo = true;

    while (canGo && pos.y < yLimit)
    {
        // std::cout << "pos : " << pos.y << ", " << pos.x << '\n';
        if (moveDown(pos))
            continue ;

        canGo = false;
        for (int dir = dirSize - 1; dir >= 0; dir -= 2)
        {
            t_golem next = {
                pos.y,
                pos.x + dx[dir],
                (pos.exitDir + 1 + (dir == dirSize - 1) * 2) % dirSize
            };

            // std::cout << "next : " << next.y << ", " << next.x << '\n';
            if (isOkWithFourDir(next) && moveDown(next))
            {
                pos = next;
                canGo = true;
                break ;
            }
        }
    }

    if (pos.y <= 0) // out of the map
    {
        // remove all
        memset(golemId, 0, sizeof(golemId));
        return ;
    }

    // std::cout << "exitDir : " << pos.exitDir << '\n';

    memset(visited, false, sizeof(visited));

    // set pos
    ++id;

    visited[pos.y][pos.x] = true;
    golemId[pos.y][pos.x] = id;
    for (int dir = 0; dir < dirSize; ++dir)
    {
        golemId[pos.y + dy[dir]][pos.x + dx[dir]] = id;
    }

    // find the biggest row
    maxRow = pos.y + 1;
    golemId[pos.y + dy[pos.exitDir]][pos.x + dx[pos.exitDir]] = -id;
    dfs({pos.y + dy[pos.exitDir], pos.x + dx[pos.exitDir]});

    rowSum += maxRow + 1;
}

int main(void)
{
    std::cin.tie(0)->sync_with_stdio(0);

    int golemCnt;

    std::cin >> ySize >> xSize >> golemCnt;
    while (golemCnt--)
    {
        t_golem pos = {-2};

        std::cin >> pos.x >> pos.exitDir;
        --pos.x;

        simulation(pos);
        // printMap("after simulation");
        // std::cout << "rowSum : " << rowSum << '\n'; 
    }

    std::cout << rowSum << '\n';
    return 0;
}