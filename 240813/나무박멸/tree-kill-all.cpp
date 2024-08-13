#include <iostream>
#include <cstring>
#include <vector>
#define CURMAP map[curIdx]
#define NEXTMAP map[nextIdx]

enum e_space
{
    WALL = -1,
    EMPTY
};

struct t_pos
{
    int y, x;

    inline bool operator<(const t_pos &other) const
    {
        if (this->y == other.y)
            return (this->x > other.x);

        return (this->y > other.y);
    }
};

struct t_killer : public t_pos
{
    int killCnt;

    inline bool operator<(const t_killer &other) const
    {
        if (this->killCnt == other.killCnt)
            return (static_cast<t_pos>(*this) < other);

        return (this->killCnt < other.killCnt);
    }
};

const int MAX_SIZE = 20;
const int dy[] = {-1, 1, 0, 0};
const int dx[] = {0, 0, -1, 1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int totalKill;
int size, curTime;
int curIdx, nextIdx;
int map[2][MAX_SIZE][MAX_SIZE];
int effectRange, effectTime;
int weedKiller[MAX_SIZE][MAX_SIZE];

inline bool inRange(int y, int x)
{
    return ((0 <= y && y < size)
            && (0 <= x && x < size));
}

inline bool inRange(const t_pos &pos)
{
    return ((0 <= pos.y && pos.y < size)
            && (0 <= pos.x && pos.x < size));
}

inline bool isWeedKiller(int y, int x)
{
    int &remainTime = weedKiller[y][x];
    if (remainTime == EMPTY)
        return (false);

    return (curTime - remainTime <= effectTime);
}

void    growTrees(void)
{
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            int &curSpace = CURMAP[y][x];
            if (curSpace <= EMPTY)
                continue ;

            for (int dir = 0; dir < dirSize; ++dir)
            {
                int checkY = y + dy[dir];
                int checkX = x + dx[dir];

                if (!inRange(checkY, checkX))
                    continue ;

                curSpace += (CURMAP[checkY][checkX] > EMPTY);
            }
        }
    }
}

void    breedTrees(void)
{
    static std::vector<t_pos> breedPos;

    nextIdx = curIdx ^ 1;

    memset(NEXTMAP, EMPTY, sizeof(NEXTMAP));
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            int &curSpace = CURMAP[y][x];
            int &nextSpace = NEXTMAP[y][x];

            if (nextSpace > EMPTY)
                continue ;

            nextSpace = curSpace;
            if (curSpace <= EMPTY)
                continue ;

            breedPos.clear();
            for (int dir = 0; dir < dirSize; ++dir)
            {
                int checkY = y + dy[dir];
                int checkX = x + dx[dir];

                if (!inRange(checkY, checkX)
                    || CURMAP[checkY][checkX] != EMPTY
                    || isWeedKiller(checkY, checkX))
                    continue ;

                breedPos.push_back({checkY, checkX});
            }

            if (breedPos.size() == 0)
                continue ;

            int toBreed = curSpace / breedPos.size();
            for (const t_pos &pos : breedPos)
            {
                NEXTMAP[pos.y][pos.x] += toBreed;
            }
        }
    }

    curIdx = nextIdx;
}

void    sprayWeedKiller(void)
{
    static const int dy[] = {-1, -1, 1, 1};
    static const int dx[] = {-1, 1, 1, -1};
    static const int dirSize = sizeof(dy) / sizeof(dy[0]);

    t_killer spray = {size, size, -1};

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            int &curSpace = CURMAP[y][x];
            if (curSpace <= EMPTY)
                continue ;

            t_killer cmp = {y, x, curSpace};

            for (int dir = 0; dir < dirSize; ++dir)
            {
                t_pos next = {y, x};

                for (int move = 0; move < effectRange; ++move)
                {
                    next.y += dy[dir];
                    next.x += dx[dir];
                    if (!inRange(next) || CURMAP[next.y][next.x] <= EMPTY)
                        break ;

                    cmp.killCnt += CURMAP[next.y][next.x];
                }
            }

            spray = std::max(spray, cmp);
        }
    }

    if (spray.killCnt == -1)
        return ;

    totalKill += spray.killCnt;
    weedKiller[spray.y][spray.x] = curTime;
    CURMAP[spray.y][spray.x] = EMPTY;
    for (int dir = 0; dir < dirSize; ++dir)
    {
        t_pos next = {spray.y, spray.x};

        for (int move = 0; move < effectRange; ++move)
        {
            next.y += dy[dir];
            next.x += dx[dir];
            if (!inRange(next))
                break ;

            weedKiller[next.y][next.x] = curTime;
            if (CURMAP[next.y][next.x] <= EMPTY)
                break ;

            CURMAP[next.y][next.x] = EMPTY;
        }
    }
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int year;

    std::cin >> size >> year >> effectRange >> effectTime;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            std::cin >> CURMAP[y][x];
        }
    }

    while (year--)
    {
        ++curTime;

        growTrees();
        breedTrees();
        sprayWeedKiller();
    }

    std::cout << totalKill << '\n';
    return 0;
}