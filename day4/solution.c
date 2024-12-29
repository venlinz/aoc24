#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int8_t x;
    int8_t y;
} Direction;

typedef struct {
    char * grid_data;
    int32_t row_count;
    int32_t column_count;
} Grid;

uint32_t part1(Grid grid);
uint32_t part2(Grid grid);
Grid get_input(char * filepath);
uint8_t get_xmas_word_count_in_all_direction(Grid grid, int32_t x, int32_t y);
bool is_mas_in_x_shape_from_origin(Grid grid, int32_t x, int32_t y);
bool within_bounds(Direction dir, int32_t x, int32_t y, int32_t max_row_count, int32_t max_column_count);
char * get_direction_for_index(uint32_t index);
char get_character_at_point(Grid grid, int32_t y, int32_t x);
bool is_legit_character_to_create_xmas_shape(char letter);
bool does_create_valid_xmas_shape(char xmas_shape_characters[], uint32_t xmas_positions_count);

#define SEARCH_DIRECTION_LEN (uint32_t) 3
Direction coordinates_of_search_paths[][SEARCH_DIRECTION_LEN] = {
    {{.x = 0, .y = -1}, {.x = 0, .y = -2}, {.x = 0, .y = -3}},    // north
    {{.x = 1, .y = -1}, {.x = 2, .y = -2}, {.x = 3, .y = -3}},    // north east
    {{.x = 1, .y = 0}, {.x = 2, .y = 0}, {.x = 3, .y = 0}},       // east
    {{.x = 1, .y = 1}, {.x = 2, .y = 2}, {.x = 3, .y = 3}},       // south-east
    {{.x = 0, .y = 1}, {.x = 0, .y = 2}, {.x = 0, .y = 3}},       // south
    {{.x = -1, .y = 1}, {.x = -2, .y = 2}, {.x = -3, .y = 3}},    // south-west
    {{.x = -1, .y = 0}, {.x = -2, .y = 0}, {.x = -3, .y = 0}},    // west
    {{.x = -1, .y = -1}, {.x = -2, .y = -2}, {.x = -3, .y = -3}}, // north-west
};
#define SEARCH_PATHS_LEN (uint32_t) (sizeof(coordinates_of_search_paths)/sizeof(coordinates_of_search_paths[0]))

#define X_MAS_SHAPE_COORDINATES_LEN 4
Direction x_mas_shap_coordinates[X_MAS_SHAPE_COORDINATES_LEN] = {
    {.x = 1, .y = -1,},  // nort-east
    {.x = 1, .y = 1,},   // south-east
    {.x = -1, .y = 1,},  // south-west
    {.x = -1, .y = -1,}, // south-west
};


uint32_t counter = 0;

int main(void) {
    char * filepath = "./input.txt";
    Grid grid = get_input(filepath);
    printf("part1 answer: %i\n", part1(grid));
    printf("part2 answer: %i\n", part2(grid));
    return EXIT_SUCCESS;
}

uint32_t part1(Grid grid) {
    uint32_t xmas_count = 0;
    for (int32_t y = 0; y < grid.column_count; y++) {
        for (int32_t x = 0; x < grid.row_count; x++) {
            xmas_count += get_xmas_word_count_in_all_direction(grid, x, y);
        }
    }
    return xmas_count;
}

uint32_t part2(Grid grid) {
    uint32_t xmas_shape_count = 0;
    for (int32_t y = 0; y < grid.column_count; y++) {
        for (int32_t x = 0; x < grid.row_count; x++) {
            xmas_shape_count += is_mas_in_x_shape_from_origin(grid, x, y) ? 1 : 0;
        }
    }
    return xmas_shape_count;
}

bool is_mas_in_x_shape_from_origin(Grid grid, int32_t x, int32_t y) {
    if (grid.grid_data[y * grid.column_count + x] != 'A') {
        return false;
    }
    uint32_t xmas_positions_count = 0;
    char xmas_shape_characters[4] = {0};
    for (uint32_t i = 0; i < X_MAS_SHAPE_COORDINATES_LEN; ++i) {
        Direction dir = x_mas_shap_coordinates[i];
        if (within_bounds(dir, x, y, grid.row_count, grid.column_count)) {
            char letter = get_character_at_point(grid, y + dir.y, x + dir.x);
            if (!is_legit_character_to_create_xmas_shape(letter)) {
                return false;
            }
            xmas_shape_characters[xmas_positions_count] = letter; 
            xmas_positions_count++;
        } else {
            return false;
        }
    }
    if (does_create_valid_xmas_shape(xmas_shape_characters, xmas_positions_count)) {
        return true;
    }
    return false;
}

bool does_create_valid_xmas_shape(char xmas_shape_characters[], uint32_t xmas_positions_count) {
    if (xmas_positions_count < 4) {
        return false;
    }
    if (xmas_shape_characters[0] == xmas_shape_characters[2]) {
        return false;
    }
    if (xmas_shape_characters[1] == xmas_shape_characters[3]) {
        return false;
    }
    uint32_t m_count = 0;
    uint32_t s_count = 0;
    for (uint32_t i = 0; i < xmas_positions_count; ++i) {
        if (xmas_shape_characters[i] == 'M') {
            m_count++;
        } else if (xmas_shape_characters[i] == 'S') {
            s_count++;
        } else { 
            fprintf(stderr, "unexpected character found %c", xmas_shape_characters[i]);
            exit(1);
        }
    }
    return s_count == m_count;
}

bool is_legit_character_to_create_xmas_shape(char letter) {
    return letter == 'M' || letter == 'S';
}
char get_character_at_point(Grid grid, int32_t y, int32_t x) {
    return grid.grid_data[y * grid.column_count + x];
}

char get_expected_letter_for_current_letter_position(uint8_t letter_position) {
    if (letter_position == 0) return 'M';
    if (letter_position == 1) return 'A';
    if (letter_position == 2) return 'S';
    assert(false && "Unreachable code");
}

void print_match(char * input, Direction * directions, int32_t x, int32_t y, int32_t max_column_count) {
    printf("    \33[32mmatched string: X");
    for (int i = 0; i < 3; ++i) {
        Direction dir = directions[i];
        printf("%c", input[(y + dir.y) * max_column_count + dir.x + x]);
    }
    printf("\x1B[0m\n");
}

uint8_t get_xmas_word_count_in_all_direction(Grid grid, int32_t x, int32_t y) {
    counter++;
    char * grid_data = grid.grid_data;
    if (grid_data[y * grid.column_count + x] != 'X') {
        return 0;
    }
    // printf("%i: \x1B[31mX match coordinate: y: %i, x: %i\x1B[0m\n", counter, y + 1, x + 1);
    uint8_t xmas_word_count = 0;
    for (uint32_t search_path_index= 0; search_path_index < SEARCH_PATHS_LEN; ++search_path_index) {
        Direction * directions = (Direction *) coordinates_of_search_paths[search_path_index];
        bool has_match = true;
        // printf("direction: %s -> \n", get_direction_for_index(search_path_index));
        for (uint32_t search_direction_coord_index = 0; search_direction_coord_index < SEARCH_DIRECTION_LEN; ++search_direction_coord_index) {
            Direction dir = directions[search_direction_coord_index];
            // printf("    %i: dy: %i, dx: %i -> ", search_direction_coord_index + 1, dir.x, dir.y);
            // printf("ny: %i, nx: %i -> ", x + dir.x + 1, y + dir.y + 1);
            if (within_bounds(dir, x, y, grid.row_count, grid.column_count)) {
                char letter = grid_data[(y + dir.y) * grid.column_count + x + dir.x];
                char expected_letter = get_expected_letter_for_current_letter_position(search_direction_coord_index);
                if (letter != expected_letter) {
                    has_match = false;
                    // printf("invalid char: %c expected: %c\n", letter, expected_letter);
                    break;
                } else {
                    // printf("valid char: %c\n", expected_letter);
                }
            } else {
                has_match = false;
                // printf("out of bounds\n");
                break;
            }
        }
        if (has_match) {
            // print_match(grid_data, directions, x, y, grid.column_count);
            xmas_word_count++;
        }
    }
    return xmas_word_count;
}

Grid get_input(char * filepath) {
    FILE * file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file '%s' for reading because of %s\n", filepath, strerror(errno));
        exit(1);
    }
    int32_t row_count = 0;
    int32_t column_count = 0;
    char c;
    char *input = malloc(20 * 1024);
    if (input == NULL) {
        fprintf(stderr, "out of memory");
        exit(1);
    }
    uint32_t i = 0;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            row_count++;
            continue;
        }
        input[i++] = c;
        if (row_count == 0) {
            column_count++;
        }
    }
    printf("row count: %i, column_count: %i\n", row_count, column_count);
    return (Grid) {.column_count = column_count, .row_count = row_count, .grid_data = input};
}

bool is_coordinates_outside_grid(Direction dir, int32_t x, int32_t y, int32_t max_row_count, int32_t max_column_count) {
    return (dir.y + y) >= max_column_count || (dir.x + x)  >= max_row_count;
}

bool is_coordinates_negative(Direction dir, int32_t x, int32_t y) {
    return dir.x + x < 0 || dir.y + y < 0;
}

bool within_bounds(Direction dir, int32_t x, int32_t y, int32_t max_row_count, int32_t max_column_count) {
    if (is_coordinates_negative(dir, x, y)) {
        return false;
    }
    if (is_coordinates_outside_grid(dir, x, y, max_row_count, max_column_count)) {
        return false;
    }
    return true;
}

char * get_direction_for_index(uint32_t index) {
    switch (index) {
        case 0: return "north";
        case 1: return "north east";
        case 2: return "east";
        case 3: return "south-east";
        case 4: return "south";
        case 5: return "south-west";
        case 6: return "west";
        case 7: return "north-west";
        default: return "invalid direction";
    }
}
