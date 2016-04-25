void skrypt()
{
  gStyle -> SetOptStat(000);
  //TF1 *fun1 = new TF1("fun1", "2*x" , 0, 10);
  TCanvas *c1 = new TCanvas("c1","lab01",10,10,800,600);//nazwa wskaznika,tytul okna,x,y- polozenie piksela
  //szerokosc x,wysokosc
  c1->Divide(2,2);//liczba kolumn,liczba wierszy

  c1->cd(1);
  TF1 *fun1 = new TF1("fun1", "TMath::Sin(x)", 0, 2*TMath::Pi());
  TF1 *fun3 = dynamic_cast<TF1*>fun1 -> Clone();
  fun1->SetTitle("sin");
  fun1->GetXaxis()->SetTitle("x");
  fun1->GetXaxis()->CenterTitle();
  fun1->GetYaxis()->SetTitle("sin(x)");
  fun1->GetYaxis()->CenterTitle();
  fun1 -> Draw();

  c1 -> cd(2);
  TF1 *fun2 = new TF1("fun2", "TMath::Cos(x)", 0, 2*TMath::Pi());
  fun2->SetLineColor(kGreen);
  fun2->SetTitle("cos");
  fun2->GetXaxis()->SetTitle("x");
  fun2->GetXaxis()->CenterTitle();
  fun2->GetYaxis()->SetTitle("cos(x)");
  fun2->GetYaxis()->CenterTitle();
  fun2 -> Draw();


  c1 -> cd(3);
  
  //dlatego mamy fun3, zeby zmienic tytul = musielismy zrobic dynamic_casts
  fun3 -> SetTitle("sin(x),cos(x)");
  fun3 -> Draw();
  fun2 -> Draw("same");

  c1 -> cd(4);
  //TH1D(nazwa, tytul, nbins, xlow, xhigh)
  TH1D *h1 = new TH1D("h1", "hist",  6, 0, 6);
  h1 -> SetBinContent(1, 2);
  h1 -> SetBinContent(2, 1);
  h1 -> SetBinContent(3, 5);
  h1 -> SetBinContent(4, 4);
  h1 -> SetBinContent(5, 10);
  h1 -> SetBinContent(6, 12);
  h1 -> Draw();
  
  TCanvas *c2 = new TCanvas("c2","lab01",100,100,800,600);
  TGraph *gr = new TGraph();
  gr -> SetMarkerSize(0.8);
  gr -> SetMarkerStyle(20);
  gr -> SetMarkerColor(kBlue);
  gr -> SetPoint(0,1,2);
  gr -> SetPoint(1,2,1);
  gr -> SetPoint(2,3,5);
  gr -> SetPoint(3,4,4);
  gr -> SetPoint(4,5,10);
  gr -> SetPoint(5,6,12);
  gr -> Draw("ALP"); //("ALP")- laczy liniami
  



}

