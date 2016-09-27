/**
 *
 *	\file os_resource.c
 *	\brief Resource implementation
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/
#include "os_resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <copperplate/threadobj.h>
#include <copperplate/heapobj.h>
#include <copperplate/internal.h>
#include "app/app_define.h"
#include "app/app_config.h"

extern struct Os os;
struct syncluster xenomai_autosar_resource_table;

static DEFINE_NAME_GENERATOR(resource_namegen, "resource",
			     struct OsResourceXenomai, name);

static struct OsResource * getResource(ResourceType ResID){

	struct OsResource * osResource = NULL;
	if(ResID <= RESOURCE_COUNT && ResID >= 1){	
		osResource =  os.OsResource[ResID-1];	
	}	
	return osResource;

	
}

static int __resourceInit(struct OsResource * osResource, const char *name)
{
	struct OsResourceXenomai *resource;
	pthread_mutexattr_t mattr;
	struct service svc;
	int ret = 0;
	if (threadobj_irq_p())
		return -EPERM;
	CANCEL_DEFER(svc);

	resource = xnmalloc(sizeof(*resource));
	if (resource == NULL) {
		ret = -ENOMEM;
		goto out;
	}

	/*
	 * XXX: we can't have priority inheritance with syncobj, so we
	 * have to base this code directly over the POSIX layer.
	 */
	generate_name(resource->name, name, &resource_namegen);
	resource->owner = INVALID_TASK;
	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_setprotocol(&mattr, PTHREAD_PRIO_INHERIT);
	pthread_mutexattr_setpshared(&mattr, mutex_scope_attribute);
	pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
	/* pthread_mutexattr_setrobust_np() might not be implemented. */
	pthread_mutexattr_setrobust_np(&mattr, PTHREAD_MUTEX_ROBUST_NP);
	__RT(pthread_mutex_init(&resource->lock, &mattr));
	pthread_mutexattr_destroy(&mattr);
	resource->magic = resource_magic;
	resource->status = 0;
	/*registry_init_file(&resource->fsobj, &registry_ops, 0);
	ret = __bt(registry_add_file(&resource->fsobj, O_RDONLY,
				     "/alchemy/mutexes/%s", resource->name));
	if (ret) {
		warning("failed to export mutex %s to registry, %s",
			resource->name, symerror(ret));
		ret = 0;
	}*/

	if (syncluster_addobj(&xenomai_autosar_resource_table, resource->name, &resource->cobj)) {
		//registry_destroy_file(&resource->fsobj);
		xnfree(resource);
		ret = -EEXIST;
	} else
		osResource->OsResourceXenomai = resource;

out:
	CANCEL_RESTORE(svc);

	return ret;
}

StatusType __CreateResource(ResourceType ResID){
	struct OsResource * osResource;
	char name[XNOBJECT_NAME_LEN];
	StatusType ret = E_OK;
	int mutex_ret;
	osResource = getResource(ResID);
	if(osResource == NULL){
		ret = E_OS_ID;
		goto out;
	}
	sprintf(name,"Resource %u",ResID);
	mutex_ret = __resourceInit(osResource,name);
	if(mutex_ret){
		ret = E_OS_ID;
		goto out;
	}	
	
out : 
	return ret;

}

StatusType __GetOsResource(struct OsResource * osResource){
	struct OsResourceXenomai * resource;
	StatusType ret = E_OK;
	int mutex_ret;
	
	resource = osResource->OsResourceXenomai;
	if(resource == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret =  E_OS_ID;
#endif
		goto out;
	}
	if(osResource->OsResourceProperty == RESOURCE_LINKED){
		ret =  __GetOsResource(osResource->OsResourceLinkedResourceRef);
		if(ret){
			warning("Linked Resource cannot be locked");
			goto out;
		}
	}

	mutex_ret = -__RT(pthread_mutex_lock(&resource->lock));
	if(mutex_ret){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ACCESS;
#endif
		goto out;
	}
	resource->status = 1;
out :
	return ret;
}

StatusType __GetResource(ResourceType ResID){
	struct OsResource * osResource;
	StatusType ret = E_OK;
	osResource = getResource(ResID);
	if(osResource == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}	
	ret = __GetOsResource(osResource);
out :
	return ret;
}

StatusType __ReleaseOsResource(struct OsResource * osResource){
	struct OsResourceXenomai * resource;
	StatusType ret = E_OK;
	int mutex_ret;
	resource = osResource->OsResourceXenomai;
	if(resource == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret =  E_OS_ID;
#endif
		goto out;
	}
	mutex_ret = -__RT(pthread_mutex_unlock(&resource->lock));
	if(mutex_ret){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ACCESS;
#endif
		goto out;
	}
	resource->status = 0;
out : 
	return ret;
}

StatusType __ReleaseResource(ResourceType ResID){
	struct OsResource * osResource;
	StatusType ret = E_OK;
	osResource = getResource(ResID);
	if(osResource == NULL){
		ret = E_OS_ID;
		goto out;
	}	
	ret = __ReleaseOsResource(osResource);
out :
	return ret;
}

int __GetStatus(struct OsResource * osResource){
	struct OsResourceXenomai * resource;
	resource = osResource->OsResourceXenomai;
	if(resource == NULL){
		return 0;
	}
	return resource->status;
}



