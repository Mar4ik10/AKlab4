#ifndef HELLO1_H
#define HELLO1_H

struct my_data {
	struct list_head list;
	ktime_t timestamp;
	ktime_t after;
};

void print_hello(int count);

#endif
