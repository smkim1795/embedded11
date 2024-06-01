#ifndef _TEXTLCD_H_
#define _TEXTLCD_H_

int textlcdInit(void); // open textlcd driver
int textlcdExit(void); // close textlcd driver 
int textlcdclear(void); // clear textlcd
int lcdtextwrite(const char *str1, const char *str2, int lineFlag); // write at textlcd

#endif
