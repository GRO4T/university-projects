#ifndef VIRTUAL_FILESYSTEM_H
#define VIRTUAL_FILESYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

class VirtualFilesystem{
private:
    struct INode;

    static const unsigned int BLOCK_SIZE = 1024;
    static const unsigned int SYSTEM_BLOCKS = 8;

    std::string name;
    unsigned int size;
    unsigned int blocksUsed;

    struct INode{
        bool used;
        unsigned int begin;
        unsigned int size;
        unsigned int blocks;
        char name[48];

        unsigned int end(){ return begin + blocks; }
    };

    std::vector<INode> inodes;

    typedef char buffer [BLOCK_SIZE];

    INode * findINodeByName(std::string name);
    unsigned int alloc(unsigned int blocks);
    void defragment();
    void close();
    int cmpINodes(INode a, INode b);
public:
    VirtualFilesystem(std::string name);
    ~VirtualFilesystem(){}
    void create(unsigned int size);
    void open();
    void uploadFile(std::string filename);
    void downloadFile(std::string filename){}
    void renameFile(std::string oldName, std::string newName){}
    void list();

    void display_filemap();



};

#endif