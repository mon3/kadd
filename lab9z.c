        // gestosc prawdopodobienstwa funkcji chi2
        double chi(double *x, double *params)
        {

          return TMath::Power(0.5, params[0]/2)/TMath::Gamma(params[0]/2)*TMath::Power(x[0],params[0]/2-1)*TMath::Exp(-x[0]/2);

        }


        // dystrybuanta funkcji chi2
        double dyst(double *x, double *params)
        {

          TF1 *f = new TF1("f",chi, 0.0, 30.0, 1);
          f->SetParameter(0,(double)(params[0]));
          return f->Integral(0.00001,x[0]);

        }





        lab9z(){

         TCanvas *c1 = new TCanvas("c1","lab9",10,10,800,600);
         c1 -> Divide(2,2);

         double min = 0.0;
         double max = 30.0;
         int nparams = 1;

         // fun1 - tablica rozkladow chi^2 dla roznych stopni swobody n=1..20
         TF1 *fun1[20];
         // fun2 - tablica dystrybuant chi^2 dla n=1..20
         TF1 *fun2[20];

         
         int n = 20;
         c1 -> cd(1);

         for (int i =0; i<n; i++)
         {
          //Tworze funkcje - rozklady chi^2 dla roznych stopni swobody n i wypelniam nimi tablice 
          TF1 *rozklad  = new TF1(Form("rozklad k=%d",i+1),chi, min, max, nparams);
           // fun2[i] = new TF1((Form("fund%d",i),dyst, min, max, nparams);
          rozklad -> SetParameter(0,double(i+1));
          rozklad -> SetTitle("rozklad #chi^{2}");
          rozklad -> GetYaxis() -> SetTitle("f(#chi^{2})");
          rozklad -> GetXaxis() -> SetTitle("#chi^{2}");
          rozklad -> GetYaxis() -> SetRangeUser(0.0,0.5);
          fun1[i] = rozklad;
          if( i== 0){
            fun1[i] -> Draw();

          }
          else {

           fun1[i]-> SetLineColor(i%9+1);
           fun1[i] -> Draw("Same");
         }

       }

       c1 -> cd(2);

       for (int i =0; i<n; i++)
       {

         fun2[i] = new TF1(Form("dystr k=%d",i+1),dyst, min, max, nparams);
           // trzeba bylo zmienic na fixparameter i na i+1
         fun2[i]->FixParameter(0,i+1);
         fun2[i] -> SetLineColor(i%9+1);
         fun2[i] -> SetTitle("dystrybuanta #chi^{2} ");
         fun2[i] -> GetYaxis() -> SetTitle("F(#chi^{2})");
         fun2[i] -> GetXaxis() -> SetTitle("#chi^{2}");
         // SetRangeUser ustawia przedzial wartosci na danej osi
         fun2[i] -> GetYaxis() -> SetRangeUser(0.0, 1.0);
         fun2[i] -> GetXaxis() -> CenterTitle();
         fun2[i] -> GetYaxis() -> CenterTitle();
         // do rysowania rozkladow dla roznych n na jednym wykresie
         if( i== 0)
         {
          fun2[i] -> Draw();
        }
        else
         {

          fun2[i] -> Draw("Same");
       }

     }


   c1 -> cd(3);
   // k - liczba rozkladow jednostajnych
   int k =1;
   minx = 0;
   maxx = 1;
   double mi2 = 0.0;
   double sigma2 = 1.0;
   // standard normal distribution: mi =0, sigma = 1
   TF1 *fun3 = new TF1("fun3", "[2]*TMath::Exp(-(x-[0])*(x-[0])/2/[1]/[1])",-10.0,10.0);
   fun3 -> SetParameter(0,mi2);
   fun3 -> SetParameter(1,sigma2);
   fun3 -> SetParameter(2,1.0); //moze byc tez 1000 - wynik jest ten sam
   double m = 1.1;

    while (m > 1.0){

   TH1D *hist = new TH1D("hist",Form("Splot %d rozkladow jedn.",k),1000,-3.0,5.0);

   // TH1D *hist = new  TH1D("hist1", "sploty", 1000, -3.0 ,5.0);

    for(int j=0;j<1000;j++)
    {
      double sum = 0.0;
      for(int i=0;i<k;i++)
      {
       sum += gRandom -> Uniform(0,1);
     }
     hist->Fill(sum);
   }


      	hist-> Fit("fun3"); //nadpisuje parametry f1
      	// for(int i=1;i<=hist->GetNbinsX();i++)
       // {
       //   hist->SetBinContent(i,0);
       //   hist->SetBinError(i,0);
       // }
       // hist->SetEntries(0);	

       cout<<"fun3->GetNDF(): "<<fun3->GetNDF()<<endl;
       m = (fun3 -> GetChisquare())/(fun3->GetNDF());
       k++;

      // for(int jj=0; jj<n;jj++)
      // {
      //   double val = 0.0;
      //   for(int ii=0; ii<k;ii++)
      //   {
      //     val += (gRandom -> Uniform(0,1));
      //   }
      //   hist -> Fill(val);
      // }
      // hist -> Fit("gauss");
      // m = (gauss -> GetChisquare())/(gauss->GetNDF());
      // k++;
     }
    

     cout<< "Ilość rozkładów jednostajnych k: "<< k<< endl;
     cout<< "#chi^{2}/ndf = " << (fun3 -> GetChisquare())/(fun3->GetNDF() )<< endl;

     fun3 -> SetTitle("Dopasowanie funkcji Gaussa");
     fun3 -> Draw();
     hist-> Draw("Same");




   }
