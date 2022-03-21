//
// Created by Salvatore Rivieccio on 20/03/22.
//
#pragma once

#ifdef _WINDLL
#ifdef __cplusplus
#define UTIL_API extern "C" __declspec(dllexport)
#else
#define UTIL_API __declspec(dllexport)
#endif
#else
#ifdef __cplusplus
#define UTIL_API extern "C"
#else
#define UTIL_API
#endif
#endif

UTIL_API int open();
UTIL_API int util_print();
UTIL_API int util_print_int(int a);
UTIL_API int util_print_params(int a, int b);
UTIL_API int close();