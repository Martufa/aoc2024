#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int comp(const void* a,const void* b) {
      return *(int*)a - *(int*)b;
}

int count_lines(FILE* in_file)
{
    int total = 0;
    char read;
    while (fread(&read, 1, sizeof(char), in_file)) {
        if (read == '\n') total++;
    }

    return total;
}

int turn_to_num(char *buff) {
    int mult = 10000;
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += (buff[i] * mult);
        mult /= 10;
    }
    return total;
}

int parse_intput(FILE* in_file, int *list_a, int *list_b) {
    rewind(in_file);
    char curr_buffer[5];
    char read;
    int i = 0;
    int i_a = 0;
    int i_b = 0;
    char last_parsing = '\n';

    while (fread(&read, 1, sizeof(char), in_file) > 0) {
        if (read == 0) break;
        if (read == '\n') {
            last_parsing = '\n';
            i = 0;
            continue;
        }
        if (read == ' ' || read == '\t') {
            last_parsing = ' ';
            i = 0;
            continue;
        }

        curr_buffer[i] = (read - '0');
        i++;

        if (i == 5) {
            int num = turn_to_num(curr_buffer);
            if (last_parsing == '\n') list_a[i_a++] = num;
            if (last_parsing == ' ') list_b[i_b++] = num;
        }
    }

    return 0;
}

int get_occurances(int a, int *list_b, int len) {
    int occurance = 0;
    for (int i = 0; i < len; i++) {
        if (list_b[i] == a) occurance++;
    }
    return occurance;
}

int similarity(int *list_a, int *list_b, int len) {
    int result = 0;
    
    int curr;
    int curr_value = -1;
    int prev = -1;

    for (int i = 0; i < len; i++) {
        curr = list_a[i];
        if (prev != curr) curr_value = get_occurances(curr, list_b, len);
        result += (curr_value * curr);

        prev = curr;
    }
    return result;
}

int main() {
    FILE *in_file = fopen("aoc01_in.txt", "r");
    if (in_file == NULL) {warn("file not opened"); goto err;}
    rewind(in_file);

    int list_size = count_lines(in_file);
    if (list_size < 0) {warn("wrong countline"); goto err;}

    int *list_a = malloc(list_size * sizeof(int));
    int *list_b = malloc(list_size * sizeof(int));
    int res_distance = 0;
    if (!list_a || !list_b) {warn("not allocated"); goto err;}

    parse_intput(in_file, list_a, list_b);

    qsort(list_a, list_size, sizeof(int), comp);
    qsort(list_b, list_size, sizeof(int), comp);

    for (int i = 0; i < list_size; i++) {
        res_distance += abs(list_a[i] - list_b[i]);
    }

    int res_similarity = similarity(list_a, list_b, list_size);

    free(list_a);
    free(list_b);
    fclose(in_file);

    printf("size = %d\n", list_size);
    printf("RESULT: %d\n", res_distance);
    printf("SIMILARITY: %d\n", res_similarity);
    return res_distance;

err:
    if (list_a) free(list_a);
    if (list_b) free(list_b);
    if (in_file) fclose(in_file);

    return -1;
}