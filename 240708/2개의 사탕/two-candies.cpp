#include <iostream>
#include <climits>

enum e_space
{
    EMPTY = '.',
    WALL = '#',
    EXIT = 'O',
    BLUE = 'B',
    RED = 'R'
};

enum e_dir
{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
};

const int MAX_SIZE = 10;
const int DEPTH_LIMIT = 10;
const int dy[] = {-1, 1, 0, 0};
const int dx[] = {0, 0, -1, 1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int ySize, xSize;
int minMove = INT_MAX;
bool visited[MAX_SIZE][MAX_SIZE][MAX_SIZE][MAX_SIZE];

struct t_pos
{
    int y, x;

    inline bool operator==(const t_pos &other) const
    {
        return (this->y == other.y && this->x == other.x);
    }
};

struct t_board
{
    enum e_idx
    {
        RED = 0,
        BLUE
    };

    enum e_status
    {
        FAIL = 0,
        KEEP_GOING,
        SUCCESS
    };

    char map[MAX_SIZE][MAX_SIZE + 1];
    t_pos candies[2];

    inline bool isBlueFirst(int dir)
    {
        switch (dir)
        {
            case UP:
                return (candies[RED].y > candies[BLUE].y);
            case DOWN:
                return (candies[RED].y < candies[BLUE].y);
            case LEFT:
                return (candies[RED].x > candies[BLUE].x);
        }
        return (candies[RED].x < candies[BLUE].x);
    }

    inline int move(int dir)
    {
        bool moveIdx = isBlueFirst(dir);
        int moveCnt = 2;

        while (moveCnt--)
        {
            t_pos &pos = candies[moveIdx];

            map[pos.y][pos.x] = EMPTY;
            do
            {
                pos.y += dy[dir];
                pos.x += dx[dir];
            } while (map[pos.y][pos.x] == EMPTY);

            if (map[pos.y][pos.x] != EXIT)
            {
                pos.y -= dy[dir];
                pos.x -= dx[dir];
                map[pos.y][pos.x] = WALL;
            }

            moveIdx = !moveIdx;
        }

        const t_pos &red = candies[RED];
        const t_pos &blue = candies[BLUE];

        if (map[blue.y][blue.x] == EXIT)
            return (FAIL);

        if (map[red.y][red.x] == EXIT)
            return (SUCCESS);

        map[red.y][red.x] = e_space::RED;
        map[blue.y][blue.x] = e_space::BLUE;
        return (KEEP_GOING);
    }
};

void    backTracking(const t_board &cur, int prevDir=-1, int depth=0)
{
    if (depth == DEPTH_LIMIT)
    {
        return ;
    }

    const t_pos &cRed = cur.candies[t_board::RED];
    const t_pos &cBlue = cur.candies[t_board::BLUE];

    visited[cRed.y][cRed.x][cBlue.y][cBlue.x] = true;
    for (int dir = 0; dir < dirSize; ++dir)
    {
        if (dir == prevDir)
            continue ;

        if (minMove <= depth + 1)
            break ;

        t_board next = cur;
        int status = next.move(dir);
        const t_pos &nRed = next.candies[t_board::RED];
        const t_pos &nBlue = next.candies[t_board::BLUE];

        switch (status)
        {
            case t_board::SUCCESS:
                minMove = std::min(minMove, depth + 1);
                break ;
            case t_board::KEEP_GOING:
                if (!visited[nRed.y][nRed.x][nBlue.y][nBlue.x])
                    backTracking(next, dir, depth + 1);
                break ;
        }
    }
    visited[cRed.y][cRed.x][cBlue.y][cBlue.x] = false;
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    t_board init;

    std::cin >> ySize >> xSize;
    for (int y = 0; y < ySize; ++y)
    {
        std::cin >> init.map[y];

        for (int x = 0; x < xSize; ++x)
        {
            switch (init.map[y][x])
            {
                case e_space::RED:
                    init.candies[t_board::RED] = {y, x};
                    break ;
                case e_space::BLUE:
                    init.candies[t_board::BLUE] = {y, x};
                    break ;
            }
        }
    }

    backTracking(init);
    std::cout << (minMove == INT_MAX ? -1 : minMove) << '\n';
    return 0;
}