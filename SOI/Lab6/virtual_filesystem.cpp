#include "virtual_filesystem.hpp"

VirtualFilesystem::VirtualFilesystem(std::string name){
    this->name = name;
}

VirtualFilesystem::~VirtualFilesystem(){
    close();
}

void VirtualFilesystem::create(unsigned int size){
    if (size <= SYSTEM_BLOCKS){
        throw std::runtime_error("Disc too small!");
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
  for(unsigned int i=0; i < SYSTEM_BLOCKS; i++)
  {
    for(unsigned int j=0; j < BLOCK_SIZE; j+=sizeof(INode), index++)
    {
      // ponizsza linijka ma prawo nie dzialac
      INode *ptri = reinterpret_cast<INode*>(bufs[i] + j);
      if(index < inodes.size())
      {
        *ptri = inodes[index];
      }
      else
      {
        // zerujemy pozostale deskryptory, na wszelki wypadek...
        for(unsigned int k=0; k < sizeof(INode); ++k)
        {
          *(bufs[i] + j + k) = 0;
        }
      }
    }
  }
  std::fstream outStream;
  outStream.open(name.c_str(), std::ios::in|std::ios::out|std::ios::binary);
  outStream.seekp(std::ios_base::beg);
  for(unsigned i=0; i < SYSTEM_BLOCKS; i++)
  {
    outStream.write(bufs[j], BLOCK_SIZE);
  }
  outStream.close();
}

void VirtualFilesystem::uploadFile(std::string filename){
    if (findINodeByName(filename) != NULL){
        throw std::runtime_error("File with given name already exists!");
    }

    unsigned int filesize = boost::filesystem::file_size(filename);
    unsigned int blocksNeeded = ((filesize - 1) / BLOCK_SIZE) + 1; // e.g. ((1024-1)/1024)+1 = 1, but ((1025-1)/1024)+1 = 2
    unsigned int blocksFree = size - blocksUsed;
    if (blocksNeeded > blocksFree){
        throw std::runtime_error("Not enough space!");
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
    outStream.open(name.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    outStream.seekg(newFilePos * BLOCK_SIZE);
    for (unsigned int i = 0; i < blocksNeeded; ++i){
        outStream.write(bufs[i], BLOCK_SIZE);
    }
    outStream.close();

    INode newINode;
    newINode.used = 1;
    newINode.begin = newFilePos;
    newINode.size = filesize;
    newINode.blocks = blocksNeeded;
    strcpy(newINode.name, filename.c_str());
    inodes.push_back(newINode);

    std::cout << inodes.size() << std::endl;
}

VirtualFilesystem::INode * VirtualFilesystem::findINodeByName(std::string name){
    for (unsigned int i = 0; i < inodes.size(); ++i){
        if (inodes[i].name == name){
            return &inodes[i];
        }
    }
    return NULL;
}

unsigned int VirtualFilesystem::alloc(unsigned int blocks){
    while(1){
        unsigned int allocatedNodes = inodes.size();

        // check if there is space after SYSTEM_BLOCKS
        if (allocatedNodes == 0 || inodes[0].begin - SYSTEM_BLOCKS >= blocks)
            return SYSTEM_BLOCKS;

        for (unsigned int i = 1; i < inodes.size(); ++i){
            unsigned int hole = inodes[i].begin - inodes[i - 1].end();
            if (hole >= blocks){
                return inodes[i - 1].end();
            }
        }

        //check if there is space at the end
        if (allocatedNodes != 0 && size - inodes[allocatedNodes - 1].end() >= blocks)
            return inodes[allocatedNodes - 1].end();

        //if no space was found during iteration defragmentQ
        defragment();
    }
}

void VirtualFilesystem::defragment(){
    unsigned int blockToMoveId = 0;
    unsigned int newBlockPos = SYSTEM_BLOCKS;

    // if first block is perfectly aligned find next that isn't
    if (inodes[0].begin == SYSTEM_BLOCKS){
        for (blockToMoveId = 1; blockToMoveId < inodes.size(); ++blockToMoveId){
            if (inodes[blockToMoveId - 1].end() < inodes[blockToMoveId - 1].begin){
                newBlockPos = inodes[blockToMoveId - 1].end();
                break;
            }
        }
    }

    unsigned int blocks = inodes[blockToMoveId].blocks;
    unsigned int oldBlockPos = inodes[blockToMoveId].begin;

    inodes[blockToMoveId].begin = newBlockPos;
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

  for(unsigned int i = 0; i < SYSTEM_BLOCKS; ++i)
  {
    tab[i] = '+';
  }
  for(unsigned int i = SYSTEM_BLOCKS; i < size; ++i)
  {
    tab[i] = '.';
  }
std::cout << inodes.size() << std::endl;
  for(unsigned int i = 0; i < inodes.size(); ++i)
  {
    for(unsigned int j = inodes[i].begin; j < inodes[i].end(); ++j)
    {
      tab[j] = inodes[i].name[0];
    }
  }
  std::cout<< "Legend: " << std::endl
      << "  '.' = free block" << std::endl 
      << "  '+' = system block (reserved for inodes)" << std::endl
      << "  'l' = block occupied by a file (l - first letter of the filename)" << std::endl
      << "1 block = "<< BLOCK_SIZE << " bytes" << std::endl << std::endl;  
      
  for(unsigned i = 0; i < size; ++i)
  {
    std::cout << tab[i];
    if(!((i+1)&31))
    {
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}

int VirtualFilesystem::cmpINodes(INode a, INode b){
    return a.begin < b.begin;
}