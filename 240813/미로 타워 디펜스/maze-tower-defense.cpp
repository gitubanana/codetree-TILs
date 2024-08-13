#include <iostream>
#include <vector>
#define CURMAP map[curIdx]
#define NEXTMAP map[nextIdx]

enum e_space
{
    EMPTY = 0
};

const int MAX_SIZE = 25;
const int dy[] = {0, 1, 0, -1};
const int dx[] = {1, 0, -1, 0};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

struct t_pos
{
    int y, x;
};

struct t_move : public t_pos
{
    int dir;
    int moved, moveCnt;

    inline void move(void)
    {
        y += dy[dir];
        x += dx[dir];

        ++moved;
        if (moved == moveCnt)
        {
            moved = 0;
            moveCnt += (dir & 1);
            dir = (dir + 3) % dirSize;
        }
    }
};

t_pos start;
int size, score;
int curIdx, nextIdx;
int map[2][MAX_SIZE][MAX_SIZE];

void    attackMonster(int dir, int dist)
{
    t_pos toRemove = {
        start.y + dy[dir],
        start.x + dx[dir]
    };

    while (true)
    {
        int &curSpace = CURMAP[toRemove.y][toRemove.x];

        --dist;
        score += curSpace;
        curSpace = EMPTY;
        if (dist == 0)
            break ;

        toRemove.y += dy[dir];
        toRemove.x += dx[dir];
    }
}

void    moveMonster(void)
{
    t_move cur = {
        start.y,
        start.x,
        2,
        0,
        1
    };
    cur.move();
    t_move next = cur;

    while (cur.x != -1)
    {
        int &curSpace = CURMAP[cur.y][cur.x];

        if (curSpace != EMPTY)
        {
            NEXTMAP[next.y][next.x] = CURMAP[cur.y][cur.x];
            next.move();
        }

        cur.move();
    }

    while (next.x != -1)
    {
        NEXTMAP[next.y][next.x] = EMPTY;
        next.move();
    }
}

bool    removeConsecutive(void)
{
    static std::vector<t_pos> toRemove;

    bool removed = false;
    t_move cur = {
        start.y,
        start.x,
        2,
        0,
        1
    };

    cur.move();
    while (cur.x != -1)
    {
        int &curSpace = CURMAP[cur.y][cur.x];
        if (curSpace == EMPTY)
            break ;

        toRemove.clear();
        do
        {
            toRemove.push_back({cur.y, cur.x});
            cur.move();
        } while (CURMAP[cur.y][cur.x] == curSpace);

        if (toRemove.size() >= 4)
        {
            removed = true;
            score += curSpace * toRemove.size();
            for (const t_pos &pos : toRemove)
            {
                CURMAP[pos.y][pos.x] = EMPTY;
            }
        }
    }

    return (removed);
}

void    makeNewMap(void)
{
    bool removed = false;
    t_move cur = {
        start.y,
        start.x,
        2,
        0,
        1
    };
    cur.move();
    t_move next = cur;

    while (cur.x != -1)
    {
        int &curSpace = CURMAP[cur.y][cur.x];
        if (curSpace == EMPTY)
            break ;

        int cnt = 0;
        do
        {
            ++cnt;
            cur.move();
        } while (CURMAP[cur.y][cur.x] == curSpace);

        NEXTMAP[next.y][next.x] = cnt;
        next.move();
        if (next.x == -1)
            break ;

        NEXTMAP[next.y][next.x] = curSpace;
        next.move();
        if (next.x == -1)
            break ;
    }

    while (next.x != -1)
    {
        NEXTMAP[next.y][next.x] = EMPTY;
        next.move();
    }
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int cmdCnt;

    std::cin >> size >> cmdCnt;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            std::cin >> CURMAP[y][x];
        }
    }

    start.y = start.x = size / 2;
    while (cmdCnt--)
    {
        int dir, dist;

        std::cin >> dir >> dist;
        attackMonster(dir, dist);
        do
        {
            nextIdx = curIdx ^ 1;
            moveMonster();
            curIdx = nextIdx;
        } while (removeConsecutive());

        nextIdx = curIdx ^ 1;
        makeNewMap();
        curIdx = nextIdx;
    }

    std::cout << score << '\n';
    return 0;
}