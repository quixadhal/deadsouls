/*    /daemon/news.c
 *    from the Dead Souls Object Library
 *    responsible for leading players through news
 *    created by Descartes of borg 951104
 */

#include <lib.h>
#include <news.h>
#include "include/news.h"

inherit LIB_DAEMON;

object ThisPlayer(){
    object ret = previous_object();
    if(ret && interactive(ret)) return ret;
    else ret = this_player();
    return ret;
}

void GeneralNews() {
    object player = ThisPlayer();
    if( file_exists(NEWS_GENERAL) ) {
        int s;

        s = stat(NEWS_GENERAL)[1];
        if( s != player->GetNews("general") ) {
            string news;

            news = GetNews("general");
            player->SetNews("general", s);
            message("news", "\n%^RED%^General news:", player);
            player->eventPrint(news);
            message("prompt", "Press <return> to continue: ", player);
            if( !(player->GetClass()) && !creatorp(player) )
                input_to((: NewbieNews :));
            else input_to((: ClassNews, "" :));
            return;
        }
    }
    if( !(player->GetClass()) && !creatorp(player) )
        NewbieNews();
    else ClassNews("");
}

static void NewbieNews() {
    object player = ThisPlayer();
    if( file_exists(NEWS_NEWBIE) ) {
        string news;

        news = GetNews("newbie");
        message("news", "\n%^RED%^Newbie news:", player);
        message("news", news, player);
        message("prompt", "Press <return> to continue: ", player);
        input_to((: ClassNews, "" :));
        return;
    }
    ClassNews("");
}

void ClassNews(string cl) {
    object player = ThisPlayer();
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
    if( player->ClassMember(cl) && file_exists(DIR_NEWS "/"+cl) ) {
        int s;

        s = stat(DIR_NEWS "/" + cl)[1];
        if( s != player->GetNews(cl) ) {
            string news;

            news = GetNews(cl);
            player->SetNews(cl, s);
            message("news", "\n%^RED%^" + capitalize(cl) + " news:",
                    player);
            message("news", news, player);
            message("prompt", "Press <return> to continue: ", player);
            input_to( (: ClassNews, cl :));
            return;
        }
    }
    ClassNews(cl);
}

static void HighMortalNews() {
    object player = ThisPlayer();
    if( high_mortalp(player ) && file_exists(NEWS_HM) ) {
        int s;

        s = stat(NEWS_HM)[1];
        if( s != player->GetNews("hm") ) {
            string news;

            news = GetNews("hm");
            player->SetNews("hm", s);
            message("news", "\n%^RED%^High mortal news:", player);
            message("news", news, player);
            message("prompt", "Press <return> to continue: ", player);
            input_to( (: AvatarNews :));
            return;
        }
    }
    AvatarNews();
}

static void AvatarNews() {
    object player = ThisPlayer();
    if( avatarp(player ) && file_exists(NEWS_AVATAR) ) {
        int s;

        s = stat(NEWS_AVATAR)[1];
        if( s != player->GetNews("avatar") ) {
            string news;

            news = GetNews("avatar");
            player->SetNews("avatar", s);
            message("news", "\n%^RED%^Avatar news:", player);
            message("news", news, player);
            message("prompt", "Press <return> to continue: ", player);
            input_to( (: CreatorNews :));
            return;
        }
    }
    CreatorNews();
}

static void CreatorNews() {
    object player = ThisPlayer();
    if( creatorp(player ) && file_exists(NEWS_CREATOR) ) {
        int s;

        s = stat(NEWS_CREATOR)[1];
        if( s != player->GetNews("creator") ) {
            string news;

            news = GetNews("creator");
            player->SetNews("creator", s);
            message("news", "\n%^RED%^Creator news:", player);
            message("news", news, player);
            message("prompt", "Press <return> to continue: ", player);
            input_to( (: AdminNews :));
            return;
        }
    }
    AdminNews();
}

static void AdminNews() {
    object player = ThisPlayer();
    if(archp(player ) && file_exists(NEWS_ADMIN) ) {
        int s;

        s = stat(NEWS_ADMIN)[1];
        if( s != player->GetNews("admin") ) {
            string news;

            news = GetNews("admin");
            player->SetNews("admin", s);
            message("news", "\n%^RED%^Admin news:", player);
            message("news", news, player);
            message("prompt", "Press <return> to continue: ", player);
            input_to( (: EndNews :));
            return;
        }
    }
    EndNews();
}

static void EndNews() {
    object player = ThisPlayer();
    if(player) player->eventDescribeEnvironment();
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






