iCalendar HTML Sync
===================

iCalendar HTML Sync is a simple little application that runs in the background, monitoring for changes to a user-specified iCalendar file. Whenever the calendar is modified, HTML reports of the calendar's events and/or tasks are generated in the user's Dropbox directory.

iCalendar HTML Sync includes two binaries:

- `icalsync` - this is the background process that monitors the calendar file and triggers report generation.
- `ics2html` - this is the tool that generates HTML reports from iCalendar files.

If Dropbox synchronisation is not required, `ics2html` can simply be used in a standalone manner to generate HTML reports from calendar files.


Installation
------------

Windows users can simply run the supplied installer, which will install the executables, set `icalsync` to run at startup, and start `icalsync` immediately.

For Linux and Mac OS X, extract the files from the compressed archive, and run `sudo ./install.sh`.


Configuration
-------------

When `icalsync` is first run, it will create the default configuration files in the following directory:

- Windows: `%APPDATA%/ical-html-sync`
- Linux and Mac OS X: `~/.config/ical-html-sync`

`icalsync` will then exit with the error `Error: file to be monitored does not exist: "/path/to/calendar.ics"`.

You will then need to edit the file `icalsync.conf`, which defaults to the following contents:

```
/path/to/calendar.ics
Calendar Events.html
Calendar Tasks.html
```

**The first line must be replaced with the full path to the calendar file which will be monitored.** The second and third lines can be changed if desired - they specify the filenames (relative to the user's Dropbox directory) of the generated HTML files for the events report and tasks report, respectively. If only one of the reports is required, the line specifying the filename for the other report can be left blank, which will prevent it from being generated.


Uninstallation
--------------

The Windows installer generates an uninstaller, which is located in the same directory as the executable files.

Linux and Mac OS X users need to run `sudo ./uninstall.sh` in the directory where the files were first extracted from the compressed archive.


Building from source
--------------------

Both `icalsync` and `ics2html` require a compiler with C++11 support. The following libraries are required when building from source:

- [libb64](http://libb64.sourceforge.net/)
- [libical](http://libical.github.io/libical/)
- libsimple-base (from the [assorted-utils](https://github.com/adamrehn/assorted-utils) repo)

Additionally, the following tools need to be present in the system PATH:

- `compile_file` (from the [assorted-utils](https://github.com/adamrehn/assorted-utils) repo)
