double function(double *x, double *params)
{
  if (x[0]*x[1]>=0 && x[0]*x[1]<=TMath::Pi())
    return params[0]*TMath::Sin(x[0]*x[1]);
  return 0;
}


lab4()
{
  TCanvas *c1 = new TCanvas("c1","lab4",10,10,800,600);
  c1 -> Divide(3,3);
  c1 -> cd(1);
  double xmin = 0;
  double xmax = TMath::Pi()/2;
  double ymin = 0;
  double ymax = TMath::Pi()/2;
  int nparams = 1; //jak podajemy liczbe parametrow, pozniej musimy ustawic "FixParameter"

  TF2 *fun1 = new TF2("fun1",function, xmin, xmax, ymin, ymax,nparams);
  fun1 -> FixParameter(0,1); //mamy jedyny parametr -> na razie ustawiamy domyslny
 
  fun1 -> SetTitle("Gestosc");
  fun1 -> GetZaxis() -> SetTitle("f(x,y)");
  fun1 -> GetXaxis() -> SetTitle("x");
  fun1 -> GetYaxis() -> SetTitle("y");
  double c = 1./(fun1->Integral(0,xmax,0,ymax));
  fun1 -> SetParameter(0,c);
  cout << "c: "<< c << endl;
  // fun1 -> Draw("surf1");

   TH2D *hist = new TH2D("hist","Histogram:gestosc",30, 0, xmax, 30, 0, ymax);
   double x_rand, y_rand;
   for(int i=0; i<=10000; i++)
    {
     
     // 
      fun1 -> GetRandom2(x_rand,y_rand);
      hist -> Fill(x_rand,y_rand);
	
  
    }
  hist -> GetZaxis() -> SetTitle("f(x,y)");
  hist -> GetXaxis() -> SetTitle("x");
  hist -> GetYaxis() -> SetTitle("y");
  hist -> Draw("lego1");
  //  fun1 -> Draw("lego1"); - przybliza ta funkcje dla duzej probki danych


  c1 -> cd(2);
  
  // unormowany histogram
  double hist_sum = hist->Integral(1,30,1,30,"width");
  TH2D *hist2 = dynamic_cast<TH2D*>hist -> Clone();
  hist2 -> SetTitle("Histogram:gestosc unormowane");
  hist2 -> Scale(1./hist_sum);
  //normalizujemy po to, zeby pole pod histogramem bylo 1, bo wtedy mamy prawdopodobienstwp
 // cout<< "Ciekawe jaka suma: " << hist2 -> Integral(1,30,1,30);
  hist2 -> Draw("lego1");


  c1 -> cd(3);
  TH2D *dystryb = new TH2D("dystryb","dystrybuanta",30, 0, xmax, 30, 0, ymax);
  // dla dystrybuanty sumujemy po binach histogramu
  // dla 1D bralismy calke z funkcji, dlatego inne przeliczenia granic
  // tutaj - calka z histogramu
  // hist2 - histogram, ktory juz ma 30 binow i jest unormowany
  for(int x=1; x<=31; x+=1)
    {
      for(int y=1; y<=31; y+=1)
      {
	dystryb -> SetBinContent(x,y, hist2 -> Integral(0, x, 0, y));
	
     }
  
    }
  dystryb -> GetZaxis() -> SetTitle("F(x,y)");
  dystryb -> GetXaxis() -> SetTitle("x");
  dystryb -> GetYaxis() -> SetTitle("y");
  dystryb -> Draw("lego1");

  c1->cd (4);
   TH1 *gx = (TH1*) hist2-> ProjectionX();
   gx -> SetTitle("Gestosc brzegowa g(x)");
   gx->GetXaxis()->SetTitle("x");
   gx->GetYaxis()->SetTitle("g(x)");
   gx -> SetFillColor(3);
   gx -> Draw();


   c1->cd (5);
   TH1 *gy = (TH1*) hist2-> ProjectionY();
   gy -> SetTitle("Gestosc brzegowa h(y)");
   gy ->GetXaxis()->SetTitle("y");
   gy ->GetYaxis()->SetTitle("h(y)");
   gy -> SetFillColor(3);
   gy -> Draw();

   cout << "E(X): "<< hist2 ->GetMean(1)<<endl;
    cout << "E(Y): "<< hist2 ->GetMean(2)<<endl;
    cout << "sigma(X): "<< hist2 ->GetRMS(1)<<endl;
    cout << "sigma(Y): "<< hist2 ->GetRMS(2)<<endl;
    cout << "cov(x,y): "<< hist2 ->GetCovariance(1,2)<<endl;
     cout << "cor(x,y): "<< hist2 ->GetCorrelationFactor(1,2)<<endl;
}
