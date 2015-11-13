#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "Analysis/Ntuplizer/plugins/jhu_hadHelper.h"

#include <iostream>

using namespace HADDF;

// Class Def:
class jhuHadronic : public edm::EDFilter
{
	// open primary class def
public:
	explicit jhuHadronic(const edm::ParameterSet &);
	~jhuHadronic();
private:
	virtual void beginJob() ;
	virtual bool filter(edm::Event &, const edm::EventSetup &); // this is essentially the Ntuplizer code
	virtual void endJob() ;
	// member data
	edm::InputTag   jetSrc_;
	edm::InputTag   genSrc_;
	edm::InputTag   npvSrc_;
	std::string 	jetName_;
	std::string 	btagType_;
	edm::InputTag   rhoSrc_;          /// mean pt per unit area
	std::string 	useNsub_;
	std::string 	subcorr_;
	signed int  	basecorr_;
	double			scale_;
	double 			smear_;
	double 			angularSmear_;
	std::vector<std::string>  jecPayloads_; /// files for JEC payloads
	bool 			isData_;
	bool 			doB_;
	bool 			doT_;
	bool 			doSubcorr_;
	double 			mkSubSize_;
	boost::shared_ptr<FactorizedJetCorrector> jec_;
	boost::shared_ptr<JetCorrectionUncertainty> jecUnc_;
	bool 			addTopTag_;
	double 			jecValue_;
	double			jecUncPos_;
	double			jecUncNeg_;
	double 			corr_;
	double 			ptSmear_;
	double 			etaScale_;
	double 			phiScale_;
	double 			matchedJetEta_;
}; // close primary class def


jhuHadronic::jhuHadronic(const edm::ParameterSet &iConfig) :
	jetSrc_      (iConfig.getParameter<edm::InputTag>("jetSrc")),
	genSrc_      (iConfig.getParameter<edm::InputTag>("genSrc")),
	npvSrc_      (iConfig.getParameter<edm::InputTag>("npvSrc")),
	jetName_     (iConfig.getParameter<std::string>("jetName")),
	btagType_    (iConfig.getParameter<std::string>("btagType")),
	rhoSrc_      (iConfig.getParameter<edm::InputTag>("rhoSrc")),
	useNsub_     (iConfig.getParameter<std::string>("useNsub")),
	subcorr_     (iConfig.getParameter<std::string>("subcorr")),
	basecorr_    (iConfig.getParameter<signed int>("basecorr")),
	scale_       (iConfig.getParameter<double>( "jetScale" )),
	smear_       (iConfig.getParameter<double>( "jetPtSmear")),
	angularSmear_(iConfig.getParameter<double>( "jetAngularSmear")),
	jecPayloads_(iConfig.getParameter<std::vector<std::string> >  ("jecPayloads")),
	isData_      (iConfig.getParameter<bool>("isData")),
	mkSubSize_   (iConfig.getParameter<double>( "mkSubSize" )),
	addTopTag_   (iConfig.getParameter<bool>( "addTopTagInfo")),
	jecValue_(-1.0),
	jecUncPos_(-1.0),
	jecUncNeg_(-1.0),
	corr_(-1.0),
	ptSmear_(-1.0),
	etaScale_(-1.0),
	phiScale_(-1.0),
	matchedJetEta_(-1.0)
{
	doB_ = (btagType_ != "" and jetName_ != "AK5");
	doT_ = (useNsub_ == "yes" or useNsub_ == "y");
	doSubcorr_ = (subcorr_ == "yes" or subcorr_ == "y");

	std::vector<JetCorrectorParameters> vPar;
	for ( std::vector<std::string>::const_iterator ipayload = jecPayloads_.begin(), ipayloadEnd = jecPayloads_.end(); ipayload != ipayloadEnd - 1; ++ipayload )
	{
		std::cout << "Adding payload " << *ipayload << std::endl;
		JetCorrectorParameters pars(*ipayload);
		vPar.push_back(pars);
	}

	jec_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) );
	jecUnc_ = boost::shared_ptr<JetCorrectionUncertainty>( new JetCorrectionUncertainty(jecPayloads_.back()));

	produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_);
	produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub0");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub1");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub2");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub3");
	produces<std::vector<double> > (jetName_ + "csv");
	if (!isData_)
	{
		produces<std::vector<int> > (jetName_ + "PartonFlavour");
	}
	if (doT_)
	{
		produces<std::vector<double> > (jetName_ + "tau1");
		produces<std::vector<double> > (jetName_ + "tau2");
		produces<std::vector<double> > (jetName_ + "tau3");
		produces<std::vector<double> > (jetName_ + "tau4");
		if (mkSubSize_ > 0.0)
		{
			produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub0CUSTkt");
			produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub1CUSTkt");
			produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub2CUSTkt");
			produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub3CUSTkt");
			produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub0CUSTak");
			produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub1CUSTak");
			produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub2CUSTak");
			produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub3CUSTak");
			produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub0exkt");
			produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub1exkt");
			produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub2exkt");
			produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub3exkt");
		}
	}
	if (doB_)
	{
		produces<std::vector<double> > (jetName_ + "sub0csv");
		produces<std::vector<double> > (jetName_ + "sub1csv");
		produces<std::vector<double> > (jetName_ + "sub2csv");
		produces<std::vector<double> > (jetName_ + "sub3csv");
	}

	if (addTopTag_)
	{
		produces<std::vector<double> > (jetName_ + "topTagMinMass");
		produces<std::vector<double> > (jetName_ + "topTagTopMass");
	}

	produces<std::vector<unsigned int>> (jetName_ + "nsub");
	produces<std::vector<double>> (jetName_ + "JEC");
	produces<std::vector<double>> (jetName_ + "JECUncPos");
	produces<std::vector<double>> (jetName_ + "JECUncNeg");
	produces<std::vector<double>> (jetName_ + "JECcorr");
	produces<std::vector<double>> (jetName_ + "JECptSmear");
	produces<std::vector<double>> (jetName_ + "JECetaScale");
	produces<std::vector<double>> (jetName_ + "JECphiScale");
	produces<std::vector<double>> (jetName_ + "JECmatchedJetEta");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "CORR");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub0CORR");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub1CORR");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub2CORR");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > (jetName_ + "sub3CORR");

}

void jhuHadronic::beginJob()
{
	std::cout << "adding jets..... " << jetName_ << "\n";
}

void jhuHadronic::endJob()
{
}

bool jhuHadronic::filter(edm::Event &iEvent, const edm::EventSetup &iSetup)
{
	edm::Handle<std::vector<pat::Jet> > h_Jets;
	iEvent.getByLabel( jetSrc_, h_Jets);

	edm::Handle<std::vector<reco::GenJet> > h_genJets;
	if (!isData_)
	{
		iEvent.getByLabel( genSrc_, h_genJets);
	}

	edm::Handle< unsigned int > h_npv;
	edm::Handle<double> h_rho;
	iEvent.getByLabel( npvSrc_, h_npv );
	iEvent.getByLabel( rhoSrc_, h_rho );
	double rhoVal = *h_rho;
	unsigned int npv = *h_npv;

	std::auto_ptr<p4_vector> jets( new p4_vector() );
	std::auto_ptr<std::vector<double>> jetsCSV( new std::vector<double> );
	std::auto_ptr<std::vector<int>> PartFlav( new std::vector<int> );
	std::auto_ptr<std::vector<double>> jetstau1( new std::vector<double> );
	std::auto_ptr<std::vector<double>> jetstau2( new std::vector<double> );
	std::auto_ptr<std::vector<double>> jetstau3( new std::vector<double> );
	std::auto_ptr<std::vector<double>> jetstau4( new std::vector<double> );
	std::auto_ptr<std::vector<unsigned int>> nsub( new std::vector<unsigned int> );
	std::auto_ptr<std::vector<double>> jecvalue( new std::vector<double> );
	std::auto_ptr<std::vector<double>> jecuncpos( new std::vector<double> );
	std::auto_ptr<std::vector<double>> jecuncneg( new std::vector<double> );
	std::auto_ptr<std::vector<double>> corr( new std::vector<double> );
	std::auto_ptr<std::vector<double>> ptSmear( new std::vector<double> );
	std::auto_ptr<std::vector<double>> etaScale( new std::vector<double> );
	std::auto_ptr<std::vector<double>> phiScale( new std::vector<double> );
	std::auto_ptr<std::vector<double>> matchedJetEta( new std::vector<double> );
	std::auto_ptr<p4_vector> sub0(new p4_vector());
	std::auto_ptr<p4_vector> sub1(new p4_vector());
	std::auto_ptr<p4_vector> sub2(new p4_vector());
	std::auto_ptr<p4_vector> sub3(new p4_vector());
	std::auto_ptr<std::vector<double>> sub0csv(new std::vector<double>);
	std::auto_ptr<std::vector<double>> sub1csv(new std::vector<double>);
	std::auto_ptr<std::vector<double>> sub2csv(new std::vector<double>);
	std::auto_ptr<std::vector<double>> sub3csv(new std::vector<double>);
	std::auto_ptr<p4_vector> jets_CORR( new p4_vector() );
	std::auto_ptr<p4_vector> sub0_CORR(new p4_vector());
	std::auto_ptr<p4_vector> sub1_CORR(new p4_vector());
	std::auto_ptr<p4_vector> sub2_CORR(new p4_vector());
	std::auto_ptr<p4_vector> sub3_CORR(new p4_vector());
	std::auto_ptr<p4_vector> sub0_CUSTkt(new p4_vector());
	std::auto_ptr<p4_vector> sub1_CUSTkt(new p4_vector());
	std::auto_ptr<p4_vector> sub2_CUSTkt(new p4_vector());
	std::auto_ptr<p4_vector> sub3_CUSTkt(new p4_vector());
	std::auto_ptr<p4_vector> sub0_CUSTak(new p4_vector());
	std::auto_ptr<p4_vector> sub1_CUSTak(new p4_vector());
	std::auto_ptr<p4_vector> sub2_CUSTak(new p4_vector());
	std::auto_ptr<p4_vector> sub3_CUSTak(new p4_vector());
	std::auto_ptr<p4_vector> sub0_exkt(new p4_vector());
	std::auto_ptr<p4_vector> sub1_exkt(new p4_vector());
	std::auto_ptr<p4_vector> sub2_exkt(new p4_vector());
	std::auto_ptr<p4_vector> sub3_exkt(new p4_vector());
	std::auto_ptr<std::vector<double> > topTagMinMass ( new std::vector<double>() );
	std::auto_ptr<std::vector<double> > topTagTopMass ( new std::vector<double>() );

	// uncorrected jets and their properties:
	for ( std::vector<pat::Jet>::const_iterator jetBegin = h_Jets->begin(), jetEnd = h_Jets->end(), ijet = jetBegin; ijet != jetEnd; ++ijet )
	{
		// Fill b-discriminant based on string passed through config option
		jetsCSV->push_back(ijet->bDiscriminator(btagType_));
		// This allows an overall correction to be applied to all jest and subjets, for now it is always 1
		reco::Candidate::LorentzVector new_jet = ijet->correctedP4(basecorr_);
		reco::Candidate::PolarLorentzVector uncorr_jet (new_jet.pt(), new_jet.eta(), new_jet.phi(), new_jet.mass());
		jets->push_back(uncorr_jet);
		// Number of subjets for some collections, number of daughters for others
		unsigned int nsub_jet = ijet->numberOfDaughters();
		nsub->push_back(nsub_jet);
		// Fills the subjets b-discriminant
		if (doB_) PopulateSubjets(ijet, sub0, sub1, sub2, sub3, nsub_jet, btagType_, sub0csv, sub1csv, sub2csv, sub3csv);
		// Fills n-subjettiness
		if (doT_ and mkSubSize_ < 0.1) CalculateTaus(ijet, jetstau1, jetstau2, jetstau3, jetstau4);
		// Fills n-subjettiness for custom subjets
		if (doT_ and mkSubSize_ >= 0.1) CalculateTaus(ijet, jetstau1, jetstau2, jetstau3, jetstau4, mkSubSize_, sub0_CUSTkt, sub1_CUSTkt, sub2_CUSTkt, sub3_CUSTkt, sub0_CUSTak, sub1_CUSTak, sub2_CUSTak, sub3_CUSTak, sub0_exkt, sub1_exkt, sub2_exkt, sub3_exkt);
		// Applies JEC to all jets and subjets
		ApplyJec(ijet, jec_, jecUnc_, isData_, h_genJets, jets_CORR, sub0_CORR, sub1_CORR, sub2_CORR, sub3_CORR, nsub_jet, npv, rhoVal, scale_, smear_, angularSmear_, doSubcorr_, jecValue_, jecUncPos_, jecUncNeg_, corr_, ptSmear_, etaScale_, phiScale_, matchedJetEta_);
		// Save the JEC values from above to apply to the other CMS Top Tagging Variables
		jecvalue->push_back(jecValue_);
		jecuncpos->push_back(jecUncPos_);
		jecuncneg->push_back(jecUncNeg_);
		corr->push_back(corr_);
		ptSmear->push_back(ptSmear_);
		etaScale->push_back(etaScale_);
		phiScale->push_back(phiScale_);
		matchedJetEta->push_back(matchedJetEta_);
		// Fill the Parton Flavor in MC
		if (!isData_) PartFlav->push_back(ijet->partonFlavour());
		// Fill the Top Tagging Variables, with appropriate JEC
		if (addTopTag_) AddTopTagInfo(ijet, topTagMinMass, topTagTopMass, jecValue_);
	}
	// corrected jets and their properties:
	iEvent.put(jets, jetName_);
	iEvent.put(jetsCSV, jetName_ + "csv");
	iEvent.put(jecvalue, jetName_ + "JEC");
	iEvent.put(jecuncpos, jetName_ + "JECUncPos");
	iEvent.put(jecuncneg, jetName_ + "JECUncNeg");
	iEvent.put(corr, jetName_ + "JECcorr");
	iEvent.put(ptSmear, jetName_ + "JECptSmear");
	iEvent.put(etaScale, jetName_ + "JECetaScale");
	iEvent.put(phiScale, jetName_ + "JECphiScale");
	iEvent.put(matchedJetEta, jetName_ + "JECmatchedJetEta");
	if (doT_)
	{
		iEvent.put(jetstau1, jetName_ + "tau1");
		iEvent.put(jetstau2, jetName_ + "tau2");
		iEvent.put(jetstau3, jetName_ + "tau3");
		iEvent.put(jetstau4, jetName_ + "tau4");
		if (mkSubSize_ > 0.0)
		{
			iEvent.put(sub0_CUSTkt, jetName_ + "sub0CUSTkt");
			iEvent.put(sub1_CUSTkt, jetName_ + "sub1CUSTkt");
			iEvent.put(sub2_CUSTkt, jetName_ + "sub2CUSTkt");
			iEvent.put(sub3_CUSTkt, jetName_ + "sub3CUSTkt");
			iEvent.put(sub0_CUSTak, jetName_ + "sub0CUSTak");
			iEvent.put(sub1_CUSTak, jetName_ + "sub1CUSTak");
			iEvent.put(sub2_CUSTak, jetName_ + "sub2CUSTak");
			iEvent.put(sub3_CUSTak, jetName_ + "sub3CUSTak");
			iEvent.put(sub0_exkt, jetName_ + "sub0exkt");
			iEvent.put(sub1_exkt, jetName_ + "sub1exkt");
			iEvent.put(sub2_exkt, jetName_ + "sub2exkt");
			iEvent.put(sub3_exkt, jetName_ + "sub3exkt");
		}
	}
	iEvent.put(nsub, jetName_ + "nsub");
	if (doB_)
	{
		iEvent.put(sub0, jetName_ + "sub0");
		iEvent.put(sub1, jetName_ + "sub1");
		iEvent.put(sub2, jetName_ + "sub2");
		iEvent.put(sub3, jetName_ + "sub3");
		iEvent.put(sub0csv, jetName_ + "sub0csv");
		iEvent.put(sub1csv, jetName_ + "sub1csv");
		iEvent.put(sub2csv, jetName_ + "sub2csv");
		iEvent.put(sub3csv, jetName_ + "sub3csv");
	}
	iEvent.put(jets_CORR, jetName_ + "CORR");
	if (doSubcorr_)
	{
		iEvent.put(sub0_CORR, jetName_ + "sub0CORR");
		iEvent.put(sub1_CORR, jetName_ + "sub1CORR");
		iEvent.put(sub2_CORR, jetName_ + "sub2CORR");
		iEvent.put(sub3_CORR, jetName_ + "sub3CORR");
	}
	if (!isData_)
	{
		iEvent.put(PartFlav, jetName_ + "PartonFlavour");
	}
	if (addTopTag_)
	{
		iEvent.put(topTagMinMass, jetName_ + "topTagMinMass");
		iEvent.put(topTagTopMass, jetName_ + "topTagTopMass");
	}
	return true;
}

jhuHadronic::~jhuHadronic() {}

//define this as a plug-in
DEFINE_FWK_MODULE(jhuHadronic);
