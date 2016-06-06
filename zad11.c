// Funkcja zwraca wartosc funkcji M
// parametry:
//  st - stopien dopasowanego wielomianu
//  n - liczba pomiarow
//  tj - tablica cosinusow kata rozproszenia
//  yj - wyniki pomiarow
//  sigmaj - bledy pomiarow
//  wsp - tablica do ktorej nalezy wpisac wartosci wyznaczonych wspolczynnikow (Wzor10.png)
//  bswp - tablica do ktorej nalezy wpisac bledy wyznaczonych wspolczynnikow (pierwiastki kwadratowe z elementów diagonalnych macierzy  Wzor11.png)
double dopasuj (int st, int n, double *tj, double *yj, double *sigmaj){


		  //tworzenie macierzy
  TMatrixD *Y= new TMatrixD(n,1);
  TMatrixD *A= new TMatrixD(n,st);
  TMatrixD *H= new TMatrixD(n,n);
  
    
 //wczytywanie do macierzy A i Y
  
  for(int i=0; i<n;i++)
  {
    
    Y(i,0) = yj[i];
      
      for(int j=0;j<st;j++){
      	if (j == 0){
      		A(i,j) = 1;
      	}
	A(i,j) = TMath::Power(tj[i],j);
       }
    

  }


   	Y -> Print();
    A -> Print();
    

    //wczytanie macierzy H
 	// errors= sigma array
	 for(int i=0; i<n;i++)
 	{
  		 for(int j=0;j<n; j++){
   			
   
     		if(i==j) 
     			H(i,j)=1/sigmaj[i];
     		
     	}
  }
   
   H -> Print();

  TMatrixD *C = Y;

 //Macierze Y'=H*Y
 TMatrixD *Cc = new TMatrixD(*H,TMatrix::kMult,*C);
 TMatrixD *Aa = new TMatrixD(*H,TMatrix::kMult,*A); 
 
  //Macierz transponowana
 TMatrixD *Aat = new TMatrixD(TMatrix::kTransposed,*Aa);

 //macierz pomocnicza
 TMatrixD *AaAat = new TMatrixD(*Aat,TMatrix::kMult,*Aa);


 //odwracanie macierzy
 	AaAat->Invert();


 	 //macierz xp pomocniczy i x z falka
TMatrixD *xp = new TMatrixD(*AaAat,TMatrix::kMult,*Aat);
TMatrixD *X = new TMatrixD(*xp,TMatrix::kMult,*Cc); 
 
 //Macierz eta

TMatrixD *eta = new TMatrixD(*A,TMatrix::kMult,*X); 
 
double suma = 0.0;
for (int j=1; j<n; j++ ){
	suma += TMath::Power(((yj[j] - eta(j,0))/sigmaj[j]),2);

}


 return suma;


}


zad11()
{

 TCanvas *c1 = new TCanvas("c1","lab11",10,10,800,600);
 c1 -> Divide(2,2);



	ofstream ofile1;
	double x;
	double y;
  	ifstream ifile;
  	ifile.open("dane.dat");
  	
  	TGraph *data = new TGraph("dane.dat","%lg %lg");
  	int n = data -> GetN();

  	cout << n << endl;

  //	TGraphErrors *data = new TGraphErrors("dane.dat","%lg %lg %lg %lg");

  	c1 -> cd(1) ;
  	data -> Draw("AC*"); //"AEP"
  	
  	double angles[10]; //katy
  	double N[10]; // liczba obserwacji
  	double sigma[10];
  	double sum = 0;
  	double N_sum =0; // liczba pomiarow
  	for (int i=0; i<10; i++)  
  		{
    		ifile>>x>>y;
    		angles[i] = x;
    		N[i] = y;
    		sigma[i] = TMath::Sqrt(y);
    		sum += y*x;
    		N_sum += y;
    		
 		}

  	ifile.close();

 	 int r=3;
  

 	 // Funkcja zwraca wartosc funkcji M
// parametry:
//  st - stopien dopasowanego wielomianu
//  n - liczba pomiarow
//  tj - tablica cosinusow kata rozproszenia
//  yj - wyniki pomiarow
//  sigmaj - bledy pomiarow
//  wsp - tablica do ktorej nalezy wpisac wartosci wyznaczonych wspolczynnikow (Wzor10.png)
//  bswp - tablica do ktorej nalezy wpisac bledy wyznaczonych wspolczynnikow (pierwiastki kwadratowe z elementów diagonalnych macierzy  Wzor11.png)
double result =  dopasuj (r, 10, angles, N, sigma);

	cout << "Wynik dla r = 3: " << result << endl;


	
}
