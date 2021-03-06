void Hists_after_adding()
{
   TFile * f[13];

  
   f[0] = new TFile("50 to 80.root", "READ");
   f[1] = new TFile("80 to 120.root", "READ");
   f[2] = new TFile("120 to 170.root" , "READ");
   f[3] = new TFile("170 to 300.root" , "READ");
   f[4] = new TFile("300 to 470.root" , "READ");
   f[5] = new TFile("470 to 600.root" , "READ");
   f[6] = new TFile("600 to 800.root" , "READ");
   f[7] = new TFile("800 to 1000.root" , "READ");
   f[8] = new TFile("1000 to 1400.root" , "READ");
   f[9] = new TFile("1400 to 1800.root" , "READ");
   f[10] = new TFile("1800 to 2400.root" , "READ");
   f[11] = new TFile("2400 to 3200.root" , "READ");
   f[12] = new TFile("3200 to inf.root" , "READ");

   std::string flavour[5] = {"b","c","l","bb","cc"};
   std::string jet[1]     = {"jet"};
   
   std::map<std::string, std::map<std::string, TH2F *> > h2_total;
   std::map<std::string, std::map<std::string, TH2F *> > h2_wp;
   std::map<std::string, std::map<std::string, TH2F *> > h2_eff;
   std::map<std::string, std::map<std::string, TH2F *> > h_eff;
   
   std::map<std::string, std::map<std::string, TH1D *> > h_pt_total[6];
   std::map<std::string, std::map<std::string, TH1D *> > h_pt_wp[6];
   std::map<std::string, std::map<std::string, TGraphAsymmErrors *> > g_pt_eff[6];
   std::map<std::string,  TMultiGraph *>  g[6];

   // READING HISTOGRAMS FROM FILES AND ADDING THEM   

   TFile *file = new TFile("final_extended_delR0.3.root","RECREATE");

   
      for ( int j = 0 ; j < 5 ; ++j ) // loop on flavours                                                           //   h2_total["b"]["jet"]   = (TH2F*) f[i] -> Get("h_bjet_pt_eta"  );
      {                                                                                                             //   h2_total["b"]["jet1"]  = (TH2F*) f[i] -> Get("h_bjet1_pt_eta" );
         for ( int k = 0; k < 1; ++k ) // loop on jet rank                                                          //   h2_total["b"]["jet2"]  = (TH2F*) f[i] -> Get("h_bjet2_pt_eta" );
         {                                                                                                          //   h2_total["c"]["jet"]   = (TH2F*) f[i] -> Get("h_cjet_pt_eta"  );
             for ( int i = 0; i < 13 ; ++i )  // loop on files
             {
		std::string type = flavour[j]+jet[k];                                                                   //   h2_total["c"]["jet1"]  = (TH2F*) f[i] -> Get("h_cjet1_pt_eta" );
		if ( i == 0 )                                                                                           //   h2_total["c"]["jet2"]  = (TH2F*) f[i] -> Get("h_cjet2_pt_eta" );
		{                                                                                                       //   h2_total["l"]["jet"]   = (TH2F*) f[i] -> Get("h_ljet_pt_eta"  );
		h2_total[flavour[j]][jet[k]] = (TH2F*)f[i]->Get(Form("h_%s_pt_eta",type.c_str()));                   //   h2_total["l"]["jet1"]  = (TH2F*) f[i] -> Get("h_ljet1_pt_eta" );
		h2_wp[flavour[j]][jet[k]] = (TH2F*)f[i]->Get(Form("h_%s_pt_eta_wp",type.c_str()));                //   h2_total["l"]["jet2"]  = (TH2F*) f[i] -> Get("h_ljet2_pt_eta" );
		}                                                                                                       //   h2_total["bb"]["jet"]  = (TH2F*) f[i] -> Get("h_bbjet_pt_eta" );
		else                                                                                                    //   h2_total["bb"]["jet1"] = (TH2F*) f[i] -> Get("h_bbjet1_pt_eta");
		{                                                                                                       //   h2_total["bb"]["jet2"] = (TH2F*) f[i] -> Get("h_bbjet2_pt_eta");
		h2_total[flavour[j]][jet[k]] -> Add((TH2F*)f[i]->Get(Form("h_%s_pt_eta",type.c_str())));             //   h2_total["cc"]["jet"]  = (TH2F*) f[i] -> Get("h_ccjet_pt_eta" );
		h2_wp[flavour[j]][jet[k]] -> Add((TH2F*)f[i]->Get(Form("h_%s_pt_eta_wp",type.c_str())));          //   h2_total["cc"]["jet1"] = (TH2F*) f[i] -> Get("h_ccjet1_pt_eta");
		}   
               }
             h2_total[flavour[j]][jet[k]] ->Write();
             h2_wp[flavour[j]][jet[k]] -> Write();                                                                                        //   h2_total["cc"]["jet2"] = (TH2F*) f[i] -> Get("h_ccjet2_pt_eta");
         }                                                                                                          // ------
      }                                                                                                             // And the same for h2_wp
   



   
      
   
   // 2D EFFICIENCIES AND PT PROJECTIONS AND EFFICIENCIES
   for ( int j = 0 ; j < 5 ; ++j ) // loop on flavours
   {
      for ( int k = 0; k < 1; ++k ) // loop on jet rank
      {
         // 2D efficiencies
         std::string type = flavour[j]+jet[k];
         h2_eff[flavour[j]][jet[k]]= (TH2F*) h2_wp[flavour[j]][jet[k]]-> Clone(Form("h_%s_eff_pt_eta",type.c_str()));
         h2_eff[flavour[j]][jet[k]] -> Divide(h2_total[flavour[j]][jet[k]]);
         // pt distribution projections in bins of eta ( 5 bins) and corresponding efficiencies
         h2_eff[flavour[j]][jet[k]] -> Write();
         int bin[2] = {0,-1}; // means project all eta bins; will be index 0
         for ( int l = 0; l < 6; ++l )  // eta bins
         {
            if ( l != 0 )
            {
               bin[0] = l;
               bin[1] = l;
            }
            h_pt_total[l][flavour[j]][jet[k]] = h2_total[flavour[j]][jet[k]]-> ProjectionX(Form("h_pt_total_%s_eta%i",type.c_str(),bin[0]),bin[0],bin[1],"e");
            h_pt_wp   [l][flavour[j]][jet[k]] = h2_wp   [flavour[j]] [jet[k]]-> ProjectionX(Form("h_pt_wp_%s_eta%i",   type.c_str(),bin[0]),bin[0],bin[1],"e");
            g_pt_eff  [l][flavour[j]] [jet[k]] = new TGraphAsymmErrors(h_pt_wp[l][flavour[j]][jet[k]],h_pt_total[l][flavour[j]][jet[k]],"cl=0.683 b(1,1) mode");
         }
      }
   }

    file->Close();
   
//     TFile *newFile = new TFile("all_files_hadron.root","RECREATE");
// 
//     for ( int j=0 ; j < 3 ; ++j)
//     {    
// 	for ( int l = 0; l < 6; ++l)
//         {       
//                 std::string type = flavour[j];
//                 g[l][flavour[j]] =  new TMultiGraph(Form("%s_jet_bin%i",type.c_str(),l),Form("%s_jet_bin%i",type.c_str(),l));
// 		g[l][flavour[j]] -> Add(g_pt_eff[l][flavour[j]]["jet1"]);
// 		g[l][flavour[j]] -> Add(g_pt_eff[l][flavour[j]]["jet2"]);
//                 g_pt_eff[l][flavour[j]]["jet2"]->SetLineColor(kRed);
// 
// 			
// 
//                 g[l][flavour[j]]-> Write();
//         }
//    }
//    
//  newFile->Close();

 return;
//   h_pt_total[0]["c"]["jet1"] -> Draw();
//   h_pt_wp[0]["c"]["jet1"] -> Draw("same");
   
}
