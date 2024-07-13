#include <iostream>

const int MAX_SIZE = 50;
const int dy[] = {-1, 0, 1, 0};
const int dx[] = {0, 1, 0, -1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int ySize, xSize;
int curY, curX, dir;
bool visited[MAX_SIZE][MAX_SIZE];
bool isSidewalk[MAX_SIZE][MAX_SIZE];

int    simulation(void)
{
    int ans = 1;

    visited[curY][curX] = true;
    while (true)
    {
        int checkCnt = 4;
        while (checkCnt--)
        {
            dir = (dir + 3) % dirSize;
            int nextY = curY + dy[dir];
            int nextX = curX + dx[dir];

            if (!isSidewalk[nextY][nextX] && !visited[nextY][nextX])
            {
                ++ans;
                curY = nextY;
                curX = nextX;
                visited[curY][curX] = true;
                break ;
            }
        }

        if (checkCnt == -1)
        {
            curY -= dy[dir];
            curX -= dx[dir];
            if (isSidewalk[curY][curX])
                break ;
        }
    }

    return (ans);
}

int main(void)
{
    std::cin.tie(0)->sync_with_stdio(0);

    
    std::cin >> ySize >> xSize;
    std::cin >> curY >> curX >> dir;
    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            std::cin >> isSidewalk[y][x];
        }
    }

    std::cout << simulation() << '\n';
    return 0;
}