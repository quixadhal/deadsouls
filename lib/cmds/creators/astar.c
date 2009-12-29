/* Tricky */

#include <astar.h>

mapping grid_points;
mapping map_costs;
mapping data;
mixed *path_ret;
string *map_ex;
string path, newpath = "";
string bap_str, map_str;
int *p_start, *p_end, *m_ind;

mixed cmd(string str)
{
    int sx, sy;
    int gx, gy;
    int cnt = 0;
    int odir, dir;
    int i, j, l, sz;
    int start, goal;

    write("foo");

    map_costs = ([
            ' ': 1,
            '*': 50,
            'X': 100,
            ]);

    bap_str =
        "          ########"+"\n"+
        "          # **XX #"+"\n"+
        "########### XX*X #"+"\n"+
        "# * #       XXX* #"+"\n"+
        "# #       ###### #"+"\n"+
        "#   ##### #   #  #"+"\n"+
        "###### #### # # ##"+"\n"+
        "       #    # # #"+"\n"+
        "## ###*#    #   #"+"\n"+
        "#      # # ######"+"\n"+
        "   ### ### #"+"\n"+
        "##  ##  ## #"+"\n"+
        "### #    # #"+"\n"+
        "###   ##   #"+"\n"+
        "############";

    map_str =
        "                                  #-#-#-#-#-#\n"+
        "                                  | | | | | |\n"+
        "                                  #-#-#-#-#-#\n"+
        "                                  | | | | | |\n"+
        "              #-#-#   #-#-#-#-#-#-#-#-#-#-#-#\n"+
        "              |   |   | | | | |             |\n"+
        "              #   #-#-#-#-#-#-#             #\n"+
        "              |   |           |             |\n"+
        "              #-#-#           #   #-#-#   #-#\n"+
        "                                  |   |   |\n"+
        "                        #         #   #   #\n"+
        "                        |         |   |   |\n"+
        "            #-#-#-#-#-#-#   #-#-#-#   #   #\n"+
        "                |       |   | | | |   |   |\n"+
        "                #       #   #-#-#-#   #-#-#\n"+
        "                |       |   |   |\n"+
        "              #-#-#-#-#-#   #   #\n"+
        "              | |       |       |\n"+
        "            #-#-#       #       #\n"+
        "                |       |       |\n"+
        "                #-#     #-#     #\n"+
        "                  |     | |     |\n"+
        "                  #   #-#-#-#   #\n"+
        "                  |   |     |   |\n"+
        "                  #-#-#     #-#-#\n";

    map_ex = explode(map_str, "\n");

    grid_points = ([
            "Start": ({ 1, 3 }),
            "End":   ({ 0, 7 }),
            ]);

    p_start = grid_points["Start"];
    p_end   = grid_points["End"];

    sx = p_start[0]; sy = p_start[1];
    gx = p_end[0]; gy = p_end[1];

    path_ret = find_path(map_str, sx, sy, gx, gy, map_costs);

    l = 0;

    for(i = 0; i < sizeof(map_ex); i++)
        for(j = 0; j < strlen(map_ex[i]); j++)
            if(member_array(map_ex[i][j], keys(map_costs)) != -1) l++;

    write("Walkable paths: " + l + "\n");

    if(sizeof(path_ret))
    {
        path = path_ret[0];
        data = ([ ]) + path_ret[1];

        m_ind = keys(data);
        sz = sizeof(m_ind);
        write("Walkable paths checked: " + sz + "\n");

        while(--sz != -1)
        {
            int x, y;

            l = m_ind[sz];

            x = data[l][AS_X];
            y = data[l][AS_Y];

            if(map_ex[y][x] == ' ') map_ex[y][x] = '.';

        }

        start = sizeof(map_ex[0]) * sy + sx;
        goal  = sizeof(map_ex[0]) * gy + gx;

        l = goal;

        while(l != start)
        {
            int x, y;

            l = data[l][AS_PARENT];
            x = data[l][AS_X];
            y = data[l][AS_Y];

            map_ex[y][x] = 'o';
        }

    }
    else path = 0;

    map_ex[sy][sx] = 'S';
    map_ex[gy][gx] = 'E';

    write("\n");

    write(map_str + "\n");
    write("\n");
    write(implode(map_ex, "\n") + "\n");
    write("\n");

    write("sx, sy: " + sx + ", " + sy + "\n");
    write("gx, gy: " + gx + ", " + gy + "\n");
    write("\n");

    if(mapp(data))
    {
        write("Path cost: " + data[goal][AS_FCOST] + "\n");
        write("\n");
    }

    if(path) write("You should go: " + path + "\n");
    else write("No path could be found.\n");

    return 1;
}
