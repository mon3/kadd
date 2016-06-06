	// h - histogram danych
	// f - przewidywanie "teoretyczne"
	double chi2(TH1D *h, TF1 *f)
	{
		double T = 0.0;
		for (int i=0; i<9; i++)

		{
		
			T += (((h -> GetBinContent(i+1)) - f-> Eval(i))*((h -> GetBinContent(i+1)) - f-> Eval(i))/(f-> Eval(i)));
			
		}
		
		return T;
	}

	//chcemy dystrybuante naszej wielkosci T
	double chi(double *x, double *params)
	{
		// funkcja gestosci prawdopodobienstwa
	  return 1./(TMath::Power(2,params[0]/2)*TMath::Gamma(params[0]/2))*TMath::Power(x[0], params[0]/2-1)*TMath::Exp(-x[0]/2);

	}

	double dyst(double *x, double *params)
	{
	  TF1 *f = new TF1("f",chi, 0.0, 9.0, 1); //(nazwa_funkcji, minx, maxx, nparams)
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
	// ndf - liczba stopni swobody rozkladu chi2 (liczba parametrow k)
	bool testChi2(double T, double alpha, int ndf)
	{
		double min = 0.0;
		double max = 30.0;
		int nparams = 1;
		
	   	TF1 *fun1;
	   	TF1 *fun2;	

	 

		// fun1 = new TF1("fun1",chi, min, max, nparams);
	 //    fun1->SetParameter(0,ndf);
	    fun2 = new TF1("fun2",dyst, min, max, nparams);
		fun2->SetParameter(0,ndf);
	   		
	   		//szukamy wartosci kwantyla z dystrybuanty
	     double y_value = 1-alpha;
	     double x = fun2 -> GetX(y_value);
	     
	     cout << "Kwantyl: " << x << endl;
	     // nasza statystyka T - obliczony chi2
	     if(T > x) 
	     {
			return false; // hipoteza falszywa
		}
		else 
		{
			return true; // hipoteza prawdziwa
		}	
		
	}




	lab10()
	{

	 TCanvas *c1 = new TCanvas("c1","lab10",10,10,800,600);
	 c1 -> Divide(1,1);



	ofstream ofile1;
		int x, y;
	  	ifstream ifile;
	  	ifile.open("file.dat");
	  	// nbins, xlow, xup
	  	TH1D *hist = new TH1D("hist","Rozklad",9, 0, 9);
	  	
	  	int k[9];
	  	int N[9];
	  	int sum = 0;
	  	int N_sum =0;
	  	for (int i=0; i<9; i++)  
	  		{
	  			
	    		ifile>>x>>y;
	    		k[i] = x;
	    		N[i] = y;
	    		sum += y*x; // bin * liczba zliczen w binie
	    		N_sum += y; // suma wszystkich zliczen

	    		// po wszystkich y-ekach dla danego binu musimy powypelniac histogram
	    		for (int j=0; j<y; j++)
	    		{
	    		hist -> Fill(x); // zwieksza bin o wartosci x o 1
				}
	    	
	 		}

	  	ifile.close();

	  	// lambda = srednia -> estymator najwiekszej wiarygodnosci dla poissona
	  	double average = (double) ((double)sum/(double)N_sum);
	  	
	  	c1 -> cd(1);
	   	
	   	// dopasowanie metoda najwiekszej wiarygodnosci z estymatorem lambda=average
		TF1 *poisson = new TF1("poisson","[0]*TMath::PoissonI(x,[1])",0.0,9.0); // zakres dla calosci
		poisson->SetParameter(0,hist -> GetEntries()); // przeskalowujemy o amplitude
		poisson->SetParameter(1,average);
		
		// kopia poissona, do ktorego bedziemy dopasowywac funkcja fit z roota
		TF1 *poisson1 = new TF1("poisson1","[0]*TMath::PoissonI(x,[1])",0.0,8.0); // w ostatnim binie jest 0 , wiec obcinamy o 1 bin do fitowania
		//cout<< "entries = " << hist -> GetEntries() << endl;
		poisson1->SetParameter(0,hist -> GetEntries()); // amplituda
		poisson1->SetParameter(1,average);
		hist -> Fit(poisson1,"LR");
		hist -> SetFillColor(4);
		hist -> SetTitle("Wynik eksperymentu");
		hist -> SetXTitle("k");
		hist -> SetYTitle("n_{k}, np_{k}");
		hist -> Draw();
		poisson -> SetLineColor(3);
		poisson -> Draw("Same");
		
		
		int ndf = 6; // ndf = n-1-liczba parametrow(tutaj 1, bo srednia)
		double chi2 = chi2(hist, poisson);
		double t = (double) chi2 /((double) ndf); 
		
		cout << "Lambda najwiekszej wiarygodnosci: " << average<< endl;
		cout<< "Lambda (ROOT Fit)" << poisson1 -> GetParameter(1) << endl;
		cout<< "chi2 (wartosc statystyki testowej T) " << chi2 <<endl;
		cout<< "chi2/NDF: "<< t << endl;
		cout<< "chi2 (ROOT Fit):" << poisson1 -> GetChisquare() << endl;
		cout<< "chi2 (ROOT Fit)/NDF:" << poisson1 -> (double)(poisson1 -> GetChisquare()/ndf) << endl;
		
		cout<<"Poziom istotnosci alpha: 0.01"<< endl;
		
		bool test = testChi2(chi2, 0.01, 6); 
		if (test == 0){
			cout<< "Wynik testu: są podstawy, żeby odrzucić hipotezę." <<endl;
		}
		else{
			cout<< "Wynik testu: nie ma podstaw do odrzucenia hipotezy." <<endl;
		}

		
	}
