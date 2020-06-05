#include "libedu_load.h"

/**
 * 从文件中加载数据
 * filename 表示文件位置
 * pclaz 指向存储 Claz 的数据块
 * pcourse 指向存储 Course 的数据块
 * pstudent 指向存储 Student 的数据块
 **/
void load_data(char* filename)
{
    FILE* fp = fopen(filename, "r");
    int i;
    LOG("\n[load_data]\n");

    if (fp == NULL)
        ERROR_LOG(TRUE, "[ERROR] failed to read: %s\n", strerror(errno));
    __load_Claz_help(fp);
    __load_Course_help(fp);
    __load_Student_help(fp);
    __generate_pointer_help();
    fclose(fp);
}

/**
 * 从文件中加载数据
 * fp 表示打开的文件
 * pclaz 指向存储 Claz 的数据块
 * student_buffer 指向储存学生 id 的数据块
 * 返回值 0 表示读取成功
 **/
int __load_Claz_help(FILE* fp)
{
    int num, i, j;
    LOG("\n[__load_Claz_help]\n");
    // 读取班级数
    LOG("读取班级数\n");
    if (fscanf(fp, "%u", &claz_num) != 1)
        ERROR_LOG(TRUE, "[ERROR] in [load_Claz] failed to read Claz number: Integer expectedn");

    if (claz_num == 0) {
        // 没有班级
        ERROR_LOG(FALSE, "[WARN] in [load_Claz] failed to read Claz: Claz number is 0 <read Claz info skipped>\n");
        return 1;
    } else {
        // 给 pclaz 分配空间
        LOG("给 pclaz 分配空间\n");
        if (pclaz != NULL)
            free(pclaz);
        pclaz = malloc(sizeof(Claz) * claz_num);
        if (pclaz == NULL)
            ERROR_LOG(TRUE, "[ERROR] in [load_Claz] malloc for Claz failed: %s\n", strerror(errno));

        // 读取班级信息
        LOG("读取班级信息\n");
        for (i = 0; i < claz_num; ++i) {
            // 读取班级编号、班级名称、学生人数
            if (fscanf(fp, "%s%s%u", pclaz[i].id, pclaz[i].name, &pclaz[i].number) != 3)
                ERROR_LOG(TRUE, "[ERROR] in [load_Claz] failed to read Student number: Integer expected\n");

            // 分配储存 Student* 的空间
            pclaz[i].students = malloc(sizeof(Student*) * pclaz[i].number);
            if (pclaz[i].students == NULL)
                ERROR_LOG(TRUE, "[ERROR] in [load_Claz] malloc for pclaz[%d].students failed: %s\n", i, strerror(errno));
        }
    }
    return 0;
}

/**
 * 从文件中加载数据
 * fp 表示打开的文件
 * pcourse 指向存储 Course 的数据块
 * student_buffer 指向储存学生 id 的数据块
 * score_buffer 指向存储学分数的数据块
 * 返回值 0 表示读取成功
 **/
int __load_Course_help(FILE* fp)
{
    int num, i, j, is_core;
    LOG("\n[__load_Course_help]\n");

    // 读取课程数
    LOG("读取课程数\n");
    if (fscanf(fp, "%u", &course_num) != 1)
        ERROR_LOG(TRUE, "[ERROR] in [load_Course] failed to read Course number: Integer expectedn");

    if (course_num == 0) {
        // 没有课程
        ERROR_LOG(FALSE, "[WARN] in [load_Course] failed to read Course: Course number is 0 <read Course info skipped>\n");
        return 1;
    } else {
        // 给 pcourse 分配空间
        LOG("给 pcourse 分配空间\n");
        if (pcourse != NULL)
            free(pcourse);
        pcourse = malloc(sizeof(Course) * course_num);
        if (pcourse == NULL)
            ERROR_LOG(TRUE, "[ERROR] in [load_Course] malloc for Course failed: %s\n", strerror(errno));

        // 读取课程信息
        LOG("读取课程信息\n");
        for (i = 0; i < course_num; ++i) {
            // 读取课程编号、课程名称、课程学分、是否核心、选课人数
            if (fscanf(fp, "%s%s%u%d%u", pcourse[i].id, pcourse[i].name, &pcourse[i].credit, &is_core, &pcourse[i].number) != 5)
                ERROR_LOG(TRUE, "[ERROR] in [load_Course] failed to read Course credit or is_core or Student number: Integer expected\n");
            pcourse[i].is_core = is_core ? TRUE : FALSE;

            // 分配储存 Student* 的空间
            pcourse[i].students = malloc(sizeof(Student*) * pcourse[i].number);
            if (pcourse[i].students == NULL)
                ERROR_LOG(TRUE, "[ERROR] in [load_Course] malloc for pcourse[%d].students failed: %s\n", i, strerror(errno));

            // 分配储存分数的空间
            pcourse[i].scores = malloc(sizeof(score_t) * pcourse[i].number);
            if (pcourse[i].scores == NULL)
                ERROR_LOG(TRUE, "[ERROR] in [load_Course] malloc for pcourse[%d].scores failed: %s\n", i, strerror(errno));
        }
    }
    return 0;
}

/**
 * 从文件中加载数据
 * fp 表示打开的文件
 * pstudent 指向存储 Student 的数据块
 * 返回值 0 表示读取成功
 **/
int __load_Student_help(FILE* fp)
{
    int i, j;
    LOG("\n[__load_Student_help]\n");

    // 读取学生数
    LOG("读取学生数\n");
    if (fscanf(fp, "%u", &student_num) != 1)
        ERROR_LOG(TRUE, "[ERROR] in [load_Student] failed to read Student number: Integer expectedn");

    if (student_num == 0) {
        // 没有学生
        ERROR_LOG(FALSE, "[WARN] in [load_Student] failed to read Student: Student number is 0 <read Student info skipped>\n");
        return 1;
    } else {
        // 分配储存学生的空间
        LOG("分配储存学生的空间\n");
        if (pstudent != NULL)
            free(pstudent);
        pstudent = malloc(sizeof(Student) * student_num);
        if (pstudent == NULL)
            ERROR_LOG(TRUE, "[ERROR] in [load_Student] malloc for Student failed: %s\n", strerror(errno));

        // 给 student_claz_buffer 分配空间
        LOG("给 student_claz_buffer 分配空间\n");
        student_claz_buffer = malloc(sizeof(name_t) * student_num);
        if (student_claz_buffer == NULL)
            ERROR_LOG(TRUE, "[ERROR] in [load_Student] malloc for student_claz_buffer failed: %s\n", strerror(errno));

        // 给 course_buffer 分配空间
        LOG("给 course_buffer 分配空间\n");
        student_course_buffer = malloc(sizeof(sid_t*) * student_num);
        if (student_course_buffer == NULL)
            ERROR_LOG(TRUE, "[ERROR] in [load_Student] malloc for student_course_buffer failed: %s\n", strerror(errno));

        // 读取学生信息
        LOG("读取学生信息\n");
        for (i = 0; i < student_num; ++i) {
            // 读取学生学号、学生名称、所属班级、选课数
            if (fscanf(fp, "%s%s%s%u", pstudent[i].id, pstudent[i].name, student_claz_buffer[i], &pstudent[i].number) != 4)
                ERROR_LOG(TRUE, "[ERROR] in [load_Student] failed to read Course number: Integer expected\n");

            // 分配储存课程编号的空间
            student_course_buffer[i] = malloc(sizeof(sid_t) * pstudent[i].number);
            if (student_course_buffer[i] == NULL)
                ERROR_LOG(TRUE, "[ERROR] in [load_Student] malloc for student_course_buffer[%d] failed: %s\n", i, strerror(errno));

            // 分配储存分数的空间
            pstudent[i].scores = malloc(sizeof(score_t) * pstudent[i].number);
            if (pstudent[i].scores == NULL)
                ERROR_LOG(TRUE, "[ERROR] in [load_Student] malloc for pstudent[%d].scores failed: %s\n", i, strerror(errno));

            // 读取学生信息
            for (j = 0; j < pstudent[i].number; ++j) {
                // 读取课程编号
                if (feof(fp))
                    ERROR_LOG(TRUE, "[ERROR] in [load_Student] failed to read Course id: End of file\n");
                else
                    fscanf(fp, "%s", student_course_buffer[i][j]);

                // 读取课程分数
                if (fscanf(fp, "%lf", &pstudent[i].scores[j]) != 1)
                    ERROR_LOG(TRUE, "[ERROR] in [load_Student] failed to read Student score[%d]: Double expected\n", j);

                // 判断分数是否合法 0-100
                if (pstudent[i].scores[j] < 0 || pstudent[i].scores[j] > 100)
                    ERROR_LOG(TRUE, "[ERROR] in [load_Student] wrong score %lf: [0,100] expected\n", pstudent[i].scores[j]);
            }

            // 分配储存课程的空间
            pstudent[i].courses = malloc(sizeof(sid_t*) * pstudent[i].number);
            if (pstudent[i].courses == NULL)
                ERROR_LOG(TRUE, "[ERROR] in [load_Student] malloc for pstudent[%d].courses failed: %s\n", i, strerror(errno));
        }
    }
    return 0;
}

/**
 * 生成指针
 * 将 Claz、Course、Student 内的指针进行关联
 **/
void __generate_pointer_help()
{
    int i, j, k, index;
    LOG("\n[__generate_pointer_help]\n");

    // 关联 Claz 和 Student
    LOG("关联 Claz 和 Student\n");
    for (i = 0; i < claz_num; ++i) {
        index = 0;
        for (j = 0; j < student_num; ++j) {
            if (strcmp(pclaz[i].id, student_claz_buffer[j]) == 0) {
                pclaz[i].students[index++] = pstudent + j;
                pstudent[j].claz = pclaz + i;
            }
        }
    }

    // 关联 Course 和 Student
    LOG("关联 Course 和 Student\n");
    for (i = 0; i < course_num; ++i) {
        index = 0;
        for (j = 0; j < student_num; ++j) {
            for (k = 0; k < pstudent[j].number; ++k) {
                if (strcmp(pcourse[i].id, student_course_buffer[j][k]) == 0) {
                    pcourse[i].students[index] = pstudent + j;
                    pcourse[i].scores[index++] = pstudent[j].scores[k];
                    pstudent[j].courses[k] = pcourse + i;
                    break;
                }
            }
        }
    }
}