#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_SHAKE;
inherit LIB_CONSULT;

string answer = "";

string *answers = ({
        "Signs point to yes.",
        "Yes.",
        "Most likely.",
        "Without a doubt.",
        "Yes - definitely.",
        "As I see it, yes.",
        "You may rely on it.",
        "Outlook good.",
        "It is certain.",
        "It is decidedly so.",
        "Reply hazy, try again.",
        "Better not tell you now.",
        "Ask again later.",
        "Concentrate and ask again.",
        "Cannot predict now.",
        "My sources say no.",
        "Very doubtful.",
        "Outlook not so good.",
        "My reply is no.",
        "Don't count on it.",
});

void create(){
    ::create();
    SetKeyName("magic 8 ball");
    SetId( ({"ball","8ball","8-ball","8 ball"}) );
    SetAdjectives( ({"giant","billiard","black","magic"}) );
    SetShort("a magic 8 ball");
    SetLong("This is a plastic sphere, about the size of a human hand, "+
            "and black in color. On one side a white circle is painted, with the "+
            "numeral 8 in the middle, as if it were a giant billiard ball. On "+
            "the other side of the sphere is a little clear window. It would "+
            "appear that one shakes this ball, then consults it, to get advice.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    answer = answers[random(sizeof(answers))];
    SetRead( (: eventConsult :) );
}

void init(){
    ::init();
}

varargs mixed eventShake(object dude, string foo){
    write("You shake your magic 8 ball.");
    say(dude->GetName()+" shakes "+possessive(dude)+" magic 8 ball.");
    answer = answers[random(sizeof(answers))];
    return 1;
}

varargs mixed eventConsult(object dude, string foo){
    write("The magic 8 ball answers: \n"+answer);
    if(!dude) dude = this_player();
    say(dude->GetName()+" consults "+possessive(dude)+" magic 8 ball.");
    return 1;
}

varargs mixed CanConsult(object who, string component){
    return 1;
}

varargs mixed CanShake(object who, string component){
    return 1;
}
