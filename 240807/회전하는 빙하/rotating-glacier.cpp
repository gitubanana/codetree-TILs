#include <iostream>
#define CURMAP map[curIdx]
#define NEXTMAP map[nextIdx]

const int VISITED = -1;
const int MAX_SIZE = 1 << 6;
const int dy[] = {-1, 1, 0, 0};
const int dx[] = {0, 0, -1, 1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int size;
int curIdx, nextIdx;
int map[2][MAX_SIZE][MAX_SIZE];

void    rotate(int startY, int startX, int level)
{
    int halfLevel = level >> 1;
    int yHalf = startY + halfLevel;
    int xHalf = startX + halfLevel;
    int ySize = startY + level;
    int xSize = startX + level;

    for (int y = startY; y < yHalf; ++y)
    {
        for (int x = startX; x < xHalf; ++x)
        {
            NEXTMAP[y][x] = CURMAP[y + halfLevel][x];
        }
    }

    for (int y = startY; y < yHalf; ++y)
    {
        for (int x = xHalf; x < xSize; ++x)
        {
            NEXTMAP[y][x] = CURMAP[y][x - halfLevel];
        }
    }

    for (int y = yHalf; y < ySize; ++y)
    {
        for (int x = xHalf; x < xSize; ++x)
        {
            NEXTMAP[y][x] = CURMAP[y - halfLevel][x];
        }
    }

    for (int y = yHalf; y < ySize; ++y)
    {
        for (int x = startX; x < xHalf; ++x)
        {
            NEXTMAP[y][x] = CURMAP[y][x + halfLevel];
        }
    }
}

void    rotateAll(int level)
{
    level = 1 << level;
    if (level == 1)
        return ;

    nextIdx = curIdx ^ 1;
    for (int y = 0; y < size; y += level)
    {
        for (int x = 0; x < size; x += level)
        {
            rotate(y, x, level);
        }
    }
    curIdx = nextIdx;

    // std::cout << "==== after rotate ====" << '\n';
    // for (int y = 0; y < size; ++y)
    // {
    //     for (int x = 0; x < size; ++x)
    //     {
    //         std::cout << CURMAP[y][x] << ' ';
    //     }
    //     std::cout << '\n';
    // }
}

inline bool isBound(int y, int x)
{
    return ((0 <= y && y < size)
            && (0 <= x && x < size));
}

void    meltIce(void)
{
    nextIdx = curIdx ^ 1;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            int &curIce = CURMAP[y][x];
            if (curIce == 0)
            {
                NEXTMAP[y][x] = 0;
                continue ;
            }

            int adjacentIce = 0;
            for (int dir = 0; dir < dirSize; ++dir)
            {
                int nextY = y + dy[dir];
                int nextX = x + dx[dir];

                if (!isBound(nextY, nextX) || CURMAP[nextY][nextX] == 0)
                    continue ;

                ++adjacentIce;
            }

            NEXTMAP[y][x] = curIce - (adjacentIce < 3);
        }
    }
    curIdx = nextIdx;
}

void    dfs(int y, int x, int &cnt, int &sum)
{
    int &curIce = CURMAP[y][x];

    ++cnt;
    sum += curIce;
    curIce = VISITED;
    for (int dir = 0; dir < dirSize; ++dir)
    {
        int nextY = y + dy[dir];
        int nextX = x + dx[dir];

        if (!isBound(nextY, nextX) || CURMAP[nextY][nextX] <= 0)
            continue ;

        dfs(nextY, nextX, cnt, sum);
    }
}

int main(void)
{
    std::cin.tie(0)->sync_with_stdio(0);

    int cmdCnt;

    std::cin >> size >> cmdCnt;
    size = 1 << size;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            std::cin >> CURMAP[y][x];
        }
    }
    while (cmdCnt--)
    {
        int level;

        std::cin >> level;
        rotateAll(level);
        meltIce();
        // std::cout << "==== after melt ice ====" << '\n';
        // for (int y = 0; y < size; ++y)
        // {
        //     for (int x = 0; x < size; ++x)
        //     {
        //         std::cout << CURMAP[y][x] << ' ';
        //     }
        //     std::cout << '\n';
        // }
    }

    // std::cout << "==== after all cmd ====" << '\n';
    // for (int y = 0; y < size; ++y)
    // {
    //     for (int x = 0; x < size; ++x)
    //     {
    //         std::cout << CURMAP[y][x] << ' ';
    //     }
    //     std::cout << '\n';
    // }

    int cnt = 0;
    int sum = 0;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            if (CURMAP[y][x] <= 0)
                continue ;

            int cmp = 0;

            dfs(y, x, cmp, sum);
            cnt = std::max(cnt, cmp);
        }
    }

    std::cout << sum << '\n';
    std::cout << cnt << '\n';
    return 0;
}