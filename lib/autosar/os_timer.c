/*
 *      \file os_timer.c
 *	\brief Xenomai Timer implementation 
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/

#include <copperplate/threadobj.h>
#include "os_timer.h"

struct clockobj autosar_clock;
