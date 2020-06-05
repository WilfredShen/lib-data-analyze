/**
 * LOAD
 * 1. load data
 **/

#ifndef __LIBEDU_LOAD_H__
#define __LIBEDU_LOAD_H__
#include "libedu_support.h"
#include <stdio.h>

/**
 * 从文件中加载数据
 * filename 表示文件位置
 * pclaz 指向存储 Claz 的数据块
 * pcourse 指向存储 Course 的数据块
 * pstudent 指向存储 Student 的数据块
 **/
void load_data(char* filename);

/**
 * 从文件中加载数据
 * fp 表示打开的文件
 * pclaz 指向存储 Claz 的数据块
 * student_buffer 指向储存学生 id 的数据块
 * 返回值 0 表示读取成功
 **/
int __load_Claz_help(FILE* fp);

/**
 * 从文件中加载数据
 * fp 表示打开的文件
 * pcourse 指向存储 Course 的数据块
 * student_buffer 指向储存学生 id 的数据块
 * score_buffer 指向存储学分数的数据块
 * 返回值 0 表示读取成功
 **/
int __load_Course_help(FILE* fp);

/**
 * 从文件中加载数据
 * fp 表示打开的文件
 * pstudent 指向存储 Student 的数据块
 * 返回值 0 表示读取成功
 **/
int __load_Student_help(FILE* fp);

/**
 * 生成指针
 * 将 Claz、Course、Student 内的指针进行关联
 **/
void __generate_pointer_help();

#endif