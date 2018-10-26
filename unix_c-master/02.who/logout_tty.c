/* logout_tty.c
 * marks a utmp reocord as logged out
 * does not blank username or remote host
 * return -1 on error, 0 on sucess
 */
#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>   // define the structure which readed
#include <fcntl.h>  // function of open
#include <unistd.h> // read, close
#include <time.h>
#include <errno.h>

int logout_tty(char * line)
{
    int fd;
    struct utmp rec;
    int len = sizeof(rec);
    int retval = -1;        // pessimism

    if ((fd = open(UTMP_FILE, O_RDWR)) == -1)
	{
		perror(UTMP_FILE);
		return -1;
	}
        

    // search and replace
    while (read(fd, &rec, len) == len)
	{
		printf("11111111  == [%s]\n",rec.ut_line);
		if (strncmp(rec.ut_line, line, sizeof(rec.ut_line)) == 0)
        {
            rec.ut_type = DEAD_PROCESS;     // set type
            //if (time(&rec.ut_tv.tv_sec != -1))

			//if (time(&rec.ut_tv.tv_sec != -1))
				printf("rec.ut_time = [%-8.8ld]\n",rec.ut_tv.tv_sec);
                if (lseek(fd, -len, SEEK_CUR) != -1)    // back up
                    if (write(fd, &rec, len) == len)    // update
                        retval = 0;                     // sucess

            break;
        }

	}


    // close the file
    if (close(fd) == -1)
        retval = -1;
    return retval;
}

int main()
{
	int i = logout_tty("tty7");
	printf("i = [%d]",i);
	return 0;

}
