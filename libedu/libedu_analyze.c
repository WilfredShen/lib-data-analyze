#include "libedu_analyze.h"

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
    mode ana_mode, mode ret_mode)
{
    LOG("\n[analyze]\n");

    if (t == ANA_CLAZ)
        return __analyze_claz_help(
            (Claz*)p, // object
            ana_mode ? ana_mode : _ANA_DEFAULT_, // analyze mode
            ret_mode ? ret_mode : _RET_DEFAULT_ // return mode
        );
    if (t == ANA_COURSE)
        return __analyze_course_help(
            (Course*)p, // object
            ana_mode ? ana_mode : _ANA_DEFAULT_, // analyze mode
            ret_mode ? ret_mode : _RET_DEFAULT_ // return mode
        );
    if (t == ANA_STUDENT)
        return __analyze_student_help(
            (Student*)p, // object
            ana_mode ? ana_mode : _ANA_DEFAULT_, // analyze mode
            ret_mode ? ret_mode : _RET_DEFAULT_ // return mode
        );
}

/**
 * 分析 Claz 对象
 * pret 指向 analysis 对象，存放处理的数据
 * p 指向 Claz 对象
 * ana_mode 指定进行分析的数据
 * ret_mode 指定返回的数据
 **/
analysis* __analyze_claz_help(
    Claz* p,
    mode ana_mode, mode ret_mode)
{
    analysis* pret;
    int i, j, weight;
    double aver, tmp;
    int is_core = ana_mode & _ANA_CORE_;
    int enable_weight = ana_mode & _ANA_WEIGHT_;
    score_t* scores;
    LOG("\n[_analyze_claz_help]\n");

    // 分配 analysis 的空间
    LOG("分配 analysis 的空间\n");
    pret = malloc(sizeof(analysis));
    if (pret == NULL)
        ERROR_LOG(TRUE, "[ERROR] in [_analyze_claz_help] malloc for pret failed: %s\n", strerror(errno));

    // 分配统计分数的空间
    LOG("分配统计分数的空间\n");
    scores = malloc(sizeof(score_t) * p->number);
    if (scores == NULL)
        ERROR_LOG(TRUE, "[ERROR] in [_analyze_claz_help] malloc for scores failed: %s\n", strerror(errno));

    // 初始化分数分布
    LOG("初始化分数分布\n");
    for (i = 0; i < 6; ++i)
        pret->data.ana_claz.distrib[i] = 0;

    // 统计全班分数
    LOG("统计全班分数\n");
    aver = 0;
    for (i = 0; i < p->number; ++i) {
        tmp = 0;
        weight = 0;
        // 计算每个学生的分数
        for (j = 0; j < p->students[i]->number; ++j) {
            if (is_core ? p->students[i]->courses[j]->is_core : TRUE) {
                tmp += p->students[i]->scores[j] * (enable_weight ? p->students[i]->courses[j]->credit : 1);
                weight += enable_weight ? p->students[i]->courses[j]->credit : 1;
            }
        }

        // 计算（加权）平均分
        if (weight)
            tmp /= weight;
        else {
            ERROR_LOG(FALSE, "[WARN] in [_analyze_claz_help] weight of Student is 0 <Student score skipped>\n");
            tmp = 0;
        }

        scores[i] = tmp;
        aver += tmp;

        // 统计分数分布
        if (tmp < 59.5)
            ++pret->data.ana_claz.distrib[0];
        else
            ++pret->data.ana_claz.distrib[(int)(tmp - 49.5) / 10];
    }

    // 计算平均分
    LOG("计算平均分\n");
    if (p->number)
        aver /= p->number;
    else {
        aver = 0;
        ret_mode &= ~_RET_CLAZ_AVER_SCORE_;
        ERROR_LOG(FALSE, "[WARN] in [_analyze_claz_help] Student number is 0 <average score skipped>\n");
    }

    // 生成返回数据

    // 添加排名
    LOG("添加排名\n");
    if (ret_mode & _RET_CLAZ_RANKS_) {
        // generate nodes
        __rank_node* nodes = malloc(sizeof(__rank_node) * p->number);
        if (nodes == NULL)
            ERROR_LOG(TRUE, "[ERROR] in [_analyze_claz_help] malloc for nodes failed: %s\n", strerror(errno));
        for (i = 0; i < p->number; ++i) {
            nodes[i].score = scores[i];
            nodes[i].student = p->students[i];
        }

        // merge sort
        __rank_node_merge_sort(nodes, 0, p->number);
        pret->data.ana_claz.ranks = malloc(sizeof(Student*) * p->number);
        if (pret->data.ana_claz.ranks == NULL)
            ERROR_LOG(TRUE, "[ERROR] in [_analyze_claz_help] malloc for ranks failed: %s\n", strerror(errno));

        // copy
        for (i = 0; i < p->number; ++i)
            pret->data.ana_claz.ranks[i] = nodes[i].student;
    }

    // 平均分
    LOG("处理平均分\n");
    pret->data.ana_claz.aver_score = (ret_mode & _RET_CLAZ_AVER_SCORE_) ? aver : 0;

    // 不及格率
    LOG("处理不及格率\n");
    pret->data.ana_claz.fail_rate = (ret_mode & _RET_CLAZ_FAIL_RATE_) ? (double)pret->data.ana_claz.distrib[0] / p->number : 0;

    // 优秀率
    LOG("处理优秀率\n");
    pret->data.ana_claz.exc_rate = (ret_mode & _RET_CLAZ_EXC_RATE_) ? (double)(pret->data.ana_claz.distrib[4] + pret->data.ana_claz.distrib[5]) / p->number : 0;

    // 分数分布
    LOG("处理分数分布\n");
    if (!(ret_mode & _RET_CLAZ_DISTRIB_))
        for (i = 0; i < 6; ++i)
            pret->data.ana_claz.distrib[i] = 0;

    pret->data.ana_claz.ana_mode = ana_mode;
    pret->data.ana_claz.ret_mode = ret_mode;
    pret->type = ANA_CLAZ;
    pret->obj_p = p;
    return pret;
}

/**
 * 分析 Course 对象
 * pret 指向 analysis 对象，存放处理的数据
 * p 指向 Course 对象
 * ana_mode 指定进行分析的数据
 * ret_mode 指定返回的数据
 **/
analysis* __analyze_course_help(
    Course* p,
    mode ana_mode, mode ret_mode)
{
    analysis* pret;
    int i;
    double aver;
    LOG("\n[__analyze_course_help]\n");

    // 分配 analysis 的空间
    LOG("分配 analysis 的空间\n");
    pret = malloc(sizeof(analysis));
    if (pret == NULL)
        ERROR_LOG(TRUE, "[ERROR] in [__analyze_course_help] malloc for pret failed: %s\n", strerror(errno));

    // 初始化分数分布
    LOG("初始化分数分布\n");
    for (i = 0; i < 6; ++i)
        pret->data.ana_course.distrib[i] = 0;

    // 统计全班分数
    LOG("统计全班分数\n");
    aver = 0;
    for (i = 0; i < p->number; aver += p->scores[i++])
        if (p->scores[i] < 59.5)
            ++pret->data.ana_course.distrib[0];
        else
            ++pret->data.ana_course.distrib[(int)(p->scores[i] - 49.5) / 10];

    // 计算平均分
    LOG("计算平均分\n");
    if (p->number)
        aver /= p->number;
    else {
        aver = 0;
        ret_mode &= ~_RET_COURSE_AVER_SCORE_;
        ERROR_LOG(FALSE, "[WARN] in [__analyze_course_help] Student number is 0 <average score skipped>\n");
    }

    // 生成返回数据

    // 添加排名
    LOG("添加排名\n");
    if (ret_mode & _RET_COURSE_RANKS_) {
        // generate nodes
        __rank_node* nodes = malloc(sizeof(__rank_node) * p->number);
        if (nodes == NULL)
            ERROR_LOG(TRUE, "[ERROR] in [__analyze_course_help] malloc for nodes failed: %s\n", strerror(errno));
        for (i = 0; i < p->number; ++i) {
            nodes[i].score = p->scores[i];
            nodes[i].student = p->students[i];
        }

        // merge sort
        __rank_node_merge_sort(nodes, 0, p->number);
        pret->data.ana_course.ranks = malloc(sizeof(Student*) * p->number);
        if (pret->data.ana_course.ranks == NULL)
            ERROR_LOG(TRUE, "[ERROR] in [__analyze_course_help] malloc for ranks failed: %s\n", strerror(errno));

        // copy
        for (i = 0; i < p->number; ++i)
            pret->data.ana_course.ranks[i] = nodes[i].student;
    }

    // 平均分
    LOG("处理平均分\n");
    pret->data.ana_course.aver_score = (ret_mode & _RET_COURSE_AVER_SCORE_) ? aver : 0;

    // 不及格率
    LOG("处理不及格率\n");
    pret->data.ana_course.fail_rate = (ret_mode & _RET_COURSE_FAIL_RATE_) ? (double)pret->data.ana_course.distrib[0] / p->number : 0;

    // 优秀率
    LOG("处理优秀率\n");
    pret->data.ana_course.exc_rate = (ret_mode & _RET_COURSE_EXC_RATE_) ? (double)(pret->data.ana_course.distrib[4] + pret->data.ana_course.distrib[5]) / p->number : 0;

    // 分数分布
    LOG("处理分数分布\n");
    if (!(ret_mode & _RET_COURSE_DISTRIB_))
        for (i = 0; i < 6; ++i)
            pret->data.ana_course.distrib[i] = 0;

    pret->data.ana_course.ana_mode = ana_mode;
    pret->data.ana_course.ret_mode = ret_mode;
    pret->type = ANA_COURSE;
    pret->obj_p = p;
    return pret;
}

/**
 * 分析 Student 对象
 * pret 指向 analysis 对象，存放处理的数据
 * p 指向 Student 对象
 * ana_mode 指定进行分析的数据
 * ret_mode 指定返回的数据
 **/
analysis* __analyze_student_help(
    Student* p,
    mode ana_mode, mode ret_mode)
{
    analysis* pret;
    analysis* pana_claz;
    analysis* pana_course;
    Course* ptmp_course;
    int i, j;
    double aver;

    LOG("\n[__analyze_student_help]\n");

    // 分配 analysis 的空间
    LOG("分配 analysis 的空间\n");
    pret = malloc(sizeof(analysis));
    if (pret == NULL)
        ERROR_LOG(TRUE, "[ERROR] in [__analyze_course_help] malloc for pret failed: %s\n", strerror(errno));

    // 统计课程分数
    LOG("统计课程分数\n");
    for (i = 0; i < p->number; aver += p->scores[i++])
        if (p->scores[i] < 59.5)
            ++pret->data.ana_student.distrib[0];
        else
            ++pret->data.ana_student.distrib[(int)(p->scores[i] - 49.5) / 10];

    // 计算平均分
    LOG("计算平均分\n");
    if (p->number)
        aver /= p->number;
    else {
        aver = 0;
        ret_mode &= ~_RET_STUDENT_AVER_SCORE_;
        ERROR_LOG(FALSE, "[WARN] in [__analyze_student_help] Course number is 0 <average score skipped>\n");
    }

    // 班级排名
    LOG("处理班级排名\n");
    if (ret_mode & _RET_STUDENT_RANK_CLAZ_) {
        pana_claz = __analyze_claz_help(p->claz, ana_mode, _RET_CLAZ_RANKS_);
        for (i = 0; i < ((Claz*)(pana_claz->obj_p))->number; ++i)
            if (!strcmp(((Claz*)(pana_claz->data.ana_claz.ranks[i]))->id, p->id))
                break;
        pret->data.ana_student.rank_claz = i + 1;
        LOG("\n[__analyze_student_help]\n");
    }

    // 课程排名
    LOG("处理课程排名\n");
    if (ret_mode & _RET_STUDENT_RANKS_) {
        // 分配 ranks 的空间
        pret->data.ana_student.ranks = malloc(sizeof(rank_t) * p->number);
        if (pret->data.ana_student.ranks == NULL)
            ERROR_LOG(TRUE, "[ERROR] in [__analyze_student_help] malloc for ranks failed: %s\n", strerror(errno));
        // 计算每个班级的排名
        for (i = 0; i < p->number; ++i) {
            pana_course = __analyze_course_help(p->courses[i], ana_mode, _RET_COURSE_RANKS_);
            for (j = 0; j < ((Course*)(pana_course->obj_p))->number; ++j)
                if (!(strcmp(((Course*)(pana_course->data.ana_course.ranks[j]))->id, p->id)))
                    break;
            pret->data.ana_student.ranks[i] = j + 1;
        }
        LOG("\n[__analyze_student_help]\n");
    }

    // 平均分
    LOG("处理平均分\n");
    pret->data.ana_student.aver_score = (ret_mode & _RET_STUDENT_AVER_SCORE_) ? aver : 0;

    // 有无挂科
    LOG("处理有无挂科\n");
    pret->data.ana_student.has_fail = (ret_mode & _RET_STUDENT_HAS_FAIL_) && pret->data.ana_student.distrib[0];

    // 分数分布
    LOG("处理分数分布\n");
    if (!(ret_mode & _RET_STUDENT_DISTRIB_))
        for (i = 0; i < 6; ++i)
            pret->data.ana_student.distrib[i] = 0;

    pret->data.ana_student.ana_mode = ana_mode;
    pret->data.ana_student.ret_mode = ret_mode;
    pret->type = ANA_STUDENT;
    pret->obj_p = p;
    return pret;
}