#ifndef s_gossip_h
#define s_gossip_h

class gossip_post {
    int Follows;
    int Date;
    int Thread;
    string Topic;
    string Poster;
    string Message;
}

class gossip_group {
    int NextPost;
    int NextThread;
    string SourceMud;
    string Feed;
    string Group;
    mapping Posts;
}
    
#endif /* s_gossip_h */
