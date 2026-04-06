#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

// ANSI Color Codes
#define COLOR_LIGHT_BLUE "\033[94m"
#define COLOR_YELLOW     "\033[93m"
#define COLOR_LIGHT_RED  "\033[91m"
#define COLOR_WHITE      "\033[97m"
#define COLOR_RESET      "\033[0m"

void print_highlighted_error(const char* error_msg) {
    if (!error_msg) return;

    // Work on a copy because strtok modifies the string
    char* copy = strdup(error_msg);
    char* line_ptr = strtok(copy, "\n");

    while (line_ptr != NULL) {
        char filename[256];
        int line_num;
        char message[512];

        // Trim leading whitespace for stack trace lines
        char* trimmed = line_ptr;
        while (*trimmed == ' ' || *trimmed == '\t') trimmed++;

        // Try to match "filename:line: message"
        if (sscanf(trimmed, "%255[^:]:%d: %[^\n]", filename, &line_num, message) == 3) {
            printf("%s%s%s:%s%d%s: %s%s%s\n",
                   COLOR_LIGHT_BLUE, filename, COLOR_RESET,
                   COLOR_YELLOW, line_num, COLOR_RESET,
                   COLOR_LIGHT_RED, message, COLOR_RESET);
        }
        else if (strstr(line_ptr, "stack traceback:")) {
            printf("%s%s%s\n", COLOR_WHITE, line_ptr, COLOR_RESET);
        }
        else {
            // Lines like "in function <example.lua:5>" or "in main chunk"
            printf("%s%s%s\n", COLOR_LIGHT_RED, line_ptr, COLOR_RESET);
        }

        line_ptr = strtok(NULL, "\n");
    }
    free(copy);
}

void print_usage() {
    printf("Lua55C Usage:\n");
    printf("  lua55c <file.lua>          - Execute a Lua script\n");
    printf("  lua55c -compile <file.lua>  - Compile Lua to bytecode\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if (strcmp(argv[1], "-compile") == 0) {
        if (argc < 3) {
            fprintf(stderr, "%sError: No file specified for compilation.%s\n", COLOR_LIGHT_RED, COLOR_RESET);
            lua_close(L);
            return 1;
        }
        printf("Lua55C: Compiling %s%s%s...\n", COLOR_LIGHT_BLUE, argv[2], COLOR_RESET);
    }
    else {
        const char *filename = argv[1];

        // Use luaL_dofile - it automatically pushes errors to the stack
        if (luaL_dofile(L, filename)) {
            print_highlighted_error(lua_tostring(L, -1));
            lua_pop(L, 1);
            lua_close(L);
            return 1;
        }
    }

    lua_close(L);
    return 0;
}
