#ifndef VIRTUAL_FILESYSTEM_H
#define VIRTUAL_FILESYSTEM_H

#include <iostream>

class VirtualFilesystem{
private:
    struct iNode;

    static const unsigned int BLOCK_SIZE = 1024;
    static const unsigned int SYSTEM_BLOCKS = 8;

    struct iNode{

    };
public:
    VirtualFilesystem(std::string name){}
    ~VirtualFilesystem(){}
    void create(unsigned int size){}
    void uploadFile(std::string name){}
    void downloadFile(std::string name){}
    void renameFile(std::string oldName, std::string newName){}
    void list();



};

#endif