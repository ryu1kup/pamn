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

    Int_t           eventid;
    Float_t         xpri;
    Float_t         ypri;
    Float_t         zpri;
    Float_t         epri;
    std::vector<std::string>  *typepri = nullptr;
    std::string          *vpri = nullptr;
    Float_t         S1;
    Int_t           ns;
    Float_t         X[40];   //[ns]
    Float_t         Y[40];   //[ns]
    Float_t         Z[40];   //[ns]
    Float_t         NR[40];   //[ns]
    Float_t         Ed[40];   //[ns]
    Float_t         S2[40];   //[ns]
    Float_t         cS2[40];   //[ns]
    Float_t         Time[40];   //[ns]
    Float_t         TotNrgTpc;
    Float_t         TotNrgBc;
    Float_t         TotNrgVeto;
    Int_t           nFax;
    Float_t         Fax_x[271];   //[nFax]
    Float_t         Fax_y[271];   //[nFax]
    Float_t         Fax_z[271];   //[nFax]
    Float_t         Fax_t[271];   //[nFax]
    Float_t         Fax_ed[271];   //[nFax]
    Float_t         Fax_nr[271];   //[nFax]
    Int_t           Fax_nph[271];   //[nFax]
    Int_t           Fax_nel[271];   //[nFax]
    Int_t           GasInteraction;
    Int_t           npmthit;
    std::vector<int>     *pmthitid = nullptr;
    std::vector<double>  *pmthittime = nullptr;
    std::vector<float>   *pmthitenergy = nullptr;

    TBranch        *b_eventid;   //!
    TBranch        *b_xpri;   //!
    TBranch        *b_ypri;   //!
    TBranch        *b_zpri;   //!
    TBranch        *b_epri;   //!
    TBranch        *b_typepri;   //!
    TBranch        *b_vpri;   //!
    TBranch        *b_S1;   //!
    TBranch        *b_ns;   //!
    TBranch        *b_X;   //!
    TBranch        *b_Y;   //!
    TBranch        *b_Z;   //!
    TBranch        *b_NR;   //!
    TBranch        *b_Ed;   //!
    TBranch        *b_S2;   //!
    TBranch        *b_cS2;   //!
    TBranch        *b_Time;   //!
    TBranch        *b_TotNrgTpc;   //!
    TBranch        *b_TotNrgBc;   //!
    TBranch        *b_TotNrgVeto;   //!
    TBranch        *b_nFax;   //!
    TBranch        *b_Fax_x;   //!
    TBranch        *b_Fax_y;   //!
    TBranch        *b_Fax_z;   //!
    TBranch        *b_Fax_t;   //!
    TBranch        *b_Fax_ed;   //!
    TBranch        *b_Fax_nr;   //!
    TBranch        *b_Fax_nph;   //!
    TBranch        *b_Fax_nel;   //!
    TBranch        *b_GasInteraction;   //!
    TBranch        *b_npmthit;   //!
    TBranch        *b_pmthitid;   //!
    TBranch        *b_pmthittime;   //!
    TBranch        *b_pmthitenergy;   //!

};



#endif
