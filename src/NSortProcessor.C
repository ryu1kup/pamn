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
    m_init(init),
    m_output(output)
{
    m_ofile = new TFile(m_output, "recreate");
    m_otree = new TTree("events", "process and integrate nSort resultsh");
}

NSortProcessor::~NSortProcessor()
{
    delete m_otree;
    delete m_ofile;
}

void NSortProcessor::ProcessForNVeto(){
    // initialize the output tree
    m_otree->Branch("ns", &m_ns);
    m_otree->Branch("fv", &m_fv);
    m_otree->Branch("NR", &m_NR);
    m_otree->Branch("Ed", &m_Ed);
    m_otree->Branch("secondS2", &m_secondS2);
    m_otree->Branch("nhits", &m_nhits);

    // read the init
    std::string ifilename;
    std::ifstream ifs(m_init);
    std::vector<TString> ifiles;
    while (std::getline(ifs, ifilename)) {
        if (ifilename == "") {
            continue;
        } else if (ifilename.at(0) == '#') {
            continue;
        } else {
            ifiles.push_back(ifilename);
        }
    }

    // loop for input files
    ULong64_t total_entries = 0;
    for (TString ifile : ifiles) {
        // display the current input file
        std::cout << "-----> inputfile: " << ifile << std::endl;

        // initialize the input tree
        m_ifile = TFile::Open(ifile, "read");
        m_ifile->cd("events");
        m_itree = new TTree();
        m_itree = dynamic_cast<TTree*>(gDirectory->Get("events"));
        m_itree->SetBranchAddress("ns", &ns);
        m_itree->SetBranchAddress("X", X);
        m_itree->SetBranchAddress("Y", Y);
        m_itree->SetBranchAddress("Z", Z);
        m_itree->SetBranchAddress("NR", NR);
        m_itree->SetBranchAddress("Ed", Ed);
        m_itree->SetBranchAddress("S2", S2);
        m_itree->SetBranchAddress("pmthitid", &pmthitid);

        // loop for events
        const ULong64_t nentries = m_itree->GetEntries();
        total_entries += nentries;
        for (UInt_t ientry = 0; ientry < nentries; ++ientry) {
            m_itree->GetEntry(ientry);

            // nSort branches
            m_ns = ns;
            const Float_t r2 = X[0] * X[0] + Y[0] * Y[0];
            const Float_t z_fv = Z[0] + 739.0;
            m_fv = TMath::Power(TMath::Abs(z_fv/629.0), 3.0) + TMath::Power(TMath::Abs(r2/396900.0), 3.0);
            m_NR = NR[0];
            m_Ed = Ed[0];
            m_secondS2 = S2[1];

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

            // fill output tree
            m_otree->Fill();
        }

        // clean up
        delete m_itree;
        delete m_ifile;
    }

    // write the output tree into the output file
    std::cout << "-----> " << total_entries << " events are written." << std::endl;
    m_ofile->cd();
    m_otree->Write();
    m_ofile->Close();
}
