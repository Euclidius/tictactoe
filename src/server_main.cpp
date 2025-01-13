#include "server.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <path/to/config_file>"<< std::endl;
        exit(1);
    }
    std::string config_file = argv[1];
    Server server(config_file);

    return 0;
}
