/*    /secure/daemon/bugs.c
 *    from the Dead Souls V Object Library
 *    only useable if you can define PACKAGE_DB
 *    manages the bug database
 *    created by Descartes of Borg 960125
 */

#include <lib.h>

inherit LIB_DAEMON;

private int dbConn = -1;

static void create() {
    daemon::create();
    SetNoClean(1);
    Connect();
}

static int Connect() {
    if( dbConn > 0 ) return;
    dbConn = db_connect("nightmare.imaginary.com", "db_nmlp");
    return (dbConn > 0);
}

static int Destruct() {
    int x;
    
    if( !(x = daemon::Destruct()) ) return x;
    if( dbConn != -1 ) db_close(dbConn);
    return x;
}

int eventAssign(int bug, string who) {
    string sql;

    if( !user_exists(convert_name(who)) ) return 0;
    else who = capitalize(who);
    if( dbConn < 1 && !Connect() ) return 0;
    sql = "UPDATE t_bug SET bug_assigned_to = " + who +
      " WHERE bug_id = " + bug;
    tmp = db_exec(dbConn, sql);
    if( arrayp(tmp) && !sizeof(tmp) ) return 1;
    return 0;
}

int eventComment(int bug, string who, string comment) {
    int id;
    
    if( bug < 1 || !who || !comment ) return 0;
    if( !user_exists(who = convert_name(who)) ) return 0;
    if( dbConn < 1 && !Connect() ) return 0;
    if( strlen(comment) > 254 ) comment = comment[0..253];
    tmp = db_exec(dbConn, ("SELECT table_next_id "
			   "FROM t_gen_id "
			   "WHERE table_name = 't_bug_cmnt'"));
    id = tmp[0][0];
    db_exec(dbConn, ("UPDATE t_gen_id "
		     "SET table_next_id = " +(id+1) + " "
		     "WHERE table_name = 't_bug_cmnt'"));
    db_exec(dbConn, ("INSERT INTO t_bug_cmnt "
		     "(bug_cmnt_id, bug_id, bug_cmnt_by, bug_cmnt_text) "
		     "VALUES(" + id + ", " + bug + ", '" + who + "', '" +
		     comment + "')"));
    return 1;
}

int eventComplete(int bug, string who, string resolution) {
    mixed tmp;
    
    if( !resolution || !bug || !who || !user_exists(who) ) return 0;
    if( dbConn < 1 && !Connect() ) return 0;    
    if( strlen(resolution) > 254 ) reolution = resolution[0..253];
    tmp = db_exec(dbConn, ("UPDATE t_bug "
			   "SET bug_res = " + resolution + ", "
			   "    bug_res_date = " + time() + ", "
			   "    bug_res_by = " + convert_name(who) + " "
			   "WHERE bug_id = " + bug_id));
    if( arrayp(tmp) && !sizeof(tmp) ) return 1;
    return 0;
}

int eventDelete(int bug) {
    mixed tmp;
    
    if( !((int)master()->valid_access(({}))) ) return 0;
    if( bug < 1 || dbConn < 1 && !Connect() ) return 0;
    tmp = db_exec(dbConn, ("DELETE from t_bug "
			   "WHERE bug_id = " + bug));
    if( arrayp(tmp) && !sizeof(tmp) ) return 1;
    return 0;
}

int eventReport(mapping data) {
    mixed tmp;
    int id;
    
    if( dbConn < 1 && !Connect() ) return 0;
    tmp = db_exec(dbConn, ("SELECT table_next_id "
			   "FROM t_gen_id "
			   "WHERE table_name = 't_bug'"));
    if( !arrayp(tmp) || sizeof(tmp) != 1 ) return 0;
    id = tmp[0][0];
    tmp = db_exec(dbConn, ("UPDATE t_gen_id "
			   "SET table_next_id = " + (id+1) + " "
			   "WHERE table_name = 't_bug'"));
    if( !arrayp(tmp) || sizeof(tmp) != 0 ) return 0;
    tmp = db_exec(dbConn, ("INSERT INTO t_bug "
			   "(bug_id, bug_type, bug_status, bug_rpt_date, "
			   "bug_rpt_by, bug_rpt_room, bug_room_cre, bug_desc) "
			   "VALUES(" + id + ", 'new report', " + time() + ", "
			   "'" + convert_name(data["bug_rpt_by"]) + "', '" +
			   data["bug_rpt_room"] + "', '" +
			   convert_name(data["bug_room_cre"]) + "', '" +
			   data["bug_desc"] + "')"));
    if( !arrayp(tmp) || sizeof(tmp) ) return 0; /* rollback is no big deal */
    foreach(string *obdata in data["inventory"]) {
	/* Need to make sure the object is in t_inventory
	 * The object *probably* already exists, so catch the error and ignore
	 */
	catch(db_exec(dbConn, ("INSERT INTO t_inventory "
			       "(inventory_id, inventory_cre) "
			       "VALUES('" + obdata[0] + "', '" +
			       obdata[1] + "')")));
	db_exec(dbConn, ("INSERT INTO t_bug_inv "
			 "(bug_id, inventory_id) "
			 "VALUES(" + id + ", '" + obdata[1] + "')"));
    }
    return 1;
}

int GetAssignedBugs(string who) {
    mixed tmp;
    
    if( !who || !user_exists(who = convert_name(who)) ) return -1;
    if( dbConn < 1 && !Connect() ) return -1;
    tmp = db_exec(dbConn, ("SELECT bug_id "
			   "FROM t_bug "
			   "WHERE bug_assigned_to = '" + who + "'"));
    return sizeof(tmp);
}

mapping GetBugs(mapping where_clause) {
    mixed tmp;
    string where = "WHERE ";

    if( sizeof(where_clause) ) {
	foreach(string key, mixed val in where_clause) {
	    where += key + " = ";
	    if( stringp(val) ) where += "'" + val + "' AND ";
	    else where += val + " AND ";
	}
    }
    tmp = db_exec(dbConn, ("SELECT t_bug.bug_id, "
			   "       t_bug.bug_type, "
			   "       t_bug.bug_status, "
			   "       t_bug.bug_rpt_date, "
			   "       t_player.player_cap_name "
			   "       t_bug.bug_rpt_room, "
			   "       t_bug.bug_room_cre, "
			   "       t_bug.bug_desc, "
			   "       t_bug.bug_res, "
			   "       t_bug.bug_res_date, "
			   "       t_bug.bug_res_by, "
			   "       t_bug.bug_assigned_to "
			   "FROM   t_bug, t_player "
			   where +
			   "       t_bug.bug_rpt_by = t_player.player_name "));
    foreach(mixed *row in tmp) {
	ret[row[0]] = ([ "bug_type" : row[1],
			"bug_status" : row[2],
			"bug_rpt_date" : row[3],
			
       
			   
}

			   
