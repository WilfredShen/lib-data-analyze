/**
 * UTIL
 * 1. print
 * 2. merge sort
 **/

#ifndef __LIBEDU_UTIL_H__
#define __LIBEDU_UTIL_H__
#include "libedu_support.h"

/**
 * 格式化打印 analysis 数据
 **/
void print_analysis(analysis* pana);

/**
 * 格式化打印 analysis Claz 数据
 **/
void __print_ana_claz_help(analysis* pana);

/**
 * 格式化打印 analysis Course 数据
 **/
void __print_ana_course_help(analysis* pana);

/**
 * 格式化打印 analysis Student 数据
 **/
void __print_ana_student_help(analysis* pana);

/**
 * 归并排序
 * 二分
 **/
void __rank_node_merge_sort(
    __rank_node* nodes,
    int start, int end);

/**
 * 归并排序
 * 归并
 **/
void __rank_node_merge_sort_help(
    __rank_node* nodes,
    size_t start, size_t mid, size_t end);

#endif