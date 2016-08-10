
#ifndef _XENOMAI_AUTOSAR_COMPILER_H
#define _XENOMAI_AUTOSAR_COMPILER_H

#define AUTOMATIC 
#define TYPEDEF
#define NULL_PTR	((void *) 0)


#define INLINE inline
#define LOCAL_INLINE static inline


#define FUNC(rettype,memclass) rettype

#define P2VAR(ptrtype, memclass, ptrclass) ptrtype *

#define P2CONST(ptrtype, memclass, ptrclass) const ptrtype *

#define CONSTP2VAR(ptrtype,memclass,ptrclass) ptrtype * const

#define CONSTP2CONST(ptrtype, memclass, ptrclass) const ptrtype * const

#define P2FUNC(rettype,ptrclass,fctname) rettype (*fctname)

#define CONSTP2FUNC(rettype,ptrclass,fctname) rettype (*fctname) const

#ifndef CONST // for WIN32
#define CONST(consttype,memclass) const consttype
#endif

#define VAR(vartype,memclass) vartype

#endif
