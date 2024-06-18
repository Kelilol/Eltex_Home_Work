//permissions.h
#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void display_permissions(mode_t mode);
void display_permissions_info(mode_t mode);
mode_t parse_permissions(const char *perms);
mode_t parse_mode(const char *mode_str);
#endif // PERMISSIONS_H