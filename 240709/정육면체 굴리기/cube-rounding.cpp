#include <iostream>
#include <vector>

enum e_dir
{
    EAST = 1,
    WEST,
    NORTH,
    SOUTH
};

enum e_idx
{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    CEIL,
    FLOOR
};

const int MAX_SIZE = 20;
const int dy[] = {0, 0, 0, -1, 1};
const int dx[] = {0, 1, -1, 0, 0};

int ySize, xSize;
char map[MAX_SIZE][MAX_SIZE + 1];

struct t_dice
{
    const static int SIZE = 6;

    int y, x;
    std::vector<char> num;

    t_dice(void) : num(SIZE, '0') { }

    inline bool move(int dir)
    {
        int nextY = y + dy[dir];
        int nextX = x + dx[dir];

        if (!(0 <= nextY && nextY < ySize)
            || !(0 <= nextX && nextX < xSize))
            return (false);

        this->y = nextY;
        this->x = nextX;
        return (true);
    }

    inline void rotate(int dir)
    {
        // up, down, left, right, ceil, floor
        switch (dir)
        {
            case EAST:
                num = {num[UP], num[DOWN], num[FLOOR], num[CEIL], num[LEFT], num[RIGHT]};
                break ;
            case WEST:
                num = {num[UP], num[DOWN], num[CEIL], num[FLOOR], num[RIGHT], num[LEFT]};
                break ;
            case NORTH:
                num = {num[CEIL], num[FLOOR], num[LEFT], num[RIGHT], num[DOWN], num[UP]};
                break ;
            case SOUTH:
                num = {num[FLOOR], num[CEIL], num[LEFT], num[RIGHT], num[UP], num[DOWN]};
                break ;
        }

        char &floor = num[FLOOR];
        char &space = map[this->y][this->x];
        if (space == '0')
        {
            space = floor;
        }
        else
        {
            floor = space;
            space = '0';
        }
    }
};

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int cmdCnt;
    t_dice dice;

    std::cin >> ySize >> xSize >> dice.y >> dice.x >> cmdCnt;
    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            std::cin >> map[y][x];
        }
    }

    while (cmdCnt--)
    {
        int dir;

        std::cin >> dir;
        if (!dice.move(dir))
            continue ;

        dice.rotate(dir);
        std::cout << dice.num[CEIL] << '\n';
    }
    return 0;
}