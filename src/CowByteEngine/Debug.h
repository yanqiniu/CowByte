#ifndef _DEBUG_H
#define _DEBUG_H

#define DbgINFO(format, ...) printf("INFO: " format "\n", ##__VA_ARGS__)
#define DbgWARNING(format, ...) printf("WARNING: " format "\n", ##__VA_ARGS__)
#define DbgERROR(format, ...) printf("ERROR: " format "\n", ##__VA_ARGS__)

#endif // !_DEBUG_H
