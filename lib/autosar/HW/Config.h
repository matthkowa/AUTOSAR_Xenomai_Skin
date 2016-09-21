#ifndef _AUTOSAR_HW_CONFIG_H
#define _AUTOSAR_HW_CONFIG_H
// RASPBERRY B+ GPIO
#include "Dio.h"

#define DIO_COUNT       16


struct DIOXenomai GPIO4_DIOXenomai = {
        4
};

struct DIOXenomai GPIO5_DIOXenomai = {
        5
};
struct DIOXenomai GPIO6_DIOXenomai = {
        6
};
struct DIOXenomai GPIO12_DIOXenomai = {
        12
};
struct DIOXenomai GPIO13_DIOXenomai = {
        13
};
struct DIOXenomai GPIO16_DIOXenomai = {
        16
};
struct DIOXenomai GPIO17_DIOXenomai = {
        17
};
struct DIOXenomai GPIO19_DIOXenomai = {
        19
};
struct DIOXenomai GPIO20_DIOXenomai = {
        20
};
struct DIOXenomai GPIO21_DIOXenomai = {
        21
};
struct DIOXenomai GPIO22_DIOXenomai = {
        22
};
struct DIOXenomai GPIO23_DIOXenomai = {
        23
};
struct DIOXenomai GPIO24_DIOXenomai = {
        24
};
struct DIOXenomai GPIO25_DIOXenomai = {
        25
};
struct DIOXenomai GPIO26_DIOXenomai = {
        26
};
struct DIOXenomai GPIO27_DIOXenomai = {
        27
};

struct DIOXenomai * DIO_table[DIO_COUNT] = {
        &GPIO4_DIOXenomai,
        &GPIO5_DIOXenomai,
        &GPIO6_DIOXenomai,
        &GPIO12_DIOXenomai,
        &GPIO13_DIOXenomai,
        &GPIO16_DIOXenomai,
        &GPIO17_DIOXenomai,
        &GPIO19_DIOXenomai,
        &GPIO20_DIOXenomai,
        &GPIO21_DIOXenomai,
        &GPIO22_DIOXenomai,
        &GPIO23_DIOXenomai,
        &GPIO24_DIOXenomai,
        &GPIO25_DIOXenomai,
        &GPIO26_DIOXenomai,
        &GPIO27_DIOXenomai
};



#endif
