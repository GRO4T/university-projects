#include "monkeywrap.hpp"
#include "utils.hpp"
#include "Ketjev2.h"


void print_help() {
    std::string ketje_name = "KetjeJr";
    #ifdef KETJE_SR
    ketje_name = "KetjeSr";
    #elif KETJE_MINOR
    ketje_name = "KetjeMinor";
    #elif KETJE_MAJOR
    ketje_name = "KetjeMajor";
    #endif 
    std::cout << "USAGE: " << ketje_name << " mode [options]\n"
                 "\n"
                 "OPTIONS:\n"
                 "mode          Mode of operation: encrypt, decrypt.\n"
                 "-i            Implementation: KRYS, KeccakTools.\n"
                 "-p            Text to encrypt.\n"
                 "-c            Ciphertext to decrypt (as hexstring).\n"
                 "-t            Tag (as hexstring).\n"
                 "-k            Key.\n"
                 "-n            Nonce.\n"
                 "-a            Associated data.\n"
                 "-h            Print help\n"
                 "\n"
                 "EXAMPLES:\n"
                 "Encrypt message\n"
                 "./" << ketje_name << " encrypt -i KRYS -p 'Hello World' -n '1234' -k '1234' -a 'abc'\n"
                 "\n"
                 "Decrypt message\n"
                 "./" << ketje_name << " decrypt -i KRYS -c 7efa811a94523e5a882596 -t 6d -n '1234' -k '1234' -a 'abc'\n"
                 ;
}

bool cmd_option_exists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

char* get_cmd_option(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    print_help();
    return 0;
}

std::string hex_to_string(const std::string& input) {
    static const char* const lut = "0123456789abcdef";
    size_t len = input.length();
    if (len & 1) throw;
    std::string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2) {
        char a = input[i];
        const char* p = std::lower_bound(lut, lut + 16, a);
        if (*p != a) throw;
        char b = input[i + 1];
        const char* q = std::lower_bound(lut, lut + 16, b);
        if (*q != b) throw;
            output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}

int main(int argc, char* argv[]) {
    if (argc <= 2 || cmd_option_exists(argv, argv+argc, "-h")) {
        print_help();
        return 0;
    }

    std::string mode = argv[1];
    if (mode == "encrypt") {
        std::string key = get_cmd_option(argv, argv+argc, "-k");
        std::string nonce = get_cmd_option(argv, argv+argc, "-n");
        std::string associated_data = get_cmd_option(argv, argv+argc, "-a");
        std::string plaintext = get_cmd_option(argv, argv+argc, "-p");
        uint extract_bits = 8;

        std::string implementation = get_cmd_option(argv, argv+argc, "-i");

        if (implementation == "KRYS") {
#ifdef KETJE_JR
            Krys::MonkeyWrap mw(16, 12, 1, 6);
#elif KETJE_SR
            Krys::MonkeyWrap mw(32, 12, 1, 6);
#elif KETJE_MINOR
            Krys::MonkeyWrap mw(128, 12, 1, 6);
#elif KETJE_MAJOR
            Krys::MonkeyWrap mw(256, 12, 1, 6);
#endif
            mw.initialize(BitString(key), BitString(nonce));
            auto [ciphertext, tag] = mw.wrap(BitString(associated_data), BitString(plaintext), extract_bits);
            std::cout << "ciphertext: " << ciphertext << std::endl;
            std::cout << "tag: " << tag << std::endl;
        } else if (implementation == "KeccakTools") {
#ifdef KETJE_JR
            KetjeJr ketje;
#elif KETJE_SR
            KetjeSr ketje;
#elif KETJE_MINOR
            KetjeMinor ketje;
#elif KETJE_MAJOR
            KetjeMajor ketje;
#endif
            ketje.initialize(key, nonce);
            std::string tag;
            auto ciphertext = ketje.wrap(associated_data, plaintext, extract_bits, tag);
            std::cout << "ciphertext: " << format_hex(ciphertext) << std::endl;
            std::cout << "tag: " << format_hex(tag) << std::endl;
        } else {
            print_help();
        }
    } else if (mode == "decrypt") {
        std::string ciphertext = hex_to_string(get_cmd_option(argv, argv+argc, "-c"));
        std::string tag = hex_to_string(get_cmd_option(argv, argv+argc, "-t"));
        std::string key = get_cmd_option(argv, argv+argc, "-k");
        std::string nonce = get_cmd_option(argv, argv+argc, "-n");
        std::string associated_data = get_cmd_option(argv, argv+argc, "-a");

        std::string implementation = get_cmd_option(argv, argv+argc, "-i");

        if (implementation == "KRYS") {
#ifdef KETJE_JR
            Krys::MonkeyWrap mw(16, 12, 1, 6);
#elif KETJE_SR
            Krys::MonkeyWrap mw(32, 12, 1, 6);
#elif KETJE_MINOR
            Krys::MonkeyWrap mw(128, 12, 1, 6);
#elif KETJE_MAJOR
            Krys::MonkeyWrap mw(256, 12, 1, 6);
#endif
            mw.initialize(BitString(key), BitString(nonce));
            auto plaintext = mw.unwrap(BitString(associated_data), BitString(ciphertext), BitString(tag));
            std::cout << "plaintext: " << plaintext.str() << std::endl;
        } else if (implementation == "KeccakTools") {
#ifdef KETJE_JR
            KetjeJr ketje;
#elif KETJE_SR
            KetjeSr ketje;
#elif KETJE_MINOR
            KetjeMinor ketje;
#elif KETJE_MAJOR
            KetjeMajor ketje;
#endif
            ketje.initialize(key, nonce);
            auto plaintext = ketje.unwrap(associated_data, ciphertext, tag);
            std::cout << "plaintext: " << plaintext << std::endl;
        } else {
            print_help();
        }
    } else {
        print_help();
    }

    return 0;
}
