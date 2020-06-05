/**
 * ANALYZE
 * 1. analyze
 **/

#ifndef __LIBEDU_ANALYZE_H__
#define __LIBEDU_ANALYZE_H__
#include "libedu_support.h"

/**
 * 进行数据分析
 * t 指定分析对象的类型
 * p 指向分析的对象
 * ana_mode 指定进行分析的数据
 * ret_mode 指定返回的数据
 **/
analysis* analyze(
    const analysis_t t,
    const analysis_p p,
    mode ana_mode, mode ret_mode);

/**
 * 分析 Claz 对象
 * pret 指向 analysis 对象，存放处理的数据
 * p 指向 Claz 对象
 * ana_mode 指定进行分析的数据
 * ret_mode 指定返回的数据
 **/
analysis* __analyze_claz_help(
    Claz* p,
    mode ana_mode, mode ret_mode);

/**
 * 分析 Course 对象
 * pret 指向 analysis 对象，存放处理的数据
 * p 指向 Course 对象
 * ana_mode 指定进行分析的数据
 * ret_mode 指定返回的数据
 **/
analysis* __analyze_course_help(
    Course* p,
    mode ana_mode, mode ret_mode);

/**
 * 分析 Student 对象
 * pret 指向 analysis 对象，存放处理的数据
 * p 指向 Student 对象
 * ana_mode 指定进行分析的数据
 * ret_mode 指定返回的数据
 **/
analysis* __analyze_student_help(
    Student* p,
    mode ana_mode, mode ret_mode);

#endif