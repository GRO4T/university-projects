#ifndef FILESYSTEM_EXCEPTION_H
#define FILESYSTEM_EXCEPTION_H

class NotEnoughSpace: public std::exception{
    const char* what() const throw() { return "Not enough space on the virtual drive."; }
};

class FileNotExist: public std::exception{
    const char* what() const throw() {
         return "File with given name doesn't exist.\nTo list all available files use -l flag.";
    }
};

class FilenameNotUnique: public std::exception{
    const char* what() const throw() {
        return "File with given name already present on the virtual drive.\n"
        "To change filename use -m flag.";
    }
};

class DiscTooSmall: public std::exception{
    const char* what() const throw() { return "Size of the virtual drive provided is too small."; }
};

#endif