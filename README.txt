Overview
--------
Command line shells allow one to access the capabilities of a computer using simple, interactive means. Type the name of a program and the shell will bring it into being, run it, and show output. Familiarizing yourself with the basics of shell job management will make work on terminal-only machines much more palatable.

Commando Commands
-----------------
help               : show this message
exit               : exit the program
list               : list all jobs that have been started giving information on each
pause nanos secs   : pause for the given number of nanseconds and seconds
output-for int     : print the output for given job number
output-all         : print output for all jobs
wait-for int       : wait until the given job number finishes
wait-all           : wait for all jobs to finish
command arg1 ...   : non-built-in is run as a job            # Runs a command as a child process

To Compile the program
----------------------
make all

To Run the program
------------------
./commando

Main system implementation
---------------------------
Basic C Memory Discipline: A variety of strings and structs are allocated and de-allocated during execution which will require attention to detail and judicious use of memory tools like Valgrind.
fork() and exec(): Text entered that is not recognized as a built-in is treated as an command (external program) to be executed. This spawns a child process which executes the new program.
Pipes, dup2(), read(): Rather than immediately print child output to the screen, child output is redirected into pipes and then retrieved on request by commando.
wait() and waitpid(), blocking and nonblocking: Child processes usually take a while to finish so the shell will check on their status every so often

Programming Language
--------------------
C

Team Member Names
------------------
- Name  : Ace Kaung
- Email : kaung006@umn.edu

- Name  : Yongsuk cho
- Email : cho00085@umn.edu(