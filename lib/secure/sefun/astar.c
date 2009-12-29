/* Tricky */

#include <astar.h>

/* ([ mappos: ({ parent, x, y, g_cost, h_cost, f_cost }), ... ]) */
static mapping data;

/* ([ id: ([ "pos": mappos, "fcost": f ]), ... ]) */
static mapping binaryHeap;

/* Number of items on the binary heap. */
static int numItems;
static int maxItems;

static int l1, l2;

void initHeap()
{
    l1 = l2 = 0;
    maxItems = numItems = 0;
    binaryHeap = ([ ]);
}

int getHeadHeap() { return binaryHeap[1]["pos"]; }

#undef LOG_FIND

int findHeap(int d)
{
    int *nodes;
    int u, v, f;
    int lc, rc;

    /* Sanity check. */
    if(!numItems) return 0; /* Heap is empty, return 0. */

    if(member_array(d, keys(data)) == -1) return 0; /* Not come across this item before. */

    if(numItems == 1)
    {

        if(d == binaryHeap[1]["pos"]) return 1;

        return 0;
    }

    nodes = allocate(numItems + 1);
    nodes[0] = 1;

    f = data[d][AS_FCOST];
    v = 1;

    while (v <= numItems)
    {
        nodes[v]++;

        if(d == binaryHeap[v]["pos"]) break;

        u = v;
        rc = (lc = u << 1) + 1;

        if(nodes[u] < 3)
        {

            if(rc <= numItems)
            {

                if(nodes[u] == 1 && f >= binaryHeap[lc]["fcost"]) v = lc;
                else
                    if(f >= binaryHeap[rc]["fcost"])
                    {
                        nodes[u] = 2;
                        v = rc;
                    }

                nodes[0]++;
            }
            else
                if(nodes[u] == 1 && lc <= numItems)
                {

                    if(f >= binaryHeap[lc]["fcost"]) v = lc;

                    nodes[0]++;
                }

        }

        if(nodes[u] == 4)
        {
            v = 0;
            break;
        }

        if(u == v)
        {
            v >>= 1;

            if(!v) break;

        }

    }

    if(v > numItems) v = 0;

#ifdef LOG_FIND
    if(l1 < 64 && !v)
    {
        int i, dx;

        l1++;

        log_file("HEAP", sprintf("Finding %d(%d). Checks made: %d ... Not Found!\n", d, f, nodes[0]));
        /* log_file("HEAP", sprintf("binaryHeap = %O\nnodes = %O\n", binaryHeap, nodes)); */

        i = 1;

        while(i <= numItems)
        {
            string *tmp = ({ });
            string str = "";

            dx = (80.0 / (float)i);

            for(int j = i; j < i << 1; j++)
            {

                if(j <= numItems) tmp += ({ "|" });
                else tmp += ({ "" });

            }

            log_file("HEAP", sprintf("%@|"+dx+"s\n", tmp));

            tmp = ({ });
            tmp = allocate(i);

            for(int j = i; j < i << 1; j++)
            {

                if(j <= numItems)
                    tmp[j - i] = sprintf("%|"+(dx >> 1)+"'_'d", binaryHeap[j]["fcost"]);
                else tmp[j - i] = "";

            }

            i <<= 1;

            log_file("HEAP", sprintf("%@|"+dx+"s\n", tmp));
        }

        log_file("HEAP", "\n");
    }

    if(l2 < 64 && v && numItems > 7)
    {
        int i, dx;

        l2++;

        log_file("HEAP", sprintf("Finding %d(%d). Checks made: %d ... Result = %d\n", d, f, nodes[0], v));
        /* log_file("HEAP", sprintf("binaryHeap = %O\nnodes = %O\n", binaryHeap, nodes)); */

        i = 1;

        while(i <= numItems)
        {
            string *tmp = ({ });
            string str = "";

            dx = (80.0 / (float)i);

            for(int j = i; j < i << 1; j++)
            {

                if(j <= numItems) tmp += ({ "|" });
                else tmp += ({ "" });

            }

            log_file("HEAP", sprintf("%@|"+dx+"s\n", tmp));

            tmp = ({ });
            tmp = allocate(i);

            for(int j = i; j < i << 1; j++)
            {

                if(j <= numItems)
                {

                    if(j == v)
                        tmp[j - i] = sprintf("%|"+(dx >> 1)+"'_'s", "["+binaryHeap[j]["fcost"]+"]");
                    else
                        tmp[j - i] = sprintf("%|"+(dx >> 1)+"'_'d", binaryHeap[j]["fcost"]);

                }
                else tmp[j - i] = "";

            }

            i <<= 1;

            log_file("HEAP", sprintf("%@|"+dx+"s\n", tmp));
        }

        log_file("HEAP", "\n");
    }
#endif

    return v;
}

void resortHeapDown(int v)
{

    while(v <= numItems)
    {
        mapping parent, lchild, rchild;
        int u, lc, rc;

        u = v;

        rc = (lc = u << 1) + 1;

        parent = ([ ]) + binaryHeap[u];

        if(rc <= numItems)
        {
            lchild = ([ ]) + binaryHeap[lc];
            rchild = ([ ]) + binaryHeap[rc];

            if(parent["fcost"] > lchild["fcost"] &&
                    parent["fcost"] > rchild["fcost"])
            {

                if(lchild["fcost"] > rchild["fcost"]) v = rc;
                else v = lc;

            }
            else
                if(parent["fcost"] > lchild["fcost"]) v = lc;
                else
                    if(parent["fcost"] > rchild["fcost"]) v = rc;

        }
        else
            if(lc <= numItems)
            {

                if(parent["fcost"] > binaryHeap[lc]["fcost"]) v = lc;

            }

        if(u == v) break;

        binaryHeap[u] = ([ ]) + binaryHeap[v];
        binaryHeap[v] = ([ ]) + parent;
    }

}

void resortHeapUp(int m)
{
    mapping tmp;

    while(m > 1)
    {
        int m2 = m >> 1;

        if (binaryHeap[m]["fcost"] >= binaryHeap[m2]["fcost"]) break;

        tmp            = ([ ]) + binaryHeap[m];
        binaryHeap[m]  = ([ ]) + binaryHeap[m2];
        binaryHeap[m2] = ([ ]) + tmp;

        m = m2;
    }

}

void addHeap(int d)
{
    numItems++;

    if(numItems > maxItems) maxItems = numItems;

    binaryHeap[numItems] = ([ "pos": d, "fcost": data[d][AS_FCOST] ]);

    if(numItems > 1) resortHeapUp(numItems);

}

void removeHeadHeap()
{

    if(!numItems) return 0; /* Heap is empty, return 0. */

    /* Replace the head item with the tail item. */
    if(numItems > 1) binaryHeap[1] = ([ ]) + binaryHeap[numItems];

    /* Chop off the tail. */
    map_delete(binaryHeap, numItems);

    if(--numItems < 2) return;

    resortHeapDown(1);

    return;
}

int calc_h(int sx, int sy, int ex, int ey)
{
    int dx, dy;

    dx = sx - ex;
    dy = sy - ey;

    return (sqrt(dx * dx + dy * dy) * 0.975);

    /* Manhattan method. */
    /* return (abs(dx) + abs(dy)); */
}

string make_path(int start, int goal, int goalx, int goaly)
{
    mapping x_dir, y_dir;
    string *dirs;
    string path;
    int i, oldx, oldy, newx, newy;

    path = "";
    i = goal;
    oldx = goalx; oldy = goaly;
    x_dir = ([ -1: ({ 3, 4, 5 }), 0: ({ 2, 9, 6 }), 1: ({ 1, 0, 7 }) ]);
    y_dir = ([ -1: ({ 3, 2, 1 }), 0: ({ 4, 9, 0 }), 1: ({ 5, 6, 7 }) ]);
    dirs  = ({ "e", "ne", "n", "nw", "w", "sw", "s", "se", "*" });

    while(i != start)
    {
        int *xd, *yd;

        i    = data[i][AS_PARENT];
        newx = data[i][AS_X];
        newy = data[i][AS_Y];

        xd   = x_dir[oldx - newx];
        yd   = y_dir[oldy - newy];
        path = dirs[(xd & yd)[0]] + path;

        oldx = newx;
        oldy = newy;
    }

    return path;
}

mixed *find_path(string map, int startx, int starty, int goalx, int goaly, mapping costs)
{
    string *split_map;
    int *closed;
    int map_width, start, goal;
    int currpos;
    int newx, newy, newpos;
    int oldf, g, h;
    int id;

    /* Process the map into an easier-to-use format. */
    split_map = explode(map, "\n");
    map_width = 0;

    /* Find the length of the longest line in the "map" */
    for(int i = sizeof(split_map); i-- ;)
        if(map_width < strlen(split_map[i]))
            map_width = strlen(split_map[i]);

    /* Make all the lines that length by padding with escape characters.
     *      (Note: I use escapes because they are an unlikely character to be
     *           chosen for walking over, and unused characters are 'walls') */
    for(int i = sizeof(split_map); i-- ;)
        split_map[i] += sprintf("%" + (map_width - strlen(split_map[i])) + "'\27's", "");

    /* Sanity checks */
    if(goalx < 0  || goalx >= map_width || goaly < 0 || goaly >= sizeof(split_map))
        return ({ });

    if(member_array(split_map[starty][startx], keys(costs)) == -1 ||
            member_array(split_map[goaly][goalx], keys(costs)) == -1)
        return ({ });

    /* Setup initial state. */
    start = startx + starty * map_width;
    goal = goalx + goaly * map_width;

    data   = ([ ]);
    closed = ({ });

    g = 0;
    h = calc_h(startx, starty, goalx, goaly);
    write("Distance estimate: " + h + "\n");

    data[start] = ({ -1, startx, starty, g, h, g + h });

    /* Initialise the binary heap. */
    initHeap();
    addHeap(start);

    while(numItems && member_array(goal, closed) == -1)
    {
        currpos = getHeadHeap();
        closed += ({ currpos });
        removeHeadHeap();

        for(int ineighbour = 0; ineighbour < 8; ineighbour += 2)
        {
            newx = (currpos % map_width) + ({  1,  1,  0, -1, -1, -1,  0,  1 })[ineighbour];
            newy = (currpos / map_width) + ({  0, -1, -1, -1,  0,  1,  1,  1 })[ineighbour];

            /* Out of bounds, ignore it. */
            if(newx < 0 || newx >= map_width ||
                    newy < 0 || newy >= sizeof(split_map))
                continue;

            /* Solid wall, ignore it. */
            if(member_array(split_map[newy][newx], keys(costs)) == -1) continue;

            newpos = newy * map_width + newx;
            /* Already checked, ignore it. */
            if(member_array(newpos, closed) != -1) continue;

            g = data[currpos][AS_GCOST] + costs[split_map[newy][newx]];

            if(!(id = findHeap(newpos)))
            {
                h = calc_h(newx, newy, goalx, goaly);

                data[newpos] = allocate(AS_DATA_SZ);
                data[newpos][AS_PARENT] = currpos;
                data[newpos][AS_X]      = newx;
                data[newpos][AS_Y]      = newy;
                data[newpos][AS_GCOST]  = g;
                data[newpos][AS_HCOST]  = h;
                data[newpos][AS_FCOST]  = g + h;

                addHeap(newpos);
            }
            else
            {

                if(g < data[newpos][AS_GCOST])
                {
                    oldf = data[newpos][AS_FCOST];

                    h = calc_h(newx, newy, goalx, goaly);

                    data[newpos][AS_PARENT] = currpos;
                    data[newpos][AS_X]      = newx;
                    data[newpos][AS_Y]      = newy;
                    data[newpos][AS_GCOST]  = g;
                    data[newpos][AS_HCOST]  = h;
                    data[newpos][AS_FCOST]  = g + h;

                    if(g + h != oldf)
                    {
                        binaryHeap[id]["fcost"] = g + h;

                        if(g + h < oldf) resortHeapUp(id);
                        else
                            if(g + h > oldf) resortHeapDown(id);

                    }

                }

            }

        }

    }

    write("Maximum binary heap size: " + maxItems + "\n");

    if(member_array(goal, closed) == -1) return ({ });

    return ({ make_path(start, goal, goalx, goaly), data });
}
