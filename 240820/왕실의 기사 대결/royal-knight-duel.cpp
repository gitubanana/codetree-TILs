#include <iostream>
#include <cstring>
#include <vector>

struct t_knight;

enum e_space
{
    EMPTY = 0,
    TRAP,
    WALL
};

const int MAX_SIZE = 40;
const int MAX_KNIGHT = 30;
const int dy[] = {-1, 0, 1, 0};
const int dx[] = {0, 1, 0, -1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int size, knightCnt;
int map[MAX_SIZE][MAX_SIZE];
int knightID[MAX_SIZE][MAX_SIZE];
bool visited[MAX_KNIGHT + 1];

struct t_pos
{
    int y, x;
};

struct t_knight : public t_pos
{
    int width, height;
    int HP, damaged;
    int id;

    inline bool isDead(void) const
    {
        return (HP - damaged <= 0);
    }

    void setValue(int value) const
    {
        int endY = y + height;
        int endX = x + width;

        for (int y = this->y; y < endY; ++y)
        {
            for (int x = this->x; x < endX; ++x)
            {
                knightID[y][x] = value;
            }
        }
    }
};

struct t_move
{
    int id;
    int damage;
};

std::vector<t_move> toMove;
t_knight knights[MAX_KNIGHT + 1];

inline std::istream &operator>>(std::istream &cin, t_knight &input)
{
    cin >> input.y >> input.x >> input.height >> input.width >> input.HP;
    --input.y, --input.x;
    return (cin);
}

inline bool inRange(int y, int x)
{
    return ((0 <= y && y < size)
            && (0 <= x && x < size));
}

bool    dfs(t_knight &cur, int dir, int depth=0)
{
    int damage = 0;
    t_pos next = {
        cur.y + dy[dir],
        cur.x + dx[dir]
    };
    t_pos end = {
        next.y + cur.height,
        next.x + cur.width
    };

    // std::cout << "==== id : " << cur.id << " ====" << '\n';
    visited[cur.id] = true;
    for (int y = next.y; y < end.y; ++y)
    {
        for (int x = next.x; x < end.x; ++x)
        {
            // std::cout << "cur : " << y << ", " << x << '\n';
            if (!inRange(y, x) || map[y][x] == WALL)
                return (false);

            int &nextId = knightID[y][x];

            // std::cout << "nextId : " << nextId << '\n';
            if (map[y][x] == TRAP)
            {
                ++damage;
            }

            if (nextId != EMPTY && !visited[nextId])
            {
                if (!dfs(knights[nextId], dir, depth + 1))
                    return (false);
            }
        }
    }

    toMove.push_back({cur.id, damage * (depth != 0)});
    return (true);
}

int getAliveTotalDamaged(void)
{
    int totalDamaged = 0;

    for (int id = 1; id <= knightCnt; ++id)
    {
        t_knight &cur = knights[id];
        if (cur.isDead())
            continue ;

        totalDamaged += cur.damaged;
    }
    return (totalDamaged);
}

void    printId(std::string str)
{
    std::cout << "=== " << str << " ===" << '\n';
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            std::cout << knightID[y][x] << ' ';
        }
        std::cout << '\n';
    }
}

int main(void)
{
    std::cin.tie(0)->sync_with_stdio(0);

    int cmdCnt;

    std::cin >> size >> knightCnt >> cmdCnt;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            std::cin >> map[y][x];
        }
    }

    for (int id = 1; id <= knightCnt; ++id)
    {
        t_knight &cur = knights[id];

        std::cin >> cur;
        cur.id = id;
        cur.setValue(cur.id);
    }

    // printId("init id");
    while (cmdCnt--)
    {
        int id, dir;
        std::cin >> id >> dir;

        t_knight &cur = knights[id];
        if (cur.isDead())
            continue ;

        toMove.clear();
        memset(visited, false, sizeof(visited));
        if (!dfs(cur, dir))
            continue ;

        for (const t_move &move : toMove)
        {
            t_knight &cur = knights[move.id];

            cur.damaged += move.damage;
            cur.setValue(EMPTY);
            if (cur.isDead())
                continue ;

            cur.y += dy[dir], cur.x += dx[dir];
            cur.setValue(cur.id);
        }
        // printId("after dfs");
    }

    std::cout << getAliveTotalDamaged() << '\n';
    return 0;
}