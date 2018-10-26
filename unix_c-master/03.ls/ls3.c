/* ls2.c
 * purpose list contents of directory or directories
 * action if no args, use . else list files in args
 * note uses stat and pwd.h and grp.h
 * BUG: try ls2 /tmp
 * add sort the filename
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>

void do_ls(char []);
void dostat(char *);
void show_file_info(char *, struct stat *);
void mode_to_letters(int , char []);
char * uid_to_name(uid_t);
char * gid_to_name(gid_t);

void my_sort(char * lst, int length);

int main(int ac, char * av[])
{
    if (ac == 1)
        do_ls(".");
    else
        while (--ac)
        {
            printf("%s:\n", * ++av);
            do_ls(*av);
        }
	return 0;
}

/* 
 * list files in directory called dirname
 */
void do_ls(char dirname[])
{
    DIR *dir_ptr;           // the directory
    struct dirent * direntp;// each entry

    if ((dir_ptr = opendir(dirname)) == NULL)
        fprintf(stderr, "ls1: cannot open %s\n", dirname);
    else
    {
        // add a simple sort of filename
        while ((direntp = readdir(dir_ptr)) != NULL)
            dostat(direntp->d_name);
        closedir(dir_ptr);
    }
}

void dostat(char *filename)
{
    struct stat info;
    if (stat(filename, &info) == -1)        // cannot stat
        perror(filename);                   // say why
    else
        show_file_info(filename, &info);    
}

/* 
 * display the info about filename.
 * the info is stored in struct at * info_p
 */
void show_file_info(char *filename, struct stat * info_p)
{
    char * uid_to_name(), *ctime(), *gid_to_name(), *filemode();
    void mode_to_letters();
    char modestr[11];

    mode_to_letters(info_p->st_mode, modestr);

    printf("%s", modestr);
    printf("%4d ", (int)info_p->st_nlink);
    printf("%-8s ", uid_to_name(info_p->st_uid));
    printf("%-8s ", gid_to_name(info_p->st_gid));
    printf("%8ld ", (long)info_p->st_size);
    printf("%.12s ", 4+ctime(&info_p->st_mtime));
    printf("%s\n", filename);
}

/*
 * utility functions
 */

/*
 * This function takes a mode value and a char array
 * and puts into the char array the file type and the
 * nine letters that correspnd to the bits in mode.
 * NOTE: It does not code setuid, setgid, and sticky
 * codes
 */
void mode_to_letters(int mode, char str[])
{
    strcpy(str, "----------");  // default = no perms
    if (S_ISDIR(mode)) str[0] = 'd';    // directory
    if (S_ISCHR(mode)) str[0] = 'c';    // char devices
    if (S_ISBLK(mode)) str[0] = 'l';    // block device

    if (mode & S_IRUSR) str[1] = 'r';   // 3 bits for user
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    if (mode & S_IRGRP) str[4] = 'r';   // 3 bits for group
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    if (mode & S_IROTH) str[7] = 'r';   // 3 bits for other
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
}



/* 
 * returns pointer to username associated with uid, uses getpw()
 */
char * uid_to_name(uid_t uid)
{
    struct passwd * getpwuid(), *pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL)
    {
        sprintf(numstr, "%d", uid);
        return numstr;
    }
    else
        return pw_ptr->pw_name;
}



/*
 * returns pointer to group number gid, used getgrid(3)
 */
char * gid_to_name(gid_t gid)
{
    struct group * getgrpid(), *grp_ptr;
    static char numstr[10];

    if ((grp_ptr = getgrgid(gid)) == NULL)
    {
        sprintf(numstr, "%d", gid);
        return numstr;
    }
    else
        return grp_ptr->gr_name;
}


void my_sort(char * lst, int length)
{
    for (int i = 0; i < length; ++i)
    {
        for (int j = i+1; j < length; ++j)
        {
            // printf("%d %d %d %d\n", i, j, lst[i], lst[j]);
            if (lst[i] > lst[j])
            {
                char tmp = lst[j];
                lst[j] = lst[i];
                lst[i] = tmp;
            }
        }
    }
}

