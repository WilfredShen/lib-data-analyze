/**
 * SUPPORT
 * 1. LOG 宏定义
 * 2. mode 宏定义
 * 3. 全局变量
 **/

#ifndef __LIBEDU_SUPPORT_H__
#define __LIBEDU_SUPPORT_H__
#include "libedu_typedef.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

/**
 * 日志信息宏定义
 * _LOG_DISABLED_   为 0 时输出日志信息
 **/
#ifndef _LOG_DISABLED_
#define _LOG_DISABLED_ 1
#endif
#if _LOG_DISABLED_ == 0
void LOG(char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}
#else
#define LOG /\
/LOG
#endif

#define _MODE_ALL_ 0xffffffff

/**
 * define analyze mode
 * _ANA_ALL_        包含所有选项
 * _ANA_DEFAULT_    默认
 * _ANA_CORE_       只分析核心课程
 * _ANA_WEIGHT_     加权分析（计算学分）
 **/
#define _ANA_ALL_ 0xffffffff
#define _ANA_DEFAULT_ 0x0
#define _ANA_CORE_ 0x1
#define _ANA_WEIGHT_ 0x2

/**
 * define return mode
 * _RET_ALL_        包含所有选项
 * _RET_DEFAULT_    默认
 **/
#define _RET_ALL_ 0xffffffff
#define _RET_DEFAULT_ 0xffffffff

/**
 * define claz mode
 * _RET_CLAZ_DEFAULT_       默认
 * _RET_CLAZ_DISTRIB_       返回 distribution
 * _RET_CLAZ_RANKS_         返回 ranks
 * _RET_CLAZ_AVER_SCORE_    返回 average score
 * _RET_CLAZ_FAIL_RATE_     返回 fail rate
 * _RET_CLAZ_EXC_RATE_      返回 excellent rate
 **/
#define _RET_CLAZ_DEFAULT_ 0xffffffff
#define _RET_CLAZ_DISTRIB_ 0x1
#define _RET_CLAZ_RANKS_ 0x2
#define _RET_CLAZ_AVER_SCORE_ 0x4
#define _RET_CLAZ_FAIL_RATE_ 0x8
#define _RET_CLAZ_EXC_RATE_ 0x10

/**
 * define course mode
 * _RET_COURSE_DEFAULT_       默认
 * _RET_COURSE_DISTRIB_       返回 distribution
 * _RET_COURSE_RANKS_         返回 ranks
 * _RET_COURSE_AVER_SCORE_    返回 average score
 * _RET_COURSE_FAIL_RATE_     返回 fail rate
 * _RET_COURSE_EXC_RATE_      返回 excellent rate
 **/
#define _RET_COURSE_DEFAULT_ 0xffffffff
#define _RET_COURSE_DISTRIB_ 0x1
#define _RET_COURSE_RANKS_ 0x2
#define _RET_COURSE_AVER_SCORE_ 0x4
#define _RET_COURSE_FAIL_RATE_ 0x8
#define _RET_COURSE_EXC_RATE_ 0x10

/**
 * define student mode
 * _RET_STUDENT_DEFAULT_    默认
 * _RET_STUDENT_DISTRIB_    返回 distribution
 * _RET_STUDENT_RANK_CLAZ_  返回 ranks in Claz
 * _RET_STUDENT_RANKS_      返回 ranks of all subjects
 * _RET_STUDENT_AVER_SCORE_ 返回 average score
 * _RET_STUDENT_HAS_FAIL_   返回 has fail
 **/
#define _RET_STUDENT_DEFAULT_ 0xffffffff
#define _RET_STUDENT_DISTRIB_ 0x1
#define _RET_STUDENT_RANK_CLAZ_ 0x2
#define _RET_STUDENT_RANKS_ 0x4
#define _RET_STUDENT_AVER_SCORE_ 0x8
#define _RET_STUDENT_HAS_FAIL_ 0x10

/**
 * 全局变量
 * claz_num     班级数
 * course_num   课程数
 * student_num  学生数
 * pclaz        指向班级的存储空间
 * pcourse      指向课程的存储空间
 * pstudent     指向学生的存储空间
 * student_claz_buffer      指向暂存学生班级信息的空间
 * student_course_buffer    指向暂存学生课程信息的空间
 **/
size_t claz_num;
size_t course_num;
size_t student_num;
Claz* pclaz;
Course* pcourse;
Student* pstudent;
name_t* student_claz_buffer;
sid_t** student_course_buffer;

#endif