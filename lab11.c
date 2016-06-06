	// Funkcja zwraca wartosc funkcji M
	// parametry:
	//  st - stopien dopasowanego wielomianu
	//  n - liczba pomiarow
	//  tj - tablica cosinusow kata rozproszenia
	//  yj - wyniki pomiarow
	//  sigmaj - bledy pomiarow
	//  wsp - tablica do ktorej nalezy wpisac wartosci wyznaczonych wspolczynnikow (Wzor10.png)
	//  bswp - tablica do ktorej nalezy wpisac bledy wyznaczonych wspolczynnikow (pierwiastki kwadratowe z elementów diagonalnych macierzy  Wzor11.png)
double dopasuj (int r, int n, double *tj, double *yj, double *sigmaj, double *wsp, double *bwsp)
{
		// mamy zaleznosc y(t)

		// tworzenie macierzy Y (1arg - liczba wierszy. 2arg - liczba kolumn)
	TMatrixD *Y = new TMatrixD(n,1);
	TMatrixD *A = new TMatrixD(n,r+1);
	TMatrixD *H = new TMatrixD(n,n);

		//wypelnianie macierzy A oraz Y

	for (int i=0; i<n; i++)
	{
		Y(i,0) = yj[i];

		for(int j=0; j<r+1; j++)
		{
			if (j == 0){
				A(i,j) = -1.0;
			}

			A(i,j) = -1.0*TMath::Power(tj[i],j);				

		}	
	}

		//Y -> Print();
		//A -> Print();

		// wypelnianie macierzy H
	for (int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			if( i == j)
				H(i,j) = 1/sigmaj[i];

		}	

	}

		//H -> Print();

		// tworzymy macierze Y' i A'

	TMatrixD *Yy = new TMatrixD(*H, TMatrix::kMult, *Y);
	TMatrixD *Aa = new TMatrixD(*H, TMatrix::kMult, *A);

		//Yy -> Print();
		//Aa -> Print();

		// macierz A' transponowana
	TMatrixD *Aat = new TMatrixD(TMatrix::kTransposed,*Aa);

		// macierz A' pomocnicza
	TMatrixD *A_pomoc = new TMatrixD(*Aat, TMatrix::kMult, *Aa);

		// macierz A' odwrotna
	A_pomoc -> Invert();

		//cout << "A_pomoc: " << endl;
		//A_pomoc -> Print();

	for (int i=0; i< r+1; i++)
	{
		for (int j=0; j<r+1; j++)
		{
			if (i == j){
				bwsp[i] = TMath::Sqrt(A_pomoc(i,j));
			}
		}

	}



		// macierz A' pomocnicza2 
	TMatrixD *A_pomoc2 = new TMatrixD(*A_pomoc, TMatrix::kMult, *Aat);

		// macierz X
	TMatrixD *X = new TMatrixD(*A_pomoc2, TMatrix::kMult, *Yy);

		//X -> Print();



		// macierz eta -macierz wspolczynnikow
	TMatrixD *Eta = new TMatrixD(*A, TMatrix::kMult, *X);

	// wypelniam tablice wspolczynnikow z dopasowania
	for (int i=0; i<r+1; i++){
		//cout << "X(i)" << i<< " " <<X(i,0) << endl;
		wsp[i] = -1.0*X(i,0);
		//cout<<  "wsp:X(i)" << i<< " " <<X(i,0) << endl;
	}

	//Eta -> Print();

	double M = 0.0;

	for (int j=0; j<n; j++)
	{
		//cout << Eta(j,0) << endl;
		M += TMath::Power((( yj[j] - Eta(j,0))/sigmaj[j]),2);
	}


	return M;

}

// h - histogram danych
// f - przewidywanie "teoretyczne"
double chi2(TGraphErrors* g, TF1 *f)
{
	double T = 0.0;
	double x = 0.0;
	double y = 0.0;
	int n = g -> GetN();
	for (int i=0; i<n; i++)
	{
		g -> GetPoint(i,x,y);
		T += TMath::Power((y - (f-> Eval(x))),2)/(f-> Eval(x));
		
	}
	
	return T;
}


//chcemy dystrybuante naszej wielkosci T

// chi - squared probability density PDF
double chi(double *x, double *params)
{
	// funkcja gestosci prawdopodobienstwa
  return 1./(TMath::Power(2,params[0]/2)*TMath::Gamma(params[0]/2))*TMath::Power(x[0], params[0]/2-1)*TMath::Exp(-x[0]/2);

}

// chi - squared distribution
double dyst(double *x, double *params)
{
  TF1 *f = new TF1("f",chi, 0.0, 30.0, 1); //(nazwa_funkcji, minx, maxx, nparams)
  f->SetParameter(0,params[0]); //tutaj ustawiamy nasze wartosci parametru k

  return f->Integral(0.00001, x[0]); // liczymy dystrybuante jako calke z f-cji gestosc dla kazdego z parametrow k
  // na wykresie bedziemy odpowiednio widziec k krzywych
  //taka calka daje nam nie jedna liczbe, tylko funkcje, wynik calki w jakiejs liczbie punktow, domyslnie 100
}
 



// true - brak podstaw do odrzucenia hipotezy
// false - sa podstawy do odrzucenia hipotezy
// Parametry:
// T - wartosc statystyki testowej chi2
// alpha - poziom istotnosci
// ndf - liczba stopni swobody rozkladu chi2
// n -liczba parametrow k
bool testChi2(double T, double alpha, int ndf)
{
	double min = 0.0;
	double max = 30.0;
	int nparams = 1;
	//int n = 8; // liczba parametrow k
   

   TF1 *fun1;	// tablica funkcji rozkladow xhi^2 dla 20 parametrow k
   TF1 *fun2;	// tablica funkcji dystrybuant xhi^2 dla 20 parametrow k

 
 
    fun2 = new TF1("fun2",dyst, min, max, nparams); //dystrybuanta - liczymy ja majac zdefiniowana funkcje chi2
	fun2->SetParameter(0,ndf);
   		
   		//szukamy wartosci kwantyla z dystrybuanty
     double y_value = 1-alpha;
     double x = fun2 -> GetX(y_value); // kwantyl bierzemy mając y dla policzonej dystrybuanty
     
     cout << "Kwantyl: " << x << endl;

     if(T > x) 
     {
		return false;
	}
	else 
	{
		return true;
	}


	
	
}



lab11()
// h - histogram danych
// f - przewidywanie "teoretyczne"

{


	TCanvas *c1 = new TCanvas("c1","lab11",10,10,800,600);
	c1 -> Divide(1,1);

	double x, y;
	ifstream ifile;
	ifile.open("dane.dat");

	TGraphErrors *data = new TGraphErrors("dane.dat" ,"%lg %lg");
	data -> Clear(); // zeby nie pokazywalo tytulu ani nazwy
	data ->GetXaxis() -> SetTitle("t, cos #theta");
	data ->GetYaxis() -> SetTitle("y, liczba obserwacji");
	int n = data -> GetN();

	
	double angles[10];
	double N[10];
	double sigma[10];
	int sum = 0;
	int N_sum =0;
	for (int i=0; i<n; i++)  
	{
		ifile>>x>>y;
		angles[i] = x;
		N[i] = y;
		sigma[i] = TMath::Sqrt(y);
		data -> SetPointError (i, 0, sigma[i]);
		sum += y*x;
		N_sum += y;

	}

	ifile.close();

	c1 -> cd(1);
		// dane pomiarowe
	data -> Draw("ac*");

	double wsp[6];
	double bwsp[6];

	TF1 *fit = new TF1("fit","[0]+x*[1]+TMath::Power(x,2)*[2]+TMath::Power(x,3)*[3]+TMath::Power(x,4)*[4]+TMath::Power(x,5)*[5]+TMath::Power(x,6)*[6]",-1,1);
	fit ->SetParameter(0,0.0);
	fit ->SetParameter(1,0.0);
	fit ->SetParameter(2,0.0);
	fit ->SetParameter(3,0.0);
	fit ->SetParameter(4,0.0);
	fit ->SetParameter(5,0.0);
	fit ->SetParameter(6,0.0);

	for (int k=0; k<6; k++){
		int ndf = n - 1 - k;
		cout<< "Dopasowanie wielomianem stopnia: " << k << endl;
		double result = dopasuj(k,10,angles,N,sigma,wsp,bwsp);
		cout<< "M = "<< result << endl;

		for (int i=0; i< k+1; i++)
		{
			cout<< "x" << i << "=" << wsp[i]<< "+/-" << bwsp[i] <<endl;
			fit -> SetParameter(i, wsp[i]) ;
		}

		fit -> SetLineColor(k%6+1);
		fit -> DrawClone("Same");

		cout<< "Liczba stopni swobody= " << ndf << endl;

		// tutaj robimy test chi2 dla kazdego z fitow
		//double chi = chi2(data, fit);
		//double t = (double) chi /((double) ndf); 
		double t = result /((double) ndf); 
	//	cout<< "chi2: "<< chi << endl;
		bool test = testChi2(result, 0.01, ndf);
	//	cout<< "chi2/NDF: "<< t << endl;
	//	cout<< "test= "<< test << endl;

		cout<<"Poziom istotnosci alpha: 0.01"<< endl;
		if (test == 0){
			cout<< "Stopien "<<k <<": odrzucamy\n" <<endl;
		}
		else{
				cout<< "Stopien "<<k <<": akceptujemy\n" <<endl;
		}

		
	}




}
