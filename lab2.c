lab2()
{
  TCanvas *c1 = new TCanvas("c1","lab2",10,10,800,600);
  c1 -> Divide(2,1);
  c1 -> cd(1);
  //nalezy podac jakas poczatkowa wartosc parametru "a"
  double a = 6.0;
  TF1 *fun1 = new TF1("fun1", "[0]*TMath::Exp(-x*[0])", 0, 50);
  fun1 -> FixParameter(0,double(1/a));
  fun1->SetTitle("Rozklad");
  fun1->GetXaxis()->SetTitle("x");;
  fun1->GetYaxis()->SetTitle("f(x)");
  fun1 -> Draw();

  c1 -> cd(2);
  //DrawIntegral zwraca obiekt TGraph, dlatego robimy rzutowanie
  TGraph *dystrybuanta = (TGraph*)(fun1 -> DrawIntegral());
  dystrybuanta -> SetTitle("Dystrybuanta");
  dystrybuanta->GetXaxis()->SetTitle("x");;
  dystrybuanta->GetYaxis()->SetTitle("F(x)");
  dystrybuanta -> Draw();
  
  TF1 *fun2 = new TF1("fun2", "1-TMath::Exp(-x*[0])", 0, 50);
  fun2 -> FixParameter(0,double(1/a));
  cout << "Prawdopodobienstwo z f(x):" <<fun1 -> Integral(5, 10)<< endl;
  cout << "Prawdopodobienstwo z F(x):" <<(fun2 -> Eval(10)) -(fun2 -> Eval(5))<< endl;
  cout << "Srednia: " << fun1 -> Mean(0,50)<<endl;
  cout << "Wariancja: " << fun1 ->Variance(0,50)<< endl;
  cout << "Moda: " << fun1 ->GetMaximumX()<< endl;


  TF1 *fun3 = new TF1("fun2", "x*[0]*TMath::Exp(-x*[0])", 0, 50);
  fun3 -> FixParameter(0,double(1/a));
  cout << "Wartosc oczekiwana: " <<fun3 -> Integral(0, 50)<< endl;



  //kwartyle - sprawdzamy zanim x bedzie mniejsze od 0,25
  
  double b = double(1/20.0); // b*1000=50 -daje nam gorny zakres funkcji
  //kwartyl x_0,25
  //lecimy po x-ach od 0.25 do 250
  //szukamy takiej wartosci x, dla ktorej dystrybuanta bedzie rowna 0.25
  //calkujemy od 0 do x i musimy otrzymac 0.25
  for (int i=0; i<1000;i++)
    {
      if ((fun2 -> Eval(double(b*i)))>= 0.25)
	{
	  cout<<"x_0.25: "<< double(b*i) << endl;
	  break;
	  
	}      
    }

  for (int i=0; i<1000;i++)
    {
      if ((fun2 -> Eval(double(b*i)))>= 0.5)
	{
	  cout<<"x_0.5: "<< double(b*i) << endl;
	  break;
	  
	}      
    }


  
  for (int i=0; i<1000;i++)
    {
      if ((fun2 -> Eval(double(b*i)))>= 0.75)
	{
	  cout<<"x_0.75: "<< double(b*i) << endl;
	  break;
	  
	}      
    }

}

