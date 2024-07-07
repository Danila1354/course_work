#include "parse_functions.h"

int *parse_coords(char *coords) {
    int *result = malloc(sizeof(int) * 2);
    char *copy_coords = strdup(coords);
    char *token = strtok(copy_coords, ".");
    result[0] = atoi(token);
    token = strtok(NULL, ".");
    result[1] = atoi(token);
    free(copy_coords);
    return result;
}
int *parse_color(char *color) {
    int *result = malloc(sizeof(int) * 3);
    char *copy_color = strdup(color);
    char *token = strtok(copy_color, ".");
    for (int i = 0; i < 3; i++) {
        result[i] = atoi(token);
        token = strtok(NULL, ".");
    }
    free(copy_color);
    return result;
}