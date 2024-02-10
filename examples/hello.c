#include <librishka.h>

int main() {
    for(int i = 0; i < 10; i++) {
        io_prints("Hello, world! [");
        io_printn(i);
        io_prints("]\n");
    }

    return 0;
}