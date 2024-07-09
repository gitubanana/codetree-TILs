#include <iostream>
#include <vector>
#include <tuple>

enum e_dir
{
    EAST = 1,
    WEST,
    NORTH,
    SOUTH
};

const int MAX_SIZE = 20;
const int dy[] = {0, 0, 0, -1, 1};
const int dx[] = {0, 1, -1, 0, 0};

int ySize, xSize;
char map[MAX_SIZE][MAX_SIZE + 1];

struct t_dice
{
    const static int FACE_NUM = 6 + 1;

    int y, x;
    int up, front, right;
    std::vector<char> num;

    t_dice(void) : up(1), front(2), right(3), num(FACE_NUM, '0') { }

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
        switch (dir)
        {
            case EAST:
                std::tie(up, front, right) = std::make_tuple(FACE_NUM - right, front, up);
                break ;
            case WEST:
                std::tie(up, front, right) = std::make_tuple(right, front, FACE_NUM - up);
                break ;
            case NORTH:
                std::tie(up, front, right) = std::make_tuple(front, FACE_NUM - up, right);
                break ;
            case SOUTH:
                std::tie(up, front, right) = std::make_tuple(FACE_NUM - front, up, right);
                break ;
        }

        char &floor = num[FACE_NUM - up];
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
        std::cout << dice.num[dice.up] << '\n';
    }
    return 0;
}