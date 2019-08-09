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
    void SetVerbose(int Verbose){this->Verbose = Verbose;};
    std::vector<TString> ifiles;
    int Verbose = 1;

private:
    // for input
    TString m_init;
    TFile* m_ifile;
    TTree* m_itree;
    Int_t ns;
    Float_t X[3000];
    Float_t Y[3000];
    Float_t Z[3000];
    Float_t NR[3000];
    Float_t Ed[3000];
    Float_t S2[3000];
    std::vector<Int_t>* pmthitid;

    // for output
    TString m_output;
    TFile* m_ofile;
    TTree* m_otree;
    Int_t m_pri;
    Int_t m_ns;
    Float_t m_fv;
    Float_t m_NR;
    Float_t m_Ed;
    Float_t m_secondS2;
    Int_t m_nhits;
};

#endif
