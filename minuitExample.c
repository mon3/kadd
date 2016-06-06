/// http://root.cern.ch/root/html402/TMinuit.html
//
//  Przyklad uzycia klasy TMinuit do minimalizacji funkcji
// 
//  W tym przypadku funkcja fcn uzywana do fitowania to f. Chi^2
// Dane maja postac tablic, w tym przypadku wypelniamy je splotem rozkladu wykladniczego
// i rozkladu Gaussa.
//
//  Bardziej dokladne informacje o funkcjach i parametrach mozna uzyskac w interaktywnej
// sesji ROOTa:
// Root > TMinuit *minuit = new TMinuit(10);
// Root > minuit->mnhelp("*") - wyswietla liste mozliwych slow kluczowych
// Root > minuit->mnhelp("SET") - dokladna informacja (jako argument podajemy slowo kluczowe)
//
//  np. chcemy wyswietlic dostepne metody minimalizacji:
// Root > minuit->mnhelp("*") 
// ==>List of MINUIT Interactive commands:                                      
// CLEar     Reset all parameter names and values undefined                       
// CONtour   Make contour map of the user function                                
// EXIT      Exit from Interactive Minuit                                         
// FIX       Cause parameter(s) to remain constant                                
// HESse     Calculate the Hessian or error matrix.                               
// IMPROVE   Search for a new minimum around current minimum                      
// !====>MIGrad    Minimize by the method of Migrad
//                 Praktycznie najlepszy algorytm (zalecany).
// !====>MINImize  MIGRAD + SIMPLEX method if Migrad fails                            
// MINOs     Exact (non-linear) parameter error analysis                          
// MNContour Calculate one MINOS function contour                                 
// PARameter Define or redefine new parameters and values                         
// RELease   Make previously FIXed parameters variable again                      
// REStore   Release last parameter fixed                                         
// SAVe      Save current parameter values on a file                              
// SCAn      Scan the user function by varying parameters                         
// !=====>SEEk      Minimize by the method of Monte Carlo                                
// SET       Set various MINUIT constants or conditions                           
// SHOw      Show values of current constants or conditions                       
// !=====>SIMplex   Minimize by the method of Simplex
//                  Omowiony w "Analizie Danych" Brandt'a str. 409

Float_t z[100],x[100],errorz[100];

//______________________________________________________________________________
void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
   const Int_t nbins = 100;
   Int_t i;

   // Obliczenie Chi^2
   Double_t chisq = 0;
   Double_t delta;
   for (i=0;i<nbins; i++) {
     delta  = (z[i]-func(x[i],par))/errorz[i];
     chisq += delta*delta;
   }
   f = chisq;
}

//______________________________________________________________________________
Double_t func(float x, Double_t *par)
{
 Double_t value=( par[0]*TMath::Exp(-par[1]*x) + ( par[2]*TMath::Exp(-(par[3]-x)*(par[3]-x)/(.001+2*par[4]*par[4]) )) );
 return value;
}

void minuitExample(TString fit_method="MIGRAD")
// wybieramy algorytm minimalizacji,
// mozliwe: MIGRAD, MINIMIZE, SIMPLEX, SEEK
{

  
    TH1D * h1 = new TH1D("h1", "Rozklad", 100, 5, 100 );

    //*****TUTAJ SYMULUJEMY EKSPERYMENT*****
    // Generacja histogramu z danymi do fitowania
    for( Int_t i=0; i<10000; i++)
    {
	h1->Fill( 100* TMath::Exp( -(3.*gRandom->Rndm(1))));

	if (!(i%4)) h1->Fill( gRandom->Gaus(40,5) );
    }
    //*****SYMULACJA EKSPERYMENTU ZAKONCZONA*****

    h1->GetXaxis()->SetTitle("Wielkosc mierzona [jednostka]");
    h1->GetYaxis()->SetTitle("Liczba wejsc");
    h1->Draw("pe");

    //Przygotowanie tabel do fitowania
    for( Int_t i=0; i<100; i++)
    {
	x[i] = i;
	z[i] = h1->GetBinContent(i);
	errorz[i] = 0.01;
    }

    // Inicjalizacja TMinuit z maxymalna liczba 5 parametrow
    TMinuit *gMinuit = new TMinuit(5);
    gMinuit->SetFCN(fcn);

    Double_t arglist[10];
    Int_t ierflg = 0;

    arglist[0] = 1;
    gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);

    // Ustalamy wartosci startowe i kroku dla kolejnych parametrow
    double vstart[8] = {260, .02 , 300 , 40,  5};
    double step[8] =   {1,    0.01, 10,  0.5, 0.1};

    gMinuit->mnparm(0, "a1", vstart[0], step[0], 0,0,ierflg);
    gMinuit->mnparm(1, "a2", vstart[1], step[1], 0,0,ierflg);
    gMinuit->mnparm(2, "a3", vstart[2], step[2], 0,0,ierflg);
    gMinuit->mnparm(3, "a4", vstart[3], step[3], 0,0,ierflg);
    gMinuit->mnparm(4, "a5", vstart[4], step[4], 0,0,ierflg);

    // Teraz rozpoczynamy minimalizacje
    arglist[0] = 500;
    arglist[1] = 1.;
    gMinuit->mnexcm(fit_method, arglist ,2,ierflg);
    cout<<"*********SHO FCN***********"<<endl;
    //gMinuit->mncomd("SHO FCN",0);    
    gMinuit->mnexcm("SHO FCN", arglist ,2,ierflg);
    cout<<"*********SHO FCN***********"<<endl;

    // Rysowanie dofitowanej funkcji

    // Odczytujemy dopasowane parametry
    Double_t fp[8], err[8];
    for ( Int_t i=0; i<5; i++ )
    {
	 gMinuit->GetParameter(i, fp[i], err[i]);
    }


    // Definiujemy funkcje parametryczna o dopasowanych parametrach
    TF1 * f1 = new TF1("f1",  "[0]*TMath::Exp(-1*[1]*x)+([2]*TMath::Exp(-([3]-x)*([3]-x)/(2*[4])))", 0, 100);
    f1->FixParameter(0,fp[0]);
    f1->FixParameter(1,fp[1]);
    f1->FixParameter(2,fp[2]);
    f1->FixParameter(3,fp[3]);
    f1->FixParameter(4,fp[4]);

    f1->Draw("SAME");
}