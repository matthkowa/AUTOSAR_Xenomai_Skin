/**
 *
 *	\file os_resource.h
 *	\brief Resource header
 *	\author  Matthieu K.
 *	\version 0.1
 *	\date 29 Juin 2016
*/
#ifndef _AUTOSAR_RESOURCE_H
#define _AUTOSAR_RESOURCE_H
#include "os_types.h"
#include <autosar/os.h>
#include <copperplate/cluster.h>
#include <copperplate/registry.h>

#define resource_magic	0x0909F6F6
/*
 *	\enum ResourceProperty
 *	\brief Type of the Resource
 */
enum ResourceProperty {
	RESOURCE_INTERNAL,					/*!< Resource linked to a task */
	RESOURCE_LINKED,					/*!< Resource linked to an another resource */
	RESOURCE_STANDARD					/*!< Resource use by task or isr */
};
/*! Create a type name for ResourceProperty */
typedef enum ResourceProperty OsResourceProperty;
/*
 *	\struct OsResource
 *	\brief Structure to describe a resource
 */
struct OsResource{
	OsResourceProperty OsResourceProperty;			/*!< OsResourceProperty */
//	struct OsApplication * OsApplicationAccessingApplication;
	struct OsResource ** OsResourceLinkedResourceRef;	/*!< Array of linked resources */
	struct OsResourceXenomai * OsResourceXenomai;		/*!< Xenomai implementation for a Resource */
};



extern struct syncluster xenomai_autosar_resource_table;
/*
 *	\struct OsResourceXenomai
 *	\brief Structure to describe a Xenomai resource
 */
struct OsResourceXenomai {
	unsigned int magic;					/*!< Number to identify a internal Xenomai resource */
	char name[XNOBJECT_NAME_LEN];				/*!< Name of the resource */
	pthread_mutex_t lock;					/*!< mutex to get and release the resource */
	struct clusterobj cobj;					/*!< Xenomai Cluster object */
	TaskType owner;						/*!< Task owner of the resource */
	struct fsobj fsobj;					/*!< File system object to register a resource with FUSE */
};





StatusType __CreateResource(ResourceType ResID);

StatusType __GetOsResource(struct OsResource * osResource);
StatusType __GetResource(ResourceType ResID);

StatusType __ReleaseOsResource(struct OsResource * osResource);
StatusType __ReleaseResource(ResourceType ResID);

int __GetStatus(struct OsResource * osResource);




























#endif
