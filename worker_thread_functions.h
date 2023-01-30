/*
	Collection of different operations that can be performed with a command
	
	The only non-static function is the one that should be called from outside this file, it picks which function to execute based on the char provided with the command
*/

#ifndef WTF
#define WTF

#include <time.h>

/* gets a command and a char indicating command type, what it will do with the command depends on the char
The return value depends on the type.
Note that none of the options change anything about the command itself
The avilable options are:
p	print the command to the screen and return NULL
s	print the command to the screen, sleep for 1 second, and then return NULL
b	execute the command on the shell, return whatever it returned
TODO: add more
If none of the above are given, an error message is printed and NULL is returned
*/
void* commandHandler(char* command, char type);

// handles p type commands (see commandHandler documentation)
static void CHp(char* command);

// handles s type commands (see commandHandler documentation)
static void CHs(char* command);

// handles b type commands (see commandHandler documentation)
static *void CHb(char* command);

#endif
