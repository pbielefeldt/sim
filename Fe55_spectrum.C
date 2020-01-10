/* A very basic simulation of the Fe55 spectrum in a detector.
 * 
 * (C) 2017 Philipp Bielefeldt
 * GPL v3+
*/

void Fe55(	Double_t DetResolution=0.10,  
			Double_t XMin=2., 
			Double_t XMax=8.,
			Bool_t Escape=true,
			Bool_t AreYouJonathan=false,
//			Bool_t Backgr=false,
			Int_t N=500000){

	Double_t Ka(5.8926); 		// Centre K alpha 1&2 (keV)
	Double_t Kb(6.49);			// Centre K beta 1&2 (keV)
	Double_t Ep(2.689+0.203);	// Escape peak position 

	// Relative intesity (percentage)
	Double_t Ia(0.162+0.082);	// Combined intensity of alpha lines
	Double_t Ib(0.0189+0.0096);	// Beta lines combined
	Double_t Ie(0.);
	if(Escape) Ie = 0.038;		// Relative intensity of escape peak
	Double_t In(0.);
//	if(Backgr) In = 0.15;		// Some value for the background
	
	stringstream title;
	title << "Fe55 Spectrum @ " << DetResolution*100 << " %";
	
	Int_t NBins(3000);
	Double_t keVN((Double_t)(XMax-XMin)/NBins);		// Energy (keV) per bin
	
//______________________________________________________________________________
	
	TF1 *a 		= new TF1("a", "gaus(0)", XMin, XMax);	// K-alpha
	TF1 *b 		= new TF1("b", "gaus(0)", XMin, XMax);	// K-beta
	TF1 *e 		= new TF1("e", "gaus(0)", XMin, XMax);	// Escape
//	TF1 *n 		= new TF1("n", "[0]*TMath::Exp(-[1]*x)", XMin, XMax);	// Noise	
	TRandom *r 	= new TRandom(42);
	TH1I *h 	= new TH1I("h", title.str().c_str(), NBins, XMin, XMax);
	
	// Intensities, normalised to N
	Double_t Itot(Ia + Ib + Ie + In);
	Ia = N*Ia/Itot;
	Ib = N*Ib/Itot;
	Ie = N*Ie/Itot;
//	In = N*In/Itot;
	
	Double_t R2PI(TMath::Sqrt(2*TMath::Pi()));	// sqrt{2*Pi}
	
	// Gauss curves
	a->SetParameter(0, Ia*keVN/(R2PI*Ka*DetResolution));	// normalised amp
	a->SetParameter(1, Ka);									// <x> (in keV)
	a->SetParameter(2, Ka*DetResolution);					// width (sigma)
	
	b->SetParameter(0, Ib*keVN/(R2PI*Kb*DetResolution));
	b->SetParameter(1, Kb);
	b->SetParameter(2, Kb*DetResolution);
	
	if(Escape){
		e->SetParameter(0, Ie*keVN/(R2PI*Ep*DetResolution));
		e->SetParameter(1, Ep);
		e->SetParameter(2, Ep*DetResolution);
	}
	
//	if(Backgr){
//		n->SetParameter(0, In*keVN);
//		n->SetParameter(1, 0.5);	
//	}
	
	TF1 *j = new TF1("j", "a+b+e", XMin, XMax);	// Sum of all lines
	
//______________________________________________________________________________

	h->GetXaxis()->SetTitle("Energy (keV)");
	h->GetYaxis()->SetTitle("N");
	
	h->FillRandom("a", Ia); 
	h->FillRandom("b", Ib); 
	if(Escape)
		h->FillRandom("e", Ie); 
//	if(Backgr)
//		h->FillRandom("n", In);
	
	TCanvas *c1 = new TCanvas("c1", "Fe55", (1920-1200)/2., (1080-800)/2.25, 1200, 800);
	
	gStyle->SetOptStat(00000);
	
	h->Draw("");
	a->Draw("same");
	b->Draw("same");
	if(Escape)
		e->Draw("same");
//	if(Backgr)
//		n->Draw("same");
	
	if(AreYouJonathan)
		j->SetLineColor(kOrange);
		j->Draw("same");
	
	c1->Update();
	delete r;

}
