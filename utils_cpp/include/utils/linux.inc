/*
 * linux.inc
 *
 *  Created on: 22 de jan de 2016
 *      Author: alan
 */

#ifndef INCLUDE_UTILS_LINUX_INC_
#define INCLUDE_UTILS_LINUX_INC_

char *  itoa ( int value, char * str, int base );
#define ltoa(v,s,b) itoa(v,s,b)


char *strlwr(char *str);


#endif /* INCLUDE_UTILS_LINUX_INC_ */
