#include <iostream>

#include "virtual_filesystem.hpp"

typedef boost::program_options::variables_map boost_var_map;
typedef boost::program_options::options_description boost_opt_desc;

void prepVariableMap(boost_var_map & vm, boost_opt_desc & optDesc,
                    int argc, char * argv[]);
void displayHelp(boost_opt_desc & optDesc, std::string programName);
int run(int argc, char * argv[]);

int main(int argc, char * argv[]){
    try{
        return run(argc, argv);
    }
    catch (std::exception &e){
        std::cout << e.what() << std::endl;
        std::cout << "For help:\n\t" << argv[0] << " -h" << std::endl;
        return -1;
    }

}

void prepVariableMap(boost_var_map & vm, boost_opt_desc & optDesc,
                    int argc, char * argv[]){
    using boost::program_options::value;

    optDesc.add_options() 
        ("file,f", value<std::string>(), 
        "name of the virtual filesystem file (required for everything except help)")
        ("create,c", value<unsigned int>()->required(),
        "create a new (empty) virtual drive with given size (in kB)")
        ("help,h", "prints help")
        ("upload,u", value<std::string>()->required(),
        "copies file with given name to the filesystem")
        ("download,d", value<std::string>()->required(),
        "copies file with given name from the filesystem")
        ("map,M", "displays current map of filesystem")
        ("name,n", value<std::string>()->required(),
        "specify the new name for the downloaded file")
        ("move,m", value<std::vector<std::string>>()->required()->multitoken(),
        "changes name of the file being on the virtual drive\n"
        "Usage:\n\t-m old_name new_name")
        ("remove,r", value<std::string>()->required(),
        "removes file from virtual drive")
        ("list,l", "lists contents of the virtual drive")
    ;
    store(boost::program_options::command_line_parser(argc, argv).options(optDesc).run(), vm);

}

void displayHelp(boost_opt_desc & optDesc, std::string programName){
    std::cout << "Usage:\n\033[1m" << "  " << programName
    << " -f file operation \n\033[m\n" << optDesc << std::endl;
}

int run(int argc, char * argv[]){ 
    boost_opt_desc optDesc ("Allowed arguments",1024,512); 
    boost_var_map varMap;

    prepVariableMap(varMap, optDesc, argc, argv);

    if (varMap.count("help") || !varMap.count("file")){
        displayHelp(optDesc, argv[0]);
        return 0;
    }

    std::string file = varMap["file"].as<std::string>();
    VirtualFilesystem virtualFilesystem(file);
    if (varMap.count("create")){
        printf("hello");
        unsigned int size = varMap["create"].as<unsigned int>();
        virtualFilesystem.create(size);
    }
    else{
        virtualFilesystem.open();

        if (varMap.count("upload")){
            std::string uploadAs = "";
            if (varMap.count("name")){ uploadAs = varMap["name"].as<std::string>(); }
            std::string sourceFilename = varMap["upload"].as<std::string>();
            virtualFilesystem.uploadFile(sourceFilename, uploadAs);
        }
        else if (varMap.count("download")){
            std::string downloadAs = "";
            if (varMap.count("name")){ downloadAs = varMap["name"].as<std::string>(); }
            std::string sourceFilename = varMap["download"].as<std::string>();
            virtualFilesystem.downloadFile(sourceFilename, downloadAs);
        }
        else if (varMap.count("move")){
            std::string oldName = varMap["move"].as<std::vector<std::string>>()[0];
            std::string newName = varMap["move"].as<std::vector<std::string>>()[1];
            virtualFilesystem.renameFile(oldName, newName);
        }
        else if (varMap.count("remove")){
            std::string filename = varMap["remove"].as<std::string>();
            virtualFilesystem.removeFile(filename);
        }
        else if (varMap.count("map")){
            virtualFilesystem.display_filemap();
        }
        else if (varMap.count("list")){
            virtualFilesystem.list();
        }
    }

    return 0;
}