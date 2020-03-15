#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <iterator>
#include <algorithm>
#include <dirent.h>

#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TRandom3.h>

#include "NSortProcessor.h"

NSortProcessor::NSortProcessor(TString output) :
        m_output(output) {
    m_ofile = new TFile(m_output, "recreate");
    m_otree = new TTree("events", "process and merge nSort results");


    // init QE table
    std::ifstream qe_file_table("./data/clip0116.txt");
    Float_t tmpwl = 0.;
    Float_t tmpqe = 0.;
    while (qe_file_table >> tmpwl >> tmpqe) {
        nv_pmt_wl.push_back(tmpwl);
        nv_pmt_qe.push_back(tmpqe);
    }
}

NSortProcessor::~NSortProcessor() {
    delete m_otree;
    delete m_ofile;
}

void NSortProcessor::Init() {

    if (!AlreadyMakeFileList) {
        std::cerr << "Input file list has not been created." << std::endl;
        exit(1);
    }

    // remove duplicated files
    std::sort(m_ifiles.begin(), m_ifiles.end());
    m_ifiles.erase(std::unique(m_ifiles.begin(), m_ifiles.end()), m_ifiles.end());

    if (Verbose > 1) {
        std::cout << "File List -------------------" << std::endl;
        for (auto ifile : m_ifiles) {
            std::cout << ifile << std::endl;
        }
        std::cout << "-----------------------------" << std::endl;
    }

    this->ActivateBranchs();
}

void NSortProcessor::ActivateBranchs() {
    // initialize the output tree
    // Copy branchs from the input files
    m_otree->Branch("ns", &ns);
    m_otree->Branch("NR", &NR[0]);
    m_otree->Branch("Ed", &Ed[0]);
    m_otree->Branch("secondS2", &S2[1]);
    m_otree->Branch("Time", &Time[0]);
    m_otree->Branch("pmthitid", &pmthitid);
    m_otree->Branch("pmthittime", &pmthittime);
    m_otree->Branch("pmthitenergy", &pmthitenergy);

    // Calc variables in Process()
    m_otree->Branch("isDangerous", &isDangerous, "isDangerous/O");
    m_otree->Branch("isLessStrict", &isLessStrict, "isLessStrict/O");
    m_otree->Branch("fv", &m_fv);
    m_otree->Branch("nhits", &m_nhits);
}

void NSortProcessor::AddFileList(const std::string &listname) {
    // read the init
    std::string ifilename;
    std::ifstream ifs(listname);
    // std::vector<TString> m_ifiles;
    while (std::getline(ifs, ifilename)) {
        if (ifilename == "") {
            continue;
        } else if (ifilename.at(0) == '#') {
            continue;
        } else {
            m_ifiles.emplace_back(ifilename);
        }
    }

    AlreadyMakeFileList = true;
}


void NSortProcessor::AddFile(const std::string &filename) {

    if (filename.find("*") != std::string::npos) {

        int placeWildCard = filename.find("*");
        std::string postname = filename.substr(1 + placeWildCard, filename.length());
        std::string filepath;
        int placeSlash = filename.rfind("/");
        if (filename.rfind("/") != std::string::npos) {
            filepath = filename.substr(0, filename.rfind("/") + 1);
        } else {
            filepath = "./";
        }

        std::string prename = filename.substr(placeSlash + 1, placeWildCard - placeSlash - 1);
        DIR *dir;
        struct dirent *dent;

        dir = opendir(filepath.c_str());
        if (dir == nullptr) {
            perror(filepath.c_str());
            exit(2);
        }
        while ((dent = readdir(dir)) != nullptr) {
            std::string ilist = dent->d_name;
            if (ilist.find(prename) != std::string::npos && ilist.find(postname) != std::string::npos) {
                m_ifiles.emplace_back(filepath + ilist);
                AlreadyMakeFileList = true;
            }
        }
        closedir(dir);


    } else {
        m_ifiles.emplace_back(filename);
        AlreadyMakeFileList = true;
    }


}

void NSortProcessor::SetInputBranchs() {
    m_itree->SetBranchAddress("ns", &ns, &b_ns);
    m_itree->SetBranchAddress("X", X, &b_X);
    m_itree->SetBranchAddress("Y", Y, &b_Y);
    m_itree->SetBranchAddress("Z", Z, &b_Z);
    m_itree->SetBranchAddress("NR", NR, &b_NR);
    m_itree->SetBranchAddress("Ed", Ed, &b_Ed);
    m_itree->SetBranchAddress("S2", S2, &b_S2);
    m_itree->SetBranchAddress("Time", Time, &b_Time);
    m_itree->SetBranchAddress("pmthitid", &pmthitid_nSort, &b_pmthitid);
    m_itree->SetBranchAddress("pmthittime", &pmthittime_nSort, &b_pmthittime);
    m_itree->SetBranchAddress("pmthitenergy", &pmthitenergy_nSort, &b_pmthitenergy);
}

void NSortProcessor::Process() {
    // loop for input files
    ULong64_t total_entries = 0;
    size_t NumOfTotalFiles = m_ifiles.size();
    size_t ReadFile = 0;

    for (const TString &ifile : m_ifiles) {
        // display the current input file
        if (Verbose > 1) std::cout << "Inputfile: " << ifile << std::endl;

        // initialize the input tree
        m_ifile = TFile::Open(ifile, "read");
        m_ifile->cd("events");
        m_itree = new TTree();
        m_itree = dynamic_cast<TTree *>(gDirectory->Get("events"));

        this->SetInputBranchs();

        TRandom3 theRandom;
        theRandom.SetSeed(0);
        bool ConsiderQE = true;

        // loop for events
        const ULong64_t nentries = m_itree->GetEntries();
        total_entries += nentries;
        for (UInt_t ientry = 0; ientry < nentries; ++ientry) {
            if (Verbose > 1) PrintVerboseEachEvent(ientry, nentries);

            m_itree->GetEntry(ientry);

            pmthitid->clear();
            pmthittime->clear();
            pmthitenergy->clear();

            // ------------------------------------>>>
            // Drop nV PMT HIT following QE


            if(ConsiderQE) {
                for (size_t i = 0; i < pmthitenergy_nSort->size(); ++i) {

                    const Float_t wavelength = 1239.84 / pmthitenergy_nSort->at(i);
                    Float_t QE = 0.;
                    const size_t Nlambda = nv_pmt_wl.size();
                    for (size_t ilambda = 0; ilambda < Nlambda - 1; ++ilambda) {
                        if (wavelength >= nv_pmt_wl.at(ilambda) && wavelength < nv_pmt_wl.at(ilambda + 1)) {
                            QE = 0.01 * ((nv_pmt_qe.at(ilambda + 1) - nv_pmt_qe.at(ilambda)) /
                                         (nv_pmt_wl.at(ilambda + 1) - nv_pmt_wl.at(ilambda)) *
                                         (wavelength - nv_pmt_wl.at(ilambda)) + nv_pmt_qe.at(ilambda));
                            break;
                        }
                    }

                    const Float_t prob = theRandom.Rndm();
                    if (prob < QE) {
                        pmthitid->push_back(pmthitid_nSort->at(i));
                        pmthittime->push_back(pmthittime_nSort->at(i));
                        pmthitenergy->push_back(pmthitenergy_nSort->at(i));
                    }
                }
            } else {

                for (size_t i = 0; i < pmthitenergy_nSort->size(); ++i) {
                    pmthitid->push_back(pmthitid_nSort->at(i));
                    pmthittime->push_back(pmthittime_nSort->at(i));
                    pmthitenergy->push_back(pmthitenergy_nSort->at(i));
                }
            }

            // <<<------------------------------------

            // ------------------------------------>>>
            // FV
            const Float_t r2 = X[0] * X[0] + Y[0] * Y[0];
            const Float_t z_fv = Z[0] + 739.0;
            m_fv = TMath::Power(TMath::Abs(z_fv / 629.0), 3.0) + TMath::Power(TMath::Abs(r2 / 396900.0), 3.0);
            // <<<------------------------------------

            // ------------------------------------>>>
            // nhits
            constexpr UInt_t nNV = 121;
            std::array<Float_t, nNV> cnt{0};
            for (UInt_t id : *pmthitid) {
                if (id >= 20000) {
                    ++cnt[id - 20000];
                }
            }
            Float_t phe = 0.;
            Int_t nhits = 0;
            constexpr Float_t phe_threshold = 0.5;
            for (UInt_t iNV = 0; iNV < nNV; ++iNV) {
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

            // ------------------------------------>>>
            // Add cut

            isLessStrict = false;
            if(ns>0 && NR[0]==1) isLessStrict = true;

            isDangerous = false;
            double FVcut = (pow(TMath::Abs((z_fv)/629.),3.0)+pow(TMath::Abs((r2)/396900.),3.0));
            if (ns==1){
                if ( (4<Ed[0] && Ed[0]<50) && FVcut<1. && NR[0]==1) isDangerous = true;
            } else if (ns>1){
                if ( (4<Ed[0] && Ed[0]<50) && FVcut<1. && NR[0]==1 && S2[1]<100.) isDangerous = true;
            }
            // <<<------------------------------------

            // fill output tree
            m_otree->Fill();
        }

        if (Verbose == 1) PrintVerboseEachFile(ReadFile, NumOfTotalFiles, ifile);

        // clean up
        delete m_itree;
        delete m_ifile;
        ++ReadFile;
    }

    // write the output tree into the output file
    if (Verbose > 0) std::cout << total_entries << " events are written." << std::endl;
}

void NSortProcessor::Terminate() {
    m_ofile->cd();
    m_otree->Write();
    m_ofile->Close();
}

void NSortProcessor::PrintVerboseEachEvent(UInt_t ientry, ULong64_t nentries) {

    if (graphicalVerbose == true) {
        double progress = (double) ientry / nentries;
        const int barlength = 50;
        int pos = progress * barlength;
        std::cout << "[";
        for (int i = 0; i < barlength; ++i) {
            if (i < pos) {
                std::cout << "-";
            } else if (i == pos) {
                std::cout << ">";
            } else {
                std::cout << " ";
            }
        }
        if (ientry == nentries - 1) {
            std::cout << "] 100%" << std::endl;
        } else {
            std::cout << "] " << int(progress * 100) << "%\r";
        }
        std::cout.flush();
    } else {
        if (ientry % (nentries / 10) == 0) std::cout << "File read " << 100.0 * ientry / nentries << "%" << std::endl;
    }

}

void NSortProcessor::PrintVerboseEachFile(Int_t ReadFile, Int_t NumOfTotalFiles, const TString &ifile) {

    if (graphicalVerbose == true) {
        double progress = (double) ReadFile / NumOfTotalFiles;
        const int barlength = 50;
        int pos = progress * barlength;
        std::cout << "[";
        for (int i = 0; i < barlength; ++i) {
            if (i < pos) {
                std::cout << "-";
            } else if (i == pos) {
                std::cout << ">";
            } else {
                std::cout << " ";
            }
        }
        if (ReadFile == NumOfTotalFiles - 1) {
            std::cout << "\r";
            std::cout << "[";
            for (int i = 0; i < barlength; ++i) {
                std::cout << "-";
            }
            std::cout << "] 100%                                                                " << std::endl;
        } else {
            std::cout << "] " << int(progress * 100) << "%  " << ifile << "\r";
        }
        std::cout.flush();
    } else {
        std::cout << "Read File: " << ifile << "(" << ReadFile+1 << "/" << NumOfTotalFiles << ")" << std::endl;
    }

}
