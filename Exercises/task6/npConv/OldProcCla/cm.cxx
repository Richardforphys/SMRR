
{

  gROOT->Reset();
  gROOT->SetStyle("Plain");

  TChain c("Gas_Tree","Gas_Tree");

  c.AddFile("neutr9M_run1.root");
  c.AddFile("neutr9M_run2.root");
  c.AddFile("neutr9M_run3.root");
  c.AddFile("neutr9M_run4.root");
  c.AddFile("neutr9M_run5.root");
  c.AddFile("neutr9M_run6.root");
  c.AddFile("neutr9M_run7.root");
  c.AddFile("neutr9M_run8.root");
  c.AddFile("neutr9M_run9.root");
  c.AddFile("neutr9M_run10.root");
  c.AddFile("neutr9M_run11.root");
  c.AddFile("neutr9M_run12.root");
  c.AddFile("neutr9M_run13.root");
  c.AddFile("neutr9M_run14.root");

  c.Process("n2.C++");

  c.Draw("Sum$(edep)>>h1(1000,0,1000)");
  h1->SetBinContent(1,0);

  TH1F* hp = (TH1F*) gROOT->FindObject("p");
  hp->SetLineColor(2);
  hp->SetBinContent(1,0);
  hp->Draw("same");

  
  TH1F* h40 = (TH1F*) gROOT->FindObject("Ar40");
  h40->SetLineColor(4);
  h40->SetBinContent(1,0);
  h40->Draw("same");

  TH1F* h12 = (TH1F*) gROOT->FindObject("C12");
  h12->SetLineColor(3);
  h12->SetBinContent(1,0);
  h12->Draw("same");

  TH1F* h16 = (TH1F*) gROOT->FindObject("O16");
  h16->SetLineColor(6);
  h16->SetBinContent(1,0);
  h16->Draw("same");

  

  TLegend lg(0.1.,0.4.,0.9.,0.1.,"Different Processes");
  lg.AddEntry(&h1,"Total","L");
  lg.AddEntry(hp,"Protons","L");
  lg.AddEntry(h40,"Ar40","L");
  lg.AddEntry(h12,"C12","L");
  lg.AddEntry(h16,"O16","L");
  

  lg.Draw("same");
  

  

}
  
