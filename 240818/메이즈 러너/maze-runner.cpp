#include <iostream>
#include <cstring>
#include <climits>
#include <queue>
#define CURMAP map[curIdx]
#define CURCNT cnt[curIdx]
#define NEXTMAP map[nextIdx]
#define NEXTCNT cnt[nextIdx]

enum e_space
{
    EXIT = -1,
    EMPTY
};

struct t_pos
{
    int y, x;

    inline bool operator<(const t_pos &other) const
    {
        if (this->y == other.y)
            return (this->x < other.x);

        return (this->y < other.y);
    }
};

const int MAX_SIZE = 10;

int size;
int playerCnt;
int totalMove;
int curIdx, nextIdx;
int map[2][MAX_SIZE][MAX_SIZE];
int cnt[2][MAX_SIZE][MAX_SIZE];
t_pos exitPos;

inline std::istream &operator>>(std::istream &cin, t_pos &pos)
{
    cin >> pos.y >> pos.x;
    --pos.y, --pos.x;
    return (cin);
}

inline bool inRange(const t_pos &pos)
{
    return ((0 <= pos.y && pos.y < size)
            && (0 <= pos.x && pos.x < size));
}

inline int getDist(const t_pos &a, const t_pos &b)
{
    return std::abs(a.y - b.y) + std::abs(a.x - b.x);
}

void    movePlayers(void)
{
    static const int dy[] = {0, 0, -1, 1};
    static const int dx[] = {-1, 1, 0, 0};
    static const int dirSize = sizeof(dy) / sizeof(dy[0]);

    memset(NEXTCNT, 0, sizeof(NEXTCNT));
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            int &curCnt = CURCNT[y][x];
            if (curCnt == 0)
                continue ;

            t_pos next = {y, x};
            int prevDist = getDist({y, x}, exitPos);
            for (int dir = 0; dir < dirSize; ++dir)
            {
                t_pos check = {
                    y + dy[dir],
                    x + dx[dir]
                };

                if (!inRange(check)
                    || CURMAP[check.y][check.x] > EMPTY)
                    continue ;

                int nextDist = getDist(check, exitPos);
                if (prevDist > nextDist)
                {
                    prevDist = nextDist;
                    next = check;
                }
                else if (prevDist == nextDist)
                {
                    next = check;
                }
            }

            totalMove += curCnt * (next.y != y || next.x != x);
            if (CURMAP[next.y][next.x] != EXIT)
            {
                NEXTCNT[next.y][next.x] += curCnt;
            }
            else
            {
                playerCnt -= curCnt;
            }

            curCnt = 0;
        }
    }
}

bool isValid(int startY, int startX, int squareSize)
{
    bool playerHere = false;
    bool exitHere = false;

    int endY = startY + squareSize;
    int endX = startX + squareSize;
    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            playerHere |= (NEXTCNT[y][x] > 0);
            exitHere |= (CURMAP[y][x] == EXIT);
        }
    }

    return (playerHere && exitHere);
}

void    rotateMinSquare(void)
{
    int leftUpY, leftUpX;
    int squareSize = INT_MAX;

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            int cmpSize = 1;

            while (inRange({y + cmpSize, x + cmpSize}))
            {
                if (isValid(y, x, cmpSize)
                    && squareSize > cmpSize)
                {
                    squareSize = cmpSize;
                    leftUpY = y;
                    leftUpX = x;
                    break ;
                }

                ++cmpSize;
            }
        }
    }

    int rightDownY = leftUpY + squareSize;
    int rightDownX = leftUpX + squareSize;

    memcpy(CURCNT, NEXTCNT, sizeof(CURCNT));
    memcpy(NEXTMAP, CURMAP, sizeof(NEXTMAP));
    for (int y = 0; y <= squareSize; ++y)
    {
        for (int x = 0; x <= squareSize; ++x)
        {
            int putY = leftUpY + y;
            int putX = leftUpX + x;
            int originY = rightDownY - x;
            int originX = leftUpX + y;

            int &curSpace = CURMAP[originY][originX];

            if (curSpace == EXIT)
            {
                exitPos = {putY, putX};
            }

            NEXTMAP[putY][putX] = curSpace - (curSpace > EMPTY);
            NEXTCNT[putY][putX] = CURCNT[originY][originX];
        }
    }
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int sec;

    std::cin >> size >> playerCnt >> sec;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            std::cin >> CURMAP[y][x];
        }
    }

    for (int i = 0; i < playerCnt; ++i)
    {
        t_pos pos;

        std::cin >> pos;
        ++CURCNT[pos.y][pos.x];
    }

    std::cin >> exitPos;
    CURMAP[exitPos.y][exitPos.x] = EXIT;

    while (sec--)
    {
        nextIdx = curIdx ^ 1;

        movePlayers();
        if (playerCnt == 0)
            break ;

        rotateMinSquare();

        curIdx = nextIdx;
    }

    std::cout << totalMove << '\n';
    std::cout << exitPos.y + 1 << ' ' << exitPos.x + 1 << '\n';
    return 0;
}