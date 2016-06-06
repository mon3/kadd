// f - tablica wskaznikow do obiektow TF1, rozklady skladowe
// k - wymiar tablicy f
// h - histogram do ktorego wrzucamy wynik splotu
// n - liczba losowan

void splot(TF1 **f, int k, TH1D *h, int n)
{

  for(int  i=0;i<n;i++){
    double sum = 0.0;

    // k - wskazuje na ilosc funkcji, ktore wchodza w splot
    for(int j=0;j<k;j++)
      {
		sum += f[j]->GetRandom();
      }

    // wypelniamy histogram dla N iteracji
    h->Fill(sum);
  }

}


// n - liczba prób (kul)
// l - liczba rzędów
// p - prawdopodobieństwo ruchu kuli w prawo (sukcesu)
// przybliza rozklad dwumianowy
void deskaGaltona(TH1D * h, int n, int l, double p){

  for(int j=0;j<n;j++)
    {
  int counter = 0;
  for (int i=0;i<l;i++)
    {
      double losowanie = gRandom->Uniform(0.0,1.0);
      if (losowanie > p)
	counter++;
      else counter--;

    }
  h->Fill(counter);

    }

}





sploty(){


  //TF1 - getRandom
   TCanvas *c1 = new TCanvas("c1","lab8",10,10,800,600);
   c1 -> Divide(2,2);

   double min = 0.0;
   double max = 2.0;
   
   // rozklad jednorodny y = 1/(b-a)
   // U(0.0, 2.0)
   // w jednorodnym(prostokat - wersja ciagla), kazda liczba wylosowana moze byc z jednakowym prawdopodobienstwem
   TF1 *fun1 = new TF1("fun1", "0.5+0*x", min,max);
   TF1 *fun2 = new TF1("fun2", "0.5+0*x", min,max);
   TF1 *fun3 = new TF1("fun3", "0.5+0*x", min,max);


   TF1 *f[3] ;
   f[0] = fun1;
   f[1] = fun2;
   f[2] = fun3;
   int n = 10000;
   TH1D *hist = new TH1D("hist","Splot 3 funkcji jedn.",50,min,3*max);
   TH1D *hist2 = new TH1D("hist2","Splot 2 funkcji jedn.",50,min,2*max);

   c1 -> cd(1);
   splot(f, 3, hist, n);

   hist -> Draw();

   c1 -> cd(2);
   splot(f, 2, hist2, n);

   hist2 -> Draw();
   
   c1 -> cd(3);
   // mi - srednia, sigma - odchylenie
   double mi1 = 1.0;
   double sigma1 = 0.2;
   TF1 *fun4 = new TF1("fun4", "1./TMath::Sqrt((2*TMath::Pi()*[1]*[1]))*TMath::Exp(-(x-[0])*(x-[0])/(2*[1]*[1]))",0.0,4.0);
    fun4 -> SetParameter(0,mi1);
    fun4 -> SetParameter(1,sigma1);




 double mi2 = 3.0;
 double sigma2 = 0.5;
 TF1 *fun5 = new TF1("fun5", "1./TMath::Sqrt((2*TMath::Pi()*[1]*[1]))*TMath::Exp(-(x-[0])*(x-[0])/(2*[1]*[1]))",1.0,6.0);
  fun5 -> SetParameter(0,mi2);
  fun5 -> SetParameter(1,sigma2);

  TH1D *hist3 = new TH1D("hist3","Splot 2 rozkl.norm.",50,0.0 ,8 .0);

  TF1 *g[2] ;
  g[0] =  fun4;
  g[1] = fun5;
 splot(g, 2, hist3, n);

   hist3 -> Draw();

   // wartosci oczekiwane
   cout<<" E(X) 1:"<<mi1<<endl;
   cout<<" E(X) 2:"<<mi2<<endl;

   // przy splocie wartosci oczekiwane sie dodaja
   cout<<" E(X) :"<<hist3->GetMean()<<endl;

   // wariancje VX = sigma^2
   // wariancje przy splocie rozkladow normalnych dodaja sie w kwadracie
   cout<<" VX 1:"<<sigma1*sigma1<<endl;
   cout<<" VX 2:"<<sigma2*sigma2<<endl;
   cout<<" VX :"<<hist2->GetRMS()<<endl;



   c1 -> cd(4);
   
   TH1D *hist4 = new TH1D("hist4","Deska Galtona",50,-10.0,10.0);
  deskaGaltona(hist4,1000,10,0.5);
  hist4 -> Draw();
  

}
