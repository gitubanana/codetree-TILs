#include <iostream>
#include <cstring>
#include <vector>
#define CURMAP map[curIdx]
#define NEXTMAP map[nextIdx]

enum e_space
{
    CLEANER = -1
};

const int MAX_SIZE = 50;
const int dy[] = {-1, 0, 1, 0};
const int dx[] = {0, 1, 0, -1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);
const int dirStart[] = {0, 2};
const int dirPlus[] = {1, 3};

struct t_move
{
    int y, x;
    int dir, dd;
    int ySmall, yBig;
    int xSmall, xBig;

    inline bool operator!=(const t_move &other) const
    {
        return (this->y != other.y || this->x != other.x);
    }

    inline void move(void)
    {
        int nextY = y + dy[dir];
        int nextX = x + dx[dir];

        if (!(ySmall <= nextY && nextY < yBig)
            || !(xSmall <= nextX && nextX < xBig))
        {
            this->dir = (this->dir + dd) % dirSize;
            this->move();
        }
        else
        {
            this->y = nextY;
            this->x = nextX;
        }
    }
};

int ySize, xSize;
int curIdx, nextIdx;
int map[2][MAX_SIZE][MAX_SIZE];
std::vector<t_move> cleaners;

void    spreadDust(void)
{
    memset(NEXTMAP, 0, sizeof(NEXTMAP));

    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            int &remainDust = CURMAP[y][x];
            if (remainDust == CLEANER)
            {
                NEXTMAP[y][x] = CLEANER;
                continue ;
            }

            int plusDust = remainDust / 5;

            for (int dir = 0; dir < dirSize; ++dir)
            {
                int nextY = y + dy[dir];
                int nextX = x + dx[dir];

                if (!(0 <= nextY && nextY < ySize)
                    || !(0 <= nextX && nextX < xSize)
                    || CURMAP[nextY][nextX] == CLEANER)
                    continue ;

                NEXTMAP[nextY][nextX] += plusDust;
                remainDust -= plusDust;
            }

            NEXTMAP[y][x] += remainDust;
        }
    }
}

void    cleanDust(const t_move &start)
{
    t_move cur = start;
    cur.move();
    t_move next = cur;
    next.move();

    while (next != start)
    {
        int &curSpace = CURMAP[cur.y][cur.x];
        int &nextSpace = CURMAP[next.y][next.x];

        curSpace = nextSpace;
        cur = next;
        next.move();
    }
    CURMAP[cur.y][cur.x] = 0;
}

int getTotalDust(void)
{
    int totalDust = 0;

    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            totalDust += CURMAP[y][x];
        }
    }
    return (totalDust + 2);
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int sec;

    std::cin >> ySize >> xSize >> sec;
    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            int &space = CURMAP[y][x];

            std::cin >> space;
            if (space == CLEANER)
            {
                if (cleaners.size() == 0)
                {
                    cleaners.push_back({
                        y,
                        x,
                        dirStart[0],
                        dirPlus[0],
                        0, y + 1,
                        0, xSize
                    });
                }
                else
                {
                    cleaners.push_back({
                        y,
                        x,
                        dirStart[1],
                        dirPlus[1],
                        y, ySize,
                        0, xSize
                    });
                }
            }
        }
    }

    while (sec--)
    {
        nextIdx = curIdx ^ 1;
        spreadDust();
        curIdx = nextIdx;
        for (const t_move &cleaner : cleaners)
        {
            cleanDust(cleaner);
        }
    }

    std::cout << getTotalDust() << '\n';
    return (0);
}