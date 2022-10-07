#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>

#define DEFAULT_BAUD 1200
#define DEFAULT_COMMAND "ssh 127.0.0.1"

struct options_s {
    long long int baud;
    char* command;
};


int parse_cmdline(struct options_s* options, int argc, char* argv[]) {
    int c;

    options->baud = DEFAULT_BAUD;
    options->command = DEFAULT_COMMAND;

    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            {"baud", required_argument, NULL, 'b'},
            {"command", required_argument, NULL, 'c'},
            {"help", no_argument, NULL, 'h'},
            {NULL, 0, NULL, 0}
        };

        c = getopt_long(argc, argv, "b:c:h", long_options, &option_index);
        if (c == -1) { break; }

        switch (c) {
            case 'b':
                options->baud = strtoll(optarg, NULL, 10);
                if (options->baud <= 0 || errno) {
                    options->baud = DEFAULT_BAUD;
                }
                break;

            case 'c':
                options->command = optarg;
                break;

            case 'h':
                printf("Usage: %s [--baud BAUD] [--command \"COMMAND\"] [--help]\n", argv[0]);
                exit(0);
                break;

            case '?':
                exit(1);
                break;
        }
    }

    return 0;
}


int main(int argc, char* argv[]) {
    struct options_s options;
    parse_cmdline(&options, argc, argv);

    struct timespec sleep_time = {0, 1000000000 / options.baud};

    FILE* shell = popen(options.command, "r");
    if (shell == NULL) {
        fprintf(stderr, "Failed to run '%s'", options.command);
        return 1;
    }

    while (1) {
        char c = fgetc(shell);
        if (c == EOF) { break; }
        fputc(c, stdout);
        fflush(stdout);
        nanosleep(&sleep_time, NULL);
    }
}
