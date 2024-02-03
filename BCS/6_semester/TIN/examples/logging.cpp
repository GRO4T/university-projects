#include <iostream>
#include <loguru.hpp>

int main(int argc, char ** argv) {
    loguru::init(argc, argv);

    loguru::add_file("test.log", loguru::Append, loguru::Verbosity_MAX);

    // Only show most relevant things on stderr:
    loguru::g_stderr_verbosity = 1;


    std::cout << "Hello Logger" << std::endl;

    LOG_SCOPE_F(INFO, "Will indent all log messages within this scope.");
    LOG_F(INFO, "I'm hungry for some %.3f!", 3.14159);
    LOG_F(2, "Will only show if verbosity is 2 or higher");
    // Each function also comes with a version prefixed with D for Debug:
    DLOG_F(INFO, "Only written in debug-builds");

    // Turn off writing to stderr:
    loguru::g_stderr_verbosity = loguru::Verbosity_OFF;

    // Turn off writing err/warn in red:
    loguru::g_colorlogtostderr = false;

    // Throw exceptions instead of aborting on CHECK fails:
    loguru::set_fatal_handler([](const loguru::Message& message){
        throw std::runtime_error(std::string(message.prefix) + message.message);
    });

    return 0;
}