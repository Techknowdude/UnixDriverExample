/*
 * =====================================================================================
 *
 *       Filename:  example.c
 *
 *    Description:  Used as a demo for the driver
 *
 *        Version:  1.0
 *        Created:  06/09/2016 12:13:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Brandon Westmoreland (OIT), brandon.westmoreland@oit.edu
 *   Organization:  Oregon Tech
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
 
#include "myioctl.h"
 
void get_status(int fd)
{
        int status;
         
        if (ioctl(fd, QUERY_GET_STATUS, &status) == -1)
        {
            perror("example ioctl get");
        }
        else
        {
            printf("Status : %d\n", status);
        }                                               }
}

void clr_buf(int fd)
{
    if (ioctl(fd, QUERY_CLR_BUF) == -1)
    {
        perror("example ioctl clr");
    }
}

void set_buf(int fd)
{
    if (ioctl(fd, QUERY_SET_VARIABLES, &q) == -1)
    {
        perror("example ioctl set");
    }
}
 
int main(int argc, char *argv[])
{
    char *file_name = "/dev/mydriver";
    int fd;
    enum
    {
        e_get,
        e_clr,
        e_set
    } option;
                 
    if (argc == 1)
    {
        option = e_get;
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-g") == 0)
        {
            option = e_get;
        }
        else if (strcmp(argv[1], "-c") == 0)
        {
            option = e_clr;
        }
        else if (strcmp(argv[1], "-s") == 0)
        {
            option = e_set;
        }
        else
        {
            fprintf(stderr, "Usage: %s [-g | -c | -s]\n", argv[0]);
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Usage: %s [-g | -c | -s]\n", argv[0]);
        return 1;
    }
    fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        perror("example open");
        return 2;
    }
                                     
    switch (option)
    {
        case e_get:
            get_vars(fd);
            break;
        case e_clr:
            clr_vars(fd);
            break;
        case e_set:
            set_vars(fd);
            break;
        default:
            break;
    }
                                         
    close (fd);
                                             
    return 0;
}
