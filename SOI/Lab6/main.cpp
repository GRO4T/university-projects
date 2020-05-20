#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

void prepVariableMap(
    boost::program_options::variables_map & vm,
    boost::program_options::options_description & optDesc,
    int argc, char * argv[]
);
void displayHelp(
    boost::program_options::options_description & optDesc,
    std::string programName
);

int main(int argc, char * argv[]){
    boost::program_options::options_description optDesc ("Allowed arguments",1024,512); 
    boost::program_options::variables_map vm;

    try{
        prepVariableMap(vm, optDesc, argc, argv);
    }
    catch (std::exception &e){
        std::cout << e.what() << std::endl;
        std::cout << "For help:\n\t" << argv[0] << " -h" << std::endl;
        return -1;
    }

    if (vm.count("help") || !vm.count("file"))
        displayHelp(optDesc, argv[0]);
}

void prepVariableMap(
    boost::program_options::variables_map & vm,
    boost::program_options::options_description & optDesc,
    int argc, char * argv[]
){
    optDesc.add_options() 
        ("file,f", boost::program_options::value<std::string>(), "nazwa pliku z wirtualnym dyskiem (wymagana dla wszystkich opcji oprocz -h)")
    ;
    store(boost::program_options::command_line_parser(argc, argv).options(optDesc).run(), vm);

}

void displayHelp(
    boost::program_options::options_description & optDesc,
    std::string programName
){
    std::cout << "Usage:\n\033[1m" << "  " << programName
    << " -f file operation \n\033[m\n" << optDesc << std::endl;
}