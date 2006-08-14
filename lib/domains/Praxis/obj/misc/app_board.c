#include <lib.h>

inherit LIB_BOARD;

void create() {
    ::create();
    SetProperty("no steal",1);
    SetKeyName("board");
    SetId( ({"board","approval board"}) );
    set_board_id("approval");
    SetShort("the approval board");
    SetLong("Posts and notes for the approval team.\n");
}
