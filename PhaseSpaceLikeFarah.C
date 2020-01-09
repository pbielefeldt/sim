void PhaseSpaceLikeFarah(TString aOutParticles="proton,pi0") {

   bool proton(false); // Else: Pion

   if (!gROOT->GetClass("TGenPhaseSpace")) gSystem.Load("libPhysics");

   TObjArray *l_DataArr = aOutParticles.Tokenize(",");
   TObject *p_ObjStr;
   TIter liter(l_DataArr);

   while((p_ObjStr=liter.Next())!=0) {
	   TString l_Str = ((TObjString *) p_ObjStr)->GetString();
   }


   // Momentum, Energy units are Gev/C, GeV
   // proton, pi0
   Double_t masses[2] = {0.938, 0.1349766} ;

   TGenPhaseSpace event;
   TLorentzVector target(0.0, 0.0, 0.0, 0.938);
   
   c1 = new TCanvas("prot");
   c2 = new TCanvas("pion");
   gStyle->SetOptStat(0);
//   gStyle->SetPalette(53); // normal black body map
//   gStyle->SetPalette(56); // inv black body map
//   c1->SetLogz(1);

   // Spectrum is I = 1/E_gamma
   Double_t Ee = 3.2;
   TF1 *ef1 = new TF1("ef1", "1/x", 0.18*Ee, 0.98*Ee);
//   TF1 *ef1 = new TF1("ef1", "1", 0.3, 3.2);

   TH2F *h_prot = new TH2F("h_ThPprot","Proton Momentum vs. Reco Angle;P_{Proton} (MeV);#Theta (deg)", 350, 0., 3500., 90, 0.0, 90.);
   TH2F *h_pion = new TH2F("h_ThPpion","Pion Momentum vs. Reco Angle;P_{Pion} (MeV);#Theta (deg)", 350, 0., 3500., 185, 0.0, 185.);

   for (Int_t n=0; n<5E6; n++) {

      Double_t l_e=ef1->GetRandom();

      TLorentzVector beam(0.0, 0.0, l_e, l_e);
      TLorentzVector W = beam + target;

      event.SetDecay(W, 2, masses);
      event.Generate();

      TLorentzVector *pProton = event.GetDecay(0);
      TLorentzVector *pPi0_0  = event.GetDecay(1);
      
	  h_prot->Fill(pProton->P()*1000., pProton->Theta()*TMath::RadToDeg());
	  h_pion->Fill(pPi0_0->P()*1000., pPi0_0->Theta()*TMath::RadToDeg());
   }
   
      // Acceptance
   Double_t a1 = 29.05;
   Double_t a0 = 10.5;
   
   TLine *l1 = new TLine(0., a1, 3500., a1);
   l1->SetLineColor(kGreen);
   l1->SetLineWidth(4);
   
   TLine *l0 = new TLine(0., a0, 3500., a0);
   l0->SetLineColor(kRed);
   l0->SetLineWidth(2);
   
   Double_t a2 = 163.;
   
   TLine *l2 = new TLine(0., a2, 3500., a2);
   l2->SetLineColor(kGreen);
   l2->SetLineWidth(2);

   // Draw
   c1->cd();
   h_prot->Draw("colz");
   l0->Draw();
   l1->Draw();
   
   c2->cd();
   h_pion->Draw("colz");
   l0->Draw();
   l1->Draw();
   l2->Draw();
   

}
