#include <iostream>

const int MAX_SIZE = 100;

int size, blockLen;
int height[MAX_SIZE + 1][MAX_SIZE + 1];

inline bool isBound(int y, int x)
{
    return ((0 <= y && y < size)
            && (0 <= x && x < size));
}

bool isWalkable(int y, int x, int dy, int dx)
{
    while (true)
    {
        const int &curHeight = height[y][x];
        if (curHeight == 0)
            break ;

        int curLen = 0;
        do
        {
            y += dy;
            x += dx;
            ++curLen;
        } while (curHeight == height[y][x]);

        if (!isBound(y, x))
            break ;

        const int &nextHeight = height[y][x];
        int diff = nextHeight - curHeight;
        if (diff == 1)
        {
            if (curLen < blockLen)
                return (false);
        }
        else if (diff == -1)
        {
            int nextLen = 1;

            while (nextLen < blockLen)
            {
                y += dy;
                x += dx;
                if (nextHeight != height[y][x])
                    break ;

                ++nextLen;
            }

            if (nextLen < blockLen)
                return (false);

            y += dy;
            x += dx;
            if (height[y][x] == nextHeight - 1)
            {
                y -= dy;
                x -= dx;
            }
            else if (height[y][x] != nextHeight)
            {
                return (false);
            }
        }
        else
        {
            return (false);
        }
    }

    return (true);
}

int main(void)
{
    std::cin.tie(0)->sync_with_stdio(0);

    std::cin >> size >> blockLen;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            std::cin >> height[y][x];
        }
    }

    int ans = 0;
    for (int pos = 0; pos < size; ++pos)
    {
        ans += isWalkable(pos, 0, 0, 1);
        ans += isWalkable(0, pos, 1, 0);
    }

    std::cout << ans << '\n';
    return 0;
}