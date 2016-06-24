
#ifndef NET_COREDEF_HPP__
#define NET_COREDEF_HPP__

#ifdef WIN32

#ifdef NETCORE_EXPORTS
		#define NETCORE_EXPORT _declspec (dllexport)
	#else
		#define NETCORE_EXPORT _declspec (dllimport)
	#endif
#else
	#define NETCORE_EXPORT
#endif

#endif