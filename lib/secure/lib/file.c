/*    /lib/file.c
 *    From the Dead Souls V Object Library
 *    Handles clean file I/O without need for the driver
 *    Created by Descartes of Borg 961219
 *    Version: @(#) file.c 1.2@(#)
 *    Last Modified: 96/12/19
 */

#include <lib.h>
#include <runtime_config.h>

inherit LIB_CLEAN;

private static string FileName    = 0;
private static int    LinePointer = 0;
private static int    MaxBytes    = get_config(__MAX_BYTE_TRANSFER__);
private static int    MaxFile     = get_config(__MAX_READ_FILE_SIZE__);
private static int    TempFile    = 0;

int isDirectory() {
    return (file_size(FileName) == -2);
}

int isFile() {
    return (file_size(FileName) > -1 );
}

int isTemporary() {
    return TempFile;
}

buffer GetBuffer() {
    int size = file_size(FileName);
    int count = (size/MaxBytes) + 1;
    buffer data = allocate_buffer(size);

    if( !FileName ) {
	return data;
    }
    else if( size < 0 ) {
	return 0;
    }
    for(int i=0; i<count; i++) {
	int length, ptr;
	
	ptr = i * MaxBytes;
	if( size - ptr > MaxBytes ) {
	    length = MaxBytes;
	}
	else {
	    length = size - ptr;
	}
	write_buffer(data, ptr, read_buffer(FileName, ptr, length));
    }
    return data;
}

buffer GetBytes(int start, int length) {
    return read_buffer(FileName, start, length);
}

string GetCharacters(int start, int length) {
    return read_bytes(FileName, start, length);
}

string array GetDirectoryList() {
    if( !isDirectory() ) {
	return 0;
    }
    return get_dir(FileName + "/");
}

mixed array GetDirectoryShort() {
    if( !isDirectory() ) {
	return 0;
    }
    return get_dir(FileName + "/", -1);
}

string GetFileName() {
    if( file_size(FileName) == -2 ) {
	return FileName + "/";
    }
    else {
	return FileName;
    }
}

int GetLastModified() {
    if( file_size(FileName) == -1 ) {
	return 0;
    }
    else {
	return get_dir(FileName, -1)[0][2];
    }
}

string GetLine() {
    return read_file(FileName, LinePointer++, 1);
}

varargs string GetLines(int arg1, int arg2) {
    int start = arg1;
    int count = arg2;

    if( undefinedp(arg2) ) {
	start = LinePointer;
	count = arg1;
	LinePointer = LinePointer + count;
    }
    return read_file(FileName, start, count);
}

int GetSize() {
    return file_size(FileName);
}

string GetString() {
    int size = file_size(FileName);
    int count = (size/MaxFile) + 1;
    string data = "";

    if( !FileName ) {
	return data;
    }
    else if( size < 0 ) {
	return 0;
    }
    for(int i=0; i<count; i++) {
	int length, ptr;
	
	ptr = i * MaxFile;
	if( size - ptr > MaxFile ) {
	    length = MaxFile;
	}
	else {
	    length = size - ptr;
	}
	data += read_bytes(FileName, ptr, length);
    }
    return data;
}

mixed eventCreateDirectory() {
    if( isFile() ) {
	return "File exists.";
    }
    else if( isDirectory() ) {
	return "Directory exists.";
    }
    else {
	if( mkdir(FileName) ) {
	    return 1;
	}
	else {
	    return 0;
	}
    }
}

mixed eventDelete() {
    if( isDirectory() ) {
	if( rmdir(FileName) ) {
	    return 1;
	}
	else {
	    return 0;
	}
    }
    else if( isFile() ) {
	if( rm(FileName) ) {
	    return 1;
	}
	else {
	    return 0;
	}
    }
    else {
	return "No such file or directory.";
    }
}

int eventDestruct() {
    if( isTemporary() ) {
	eventDelete();
    }
    return clean::eventDestruct();
}

varargs mixed eventRename(string name, int clobber) {
    object file = new(LIB_FILE, name);
    object tmpfile = 0;
    
    if( file->isFile() || file->isDirectory() ) {
	mixed tmp;
	
	if( !clobber ) {
	    return "File or directory already exists.";
	}
	tmpfile = new(LIB_FILE);
	tmp = file->eventRename(tmpfile->GetFileName());
	if( tmp != 1 ) {
	    if( !tmp ) {
		tmp = "";
	    }
	    tmp = "Failed to make temp file: " + tmp;
	    return tmp;
	}
    }
    if( rename(FileName, name) ) {
	if( tmpfile ) {
	    mixed tmp = tmpfile->eventRename(name);

	    if( tmp != 1 ) {
		if( !tmp ) {
		    tmp = "";
		}
		return "Failed to restore destination!! Data lost: " + tmp;
	    }
	}
	return 0;
    }
    if( tmpfile ) {
	tmpfile->eventDelete();
    }
    FileName = name;
    while( strlen(FileName) && FileName[<1] == '/' ) {
	FileName = FileName[0..<2];
    }
    return 1;
}

mixed eventWrite(mixed val) {
    int size = sizeof(val);
    int max = (bufferp(val) ? MaxBytes : MaxFile);
    int count = (size/max) + 1;

    if( file_size(FileName) == -2 ) {
	return "File is a directory.";
    }
    for(int i = 0; i<count; i++) {
	int ptr = i * max;
        int length;

	if( size - ptr > max ) {
	    length = max;
	}
	else {
	    length = size - ptr;
	}
	if( bufferp(val) ) {
	    write_buffer(FileName, ptr, read_buffer(val, ptr, length));
	}
	else {
	    write_bytes(FileName, ptr, val[ptr..(ptr+length-1)]);
	}
    }
    return 1;
}

varargs static void create(string file) {
    if( !clonep() ) {
        return;
    }
    if( FileName ) {
	error("Odd, but illegal file name rename attempt.\n");
    }
    if( !file ) {
	int t = time();
	int count = 0;
	
	file = DIR_TMP "/" + t;
	while( file_size(file) != -1 ) {
	    if( count++ > 100 ) {
		error("Unable to generate temporary file.");
	    }
	    t++;
	    file = DIR_TMP "/" + t;
	}
	write_file(file, "");
	TempFile = 1;
    }
    else {
	SetNoClean(1);
    }
    FileName = file;
    while( strlen(FileName) && FileName[<1] == '/' ) {
	if( strlen(FileName) == 1 ) {
	    FileName = 0;
	    error("Invalid file name.\n");
	}
	FileName = FileName[0..<2];
    }
}
