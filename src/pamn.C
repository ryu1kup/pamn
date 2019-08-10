// ------------------------------------ //
// pamn.C
// ------------------------------------ //

#include <iostream>
#include <unistd.h>
#include <string>

#include <TString.h>
#include <TFile.h>

#include "cmdline.h"
#include "NSortProcessor.h"


int main(int argc, char** argv){
    // parse arguments
    cmdline::parser pars;
    pars.add<std::string>("input_file", 'i', "File name of input file(s)", false, "../../data/output0001_Sort.root");
    pars.add<std::string>("input_list", 'l', "List file name of input files", false, "./input.txt");
    pars.add<std::string>("output_name", 'o', "Output root file name", true, "./output.root");
    pars.add<int>("verbose", 'v', "Verbose level", false, 1);

    pars.parse_check(argc, argv);

    const int Verbose= pars.get<int>("verbose");
    const TString InputListName = pars.get<std::string>("input_list");
    const TString OutputName = pars.get<std::string>("output_name");

    if (Verbose > 1) std::cout << "-----> Start processing and merging nSort results" << std::endl;
    
    // process nSort outputs
    NSortProcessor* processor = new NSortProcessor(OutputName);
    processor->SetVerbose(Verbose);

    if (pars.exist("input_file")) processor->AddFile(pars.get<std::string>("input_file"));
    if (pars.exist("input_list")) processor->AddFileList(pars.get<std::string>("input_list"));

    processor->Init();
    processor->Process();
    processor->Terminate();

    if (Verbose > 1) std::cout << "-----> done!" << std::endl;

    return 0;
}

