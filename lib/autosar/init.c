
/**
 *
 *	\file init.c
 *	\brief Xenomai init file 
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <xenomai/init.h>
#include "os_timer.h"
#include <copperplate/registry.h>
#include "os_task.h"
#include "os_alarm.h"
#include "os_resource.h"

/**
 * @defgroup autosar Autosar API
 *
 * DESC
 *
 */

static unsigned int clock_resolution = 1000000; /* microsecond. */

static const struct option autosar_options[] = {
	{
#define clock_resolution_opt	0
		.name = "autosar-clock-resolution",
		.has_arg = required_argument,
	},
	{ /* Sentinel */ }
};

static int autosar_parse_option(int optnum, const char *optarg)
{
	switch (optnum) {
	case clock_resolution_opt:
		clock_resolution = atoi(optarg);
		break;
	default:
		/* Paranoid, can't happen. */
		return -EINVAL;
	}

	return 0;
}

static void autosar_help(void)
{
        fprintf(stderr, "--autosar-clock-resolution=<ns> tick value (default 1ns, tickless)\n");
}

#ifdef CONFIG_XENO_COBALT

static inline void init_corespec(void)
{
	/*syncluster_init(&alchemy_pipe_table, "alchemy.pipe");
	registry_add_dir("/alchemy/pipes");*/
}

#else

static inline void init_corespec(void) { }

#endif

static int autosar_init(void)
{
	int ret;
	syncluster_init(&xenomai_autosar_task_table, "autosar.task");
	syncluster_init(&xenomai_autosar_resource_table, "autosar.resource");
	pvcluster_init(&xenomai_autosar_alarm_table, "autosar.alarm");

	ret = clockobj_init(&autosar_clock, clock_resolution);
	if (ret) {
		warning("%s: failed to initialize Autosar clock (res=%u ns)",
			__FUNCTION__, clock_resolution);
		return __bt(ret);
	}

	registry_add_dir("/autosar");
	registry_add_dir("/autosar/tasks");
	registry_add_dir("/autosar/alarms");	
	registry_add_dir("/autosar/resource");
	

	init_corespec();

	return 0;
}

static struct setup_descriptor autosar_skin = {
	.name = "autosar",
	.init = autosar_init,
	.options = autosar_options,
	.parse_option = autosar_parse_option,
	.help = autosar_help,
};

interface_setup_call(autosar_skin);
