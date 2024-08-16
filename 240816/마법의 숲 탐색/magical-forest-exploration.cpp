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

int rowSum, maxRow;
int ySize, xSize;
bool isGolem[MAX_SIZE][MAX_SIZE];
bool visited[MAX_SIZE][MAX_SIZE];

void    printMap(std::string str)
{
    std::cout << "=== " << str << " ===" << '\n';
    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            std::cout << isGolem[y][x] << ' ';
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

        if (isGolem[next.y][next.x])
            return (false);
    }
    return (true);
}

bool    moveDown(t_golem &golem)
{
    ++golem.y;

    if (isOkWithFourDir(golem))
        return (true);

    --golem.y;
    return (false);
}

void    dfs(const t_pos &cur)
{
    maxRow = std::max(maxRow, cur.y);
    visited[cur.y][cur.x] = true;

    for (int dir = 0; dir < dirSize; ++dir)
    {
        t_pos next = {
            cur.y + dy[dir],
            cur.x + dx[dir]
        };

        if (!inRange(next)
            || !isGolem[next.y][next.x]
            || visited[next.y][next.x])
            continue ;

        dfs(next);
    }
}

void    simulation(t_golem &golem)
{
    static const int yLimit = ySize - 2;

    bool canGo = true;

    while (canGo && golem.y < yLimit)
    {
        // std::cout << "golem : " << golem.y << ", " << golem.x << '\n';
        if (moveDown(golem))
            continue ;

        canGo = false;
        for (int dir = dirSize - 1; dir >= 0; dir -= 2)
        {
            t_golem next = {
                golem.y,
                golem.x + dx[dir],
                (golem.exitDir + 1 + (dir == dirSize - 1) * 2) % dirSize
            };

            // std::cout << "next : " << next.y << ", " << next.x << '\n';
            if (isOkWithFourDir(next) && moveDown(next))
            {
                golem = next;
                canGo = true;
                break ;
            }
        }
    }

    if (golem.y <= 0) // out of the map
    {
        // remove all
        memset(isGolem, false, sizeof(isGolem));
        return ;
    }

    // std::cout << "exitDir : " << golem.exitDir << '\n';

    memset(visited, false, sizeof(visited));

    // set isGolem
    visited[golem.y][golem.x] = true;
    isGolem[golem.y][golem.x] = true;
    for (int dir = 0; dir < dirSize; ++dir)
    {
        isGolem[golem.y + dy[dir]][golem.x + dx[dir]] = true;
    }

    // find the biggest row
    maxRow = golem.y + 1;
    dfs({golem.y + dy[golem.exitDir], golem.x + dx[golem.exitDir]});

    rowSum += maxRow + 1;
}

int main(void)
{
    std::cin.tie(0)->sync_with_stdio(0);

    int golemCnt;

    std::cin >> ySize >> xSize >> golemCnt;
    while (golemCnt--)
    {
        t_golem golem = {-2};

        std::cin >> golem.x >> golem.exitDir;
        --golem.x;

        simulation(golem);
        // printMap("after simulation");
        // std::cout << "rowSum : " << rowSum << '\n'; 
    }

    std::cout << rowSum << '\n';
    return 0;
}