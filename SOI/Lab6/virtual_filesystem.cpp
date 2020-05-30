#include "virtual_filesystem.hpp"

VirtualFilesystem::VirtualFilesystem(std::string name){
    this->name = name;
}

VirtualFilesystem::~VirtualFilesystem(){
    close();
}

void VirtualFilesystem::create(unsigned int size){
    if (size <= SYSTEM_BLOCKS){
        throw DiscTooSmall();
    }

    this->size = size;
    buffer buf;
    for (unsigned int i = 0; i < BLOCK_SIZE; ++i){
        buf[i] = '\0';
    }
    blocksUsed = SYSTEM_BLOCKS;
    std::fstream outStream;
    outStream.open(name.c_str(), std::ios::out | std::ios::binary);
    for (unsigned int i = 0; i < size; ++i){
        outStream.write(buf, BLOCK_SIZE);
    }
    outStream.close();
}

void VirtualFilesystem::open(){
    size = boost::filesystem::file_size(name) / BLOCK_SIZE;

    buffer buf[SYSTEM_BLOCKS];

    // read system blocks
    std::ifstream inStream;
    inStream.open(name.c_str(), std::ios::in | std::ios::binary);
    for (unsigned int i = 0; i < SYSTEM_BLOCKS; ++i){
        inStream.read(buf[i], BLOCK_SIZE);
    }

    blocksUsed = SYSTEM_BLOCKS;

    // find inodes in system blocks
    for (unsigned int i = 0; i < SYSTEM_BLOCKS; ++i){
        for (unsigned int j = 0; j < BLOCK_SIZE; j += sizeof(INode)){
            INode nodeCandidate = *reinterpret_cast<INode*>(buf[i] + j);
            if (nodeCandidate.used){
                inodes.push_back(nodeCandidate);
                blocksUsed += nodeCandidate.blocks;
            }
        }
    }
    inStream.close();
}

void VirtualFilesystem::close(){
  sort(inodes.begin(), inodes.end(), cmpINodes);
  buffer bufs[SYSTEM_BLOCKS];
  unsigned int index = 0;
  for(unsigned int i = 0; i < SYSTEM_BLOCKS; ++i){
    for(unsigned int j = 0; j < BLOCK_SIZE; j += sizeof(INode), ++index){
      INode * placeForINode = reinterpret_cast<INode*>(bufs[i] + j);
      // save INode
      if(index < inodes.size()){
        *placeForINode = inodes[index];
      }
      // fill with '0's everything else
      else{
        for(unsigned int k = 0; k < sizeof(INode); ++k){
          *(bufs[i] + j + k) = 0;
        }
      }
    }
  }

  std::fstream outStream;
  outStream.open(name.c_str(), std::ios::in|std::ios::out|std::ios::binary);
  outStream.seekp(std::ios_base::beg);
  for(unsigned i=0; i < SYSTEM_BLOCKS; i++){
    outStream.write(bufs[i], BLOCK_SIZE);
  }
  outStream.close();
}

void VirtualFilesystem::uploadFile(std::string filename, std::string uploadAs){
    if (findINode(uploadAs) != NULL || findINode(filename) != NULL){
        throw FilenameNotUnique();
    }

    unsigned int filesize = boost::filesystem::file_size(filename);
    unsigned int blocksNeeded = ((filesize - 1) / BLOCK_SIZE) + 1; // e.g. ((1024-1)/1024)+1 = 1, but ((1025-1)/1024)+1 = 2
    unsigned int blocksFree = size - blocksUsed;
    if (blocksNeeded > blocksFree){
        throw NotEnoughSpace();
    }

    unsigned int newFilePos = alloc(blocksNeeded);
    buffer bufs[blocksNeeded];

    std::ifstream inStream;
    inStream.open(filename.c_str());
    for (unsigned int i = 0; i < blocksNeeded; ++i){
        inStream.read(bufs[i], BLOCK_SIZE);
    }
    inStream.close();

    std::fstream outStream;
    outStream.open(name.c_str(), std::ios::out | std::ios::in | std::ios::binary);
    outStream.seekg(newFilePos * BLOCK_SIZE);
    for (unsigned int i = 0; i < blocksNeeded; ++i){
        outStream.write(bufs[i], BLOCK_SIZE);
    }
    outStream.close();

    INode newINode;
    newINode.used = 1;
    newINode.firstBlock = newFilePos;
    newINode.size = filesize;
    newINode.blocks = blocksNeeded;
    strcpy(newINode.name, uploadAs != "" ? uploadAs.c_str() : filename.c_str());
    inodes.push_back(newINode);
}

void VirtualFilesystem::downloadFile(std::string filename, std::string downloadAs = ""){
    INode * inode = findINode(filename);
    if (inode == NULL){
        throw FileNotExist();
    }
    
    buffer bufs[inode->blocks];
    std::ifstream inStream;
    inStream.open(name);
    inStream.seekg(inode->firstBlock * BLOCK_SIZE);
    for (unsigned int i = 0; i < inode->blocks; ++i){
        inStream.read(bufs[i], BLOCK_SIZE);
    }
    inStream.close();

    std::cout << inode->name << std::endl;
    std::ofstream outStream;
    if (downloadAs != ""){
        outStream.open(downloadAs);
    }
    else{
        outStream.open(inode->name);
    }

    for (unsigned int i = 0; i < inode->blocks; ++i){
        if (i + 1 < inode->blocks){
            outStream.write(bufs[i], BLOCK_SIZE);
        }
        else{
            std::cout << inode->size << std::endl;
            outStream.write(bufs[i], inode->size - (inode->blocks - 1) * BLOCK_SIZE);
        }
        
    }
    outStream.close();
}

VirtualFilesystem::INode* VirtualFilesystem::findINode(std::string name){
    for (unsigned int i = 0; i < inodes.size(); ++i){
        if (inodes[i].name == name){
            return &inodes[i];
        }
    }
    return NULL;
}

int VirtualFilesystem::findINodeId(std::string name){
    for (unsigned int i = 0; i < inodes.size(); ++i){
        if (inodes[i].name == name){
            return i;
        }
    }
    return -1;
}

unsigned int VirtualFilesystem::alloc(unsigned int blocks){
    while(1){
        unsigned int allocatedNodes = inodes.size();

        // check if there is space after SYSTEM_BLOCKS
        if (allocatedNodes == 0 || inodes[0].firstBlock - SYSTEM_BLOCKS >= blocks)
            return SYSTEM_BLOCKS;

        for (unsigned int i = 1; i < inodes.size(); ++i){
            unsigned int hole = inodes[i].firstBlock - inodes[i - 1].lastBlock();
            if (hole >= blocks){
                return inodes[i - 1].lastBlock();
            }
        }

        //check if there is space at the end
        if (allocatedNodes != 0 && size - inodes[allocatedNodes - 1].lastBlock() >= blocks)
            return inodes[allocatedNodes - 1].lastBlock();

        //if no space was found during iteration defragmentQ
        defragment();
    }
}

void VirtualFilesystem::defragment(){
    unsigned int blockToMoveId = 0;
    unsigned int newBlockPos = SYSTEM_BLOCKS;

    // if first block is perfectly aligned find next that isn't
    if (inodes[0].firstBlock == SYSTEM_BLOCKS){
        for (blockToMoveId = 1; blockToMoveId < inodes.size(); ++blockToMoveId){
            if (inodes[blockToMoveId - 1].lastBlock() < inodes[blockToMoveId - 1].firstBlock){
                newBlockPos = inodes[blockToMoveId - 1].lastBlock();
                break;
            }
        }
    }

    unsigned int blocks = inodes[blockToMoveId].blocks;
    unsigned int oldBlockPos = inodes[blockToMoveId].firstBlock;

    inodes[blockToMoveId].firstBlock = newBlockPos;
    buffer bufs[blocks];

    std::ifstream inStream;
    inStream.open(name.c_str());
    inStream.seekg(oldBlockPos * BLOCK_SIZE);
    for (unsigned int i = 0; i < blocks; ++i){
        inStream.read(bufs[i], BLOCK_SIZE);
    }

    std::fstream outStream;
    outStream.open(name.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    outStream.seekg(newBlockPos * BLOCK_SIZE);
    for (unsigned int i = 0; i < blocks; ++i){
        outStream.write(bufs[i], BLOCK_SIZE);
    }

}

void VirtualFilesystem::display_filemap(){
    char tab[size];

    for(unsigned int i = 0; i < SYSTEM_BLOCKS; ++i){
        tab[i] = '+';
    }
    for(unsigned int i = SYSTEM_BLOCKS; i < size; ++i){
        tab[i] = '.';
    }
    for(unsigned int i = 0; i < inodes.size(); ++i){
        for(unsigned int j = inodes[i].firstBlock; j < inodes[i].lastBlock(); ++j){
            tab[j] = inodes[i].name[0];
        }
    }
    std::cout<< "Legend: " << std::endl
        << "  '.' = free block" << std::endl 
        << "  '+' = system block (reserved for inodes)" << std::endl
        << "  'l' = block occupied by a file (l - first letter of the filename)" << std::endl
        << "1 block = "<< BLOCK_SIZE << " bytes" << std::endl << std::endl;
      
    for(unsigned i = 0; i < size; ++i){
        std::cout << tab[i];

        if ((i + 1) % 32 == 0) std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool VirtualFilesystem::cmpINodes(INode a, INode b){
    return a.firstBlock < b.firstBlock;
}

void VirtualFilesystem::renameFile(std::string oldName, std::string newName){
    if (findINode(newName) != NULL){
        throw FilenameNotUnique();
    }

    INode * inode;
    if ((inode = findINode(oldName)) == NULL){
        throw FileNotExist();
    }

    strcpy(inode->name, newName.c_str());
}

void VirtualFilesystem::removeFile(std::string filename){
    int nodeId = findINodeId(filename);
    if (nodeId == -1){
        throw FileNotExist();
    }
    inodes.erase(inodes.begin() + nodeId);
}

void VirtualFilesystem::list(){
    using std::setw;

    std::cout << setw(10) << "[block]" 
            << setw(10) << "[block]"   
            << setw(10) << "[blocks]"   
            << setw(10) << "[bytes]" << std::endl;
    std::cout << setw(10) << "start" 
            << setw(10) << "end" 
            << setw(10) << "size" 
            << setw(10) << "size" 
            << setw(30) << "name" << std::endl;
    for (int i = 0; i < 70; ++i) std::cout << "-";
    std::cout << std::endl;
    for(unsigned i = 0; i < inodes.size(); ++i){
    std::cout << setw(10) << inodes[i].firstBlock
            << setw(10) << inodes[i].lastBlock() - 1
            << setw(10) << inodes[i].blocks
            << setw(10) << inodes[i].size
            << setw(30) << inodes[i].name << std::endl;
    }
}