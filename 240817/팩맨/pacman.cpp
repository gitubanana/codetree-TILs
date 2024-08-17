#include <iostream>
#include <cstring>
#include <vector>
#define CURMAP map[curIdx]
#define NEXTMAP map[nextIdx]

struct t_pos
{
    int y, x;

    inline bool operator==(const t_pos &other) const
    {
        return (this->y == other.y && this->x == other.x);
    }

    inline bool operator!=(const t_pos &other) const
    {
        return !(*this == other);
    }
};

const int SIZE = 4;
const int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dx[] = {0, -1, -1, -1, 0, 1, 1, 1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int maxEat;
t_pos pacMan;
int curTime;
int curIdx, nextIdx;
std::vector<t_pos> ate, cmp;
bool visited[SIZE][SIZE];
int deadTime[SIZE][SIZE];
std::vector<int> map[2][SIZE][SIZE];

inline std::istream &operator>>(std::istream &cin, t_pos &input)
{
    cin >> input.y >> input.x;
    --input.y, --input.x;
    return (cin);
}

inline bool inRange(const t_pos &pos)
{
    return ((0 <= pos.y && pos.y < SIZE)
            && (0 <= pos.x && pos.x < SIZE));
}

inline bool isDead(const t_pos &pos)
{
    static const int LAST_TIME = 2;

    int &cmpTime = deadTime[pos.y][pos.x];
    if (cmpTime == 0)
        return (false);

    return (curTime - cmpTime <= LAST_TIME);
}

void    moveMonsters(void)
{
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            for (const int &dir : CURMAP[y][x])
            {
                int curDir = dir;
                int dirCnt = dirSize;
                t_pos next = {y, x};

                while (dirCnt--)
                {
                    t_pos check = {
                        y + dy[curDir],
                        x + dx[curDir]
                    };

                    if (inRange(check)
                        && check != pacMan
                        && !isDead(check))
                    {
                        next = check;
                        break ;
                    }

                    curDir = (curDir + 1) % dirSize;
                }

                NEXTMAP[next.y][next.x].push_back(curDir);
            }
        }
    }
}

void    backTracking(const t_pos &cur, int curEat=0, int depth=0)
{
    static const int DEPTH_LIMIT = 3;
    static const int dy[] = {-1, 0, 1, 0};
    static const int dx[] = {0, -1, 0, 1};
    static const int dirSize = sizeof(dy) / sizeof(dy[0]);

    if (depth == DEPTH_LIMIT)
    {
        if (maxEat < curEat)
        {
            maxEat = curEat;
            pacMan = cur;
            ate = cmp;
        }
        return ;
    }

    for (int dir = 0; dir < dirSize; ++dir)
    {
        t_pos next = {
            cur.y + dy[dir],
            cur.x + dx[dir]
        };

        if (!inRange(next)
            || visited[next.y][next.x])
            continue ;

        visited[next.y][next.x] = true;
        cmp.push_back(next);

        backTracking(next, curEat + NEXTMAP[next.y][next.x].size(), depth + 1);

        visited[next.y][next.x] = false;
        cmp.pop_back();
    }
}

void    movePacMan(void)
{
    maxEat = -1;
    t_pos start = pacMan;

    memset(visited, false, sizeof(visited));
    backTracking(start);

    for (const t_pos &pos : ate)
    {
        NEXTMAP[pos.y][pos.x].clear();
        deadTime[pos.y][pos.x] = curTime;
    }
}

void    duplicateMonsters(void)
{
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            for (const int &dir : CURMAP[y][x])
            {
                NEXTMAP[y][x].push_back(dir);
            }
            CURMAP[y][x].clear();
        }
    }
}

int getMonsterCnt(void)
{
    int cnt = 0;

    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            cnt += CURMAP[y][x].size();
        }
    }
    return (cnt);
}

void    printMap(std::string str)
{
    std::cout << "=== " << str << " ===" << '\n';
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            std::cout << CURMAP[y][x].size() << ' ';
        }
        std::cout << '\n';
    }
}

void    printNextMap(std::string str)
{
    std::cout << "=== " << str << " ===" << '\n';
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            std::cout << NEXTMAP[y][x].size() << ' ';
        }
        std::cout << '\n';
    }
}

int main(void)
{
    std::cin.tie(0)->sync_with_stdio(0);

    int monsterCnt, turn;

    std::cin >> monsterCnt >> turn >> pacMan;
    while (monsterCnt--)
    {
        int dir;
        t_pos pos;

        std::cin >> pos >> dir;
        CURMAP[pos.y][pos.x].push_back(dir - 1);
    }

    while (turn--)
    {
        ++curTime;
        nextIdx = curIdx ^ 1;

        // printMap("before move Monsters curMap");
        moveMonsters();
        // printNextMap("after move Monsters NextMap");

        movePacMan();
        // printNextMap("after move PacMan NextMap");

        duplicateMonsters();
        // printNextMap("after duplicate NextMap");

        curIdx = nextIdx;
    }

    std::cout << getMonsterCnt() << '\n';
    return 0;
}