
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <stdint.h>

#include "cmd.h"
#include "duc.h"


static int index_main(int argc, char **argv)
{
	int c;
	char *path_db = NULL;
	int flags = 0;

	struct option longopts[] = {
		{ "database",        required_argument, NULL, 'd' },
		{ "one-file-system", required_argument, NULL, 'x' },
		{ "quiet",           no_argument,       NULL, 'q' },
		{ "verbose",         required_argument, NULL, 'v' },
		{ NULL }
	};

	while( ( c = getopt_long(argc, argv, "d:qxv", longopts, NULL)) != EOF) {

		switch(c) {
			case 'd':
				path_db = optarg;
				break;
			case 'q':
				flags |= DUC_INDEX_QUIET;
				break;
			case 'x':
				flags |= DUC_INDEX_XDEV;
				break;
			case 'v':
				flags |= DUC_INDEX_VERBOSE;
				break;
			default:
				return(-1);
		}
	}
	
	argc -= optind;
	argv += optind;

	if(argc < 1) {
		fprintf(stderr, "Required index path missing.\n");
		return -1;
	}

	struct duc *duc = duc_open(path_db, DUC_OPEN_RW);
	if(duc == NULL) return -1;


	/* Index all paths passed on the cmdline */

	int i;
	for(i=0; i<argc; i++) {
		duc_index(duc, argv[i], flags);
	}

	duc_close(duc);

	return 0;
}



struct cmd cmd_index = {
	.name = "index",
	.description = "Index filesystem",
	.usage = "[options] PATH ...",
	.help = 
		"Valid options:\n"
		"\n"
		"  -d, --database=ARG      use database file ARG [~/.duc.db]\n"
		"  -q, --quiet             do not report errors\n"
		"  -x, --one-file-system   don't cross filesystem boundaries\n"
		"  -v, --verbose           show what is happening\n"
		,
	.main = index_main
};
/*
 * End
 */
