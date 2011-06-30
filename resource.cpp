#include "resource.h"

#include <cstdio>
#include <cstdlib>

FAMConnection* fam_connection();

Resource::Resource(const char* path){
#ifdef HAVE_FAM
	char* abspath = realpath(path, NULL);
	if ( FAMMonitorFile(fam_connection(), abspath, &_fam_request, this) != 0 ){
		fprintf(stderr, "FAM failed to monitor %s\n", abspath);
	}
	free(abspath);
#endif /* HAVE_FAM */
}

Resource::~Resource(){
#ifdef HAVE_FAM
	FAMCancelMonitor(fam_connection(), &_fam_request);
#endif
}
