#include <iostream>

#include "virtual_filesystem.hpp"

typedef boost::program_options::variables_map boost_var_map;
typedef boost::program_options::options_description boost_opt_desc;

void prepVariableMap(
    boost_var_map & vm,
    boost_opt_desc & optDesc,
    int argc, char * argv[]
);
void displayHelp(
    boost_opt_desc & optDesc,
    std::string programName
);

int main(int argc, char * argv[]){
    boost_opt_desc optDesc ("Allowed arguments",1024,512); 
    boost_var_map varMap;

    try{
        prepVariableMap(varMap, optDesc, argc, argv);
    }
    catch (std::exception &e){
        std::cout << e.what() << std::endl;
        std::cout << "For help:\n\t" << argv[0] << " -h" << std::endl;
        return -1;
    }

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
            std::string filename = varMap["upload"].as<std::string>();
            virtualFilesystem.uploadFile(filename);
        }
        else if (varMap.count("map")){
            virtualFilesystem.display_filemap();
        }
    }


}

void prepVariableMap(
    boost_var_map & vm,
    boost_opt_desc & optDesc,
    int argc, char * argv[]
){
    optDesc.add_options() 
        ("file,f", boost::program_options::value<std::string>(), 
        "name of the virtual filesystem file (required for everything except help)")
        ("create,c", boost::program_options::value<unsigned int>()->required(),
        "create a new (empty) virtual drive with given size (in kB)")
        ("help,h", "prints help")
        ("upload,u", boost::program_options::value<std::string>()->required(),
        "copies file with given name to the filesystem")
        ("map,M", "displays current map of filesystem")
    ;
    store(boost::program_options::command_line_parser(argc, argv).options(optDesc).run(), vm);

}

void displayHelp(
    boost_opt_desc & optDesc,
    std::string programName
){
    std::cout << "Usage:\n\033[1m" << "  " << programName
    << " -f file operation \n\033[m\n" << optDesc << std::endl;
}