#include <stdio.h>
#include <sys/ptrace.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int rebuildDataLen(char* data, int n, int ll, char* outdata) {
	int length = 0;
	int npr = 0; //chars per row
	int nlast = 0;
	int i = 0, npi = 0;
	char **ndata;
	int j, k, outn;
	outn = 0;
	length = ll;
	npr = length / n;
	nlast = length % n;
	ndata = (char**) calloc(n, sizeof(unsigned char*));
	for (i = 0; i < n; i++) {
		ndata[i] = (char*) calloc(npr, sizeof(unsigned char));
	}
//	char ndata[n][npr];
	for (i = 0; i < n; i++) {
		int start;
		start = i * npr;
		for (npi = 0; npi < npr; npi++) {
			ndata[i][npi] = data[start + npi];
		}
	}

	for (j = 0; j < npr; j++) {
		for (k = 0; k < n; k++) {
			outdata[outn++] = ndata[k][j];
		}
	}
	if (nlast > 0) {
		int ilast;
		for (ilast = length - nlast; ilast < length; ilast++) {
			outdata[ilast] = data[ilast];
		}
	}

	for (i = 0; i < n; i++) {
		free(ndata[i]);

	}
	free(ndata);

	return length;
}
