int tail(string file) {
    string str;
    int diff;

    diff = file_size(file);
    if (diff < 0) return 0;

    diff -= 1024;
    if (diff < 0) diff = 0;
    str = read_bytes(file, diff, 1024);
    if (!str) return 0;
    if (diff) str = str[strsrch(str, "\n")+1..];

    write(str);

    return 1;
}
