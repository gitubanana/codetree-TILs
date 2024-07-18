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
    for (int x = 1; x <= xSize; ++x)
    {
        int curX = x;
        int curY = 1;

        while (curY <= ySize)
        {
            for (int dx = -1; dx <= 0; ++dx)
            {
                if (connected[curY][curX + dx])
                {
                    curX += (dx == 0 ? 1 : -1);
                    break ;
                }
            }

            ++curY;
        }

        if (curX != x)
            return (false);
    }

    return (true);
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
            curConnected = true;
            backTracking(y, x + 2, depth + 1);
            curConnected = false;

            if (min <= depth + 1)
                break ;
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