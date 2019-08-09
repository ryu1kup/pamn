#ifndef __NSORTPROCESSOR_H__
#define __NSORTPROCESSOR_H__

#include <vector>
#include <string>

#include <TFile.h>
#include <TTree.h>

class NSortProcessor {
public:
    NSortProcessor() = delete;
    NSortProcessor(TString init, TString output);
    ~NSortProcessor();

    void Init();
    void Process();
    void Terminate();

    void ActivateBranchs();
    void SetFileList();

    void SetInputBranchs();

    void SetVerbose(int Verbose){this->Verbose = Verbose;};

    std::vector<TString> ifiles;
    int Verbose = 1;

private:
    // For an output file
    TString m_output;
    TFile* m_ofile;
    TTree* m_otree;

    // Output parameters
    Float_t m_fv;
    Int_t m_nhits;

    // For input files
    TString m_init;
    TFile* m_ifile;
    TTree* m_itree;

    constexpr static UInt_t nsmax = 3000;
    Int_t           ns;
    Float_t         X[nsmax];
    Float_t         Y[nsmax];
    Float_t         Z[nsmax];
    Float_t         NR[nsmax];
    Float_t         Ed[nsmax];
    Float_t         S2[nsmax];
    std::vector<Int_t>     *pmthitid = nullptr;

    TBranch        *b_ns;
    TBranch        *b_X;
    TBranch        *b_Y;
    TBranch        *b_Z;
    TBranch        *b_NR;
    TBranch        *b_Ed;
    TBranch        *b_S2;
    TBranch        *b_pmthitid;
};



#endif
