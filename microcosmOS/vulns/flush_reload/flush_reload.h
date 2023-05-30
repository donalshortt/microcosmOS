#ifndef __FLUSH_RELOAD_H__
#define __FLUSH_RELOAD_H__

void fr_victim(char* addr);
void fr_attacker(int threshold, char* addr);

void flush_reload();

#endif
