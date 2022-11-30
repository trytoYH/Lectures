#include "file.h"

/* Note, DO NOT MODIFY this file except for the student ID */

int main()
{  
  	char command[MAX_LEN];  // maximum input char length = 100
    int studentid=20200124; // change this value to your studnetid
    int flag;               // command flag 
    LOGO();                 // to show examples
	while (True) {
        PROMPT(studentid);  // shell prompt GUI, You don't need to implement it
		fgets(command, sizeof(command), stdin); // get command line
        flag = file(command); // verify whether the command is done correctly or not
        PRINT_COMMAND_RESULT(flag, command); // It raises an error message only if your command cannot run or not valid. You don't need to implement it
	}
    return 0;
}
