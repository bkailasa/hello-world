// system include files
#include <memory>
#include <iomanip>

#include <TLorentzVector.h>
#include <TVector3.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "Math/GenVector/VectorUtil.h"

//
// class declaration
//

class DarkSusyEx1 : public edm::EDAnalyzer {

  public:

    explicit DarkSusyEx1(const edm::ParameterSet&);
    ~DarkSusyEx1();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:

    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
    //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

    // ----------member data ---------------------------
     
    edm::EDGetTokenT<pat::MuonCollection> muonCollToken;
    edm::EDGetTokenT<pat::PackedGenParticleCollection> genCollToken;

    TH1D* h_genpt;
    TH1D* h_pt;
      
};

//
// constructors and destructor
//
DarkSusyEx1::DarkSusyEx1(const edm::ParameterSet& iConfig) {

  edm::InputTag theMuonLabel("slimmedMuons");
  muonCollToken = consumes<pat::MuonCollection>(theMuonLabel);
  
  
  edm::InputTag theGenMuonLabel("packedGenParticles");
  genCollToken = consumes<pat::PackedGenParticleCollection>(theGenMuonLabel);

  edm::Service<TFileService> fs;
  h_pt = fs->make<TH1D>("pt", "RECO pt", 100, 0.0, 200.0);
  h_genpt = fs->make<TH1D>("genpt", "GEN pt", 100, 0.0, 200.0);

}


DarkSusyEx1::~DarkSusyEx1() {
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void DarkSusyEx1::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;
  using namespace std;
  using namespace reco;
  using namespace pat;
  using namespace l1extra;

  //
  // RECO Muons
  //
  edm::Handle<vector<pat::Muon>> muons;
  iEvent.getByToken(muonCollToken, muons);
  cout << "Number of RECO muons: " << muons->size() << endl;

  for (auto it = muons->cbegin(); it != muons->cend(); ++it) {

    h_pt->SetFillColor(44);
	h_pt->Fill((*it).pt());
    
  }

  //
  // GEN Muons
  //  
  edm::Handle <pat::PackedGenParticleCollection> genColl;
  iEvent.getByToken(genCollToken, genColl);
  int n = 0;
  for (auto it = genColl->cbegin(); it != genColl->cend(); ++it) if ( abs((*it).pdgId()) == 13 && fabs((*it).eta()) < 2.4 && (*it).pt() > 1.5 ) n++;
  cout << "Number of GEN muons: " << n << endl;

  for (auto it = genColl->cbegin(); it != genColl->cend(); ++it) {

    const pat::PackedGenParticle& mcParticle = (*it);
        if ( abs(mcParticle.pdgId()) != 13 ) continue; // skip this particle if it isn't a muon
		h_genpt->SetFillColor(38);
        h_genpt->Fill(mcParticle.pt());
    
  }

}


// ------------ method called once each job just before starting event loop  ------------
void DarkSusyEx1::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void DarkSusyEx1::endJob() {
}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void DarkSusyEx1::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DarkSusyEx1);
