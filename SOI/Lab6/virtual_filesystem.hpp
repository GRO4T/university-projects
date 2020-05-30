#ifndef VIRTUAL_FILESYSTEM_H
#define VIRTUAL_FILESYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "filesystem_exception.hpp"

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
        unsigned int firstBlock;
        unsigned int size;
        unsigned int blocks;
        char name[48];

        unsigned int lastBlock(){ return firstBlock + blocks; }
    };

    std::vector<INode> inodes;

    typedef char buffer [BLOCK_SIZE];

    INode * findINode(std::string name);
    int findINodeId(std::string name);
    unsigned int alloc(unsigned int blocks);
    void defragment();
    void close();
    static bool cmpINodes(INode a, INode b);
public:
    VirtualFilesystem(std::string name);
    ~VirtualFilesystem();
    void create(unsigned int size);
    void open();
    void uploadFile(std::string sourceFilename, std::string uploadAs);
    void downloadFile(std::string sourceFilename, std::string downloadAs);
    void renameFile(std::string oldName, std::string newName);
    void removeFile(std::string filename);
    void list();

    void displayFilemap();
    void displayDetailedFilemap();



};

#endif