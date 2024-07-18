#include <iostream>
#include <climits>

const int MAX_Y = 30 + 1;
const int MAX_X = 10 + 1;
const int DEPTH_LIMIT = 3;

int min = INT_MAX;
int ySize, xSize;
bool connected[MAX_Y][MAX_X];

bool isOk(void)
{
    static int finalX[MAX_X];

    for (int x = 1; x <= xSize; ++x)
    {
        finalX[x] = x;
    }

    for (int y = 1; y <= ySize; ++y)
    {
        for (int x = 1; x < xSize; ++x)
        {
            if (connected[y][x])
            {
                std::swap(finalX[x], finalX[x + 1]);
            }
        }
    }

    for (int x = 1; x <= xSize; ++x)
    {
        if (finalX[x] != x)
        {
            return (false);
        }
    }

    return (true);
}

inline bool nextOk(int y, int x)
{
    ++x;
    if (x > xSize)
        return (true);

    return !connected[y][x];
}

void    backTracking(int y=1, int x=1, int depth=0)
{
    if (isOk())
    {
        min = depth;
        return ;
    }

    if (depth == DEPTH_LIMIT)
    {
        return ;
    }

    while (true)
    {
        if (x > xSize)
        {
            x = 0;
            ++y;
            if (y > ySize)
                break ;
        }

        bool &curConnected = connected[y][x];
        if (!curConnected)
        {
            if (nextOk(y, x))
            {
                curConnected = true;
                backTracking(y, x + 2, depth + 1);
                curConnected = false;

                if (min <= depth + 1)
                    break ;
            }
        }
        else
        {
            ++x;
        }

        ++x;
    }
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int eCnt;

    std::cin >> xSize >> eCnt >> ySize;
    while (eCnt--)
    {
        int y, x;

        std::cin >> y >> x;
        connected[y][x] = true;
    }

    backTracking();
    std::cout << (min == INT_MAX ? -1 : min) << '\n';
    return 0;
}