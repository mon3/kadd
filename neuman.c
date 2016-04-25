double losujVonNeumann(TF1 *g, double min, double max) // funkcja zwraca jedną liczbę pseudolosową z funkcji g(y)
{
  double y, u;
  // funkcja stala u=1
 

  do
    {
      
      y = gRandom -> Uniform(min, max);
      u = gRandom -> Uniform(min, max);
    
     
    }
  while( u > g -> Eval(y));
  
  return y;
}


double losujVonNeumann1(TF1 *g,TF1 *s, double min, double max) 
{
 TF1 *S = new TF1("fun3", "-1.0*TMath::Sqrt(1-2*x)+1", 0.0,1.0);


  do
    {
     double x = gRandom -> Uniform(0, 0.5);
     double u = gRandom -> Uniform(min, max);
     double y = S ->Eval(x);
  
    }
  while(u > g->Eval(y)/s->Eval(y));
  
  return y;
}




double wydajnoscVonNeumann(TF1 *g, double min, double max, int n)
{

  int n_under_curve = 0;
  for (int i=0; i<n; i++)
    {
      
     double y = gRandom -> Uniform(min, max);
     double u = gRandom -> Uniform(min, max);
     
     if ( u < g->Eval(y))
       {
	 n_under_curve ++;

       }

    }
  double wyd = (double)n_under_curve/n;

  return wyd;


}

double wydajnoscVonNeumann1(TF1 *g,TF1 *s, double min, double max, int n)
{

// odwrotnosc dystrybuanty funkcji pomocniczej
 TF1 *S = new TF1("fun3", "-1.0*TMath::Sqrt(1-2*x)+1", 0.0,0.5); // limity na x 

  int n_under_curve = 0;
  for (int i=0; i<n; i++)
    {
      
     double x = gRandom -> Uniform(0, 0.5); //liczba losowa z przedizlu x dla funkcji pomocniczej
     double u = gRandom -> Uniform(min, max); // liczba losowa z rokladu jednorodnego
   double  y = S ->Eval(x); // wstawiamy x do wzoru na odwrotna dystrybuante, by otrzymac y_i z rozkladu s(y)s
     
     if ( u < g->Eval(y)/s->Eval(y))
       {
	 n_under_curve ++;

       }

    }
  double wyd = (double)n_under_curve/n;

  return wyd;


}



double calkaVonNeumann(TF1 *g, double min, double max, int n) 
{

  // wydajnosc Neumanna * dlugosc przedzialu
  return wydajnoscVonNeumann(g,min,max,n)*(max-min);


}

double calkaVonNeumannZPomoc(TF1 *g, TF1 *s, double min, double max, int n, double &wydajnosc)
{
  wydajnosc = wydajnoscVonNeumann1(g,s,min,max,n);
  
  return wydajnosc* s ->Integral(0,1); // byloby wydajnosc * pole prostokata
  // ale mamy wydajnosc  * pole pod krzywa, opisana f-cja pomocnicza
  
}




neuman()
{

  
  TCanvas *c1 = new TCanvas("c1","lab4",10,10,800,600);
  c1 -> Divide(2,2);


  int N = 10000;
  double min = 0.0;
  double max = 1.0;
  TGraph *g_acc = new TGraph();
  TGraph *g_not_acc = new TGraph();
  double x,y;
  int n_acc = 0;
  int n_not_acc = 0;

  
  TH1D *hist = new TH1D("hist","Rozklad",50,min,max);
  
  for (int i =0; i<N; i++)
    {
      x = gRandom -> Uniform(min, max);
      y = gRandom -> Uniform(min, max);
      
      if (x*x + y*y < 1.0)
	{
	  
	  g_acc ->SetPoint(n_acc,x,y);
	  n_acc ++;
	  hist -> Fill(x);

	}
      else
	{
	  g_not_acc ->SetPoint(n_not_acc,x,y);
	  n_not_acc ++;

	}
    }



  cout << "Estimated pi: " << double(4.0*n_acc/N) << endl;

 c1 -> cd(1);
 g_not_acc -> SetMarkerColor(kBlue);
 g_acc -> SetMarkerColor(kRed);
 g_acc -> GetXaxis() -> SetRangeUser(0,1);
 g_acc -> GetYaxis() -> SetRangeUser(0,1);
 g_acc -> Draw("ap"); // ap -axis, points, l line
 g_not_acc -> Draw("samep");


 c1->cd(2);
 hist -> Draw();



 c1 -> cd(2);
 TF1 *fun1 = new TF1("fun1", "TMath::Exp(-x*[0])", 0.0,1.0);
 fun1 -> FixParameter(0,5.0); // mamy  nasza funkcje


 cout<< losujVonNeumann(fun1, 0 ,1)<<endl;
 cout<<"Wydajnosc Neumann zwykly: "<< wydajnoscVonNeumann(fun1, 0 ,1,10000)<<endl;
 cout<< calkaVonNeumann(fun1, 0 ,1,10000)<<endl;

 c1 -> cd(3);
 TF1 *s= new TF1("s   ", "1-x", 0.0,1.0); //funkcja pomocnicza
 TH1D *hist2 = new TH1D("hist2","Rozklad dla Neumanna",50,min,max);
 TH1D *hist3 = new TH1D("hist3","Rozklad dla Neumanna poprawka",50,min,max);
 
 for (int i =0; i<N; i++)
    {
      hist2 -> Fill(losujVonNeumann(fun1,0,1));
      hist3 -> Fill(losujVonNeumann1(fun1,s,0,1));
    }

     
 hist2 -> Scale(1./1000);
 hist3 -> Scale(1./1000);
 hist2 -> SetLineColor(kRed);
 hist3 -> SetLineColor(kGreen);
 hist2 -> Draw();
 hist3 -> Draw("same");
 fun1 -> Draw("same");
 s -> Draw("same");

 
 
 
 double wydajnosc = 0.0;

 cout<< calkaVonNeumannZPomoc(fun1,s,min,max,10000,wydajnosc)<<endl;
 cout<< "Nowa wydajnosc: " <<wydajnosc<<endl;

}
