/**
 *
 *	\file os_resource.h
 *	\brief Resource header
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/
#ifndef _AUTOSAR_RESOURCE_H
#define _AUTOSAR_RESOURCE_H
#include "os_types.h"
#include <autosar/os.h>
#include <copperplate/cluster.h>
#include <copperplate/registry.h>

/************************** MACRO DEFINITION ****************************/
#define resource_magic	0x0909F6F6

/************************* STRUCTURE DEFINITION *************************/
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
	struct OsResource * OsResourceLinkedResourceRef;	/*!< Array of linked resources */
	struct OsResourceXenomai * OsResourceXenomai;		/*!< Xenomai implementation for a Resource */
        //	struct OsApplication * OsApplicationAccessingApplication;
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
	unsigned int status;
};




/************************* FUNCTION DEFINITION *************************/

/**
 * \fn StatusType __CreateResource(ResourceType ResID)
 * \brief Create and init AUTOSAR resource with OsResource configuration 
 * \param resourceID Resource reference.
 * \return E_OK if no error, E_OS_ID if resourceID doesn't exist.
 */
StatusType __CreateResource(ResourceType ResID);

/**
 * \fn StatusType __GetOsResource(struct OsResource * osResource)
 * \brief Lock the resource<ResID> 
 * \param osResource Resource structure to lock
 * \return E_OK if no error  
 */
StatusType __GetOsResource(struct OsResource * osResource);

/**
 * \fn StatusType __GetResource(ResourceType ResID)
 * \brief Cf ::GetResource 
 */
StatusType __GetResource(ResourceType ResID);

/**
 * \fn StatusType __ReleaseOsResource(struct OsResource * osResource)
 * \brief Unlock the resource<ResID> 
 * \param osResource Resource structure to unlock
 * \return E_OK if no error  
 */
StatusType __ReleaseOsResource(struct OsResource * osResource);

/**
 * \fn StatusType __ReleaseResource(struct OsResource * ResID)
 * \brief Cf ::ReleaseResource
 */
StatusType __ReleaseResource(ResourceType ResID);


/**
 * \fn StatusType __GetStatus(struct OsResource * osResource)
 * \brief Get the current state of the resource (Lock or Unlock)
 * \param osResource Resource structure to be check
 * \return 0 if unlock 1 if lock
 */
int __GetStatus(struct OsResource * osResource);




























#endif
