Extras directory

- restore_cfg If you screw up your mudos.cfg file, you can use this to
  get it back.

- To rebuild the driver with maximum warning verbosity, use the 
  noisy.local_options provided here.

- For those who want to use Cygwin to compile their own Win32
  binary, you can use mudos_cygwin.diff to patch the 22.2b14 source
  you download from mudos.org. Do *NOT* try to patch the mudos
  source included in this distribution with that file. Download
  from mudos.org, and patch that.

- I've included the mingw diffs. As is the case with the Cygwin diffs,
  download the clean 22.2b14 mudos distribution from mudos.org, and
  apply this patch. I haven't tested it, but it should work, or damn
  close to it, for you. The build environment was Cygwin because the
  mingw build environment gave me bitchy bison errors. There is, to
  the best of my primitive knowledge, no Cygwin technology included
  in this patch. Use mudos_mingw.diff for this masochism.

- I added a creremote/ dir with stuff you'll need to take advantage
  of Dead Souls RCP support. However, the stuff in this dir is not
  Dead Souls stuff, so it is not supported by me.
