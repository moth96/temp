/***********************************************************************
 * Filename:            log.h
 * Version:             V1.0
 * Description:         打印输出头文件
 *
 * Authors:             moth96
 **********************************************************************/
#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <string.h>

#define FILENAME(x) (strrchr((x), '/') ? strrchr((x), '/') + 1 : (x))

#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4
#define LOG_PRINT_LEVEL LOG_LEVEL_DEBUG

#define PRINT_LOG(level, ...)                           \
    do                                                  \
    {                                                   \
        if (level <= LOG_PRINT_LEVEL)                   \
            printf("[" #level "]"                       \
                   "[%s:%d]"                            \
                   __VA_ARGS__,                         \
                   FILENAME(__FILE__),                  \
                   __LINE__);                           \
            printf("\n");                               \
    } while (0);

#define LOG_ERROR(...)  PRINT_LOG(LOG_LEVEL_ERROR,  ##__VA_ARGS__)
#define LOG_WARN(...)   PRINT_LOG(LOG_LEVEL_WARN,   ##__VA_ARGS__)
#define LOG_INFO(...)   PRINT_LOG(LOG_LEVEL_INFO,   ##__VA_ARGS__)
#define LOG_DEBUG(...)  PRINT_LOG(LOG_LEVEL_DEBUG,  ##__VA_ARGS__)

#endif