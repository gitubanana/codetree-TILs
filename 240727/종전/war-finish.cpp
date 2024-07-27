#include <iostream>
#include <algorithm>
#include <cstring>
#include <climits>

const int MAX_SIZE = 20;

int dist[2];
int size, minDiff = INT_MAX;
int startY, startX;
int map[MAX_SIZE][MAX_SIZE];
bool tribeZero[MAX_SIZE][MAX_SIZE];

void    dfs(int y, int x)
{
    static const int dy[] = {-1, 1, 0, 0};
    static const int dx[] = {0, 0, -1, 1};
    static const int dirSize = sizeof(dy) / sizeof(dy[0]);

    tribeZero[y][x] = true;
    for (int dir = 0; dir < dirSize; ++dir)
    {
        int nextY = y + dy[dir];
        int nextX = x + dx[dir];

        if (!(0 <= nextY && nextY < size)
            || !(0 <= nextX && nextX < size)
            || tribeZero[nextY][nextX])
            continue ;

        dfs(nextY, nextX);
    }
}

void    fillDiagonal(int y, int x)
{
    static const int dy[] = {-1, -1};
    static const int dx[] = {1, -1};

    int dIdx = (dist[0] == 1);
    int move = dist[dIdx];

    while (move--)
    {
        tribeZero[y][x] = true;
        y += dy[dIdx];
        x += dx[dIdx];
    }
}

void    makeBorder(int y, int x)
{
    static const int dy[] = {-1, -1, 1, 1};
    static const int dx[] = {1, -1, -1, 1};
    static const int dirSize = sizeof(dy) / sizeof(dy[0]);

    int dIdx;

    memset(tribeZero, false, sizeof(tribeZero));
    for (int dir = 0; dir < dirSize; ++dir)
    {
        dIdx = (dir & 1);

        int move = dist[dIdx];
        do
        {
            y += dy[dir];
            x += dx[dir];
            tribeZero[y][x] = true;
        } while (--move);
    }

    // diagonal
    if (dist[0] == 1 || dist[1] == 1)
    {
        fillDiagonal(y - 1, x);
    }
    else
    {
        dfs(y - 1, x);
    }
}

inline int getTribeIdx(int y, int x)
{
    if (tribeZero[y][x])
        return (0);

    int &d1 = dist[0];
    int &d2 = dist[1];

    if (y < startY - d2 && x <= startX + (d1 - d2))
        return (1);

    if (y <= startY - d1 && x > startX + (d1 - d2))
        return (2);

    if (y >= startY - d2 && x < startX)
        return (3);

    return (4);
}

void    calculate(void)
{
    static const int tribeCnt = 5;
    static int people[tribeCnt];

    memset(people, 0, sizeof(people));
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            people[getTribeIdx(y, x)] += map[y][x];
        }
    }

    minDiff = std::min(
        minDiff,
        *std::max_element(people, people + tribeCnt)
            - *std::min_element(people, people + tribeCnt)
    );
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    std::cin >> size;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            std::cin >> map[y][x];
        }
    }

    startY = 2;
    while (startY < size)
    {
        startX = 1;
        while (startX < size - 1)
        {
            int &d1 = dist[0];
            int &d2 = dist[1];

            d1 = 1;
            while (startX + d1 < size)
            {
                d2 = 1;
                while (true)
                {
                    if (startX - d2 < 0 || startY - (d1 + d2) < 0)
                        break ;

                    makeBorder(startY, startX);
                    calculate();
                    ++d2;
                }

                ++d1;
            }

            ++startX;
        }

        ++startY;
    }

    std::cout << minDiff << '\n';
    return 0;
}