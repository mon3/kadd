double function(double *x, double *params)
{
  if (x[0]>=0 && x[0]<=1 && x[1]<=1 && x[1]>=0)
  return params[0]*TMath::Power(x[0],params[1])*TMath::Power(x[1],params[2]);
  return 0;
}


zmienne()
{
  TCanvas *c1 = new TCanvas("c1","lab3",10,10,800,600);
  c1 -> Divide(2,2);
  c1 -> cd(1);
  double xmin = 0;
  double xmax = 1;
  double ymin = 0;
  double ymax = 1;
  int nparams = 3;
  int n = 2;
  int m = 1;
  TF2 *fun1 = new TF2("fun1",function, xmin, xmax, ymin, ymax,nparams);
  //ustawic dowolne c przed normalizacja
  fun1 -> FixParameter(0,1);
  // m i n beda sie zmieniac, dlatego SetParameter
  fun1 -> SetParameter(1,m);
  fun1 -> SetParameter(2,n);
  fun1 -> SetTitle("Gestosc");
  fun1 -> GetZaxis() -> SetTitle("f(x,y)");
  fun1 -> GetXaxis() -> SetTitle("x");
  fun1 -> GetYaxis() -> SetTitle("y");


  // normalizacja
 double c = 1./(fun1->Integral(0,1,0,1));
  fun1 -> SetParameter(0,c);
  cout << "c: "<< c << endl;
  fun1 -> Draw("surf1");

  c1 -> cd(2);
  //liczymy dystrybuante
  n = 1000;
  
  TH2D *hist = new TH2D("hist","dystrybuanta",10, 0, 1, 10, 0, 1);
  for(double x=0; x<=xmax; x+=0.1)
    {
    for(double y=0; y<=ymax; y+=0.1)
      {
        // x i y trzeba przeliczyc na numery binow, bo integral po x-ach z funkcji
	hist -> SetBinContent(x*10+1,y*10+1, fun1 -> Integral(0, x, 0, y));
	
     }
  
    }
  hist -> GetZaxis() -> SetTitle("F(x,y)");
  hist -> GetXaxis() -> SetTitle("x");
  hist -> GetYaxis() -> SetTitle("y");
  hist -> Draw("surf1");

   c1 -> cd(3);
   // histogram dobrze przybliża naszą funkcję
   TH2D *hist1 = (TH2D*) fun1 -> GetHistogram();
  
   TH1 *g = (TH1*) hist1-> ProjectionX();
   g -> SetTitle("Gestosc brzegowa g(x)");
   g->GetXaxis()->SetTitle("x");
   g->GetYaxis()->SetTitle("g(x)");
   g -> Draw();

   c1 -> cd(4);
   TH1 *h = (TH1*) hist1-> ProjectionY();
   h -> SetTitle("Gestosc brzegowa h(y)");
   h->GetXaxis()->SetTitle("y");
   h->GetYaxis()->SetTitle("h(y)");
   h -> Draw();


}
