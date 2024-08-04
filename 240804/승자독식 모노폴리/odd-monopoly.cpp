#include <iostream>
#include <cstring>
#include <vector>

enum e_space
{
    EMPTY = 0
};

struct t_pos
{
    std::vector<int> dirs[5];
    int y, x;
    int dir;
    bool gatcha;
};

const int MAX_SIZE = 20;
const int MAX_PLAYER = MAX_SIZE * MAX_SIZE + 1;
const int MAX_TURN = 1000;
const int dy[] = {0, -1, 1, 0, 0};
const int dx[] = {0, 0, 0, -1, 1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int size, playerCnt, remainCnt;
int curTime, contractTerm;
t_pos players[MAX_PLAYER];
int map[MAX_SIZE][MAX_SIZE];
int owner[MAX_SIZE][MAX_SIZE];
int contract[MAX_SIZE][MAX_SIZE];

inline bool belongsToSomeone(int signTime)
{
    if (signTime == -1)
        return (false);

    return (curTime - signTime < contractTerm);
}

void    findNextPos(int num, t_pos &cur)
{
    int y = -1, x = -1;
    int nextDir;

    for (const int &dir : cur.dirs[cur.dir])
    {
        int nextY = cur.y + dy[dir];
        int nextX = cur.x + dx[dir];

        if (!(0 <= nextY && nextY < size)
            || !(0 <= nextX && nextX < size))
            continue ;

        if (belongsToSomeone(contract[nextY][nextX]))
        {
            if (owner[nextY][nextX] == num
                && y == -1)
            {
                y = nextY;
                x = nextX;
                nextDir = dir;
            }
        }
        else
        {
            y = nextY;
            x = nextX;
            nextDir = dir;
            break ;
        }
    }

    cur.y = y;
    cur.x = x;
    cur.dir = nextDir;
}

void    movePlayer(int num)
{
    t_pos &cur = players[num];
    int &curSpace = map[cur.y][cur.x];

    if (curSpace == num)
    {
        curSpace = EMPTY;
    }

    findNextPos(num, cur);

    int &nextSpace = map[cur.y][cur.x];
    if (nextSpace == EMPTY || nextSpace > num)
    {
        nextSpace = num;
    }
    else
    {
        players[num].gatcha = true;
        --remainCnt;
    }
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    std::cin >> size >> playerCnt >> contractTerm;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            int &space = map[y][x];

            std::cin >> space;
            contract[y][x] = -1;
            if (space != EMPTY)
            {
                players[space].y = y;
                players[space].x = x;
                contract[y][x] = 0;
                owner[y][x] = space;
            }
        }
    }

    for (int i = 1; i <= playerCnt; ++i)
    {
        std::cin >> players[i].dir;
    }

    for (int i = 1; i <= playerCnt; ++i)
    {
        t_pos &cur = players[i];
        for (int y = 1; y < dirSize; ++y)
        {
            for (int x = 1; x < dirSize; ++x)
            {
                int dir;

                std::cin >> dir;
                cur.dirs[y].push_back(dir);
            }
        }
    }

    curTime = 1;
    remainCnt = playerCnt;
    while (curTime <= MAX_TURN)
    {
        for (int i = 1; i <= playerCnt; ++i)
        {
            if (players[i].gatcha)
                continue ;

            movePlayer(i);
        }

        for (int i = 1; i <= playerCnt; ++i)
        {
            t_pos &cur = players[i];

            contract[cur.y][cur.x] = curTime;
            owner[cur.y][cur.x] = i;
        }

        if (remainCnt == 1)
            break ;

        ++curTime;
    }

    std::cout << (curTime > MAX_TURN ? -1 : curTime) << '\n';
    return 0;
}