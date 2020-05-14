#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TEfficiency.h"

void Efficiency()
{

TFile *f = TFile::Open("histos1.root");
if(!f || f->IsZombie())
  {
    cout << "Failed to load" << endl;
    return;
  }

 TDirectory *f2 = (TDirectory*)f->Get("demo;1");
 f2->cd();
 TH1D *genpt_1 = (TH1D*)f2->Get("genpt");
 TH1D *recopt_1 = (TH1D*)f2->Get("pt");
 
 //genpt_1->Draw();
 //recopt_1->Draw();
 
   TEfficiency* pEff = 0;
  TFile* pFile = new TFile("Efficiency.root","recreate");
    if(TEfficiency::CheckConsistency(recopt_1,genpt_1))
      {
      pEff = new TEfficiency(recopt_1,genpt_1);
      pEff->SetDirectory(gDirectory);
      pFile->Write();
      }
 }
