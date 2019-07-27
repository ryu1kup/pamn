#include "NSortProcessor.h"

// ROOT headers
#include <TString.h>
#include <TFile.h>

// C++ headers
#include <iostream>
#include <unistd.h>

void GetArgs(Int_t argc, Char_t** argv);
TString init = "./init.txt";
TString output = "./output.root";

Int_t main(Int_t argc, Char_t** argv){
    // get options
    GetArgs(argc, argv);

    // analysis
    std::cout << "-----> Start processing and merging nSort results" << std::endl;
    NSortProcessor* processor = new NSortProcessor(init, output);
    processor->ProcessForNVeto();

    std::cout << "-----> done!" << std::endl;
}

void GetArgs(Int_t argc, Char_t** argv){
    while ((argc > 1) && (argv[1][0] == '-')) {
        switch (argv[1][1]) {
            case 'i':
                init = argv[2];
                ++argv;
                --argc;
                break;

            case 'o':
                output = argv[2];
                ++argv;
                --argc;
                break;

            default:
                std::cout << "Invalid option specified" << std::endl;
                ++argv;
                --argc;
                break;
        }
        ++argv;
        --argc;
    }
}
