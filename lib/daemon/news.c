/*    /daemon/news.c
 *    from the Dead Soulsr2 Object Library
 *    responsible for leading players through news
 *    created by Descartes of borg 951104
 */

#include <lib.h>
#include <news.h>
#include "include/news.h"

inherit LIB_DAEMON;

void GeneralNews() {
    if( file_exists(NEWS_GENERAL) ) {
	int s;

	s = stat(NEWS_GENERAL)[1];
	if( s != (int)this_player()->GetNews("general") ) {
	    string news;
	    
	    news = GetNews("general");
	    this_player()->SetNews("general", s);
	    message("news", "\n%^RED%^General news:", this_player());
             this_player()->eventPrint(news);
              message("prompt", "Press <return> to continue: ", this_player());
	    if( !((int)this_player()->GetClass()) && !creatorp(this_player()) )
	      input_to((: NewbieNews :));
	    else input_to((: ClassNews, "" :));
	    return;
	}
    }
    if( !((int)this_player()->GetClass()) && !creatorp(this_player()) )
      NewbieNews();
    else ClassNews("");
}

static void NewbieNews() {
    if( file_exists(NEWS_NEWBIE) ) {
	string news;

	news = GetNews("newbie");
	message("news", "\n%^RED%^Newbie news:", this_player());
	message("news", news, this_player());
	message("prompt", "Press <return> to continue: ", this_player());
	input_to((: ClassNews, "" :));
	return;
    }
    ClassNews("");
}

void ClassNews(string cl) {
    if( !cl || cl == "" ) cl = "cleric";
    else {
	switch(cl) {
	    case "cleric":
	    cl = "fighter";
	    break;

	    case "fighter":
	    cl = "fisher";
	    break;

	    case "fisher":
	    cl = "kataan";
	    break;
	    
	    case "kataan":
	    cl = "mage";
	    break;

	    case "mage":
	    cl = "monk";
	    break;

	    case "monk":
	    cl = "rogue";
	    break;

	    case "rogue":
	    HighMortalNews();
	    return;
	}
    }
    if( (int)this_player()->ClassMember(cl) && file_exists(DIR_NEWS "/"+cl) ) {
	int s;

	s = stat(DIR_NEWS "/" + cl)[1];
	if( s != (int)this_player()->GetNews(cl) ) {
	    string news;

	    news = GetNews(cl);
	    this_player()->SetNews(cl, s);
	    message("news", "\n%^RED%^" + capitalize(cl) + " news:",
		    this_player());
	    message("news", news, this_player());
	    message("prompt", "Press <return> to continue: ", this_player());
	    input_to( (: ClassNews, cl :));
	    return;
	}
    }
    ClassNews(cl);
}

static void HighMortalNews() {
    if( high_mortalp(this_player() ) && file_exists(NEWS_HM) ) {
	int s;

	s = stat(NEWS_HM)[1];
	if( s != (int)this_player()->GetNews("hm") ) {
	    string news;
	    
	    news = GetNews("hm");
	    this_player()->SetNews("hm", s);
	    message("news", "\n%^RED%^High mortal news:", this_player());
	    message("news", news, this_player());
	    message("prompt", "Press <return> to continue: ", this_player());
	    input_to( (: AvatarNews :));
	    return;
	}
    }
    AvatarNews();
}

static void AvatarNews() {
    if( avatarp(this_player() ) && file_exists(NEWS_AVATAR) ) {
	int s;

	s = stat(NEWS_AVATAR)[1];
	if( s != (int)this_player()->GetNews("avatar") ) {
	    string news;
	    
	    news = GetNews("avatar");
	    this_player()->SetNews("avatar", s);
	    message("news", "\n%^RED%^Avatar news:", this_player());
	    message("news", news, this_player());
	    message("prompt", "Press <return> to continue: ", this_player());
	    input_to( (: CreatorNews :));
	    return;
	}
    }
    CreatorNews();
}

static void CreatorNews() {
    if( creatorp(this_player() ) && file_exists(NEWS_CREATOR) ) {
	int s;

	s = stat(NEWS_CREATOR)[1];
	if( s != (int)this_player()->GetNews("creator") ) {
	    string news;
	    
	    news = GetNews("creator");
	    this_player()->SetNews("creator", s);
	    message("news", "\n%^RED%^Creator news:", this_player());
	    message("news", news, this_player());
	    message("prompt", "Press <return> to continue: ", this_player());
	    input_to( (: AdminNews :));
	    return;
	}
    }
    AdminNews();
}

static void AdminNews() {
    if(archp(this_player() ) && file_exists(NEWS_ADMIN) ) {
	int s;

	s = stat(NEWS_ADMIN)[1];
	if( s != (int)this_player()->GetNews("admin") ) {
	    string news;
	    
	    news = GetNews("admin");
	    this_player()->SetNews("admin", s);
	    message("news", "\n%^RED%^Admin news:", this_player());
	    message("news", news, this_player());
	    message("prompt", "Press <return> to continue: ", this_player());
	    input_to( (: EndNews :));
	    return;
	}
    }
    EndNews();
}

static void EndNews() {
    this_player()->eventDescribeEnvironment(0);
}

string GetNews(string type) {
    string file;
    
    switch(type) {
	case "admin":
	file = NEWS_ADMIN;
	break;

	case "avatar":
	file = NEWS_AVATAR;
	break;

	case "creator":
	file = NEWS_CREATOR;
	break;

	case "general":
	file = NEWS_GENERAL;
	break;
	
	case "hm":
	file = NEWS_HM;
	break;

	case "newbie":
	file = NEWS_NEWBIE;
	break;

	default:
	file = DIR_NEWS "/" + type;
	break;
    }
    if( !file_exists(file) ) return "No " + type + " news.";
    file = read_file(file) || ("No " + type + " news.");
    return file;
}






