lab5()
{

  TCanvas *c1 = new TCanvas("c1","lab4",10,10,800,600);
  c1 -> Divide(2,2);

 
  TH2D *hist12 = new TH2D("hist12","Hist12",100, 1.5, 5, 100, 0, 3);
  TH2D *hist13 = new TH2D("hist13","Hist13",100, 1.5 , 5, 100, 4, 16);
  TH2D *hist23 = new TH2D("hist23","Hist23",100, 0, 3, 100, 4, 16);

  TH1D *hist1 = new TH1D("hist1","Hist1",100, 1.5, 5);
  TH1D *hist2 = new TH1D("hist2","Hist2",100, 0, 3);
  TH1D *hist3 = new TH1D("hist3","Hist3",100, 4, 16);

  
  TH1D *histy1 = new TH1D("histy1","Histy1",100, 0, 15);
  TH1D *histy2 = new TH1D("histy2","Histy2",100, 0, 15);

  ifstream ifile,ifile2,ifile3;
  ifile.open("Dane1.dat");
  ifile2.open("Dane2.dat");
  ifile3.open("Dane3.dat");
  double val,val2,val3;
  double y1, y2;
  while(ifile>>val && ifile2>>val2 && ifile3>>val3)
  {
    hist12 -> Fill(val,val2);
    hist13 -> Fill(val,val3);
    hist23 -> Fill(val2,val3);
    hist1 -> Fill(val);
    hist2 -> Fill(val2);
    hist3 -> Fill(val3);

    y1= 2*val + 5* val2 + val3;
    y2= 3 + 0.5*val + 4*val2;

    histy1 -> Fill(y1);
    histy2 -> Fill(y2);
    //cout<<val<<''<<val2<<''<<val3<<endl;
  }
  ifile.close();
  ifile2.close();
  ifile3.close();
 
  cout<< "X1= "<< hist1 ->GetMean()<<', '<<"u(x1)= "<< hist1 ->GetRMS()<<endl;
  cout<< "X2= "<< hist2 ->GetMean()<<', '<<"u(x2)= "<< hist2 ->GetRMS())<<endl;
  cout<< "X3= "<< hist3 ->GetMean()<<', '<<"u(x3)= "<< hist3 ->GetRMS())<<endl;

cout<< "rho(X1,X2)= "<< hist12 -> GetCorrelationFactor()<< endl;
cout<< "rho(X1,X3)= "<< hist13 -> GetCorrelationFactor()<< endl;
cout<< "rho(X2,X3)= "<< hist12 -> GetCorrelationFactor()<< endl;

  c1 -> cd(1);
  hist12 -> GetXaxis() -> SetTitle("x1");
  hist12 -> GetYaxis() -> SetTitle("x2");
  hist12 -> Draw("colz");

  c1 ->cd(2);
  hist13 -> GetXaxis() -> SetTitle("x1");
  hist13 -> GetYaxis() -> SetTitle("x3");
  hist13 -> Draw("colz");

  c1 -> cd(3);
  hist23 -> GetXaxis() -> SetTitle("x2");
  hist23 -> GetYaxis() -> SetTitle("x3");
  hist23 -> Draw("colz");


  TMatrixD macx(3,3); //pierwsza jes liczba wierszy
macx(0,0) = ( hist1 -> GetRMS()) *( hist1 -> GetRMS()); // dla tych samych zm.los. mamy sigma^2
macx(1,1) =  (hist2 -> GetRMS()) *( hist2 -> GetRMS());
macx(2,2) = (hist3 -> GetRMS()) *( hist3 -> GetRMS());
macx(0,1) = hist12 -> GetCovariance();
macx(0,2) = hist13 -> GetCovariance();
macx(1,0) = hist12 -> GetCovariance(axis1=2,axis2=1);
macx(1,2) = hist23 -> GetCovariance();
macx(2,0) = hist13 -> GetCovariance(axis1=2,axis2=1);
macx(2,1) = hist23 -> GetCovariance(axis1=2,axis2=1); 
macx.Print();



  TMatrixD macT(2,3); //pierwsza jes liczba wierszy
macT(0,0) = 2;
macT(0,1) = 5;
macT(0,2) = 1;
macT(1,0) = 0.5;
macT(1,1) = 4;
macT(1,2) = 0.;
 
macT.Print();

TMatrixD Tt(3,2);
Tt.Transpose(macT);

TMatrixD mactemp(2,3);
mactemp=macT*macx; //wspolczynniki * (x1,x2,x3)

TMatrixD macy(2,2);
macy=mactemp*Tt;

// macy jest macierza kowariancji nowych zmiennych y1, y2
macy.Print();

cout<< "Y1= "<< 2*hist1 ->GetMean()+5*hist2->GetMean()+hist3 ->GetMean()<<endl;
cout<< "Y2= "<<3+ 0.5*hist1 ->GetMean()+4*hist2->GetMean()<<endl;


cout<< "u(Y1)= "<<TMath::Sqrt(macy(0,0)) <<endl;
cout<< "u(Y2)= "<<TMath::Sqrt(macy(1,1))<<endl;
// wsp. korelacji = kowariancja/( sigma(y1) sigma (y2) )
cout<< "rho(Y1,Y2)= "<< macy(0,1)/(TMath::Sqrt(macy(0,0))*TMath::Sqrt(macy(1,1)))<<endl;
cout<< "rho(Y2,Y1)= "<< macy(0,1)/(TMath::Sqrt(macy(0,0))*TMath::Sqrt(macy(1,1)))<<endl;


}
