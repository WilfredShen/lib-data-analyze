/**
 * 示例文件
 **/

#include <stdio.h>
#include <stdlib.h>
#include "libedu.h"
// #include <dlfcn.h>// 显式加载动态库所需要的头文件

int main()
{
    char* filename = "data.txt";
    // void* dl = dlopen("libedu.so", RTLD_NOW);// 用于显式加载动态库
    analysis* pana;
    load_data(filename);

    printf("============================  MAIN  ============================\n\n");

    // 分析第一个 Claz 对象，分析核心、加权分数，返回所有数据
    pana = analyze(ANA_CLAZ, &pclaz[0], _ANA_ALL_, _RET_ALL_);
    print_analysis(pana);

    // 分析第一个 Course 对象，分析核心、加权分数，返回所有数据
    pana = analyze(ANA_COURSE, &pcourse[0], _ANA_ALL_, _RET_ALL_);
    print_analysis(pana);

    // 分析第一个 Student 对象，分析核心、加权分数，返回所有数据
    pana = analyze(ANA_STUDENT, &pstudent[0], _ANA_ALL_, _RET_ALL_);
    print_analysis(pana);

    printf("\n============================  END  =============================\n");

    return 0;
}