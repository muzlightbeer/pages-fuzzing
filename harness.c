/*
BSD 2
Copyright (c) 2014, muzlightbeer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

static int counter;
char open_command[200];
char full_path[255];
char filename[100];
pid_t ret;

int main(void) {
	FILE *fp = fopen("crashes.txt", "a");
	if (!fp) {
		perror("fopen: ");
		exit(1);
	}
	char logbuf[512];
	memset(&logbuf, '\0', sizeof(logbuf));
	int childstatus;
	system("ps -axc | grep -i Pages | awk '{print $1}' | tr -d '\n' | xargs -0 kill");
	sleep(3); // sleep everywhere until time to debug it properly. sleep longer if using libgmalloc perhaps
	for (counter = 0; counter < 2001; counter++) { // adjust to the amount of test cases you have	
		ret = fork();
		if (ret == 0 ) { // child	
			execl("/Applications/Pages.app/Contents/MacOS/Pages", "/Applications/Pages.app/Contents/MacOS/Pages", 0);
		}

		if (ret != 0) { // parent		
			sleep(3);
			memset(full_path, '\0', sizeof(full_path));
			memset(filename, '\0', sizeof(filename));
			memset(open_command, '\0', sizeof(open_command));
			strcpy(full_path, "/Users/FIXME/FIXME/"); // change this to the cwd of your test cases
			sprintf(filename, "doc%d.pages", counter);
			strcat(full_path, filename);
			strcpy(open_command, "/usr/bin/open -F ");
			strcat(open_command, full_path);
			printf("%s \n", open_command);	
			system(open_command);
			sleep(8);	
			if (waitpid(ret, &childstatus, WNOHANG) != 0) {
				if (WIFSIGNALED(childstatus)) {
					printf("DEBUG: Abnormal exit \n");
					sprintf(logbuf, "doc%d.pages\n", counter);
					fwrite(&logbuf, strlen(logbuf), 1, fp);
					memset(&logbuf, '\0', sizeof(logbuf)); // not sure this is needed
					}	
				}
			kill(ret, SIGKILL);
		
		}
	}
	fclose(fp);
}
