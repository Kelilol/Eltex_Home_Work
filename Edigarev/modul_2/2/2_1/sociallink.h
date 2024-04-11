//sociallink.h
#ifndef SOCIAL_LINK_H
#define SOCIAL_LINK_H
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME_LENGTH 10
typedef struct {
    char socialLink[MAX_NAME_LENGTH];
} SocialLink;

void initSocialLink(SocialLink *socialLink);
void addSocialLink(SocialLink *socialLink);
void DeleteSocialLink(SocialLink *socialLink);
#endif /* SOCIAL_LINK_H */

