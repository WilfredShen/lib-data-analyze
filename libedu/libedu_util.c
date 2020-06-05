#include "libedu_util.h"

/**
 * 错误信息输出
 * EXIT     是否结束进程
 * format   格式化字符串
 * ...      可变参数
 **/
void ERROR_LOG(BOOLEAN EXIT, char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    if (EXIT)
        exit(1);
}

/**
 * 格式化打印 analysis 数据
 **/
void print_analysis(analysis* pana)
{
    if (pana->type == ANA_CLAZ)
        __print_ana_claz_help(pana);
    else if (pana->type == ANA_COURSE)
        __print_ana_course_help(pana);
    else if (pana->type == ANA_STUDENT)
        __print_ana_student_help(pana);
}

/**
 * 格式化打印 analysis Claz 数据
 **/
void __print_ana_claz_help(analysis* pana)
{
    int i;

    printf("\n============================  Claz  ============================\n\n");
    printf("type: %d\n", pana->type);
    printf("Claz id: %s\n", ((Claz*)(pana->obj_p))->id);
    printf("\ndistribution:\n");
    if (pana->data.ana_claz.ret_mode & _RET_CLAZ_DISTRIB_) {
        for (i = 0; i < 6; ++i)
            printf("%u ", pana->data.ana_claz.distrib[i]);
        printf("\n");
    } else
        printf("not analyzed\n");
    printf("\nranks:\n");
    if (pana->data.ana_claz.ret_mode & _RET_CLAZ_RANKS_)
        for (i = 0; i < ((Claz*)(pana->obj_p))->number; ++i)
            printf("NO.%d \t%s\n", i + 1, ((Claz*)(pana->data.ana_claz.ranks[i]))->id);
    else
        printf("not analyzed\n");
    printf("\naverage score:\n");
    if (pana->data.ana_claz.ret_mode & _RET_CLAZ_AVER_SCORE_)
        printf("%lf\n", pana->data.ana_claz.aver_score);
    else
        printf("not analyzed\n");
    printf("\nfail rate:\n");
    if (pana->data.ana_claz.ret_mode & _RET_CLAZ_FAIL_RATE_)
        printf("%lf\n", pana->data.ana_claz.fail_rate);
    else
        printf("not analyzed\n");
    printf("\nexcellent rate:\n");
    if (pana->data.ana_claz.ret_mode & _RET_CLAZ_EXC_RATE_)
        printf("%lf\n", pana->data.ana_claz.exc_rate);
    else
        printf("not analyzed\n");
    printf("\n============================  END  =============================\n\n");
}

/**
 * 格式化打印 analysis Course 数据
 **/
void __print_ana_course_help(analysis* pana)
{
    int i;

    printf("\n===========================  Course  ===========================\n\n");
    printf("type: %d\n", pana->type);
    printf("Course id: %s\n", ((Course*)(pana->obj_p))->id);
    printf("\ndistribution:\n");
    if (pana->data.ana_course.ret_mode & _RET_COURSE_DISTRIB_) {
        for (i = 0; i < 6; ++i)
            printf("%u ", pana->data.ana_course.distrib[i]);
        printf("\n");
    } else
        printf("not analyzed\n");
    printf("\nranks:\n");
    if (pana->data.ana_course.ret_mode & _RET_COURSE_RANKS_)
        for (i = 0; i < ((Course*)(pana->obj_p))->number; ++i)
            printf("NO.%d \t%s\n", i + 1, ((Course*)(pana->data.ana_course.ranks[i]))->id);
    else
        printf("not analyzed\n");
    printf("\naverage score:\n");
    if (pana->data.ana_course.ret_mode & _RET_COURSE_AVER_SCORE_)
        printf("%lf\n", pana->data.ana_course.aver_score);
    else
        printf("not analyzed\n");
    printf("\nfail rate:\n");
    if (pana->data.ana_course.ret_mode & _RET_COURSE_FAIL_RATE_)
        printf("%lf\n", pana->data.ana_course.fail_rate);
    else
        printf("not analyzed\n");
    printf("\nexcellent rate:\n");
    if (pana->data.ana_course.ret_mode & _RET_COURSE_EXC_RATE_)
        printf("%lf\n", pana->data.ana_course.exc_rate);
    else
        printf("not analyzed\n");
    printf("\n============================  END  =============================\n\n");
}

/**
 * 格式化打印 analysis Student 数据
 **/
void __print_ana_student_help(analysis* pana)
{
    int i;

    printf("\n===========================  Student  ==========================\n\n");
    printf("type: %d\n", pana->type);
    printf("Student id: %s\n", ((Student*)(pana->obj_p))->id);
    if (pana->data.ana_student.ret_mode & _RET_STUDENT_AVER_SCORE_)
        printf("\naverage score: %lf\n", pana->data.ana_student.aver_score);
    else
        printf("not analyzed\n");
    printf("\ndistribution:\n");
    if (pana->data.ana_student.ret_mode & _RET_STUDENT_DISTRIB_) {
        for (i = 0; i < 6; ++i)
            printf("%u ", pana->data.ana_student.distrib[i]);
        printf("\n");
    } else
        printf("not analyzed\n");
    printf("\nrank in Claz: ");
    if (pana->data.ana_student.ret_mode & _RET_STUDENT_RANK_CLAZ_)
        printf("%u\n", pana->data.ana_student.rank_claz);
    else
        printf("not analyzed\n");
    printf("\nranks:\n");
    if (pana->data.ana_student.ret_mode & _RET_STUDENT_RANKS_)
        for (i = 0; i < ((Student*)(pana->obj_p))->number; ++i)
            printf("%s\tNO.%u\n", ((Student*)(pana->obj_p))->courses[i]->id, pana->data.ana_student.ranks[i]);
    else
        printf("not analyzed\n");
    printf("\nhas fail: ");
    if (pana->data.ana_student.ret_mode & _RET_STUDENT_HAS_FAIL_)
        printf("%s\n", pana->data.ana_student.has_fail ? "TRUE" : "FALSE");
    else
        printf("not analyzed\n");
    printf("\n============================  END  =============================\n\n");
}

/**
 * 归并排序
 * 二分
 **/
void __rank_node_merge_sort(
    __rank_node* nodes,
    int start, int end)
{
    int mid;

    if (end - start < 2)
        return;
    mid = (start + end) >> 1;
    __rank_node_merge_sort(nodes, start, mid);
    __rank_node_merge_sort(nodes, mid, end);
    __rank_node_merge_sort_help(nodes, start, mid, end);
}

/**
 * 归并排序
 * 归并
 **/
void __rank_node_merge_sort_help(
    __rank_node* nodes,
    size_t start, size_t mid, size_t end)
{
    __rank_node* tmp_nodes = malloc(sizeof(__rank_node) * (end - start));
    int i = start, j = mid, index = 0;

    if (tmp_nodes == NULL)
        ERROR_LOG(FALSE, "[ERROR] in [__rank_node_merge_sort_help] malloc for tmp_nodes failed: %s\n", strerror(errno));
    while (i < mid && j < end)
        tmp_nodes[index++] = nodes[i].score >= nodes[j].score ? nodes[i++] : nodes[j++];
    while (i < mid)
        tmp_nodes[index++] = nodes[i++];
    while (j < end)
        tmp_nodes[index++] = nodes[j++];
    i = start;
    index = 0;
    while (i < end)
        nodes[i++] = tmp_nodes[index++];
}