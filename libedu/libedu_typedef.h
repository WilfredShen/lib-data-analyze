/**
 * TYPEDEF
 * 1. 基础类型别名
 * 2. BOOLEAN 宏定义
 * 3. Claz
 * 4. Course
 * 5. Student
 * 6. analysis
 **/

#ifndef __LIBEDU_TYPEDEF_H__
#define __LIBEDU_TYPEDEF_H__

#include <stddef.h>

/* 编号 */
typedef char sid_t[16];

/* 分数 */
typedef double score_t;

/* 学分 */
typedef unsigned int credit_t;

/* 名称 */
typedef char name_t[32];

/* 排名 */
typedef unsigned int rank_t;

/* 百分比 */
typedef double per_t;

/* 真假值 */
typedef int BOOLEAN;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* 对象指针 */
typedef void* obj_p;

/* 模式 */
typedef unsigned int mode;

/* 班级 */
typedef struct claz_s Claz;

/* 课程 */
typedef struct course_s Course;

/* 学生 */
typedef struct student_s Student;

/* 分析 */
typedef struct analysis_s analysis;

/** 
 * 班级
 * id       班级编号
 * name     班级名称
 * number   学生人数
 * students 班级学生
 **/
typedef struct claz_s {
    sid_t id;
    name_t name;
    size_t number;
    Student** students;
} Claz;

/** 
 * 课程
 * id       课程编号
 * name     课程名称
 * credit   课程学分
 * is_core  是否为核心课程
 * number   选课人数
 * students 选课学生
 * scores   学生分数
 **/
typedef struct course_s {
    sid_t id;
    name_t name;
    credit_t credit;
    BOOLEAN is_core;
    size_t number;
    Student** students;
    score_t* scores;
} Course;

/** 
 * 课程
 * id       学生学号
 * name     学生姓名
 * claz     所属班级
 * number   选课数
 * courses  选课
 * scores   每科分数
 **/
typedef struct student_s {
    sid_t id;
    name_t name;
    Claz* claz;
    size_t number;
    Course** courses;
    score_t* scores;
} Student;

/**
 * 分析类型定义
 * ANA_CLAZ     分析 Claz
 * ANA_COURSE   分析 Course
 * ANA_STUDENT  分析 Student
 **/
typedef enum analysis_t {
    ANA_CLAZ,
    ANA_COURSE,
    ANA_STUDENT
} analysis_t;

/* 分析对象类型定义 */
typedef void* analysis_p;

/**
 * rank node
 **/
typedef struct __rank_node_s {
    obj_p student;
    score_t score;
} __rank_node;

/**
 * 班级分析结果类型定义
 * ana_data     分析了哪些数据
 * ret_data     返回了哪些数据
 * distrib      学生分数分布：0-60,60-70,70-80,80-90,90-100,100
 * ranks        成绩排名
 * aver_score   平均分
 * fail_rate    挂科率
 * exc_rate     优秀率（90分及以上）
 **/
typedef struct analysis_claz_s {
    mode ana_mode;
    mode ret_mode;
    size_t distrib[6];
    obj_p* ranks;
    score_t aver_score;
    per_t fail_rate;
    per_t exc_rate;
} analysis_claz;

/**
 * 课程分析结果类型定义
 * ana_data     分析了哪些数据
 * ret_data     返回了哪些数据
 * distrib      学生分数分布：0-60,60-70,70-80,80-90,90-100,100
 * ranks        成绩排名
 * aver_score   平均分
 * fail_rat     挂科率
 * exc_rate     优秀率（90分及以上）
 **/
typedef struct analysis_course_s {
    mode ana_mode;
    mode ret_mode;
    size_t distrib[6];
    obj_p* ranks;
    score_t aver_score;
    per_t fail_rate;
    per_t exc_rate;
} analysis_course;

/**
 * 学生分析结果类型定义
 * ana_data     分析了哪些数据
 * ret_data     返回了哪些数据
 * distrib      各科分数分布：0-60,60-70,70-80,80-90,90-100,100
 * rank_claz    班级成绩排名
 * ranks        各科成绩排名
 * aver_score   平均分
 * has_fail     有无挂科
 **/
typedef struct analysis_student_s {
    mode ana_mode;
    mode ret_mode;
    size_t distrib[6];
    rank_t rank_claz;
    rank_t* ranks;
    score_t aver_score;
    BOOLEAN has_fail;
} analysis_student;

/**
 * 分析结果类型定义
 * ana_claz     Claz 分析对象
 * ana_course   Course 分析对象
 * ana_student  Student 分析对象
 **/
typedef union analysis_data_u {
    analysis_claz ana_claz;
    analysis_course ana_course;
    analysis_student ana_student;
} analysis_data;

/**
 * 数据分析
 * type     分析对象类型
 * obj_p    分析对象
 * data     分析结果
 **/
typedef struct analysis_s {
    analysis_t type;
    analysis_p obj_p;
    analysis_data data;
} analysis;

#endif