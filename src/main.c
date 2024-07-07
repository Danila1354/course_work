#include <stdio.h>
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "drawing_line_functions.h"
#include "input_functions.h"



int main(int argc, char *argv[]) {
    const char *short_options = "hi:o:";
    Element *dict = malloc(sizeof(Element) * 200);
    int len_dict = 0;

    const struct option long_options[] = {
            {"input",      required_argument, NULL, 'i'},
            {"output",     required_argument, NULL, 'o'},
            {"info",       no_argument,       NULL, 0},
            {"help",       no_argument,       NULL, 'h'},
            {"mirror",     no_argument,       NULL, 0},
            {"axis",       required_argument, NULL, 0},
            {"left_up",    required_argument, NULL, 0},
            {"right_down", required_argument, NULL, 0},
            {"pentagram",  no_argument,       NULL, 0},
            {"center",     required_argument, NULL, 0},
            {"radius",     required_argument, NULL, 0},
            {"thickness",  required_argument, NULL, 0},
            {"color",      required_argument, NULL, 0},
            {"fill",       no_argument, NULL, 0},
            {"fill_color", required_argument, NULL, 0},
            {"rect",       no_argument,       NULL, 0},
            {"hexagon",    no_argument,       NULL, 0},
            {"outside_ornament", no_argument, NULL, 0},
            {0, 0, 0, 0}
    };
    int option;
    int option_index = 0;

    while ((option = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        if (option == '?') {
            printf("unknown option - %s\n", argv[optind - 1]);
            continue;
        }

        const char *option_name = NULL;
        if (option == 0) {
            option_name = long_options[option_index].name;
        } else {
            for (int i = 0; long_options[i].name != NULL; i++) {
                if (long_options[i].val == option) {
                    option_name = long_options[i].name;
                    break;
                }
            }
        }
        if (option_name != NULL) {
            dict[len_dict].key = strdup(option_name);
            dict[len_dict++].value = optarg != NULL ? optarg : "";
        }
    }
    int input_flag = 0;
    for (int i = 0; i < len_dict; i++) {
        if (strcmp("input", dict[i].key) == 0) {
            input_flag = 1;
        }
    }
    if (input_flag == 0) {
        if (optind < argc) {
            dict[len_dict].key = "input";
            dict[len_dict++].value = argv[argc - 1];
        }
    }
    run(dict, len_dict);
    for (int i = 0; i < len_dict; i++) {
        free(dict[i].key);
    }
    free(dict);


}