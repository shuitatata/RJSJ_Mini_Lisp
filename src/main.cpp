#include <iostream>
#include <string>

#include "repl.h"
#include "rjsj_test.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        repl();
    } else {
        loadFile(argv[1]);
    }
}
