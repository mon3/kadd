double function(double *x, double *params)
{
	//min - params[0] ; max - params[1]
  if (x[0]>=0 && x[0]<=1)
    return (x[0]-params[0])/(params[1]-params[0]);
  return 0;
}

void myfunc()
{
   TF1 *fun1 = new TF1("fun1",function, xmin, xmax,nparams);
  	fun1 -> FixParameter(0,0.0);
  	fun1 -> FixParameter(1,1.0);
   f1->Draw();
}

lab7()
{
	TCanvas *c1 = new TCanvas("c1","lab4",10,10,800,600);
  	c1 -> Divide(2,2);
  	c1 -> cd(1);
	double xmin = 0.0;
	double xmax = 1.0;
	int nparams = 2;


	TRandom *grand      = new TRandom();



	// TF1 *fun1 = new TF1("fun1",function, xmin, xmax,nparams);
 //  	fun1 -> FixParameter(0,0.0);
 //  	fun1 -> FixParameter(1,1.0);
 
 //  	fun1->SetTitle("Rozklad");
 //  	fun1->GetXaxis()->SetTitle("x");;
 //  	fun1->GetYaxis()->SetTitle("F(x)");
 //  	fun1 -> Draw();

  	double x[10000];
  	double y[10000];
  	int n_acc = 0;
  	int n_not = 0;
  	TGraph *g_acc = new TGraph();
  	TGraph *g_not = new TGraph();

  	TH1D *hist = new TH1D("hist","Rozklad",10, xmin, xmax);
  	for (int i=0;i<10000;i++)
  	{
  		x[i] = grand -> Uniform(1);
  		y[i] = grand -> Uniform(1);
  		cout<< i << " "<< x[i]<<" "<< y[i] << endl;

  		if (x[i]*x[i]+y[i]*y[i] < 1.0)
  		{
  			n_acc ++;
  			g_acc -> SetPoint(n_acc,x[i],y[i])
  			hist -> Fill(x[i]);
  		}
  		else 
  		{
  			n_not ++;
  			g_not -> SetPoint(n_not,x[i],y[i])
  		}

  	}

  	cout<< n_acc << endl;
  	cout<< "pi: "<<double(4.0 * n_acc / 10000) <<endl;

  	hist -> Draw();
  	c1 -> cd(2);
  	g_acc -> Draw("ac");
  	//g_not -> Draw("same");




}