/*  Metrowerks Standard Library  *//*  $Date: 2000/02/02 19:49:35 $  *  $Revision: 1.10.2.1 $  *  $NoKeywords: $  * *		Portions Copyright � 1995-2000 Metrowerks, Inc. *		All rights reserved. */ /** **  hashset.h **//*	WARNING - WARNING - WARNING		This header is OBSOLETE		Please use <hash_set> instead*/#ifndef _HASHSET_H#define _HASHSET_H#include <hash_set>#include <hashfun.h>#ifndef _MSL_NO_CPP_NAMESPACE	namespace std	{		using Metrowerks::hash_set;	}#endif#endif // _HASHSET_H// 970723 scm  wrapped file in #ifndef _No_Floating_Point// hh 971220 fixed MOD_INCLUDE// hh 971222 added alignment wrapper// hh 991224 Rewrote