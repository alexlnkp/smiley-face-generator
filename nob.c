#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char* argv[]) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, "gcc", "-std=c99");
    nob_cmd_append(&cmd, "-Wall", "-Wextra", "-Wpedantic");
    nob_cmd_append(&cmd, "-o", "out/main");
    nob_cmd_append(&cmd, "src/main.c");
    nob_cmd_append(&cmd, "-lSDL2", "-lm", "-lSDL2_gfx");
    
    nob_cmd_run_sync(cmd);

    return 0;
}