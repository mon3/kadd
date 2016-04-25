int seed ;
int g = 23;
int m = 97;

double Uniform(int xmin, int xmax)
{
  int c = 0;
  double x_new =(g*seed +c )%m;
  seed = x_new;
  //return  double(xmin)+double(xmax-xmin)*double((g*seed +c )%m)/double(m)*double(xmax-xmin);
 
return  double(xmin)+x_new/double(m)*double(xmax-xmin);

}

double function1(double y){
  double tau = 2.0;
  return -tau*TMath::Log(1-y);

}

generator(){
  seed = time(NULL);
  int N = 10000;
  double data1 [10000];
  double data2 [10000];
  double data3 [10000];
  


  ofstream ofile1,ofile2,ofile3;
  ofile1.open("losowe1.dat");
  for(int i=0;i<N;i++){
    double val = Uniform(0,1);
    data1[i] = val;
  ofile1<<val<<endl;
  }

ofile1.close();

 m = 32363;
 g = 157;
 seed = time(NULL);
 ofile2.open("losowe2.dat");
  for(int i=0;i<N;i++){
  double val2 = Uniform(0,1);
    data2[i] = val2;
  ofile2<<val2<<endl;
  }

ofile2.close();

 m = 147483647;
 g = 16807;
 seed = time(NULL);

 ofile3.open("losowe3.dat");
  for(int i=0;i<N;i++){

   double val3 = Uniform(0,1);
    data3[i] = val3;
  ofile3<<val3<<endl; 
 }

ofile3.close();


  TCanvas *c1 = new TCanvas("c1","generator",10,10,800,600);
  c1 -> Divide(3,3);
  

   TH2D *test1 = new TH2D("test1","Widmo1",100, 0, 1, 100, 0, 1);

   TH2D *test2 = new TH2D("test2","Widmo2",100, 0, 1, 100, 0, 1);

   TH2D *test3 = new TH2D("test3","Widmo3",100, 0, 1, 100, 0, 1);
  

for (int i=0; i<N-1;i++){
		test1 -> Fill(data1[i],data1[i+1]);
		test2 -> Fill(data2[i],data2[i+1]);
		test3 -> Fill(data3[i],data3[i+1]);
		
	
}

	c1 -> cd(1);
  test1 -> GetXaxis() -> SetTitle("x_n");
  test1 -> GetYaxis() -> SetTitle("x_n+1");
  test1 -> Draw("colz");

  c1 -> cd(2);
  test2 -> GetXaxis() -> SetTitle("x_n");
  test2 -> GetYaxis() -> SetTitle("x_n+1");
  test2-> Draw("colz");

  c1 -> cd(3);
  test3 -> GetXaxis() -> SetTitle("x_n");
  test3 -> GetYaxis() -> SetTitle("x_n+1");
  test3-> Draw("colz");


  
  c1 -> cd(4);
  TH1 *f1 = (TH1*) test1-> ProjectionX();
   f1 -> SetTitle("f1");
   f1->GetXaxis()->SetTitle("x");
   f1->GetYaxis()->SetTitle("f(x)");
   f1 -> SetFillColor(3);
   f1 -> Draw();

 c1 -> cd(5);
  TH1 *f2 = (TH1*) test2-> ProjectionX();
   f2 -> SetTitle("f2");
   f2->GetXaxis()->SetTitle("x");
   f2->GetYaxis()->SetTitle("f(x)");
   f2 -> SetFillColor(3);
   f2 -> Draw();


 c1 -> cd(6);
  TH1 *f3 = (TH1*) test3-> ProjectionX();
   f3 -> SetTitle("f3");
   f3->GetXaxis()->SetTitle("x");
   f3->GetYaxis()->SetTitle("f(x)");
   f3 -> SetFillColor(3);
   f3 -> Draw();

    c1 -> cd(7);
 TF1 *function = new TF1("fun1", "[0]*TMath::Exp(-x*[0])", 0, 8);
 double a = 2.0;
  function -> FixParameter(0,1./a);
  
  function -> Draw();

  // c1 -> cd(8);
  TH1D *hist = new TH1D("hist","Histogram",100, 0, 8);
  double data4[10000];
  for (int j=0;j<N;j++){
    // generujemy rozklad f(x) majac data3 - 10000 liczb wygenerowanych z generatora
    data4[j]=function1(data3[j]);  
    // moglo byc tez wypelnianie losowymi wartosciami z root'a: gRandom->Rndm(1)
    hist -> Fill(data4[j]);
    //cout << function1(5.2) << " czy tak samo? "<< function->Eval(0.2) << endl;

  }

  // musimy przeskalowac, zeby byla gestosc prawdopodobienstwa
  double s = hist -> Integral("width"); //gdyby nie bylo width, byloby rownowazne GetEntries
  hist -> Scale(1./s);
  hist ->Draw("same");
  


}
