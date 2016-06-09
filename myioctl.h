/*
 * =====================================================================================
 *
 *       Filename:  myioctl.h
 *
 *    Description:  Header for the ioctl functions
 *
 *        Version:  1.0
 *        Created:  06/09/2016 11:42:03 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Brandon Westmoreland (OIT), brandon.westmoreland@oit.edu
 *   Organization:  Oregon Tech
 *
 * =====================================================================================
 */

#ifndef _MYIOCTL_H
#define _MYIOCTL_H

#define QUERY_GET_STATUS _IOR('q',1,int*)
#define QUERY_SET_BUF _IO('q',2)
#define QUERY_CLR_BUF _IO('q',3)

#endif
