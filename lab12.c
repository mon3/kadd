double function(double *x, double *params)
{
   return params[0]*(1+params[1]*TMath::Exp(-0.5*(x[0]*x[0]/params[2]+x[1]*x[1]/params[3]))-params[4]*TMath::Cos(x[0]));
}
  

void generate_data(TF2 *fun2){
	int N = 1000;
  double x [1000];
  double y [1000];

  ofstream ofile1;
  ofile1.open("losowe_gen.dat");
  double x_rand, y_rand;
  for(int i=0;i<N;i++){
    fun2 -> GetRandom2(x_rand,y_rand);

    x[i] = x_rand;
    y[i] = y_rand;
  ofile1<<x_rand<<"\t"<<y_rand<<endl;
  }
}


lab12(){

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
	fun2 -> Draw("surf1");

	int N = 1000;
  double x [1000];
  double y [1000];

   TH2D *doswiad = new TH2D("doswiad","doswiad",1000, xmin, xmax, 1000, ymin, ymax);

  ofstream ofile1;
  ofile1.open("losowe_gen.dat");
  double x_rand, y_rand, scale;
  double e = 0.3;
  for(int i=0;i<N;i++){
    fun2 -> GetRandom2(x_rand,y_rand);
    scale = gRandom -> Uniform(1.0-e,1.0+e);

    x[i] = x_rand/scale;
    y[i] = y_rand/scale;
  ofile1<<x[i]<<"\t"<<y[i]<<endl;
  doswiad ->Fill(x[i],y[i]);
  }

  c1 -> cd(2);
  doswiad -> Draw("surf1");


}