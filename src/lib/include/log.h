#ifndef LOG_H
#define LOG_H
#include "ia_arch.h"
#include "board.h"

#define LOGC_RESET   "\033[0m"
#define LOGC_BLACK   "\033[30m"      /* Black */
#define LOGC_RED     "\033[31m"      /* Red */
#define LOGC_GREEN   "\033[32m"      /* Green */
#define LOGC_YELLOW  "\033[33m"      /* Yellow */
#define LOGC_BLUE    "\033[34m"      /* Blue */
#define LOGC_MAGENTA "\033[35m"      /* Magenta */
#define LOGC_CYAN    "\033[36m"      /* Cyan */
#define LOGC_WHITE   "\033[37m"      /* White */
#define LOGC_BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define LOGC_BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define LOGC_BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define LOGC_BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define LOGC_BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define LOGC_BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define LOGC_BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define LOGC_BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define CR "\r\n"

extern
void log_init(void);

#ifdef LOGGING
#define LOGI(...)\
    printf(LOGC_GREEN "[I]");\
    printf(__VA_ARGS__);\
    printf(CR LOGC_RESET)

#define LOGE(...)\
    printf(LOGC_RED "[E]");\
    printf(__VA_ARGS__);\
    printf(CR LOGC_RESET)

#define LOGW(...)\
    printf(LOGC_YELLOW "[W]");\
    printf(__VA_ARGS__);\
    printf(CR LOGC_RESET)

#else
#define LOGI(...)
#define LOGE(...)
#define LOGW(...)
#endif

#endif//LOG_H

#if defined(DEVICE_TREE_H)
/* nothing to extern */
#endif

#if defined(DEVICE_TREE_MODULE) && !defined(LOG_DEVICE_TREE)
#define LOG_DEVICE_TREE
#define LOG_INIT()		log_init()
#else
#define LOG_INIT()
#endif