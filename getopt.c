#include <getopt.h>
#include <stdio.h>

void parse_cmd_args(int argc, char **argv)
{
	const struct option long_options[] = {
		{"help", no_argument      , NULL, 'h'},
		{"addr", required_argument, NULL, 'a'},
		{"port", required_argument, NULL, 'p'},
		{0, 0, 0, 0}
	};
	
	char opt_buf[80];
	size_t idx, opt_buf_idx = 0;
	int ch;
	
	for (idx = 0; idx < sizeof(long_options) / sizeof(long_options[0]); ++idx) {
		if (!long_options[idx].name) {
			break;
		}
		
		opt_buf[opt_buf_idx++] = (char) (long_options[idx].val);
		if (required_argument == long_options[idx].has_arg) {
			opt_buf[opt_buf_idx++] = ':';
		}
	}
	
	while (-1 != (ch = getopt_long(argc, argv, opt_buf, long_options, NULL))) {
		for (idx = 0; idx < sizeof(long_options) / sizeof(long_options[0]); ++idx) {
			if (ch == (char) (long_options[idx].val)) {
				printf("[%s] => [%s]\n", long_options[idx].name, optarg);
			}
		}
	}
}

int main(int argc, char **argv)
{
	parse_cmd_args(argc, argv);
	return 0;
}
