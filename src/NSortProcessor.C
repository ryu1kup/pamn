// This own header
#include "NSortProcessor.h"

// ROOT headers
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TRandom3.h>

// C++ headers
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <iterator>
#include <algorithm>

NSortProcessor::NSortProcessor(TString init, TString output):
    m_output(output), m_init(init)
{
    m_ofile = new TFile(m_output, "recreate");
    m_otree = new TTree("events", "process and integrate nSort resultsh");
}

NSortProcessor::~NSortProcessor()
{
    delete m_otree;
    delete m_ofile;
}

void NSortProcessor::Init(){
    this->SetFileList();
    this->ActivateBranchs();
}


void NSortProcessor::ActivateBranchs(){

    // initialize the output tree
    // Copy branchs from the input files
    m_otree->Branch("ns", &ns);
    m_otree->Branch("NR", &NR[0]);
    m_otree->Branch("Ed", &Ed[0]);
    m_otree->Branch("secondS2", &S2[1]);
    m_otree->Branch("pmthitid", &pmthitid);

    // Calc variables in Process()
    m_otree->Branch("fv", &m_fv);
    m_otree->Branch("nhits", &m_nhits);

}


void NSortProcessor::SetFileList(){

    // read the init
    std::string ifilename;
    std::ifstream ifs(m_init);
    // std::vector<TString> ifiles;
    while (std::getline(ifs, ifilename)) {
        if (ifilename == "") {
            continue;
        } else if (ifilename.at(0) == '#') {
            continue;
        } else {
            ifiles.emplace_back(ifilename);
        }
    }

}


void NSortProcessor::SetInputBranchs(){

    m_itree->SetBranchAddress("eventid", &eventid, &b_eventid);
    m_itree->SetBranchAddress("xpri", &xpri, &b_xpri);
    m_itree->SetBranchAddress("ypri", &ypri, &b_ypri);
    m_itree->SetBranchAddress("zpri", &zpri, &b_zpri);
    m_itree->SetBranchAddress("epri", &epri, &b_epri);
    m_itree->SetBranchAddress("typepri", &typepri, &b_typepri);
    m_itree->SetBranchAddress("vpri", &vpri, &b_vpri);
    m_itree->SetBranchAddress("S1", &S1, &b_S1);
    m_itree->SetBranchAddress("ns", &ns, &b_ns);
    m_itree->SetBranchAddress("X", X, &b_X);
    m_itree->SetBranchAddress("Y", Y, &b_Y);
    m_itree->SetBranchAddress("Z", Z, &b_Z);
    m_itree->SetBranchAddress("NR", NR, &b_NR);
    m_itree->SetBranchAddress("Ed", Ed, &b_Ed);
    m_itree->SetBranchAddress("S2", S2, &b_S2);
    m_itree->SetBranchAddress("cS2", cS2, &b_cS2);
    m_itree->SetBranchAddress("Time", Time, &b_Time);
    m_itree->SetBranchAddress("TotNrgTpc", &TotNrgTpc, &b_TotNrgTpc);
    m_itree->SetBranchAddress("TotNrgBc", &TotNrgBc, &b_TotNrgBc);
    m_itree->SetBranchAddress("TotNrgVeto", &TotNrgVeto, &b_TotNrgVeto);
    m_itree->SetBranchAddress("nFax", &nFax, &b_nFax);
    m_itree->SetBranchAddress("Fax_x", Fax_x, &b_Fax_x);
    m_itree->SetBranchAddress("Fax_y", Fax_y, &b_Fax_y);
    m_itree->SetBranchAddress("Fax_z", Fax_z, &b_Fax_z);
    m_itree->SetBranchAddress("Fax_t", Fax_t, &b_Fax_t);
    m_itree->SetBranchAddress("Fax_ed", Fax_ed, &b_Fax_ed);
    m_itree->SetBranchAddress("Fax_nr", Fax_nr, &b_Fax_nr);
    m_itree->SetBranchAddress("Fax_nph", Fax_nph, &b_Fax_nph);
    m_itree->SetBranchAddress("Fax_nel", Fax_nel, &b_Fax_nel);
    m_itree->SetBranchAddress("GasInteraction", &GasInteraction, &b_GasInteraction);
    m_itree->SetBranchAddress("npmthit", &npmthit, &b_npmthit);
    m_itree->SetBranchAddress("pmthitid", &pmthitid, &b_pmthitid);
    m_itree->SetBranchAddress("pmthittime", &pmthittime, &b_pmthittime);
    m_itree->SetBranchAddress("pmthitenergy", &pmthitenergy, &b_pmthitenergy);

    m_itree->SetBranchStatus("*", 0);

}



void NSortProcessor::Process(){
    // loop for input files
    ULong64_t total_entries = 0;
    for (const TString & ifile : ifiles) {
        // display the current input file
        if(Verbose>1) std::cout << "-----> inputfile: " << ifile << std::endl;

        // initialize the input tree
        m_ifile = TFile::Open(ifile, "read");
        m_ifile->cd("events");
        m_itree = new TTree();
        m_itree = dynamic_cast<TTree*>(gDirectory->Get("events"));

        this->SetInputBranchs();

        // Activate used branchs
        m_itree->SetBranchStatus("ns");
        m_itree->SetBranchStatus("X");
        m_itree->SetBranchStatus("Y");
        m_itree->SetBranchStatus("Z");
        m_itree->SetBranchStatus("NR");
        m_itree->SetBranchStatus("Ed");
        m_itree->SetBranchStatus("S2");
        m_itree->SetBranchStatus("pmthitid");

        // loop for events
        const ULong64_t nentries = m_itree->GetEntries();
        total_entries += nentries;
        for (UInt_t ientry = 0; ientry < nentries; ++ientry) {
            if(Verbose>2 && ientry % (nentries/10) == 0) std::cout << "File read " << (double)ientry/nentries*100 << "%" << std::endl;
            m_itree->GetEntry(ientry);


            // ------------------------------------>>>
            // FV
            const Float_t r2 = X[0] * X[0] + Y[0] * Y[0];
            const Float_t z_fv = Z[0] + 739.0;
            m_fv = TMath::Power(TMath::Abs(z_fv/629.0), 3.0) + TMath::Power(TMath::Abs(r2/396900.0), 3.0);
            // <<<------------------------------------


            // ------------------------------------>>>
            // nhits
            constexpr UInt_t nNV = 121;
            std::array<Float_t, nNV> cnt {0};
            for (UInt_t id : *pmthitid) {
                if (id >= 20000) {
                    ++cnt[id - 20000];
                }
            }
            Float_t phe = 0.;
            Int_t nhits = 0;
            constexpr Float_t phe_threshold = 0.5;
            for (UInt_t  iNV = 0; iNV < nNV; ++iNV) {
                if (cnt[iNV] > 0) {
                    phe = gRandom->Gaus(cnt[iNV], TMath::Sqrt(cnt[iNV]) * 0.3);
                } else {
                    phe = 0.;
                }
                if (phe >= phe_threshold) {
                    ++nhits;
                }
            }
            m_nhits = nhits;
            // <<<------------------------------------


            // fill output tree
            m_otree->Fill();
        }

        // clean up
        delete m_itree;
        delete m_ifile;
    }

    // write the output tree into the output file
    if(Verbose>1) std::cout << "-----> " << total_entries << " events are written." << std::endl;

}



void NSortProcessor::Terminate(){
    m_ofile->cd();
    m_otree->Write();
    m_ofile->Close();

}
