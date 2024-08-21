#include <iostream>
#include <vector>
#include <queue>
#include <set>
#define CURBEST bestSales[curIdx]
#define NEXTBEST bestSales[nextIdx]

enum e_cmd
{
    CREATE = 200,
    CANCEL = 300,
    BEST = 400,
    CHANGE_START = 500
};

struct t_edge
{
    int v;
    int cost;

    inline bool operator<(const t_edge &other) const
    {
        return (this->cost > other.cost);
    }
};

struct t_sale
{
    int revenue;
    int dest;
};

struct t_cmp
{
    int id;
    int money;

    inline bool operator<(const t_cmp &other) const
    {
        if (this->money == other.money)
            return (this->id < other.id);

        return (this->money > other.money);
    }
};

const int MAX_ID = 30000;
const int MAX_V = 2000;
const int INF = 1e9;

int vCnt;
int dists[MAX_V];
t_sale sales[MAX_ID + 1];
int curIdx, nextIdx;
std::set<t_cmp> bestSales[2];
std::vector<t_edge> edges[MAX_V];

void    dijkstra(int start=0)
{
    std::priority_queue<t_edge> pq;

    for (int v = 0; v < vCnt; ++v)
    {
        dists[v] = INF;
    }

    dists[start] = 0;
    pq.push({start, 0});
    while (!pq.empty())
    {
        const t_edge cur = pq.top();
        pq.pop();

        if (dists[cur.v] != cur.cost)
            continue ;

        for (const t_edge &next : edges[cur.v])
        {
            int &cmpDist = dists[next.v];
            int nextDist = dists[cur.v] + next.cost;
            if (cmpDist <= nextDist)
                continue ;

            cmpDist = nextDist;
            pq.push({next.v, nextDist});
        }
    }
}

void    createSale(void)
{
    int id;
    std::cin >> id;

    t_sale &cur = sales[id];
    std::cin >> cur.revenue >> cur.dest;

    CURBEST.insert({id, cur.revenue - dists[cur.dest]});
}

void    cancelSale(void)
{
    int id;
    std::cin >> id;

    t_sale &cur = sales[id];
    CURBEST.erase({id, cur.revenue - dists[cur.dest]});
}

void    printBestSale(void)
{
    int bestId = -1;

    for (const t_cmp &cur : CURBEST)
    {
        if (cur.money >= 0)
        {
            bestId = cur.id;
            CURBEST.erase(cur);
            break ;
        }
    }

    std::cout << bestId << '\n';
}

void    changeStart(void)
{
    int start;
    std::cin >> start;

    dijkstra(start);

    nextIdx = curIdx ^ 1;

    NEXTBEST.clear();
    for (const t_cmp &cur : CURBEST)
    {
        t_sale &sale = sales[cur.id];

        NEXTBEST.insert({cur.id, sale.revenue - dists[sale.dest]});
    }

    curIdx = nextIdx;
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int cmd;
    int eCnt, cmdCnt;

    std::cin >> cmdCnt;
    std::cin >> cmd >> vCnt >> eCnt;
    while (eCnt--)
    {
        int from, to, cost;

        std::cin >> from >> to >> cost;
        edges[from].push_back({to, cost});
        edges[to].push_back({from, cost});
    }

    dijkstra();
    while (--cmdCnt)
    {
        std::cin >> cmd;
        switch (cmd)
        {
            case CREATE:
                createSale();
                break;
            case CANCEL:
                cancelSale();
                break;
            case BEST:
                printBestSale();
                break;
            case CHANGE_START:
                changeStart();
                break;
        }
    }
    return (0);
}