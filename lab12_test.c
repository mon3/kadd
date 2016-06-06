const int nd=30;
// tablica x-ow odpowiada 30binom, tak samo tablica y-eko, a tablica z -ida po podwojnej petli
Double_t z[nd][nd],x[nd],y[nd],errorz[nd][nd];
Double_t errorx[nd]={0.};


double function(double *x, double *params)
{
  
  return params[0]*(1.0+params[1]*TMath::Exp(-0.5*(x[0]*x[0]/params[2]+x[1]*x[1]/params[3]))-params[4]*TMath::Cos(x[0]));
 
}

// identyczne jak function, tylko dla konkretnego x i y
double func(double x,double y, double *params)
{
  
  return params[0]*(1.0+params[1]*TMath::Exp(-0.5*(x*x/params[2]+y*y/params[3]))-params[4]*TMath::Cos(x));
 
}


void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
  const Int_t nbins = nd;
  Int_t i;

  //calculate chisquare
  Double_t chisq = 0;
  Double_t delta;
  for (i=0;i<nbins; i++) {
	  for(int j=0; j<nbins;j++){
    delta  = (z[i][j]-func(x[i],y[i],par))/errorz[i][j];
    chisq += delta*delta;
		}
  }
  f = chisq;
}



void lab12_test(TString fit_method="MIGRAD"){

	double A0 = 1.0;
	double A1 = 3.0;
	double A2 = 0.1;
	double A3 = 0.6;
	double A4 = 0.5;


	double xmin = -1.0;
	double xmax = 4.0;
	double ymin = -2.5;
	double ymax = 2.5;
	int nparams = 5;

	// th2d, 3 tablice - xow,yekow,zetow(2D)
	TF2 * fun1 = new TF2("fun1",  "[0]*(1.0+[1]*TMath::Exp(-0.5*(x*x/[2]+y*y/[3]))-[4]*TMath::Cos(x))", xmin, xmax, ymin, ymax);
	fun1 -> SetParameter(0,A0);
	fun1 -> SetParameter(1,A1);
	fun1 -> SetParameter(2,A2);
	fun1 -> SetParameter(3,A3);
	fun1 -> SetParameter(4,A4);


	TCanvas *c1 = new TCanvas("c1","lab12",10,10,800,600);
	c1 -> Divide(2,2);
	TF2 *fun2 = new TF2("fun2", function, xmin,xmax,ymin,ymax,nparams);
	fun2 -> SetParameter(0,A0);
	fun2 -> SetParameter(1,A1);
	fun2 -> SetParameter(2,A2);
	fun2 -> SetParameter(3,A3);
	fun2 -> SetParameter(4,A4);

	c1-> cd(1);
	fun2 -> SetTitle("Teoria");
	fun2 -> GetZaxis() -> SetTitle("f(x,y)");
	fun2 -> GetXaxis() -> SetTitle("x");
	fun2 -> GetYaxis() -> SetTitle("y");

	fun2 -> Draw("surf2");

	int N = 10000;

	// 30 binow - optymalnie
	TH2D *doswiad = new TH2D("doswiad","doswiadczenie",30, xmin, xmax, 30, ymin, ymax);
	double x_rand, y_rand;
   
   for (int i=0; i<N ; i++)
   {
		fun2->GetRandom2(x_rand, y_rand);
		doswiad -> Fill(x_rand, y_rand);
	   
	   
   }

	c1-> cd(2);
	doswiad -> GetZaxis() -> SetTitle("f(x,y)");
	doswiad -> GetXaxis() -> SetTitle("x");
	doswiad -> GetYaxis() -> SetTitle("y");
	
	TH1D *histx = new TH1D("hist","x",30, xmin, xmax);
	TH1D *histy = new TH1D("hist","y",30, ymin, ymax);
	histx = doswiad ->ProjectionX();
	histy = doswiad ->ProjectionY();
	double szum;
	double e = 0.3;
	
	for (int i=0;i <30 ;i++)
	{
		for(int j=0; j<30; j++)
		{
			szum= gRandom->Uniform(1.0 -e, 1.0 +e);
			// mozna tez przeskalowac na koncu jako s= hist->getEntries i pozniej Scale
			z[i][j] = doswiad->GetBinContent(i+1,j+1)*(1.0 + szum);
			//doswiad->SetBinContent(i+1,j+1,z[i][j]);
		//	errorz[i][j] = TMath::Sqrt(z[i][j]);
			errorz[i][j] = 0.01;
			
			
		}
		
	}
	doswiad -> Draw("surf1");

	
	for (int i=0; i<30; i++)
	{
		
		x[i] = histx-> GetBinCenter(i+1);
		y[i] = histy-> GetBinCenter(i+1);
	//	cout<< "x[i] = " << x[i] << "\t y[i] = " << y[i] << endl;
		
			
	}
	
	
	TMinuit *gMinuit = new TMinuit(5);  //initialize TMinuit
  	gMinuit->SetFCN(fcn);

  	Double_t arglist[10];
  	Int_t ierflg = 0;

  	arglist[0] = 1;
  	gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
   
 // Ustalamy wartosci startowe i kroku dla kolejnych parametrow
    double vstart[8] = {40.0, .02 , 3.1, 13.6,3.5};
    double step[8] =   {0.1,    0.1, 0.1,  0.1, 0.1};

    gMinuit->mnparm(0, "a0", vstart[0], step[0], 0,0,ierflg);
    gMinuit->mnparm(1, "a1", vstart[1], step[1], 0,0,ierflg);
    gMinuit->mnparm(2, "a2", vstart[2], step[2], 0,0,ierflg);
    gMinuit->mnparm(3, "a3", vstart[3], step[3], 0,0,ierflg);
    gMinuit->mnparm(4, "a4", vstart[4], step[4], 0,0,ierflg);

  gMinuit->mncomd("SHO FCN",0);
   
   
   
   
  // Now ready for minimization step
  arglist[0] = 1000;
  arglist[1] = 1.;
  gMinuit->mnexcm(fit_method, arglist ,2,ierflg);
  gMinuit->mncomd("SHO FCN",0);
   
  // Print results
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
   // Check fitted params
  Double_t fp[4], err[4];
  for ( Int_t i=0; i<5; i++ )
    {
      gMinuit->GetParameter(i, fp[i], err[i]);
      
    }
	
	  


  	double xmin = x[TMath::LocMin(nd,x)]; //szuka minimum w tablicy
  	double xmax = x[TMath::LocMax(nd,x)];
  	double ymin = y[TMath::LocMin(nd,y)];
  	double ymax = y[TMath::LocMax(nd,y)];

 	fun1->FixParameter(0,fp[0]);
 	fun1->FixParameter(1,fp[1]);
 	fun1->FixParameter(2,fp[2]);
 	fun1->FixParameter(3,fp[3]);
 	fun1->FixParameter(4,fp[4]); 

 // f2->SetLineColor(kBlack);
  //f2->SetLineWidth(0.1);
  
  	fun1->SetLineColor(kBlack);
  	fun1->SetLineWidth(0.1);

	
	c1-> cd(3);
	
 /*fun1->FixParameter(0,A0);
  fun1->FixParameter(1,A1);
  fun1->FixParameter(2,A2);
  fun1->FixParameter(3,A3);
  fun1->FixParameter(4,A4); */
	fun1 -> Draw("surf1");

c1 -> cd(4);

	for (int i=0;i <30 ;i++)
	{
		for(int j=0; j<30; j++)
		{
			szum= gRandom->Uniform(1.0 -e, 1.0 +e);
			// mozna tez przeskalowac na koncu jako s= hist->getEntries i pozniej Scale
			z[i][j] = doswiad->GetBinContent(i+1,j+1)*(1.0 + szum);
			//doswiad->SetBinContent(i+1,j+1,z[i][j]);
		//	errorz[i][j] = TMath::Sqrt(z[i][j]);
			errorz[i][j] = 0.01;
			
			
		}
		
	}
	
	
}
	
	
	
	
	

