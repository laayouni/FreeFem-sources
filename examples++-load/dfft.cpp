// Example C++ function "myfunction", dynamically loaded into "ff-c++ dfft.cpp "
// ---------------------------------------------------------------------
// $Id$

//   for automatic  compilation with ff-c++
//ff-c++-LIBRARY-dep:   fftw3 
//ff-c++-cpp-dep: 
//  
#include "ff++.hpp"
#include "AFunction_ext.hpp"
#include <fftw3.h>

class DFFT_1d2dor3d
{  
public: 
  Complex * x;
  int n, m,k;
  int sign;   
  DFFT_1d2dor3d(KN<Complex> * xx,long signn,long nn=1,long kk=1) : x(*xx),n(nn),m(xx->N()/(nn*kk)),k(kk),sign(signn) {
    cout << xx << " " << signn << " " << nn << " " << xx->N() << " n: " << n << " m:" << m << " k:  " << k <<endl;
    ffassert(n>0 && (n*m*k ==xx->N()));
    
  } 


  DFFT_1d2dor3d(KNM<Complex> * xx,long signn) : x(*xx),n(xx->M()),m(xx->N()),sign(signn) {
  }
  
};

DFFT_1d2dor3d dfft(KN<Complex> * const  & x,const long &sign)
{
  return DFFT_1d2dor3d(x,sign);
}

DFFT_1d2dor3d dfft(KN<Complex> *const  &x,const long &nn,const long &sign)
{
  return DFFT_1d2dor3d(x,sign,nn);
}
DFFT_1d2dor3d dfft(KN<Complex> *const  &x,const long &nn,const long &kk,const long &sign)
{
    return DFFT_1d2dor3d(x,sign,nn,kk);
}

DFFT_1d2dor3d dfft(KNM<Complex> * const &  x,const long &sign)
{
  return DFFT_1d2dor3d(x,sign);
}

KN<Complex> * dfft_eq(  KN<Complex> * const   &x,const DFFT_1d2dor3d & d)
{
  ffassert(x->N()==d.n*d.m*d.k );
  Complex *px =  *x;
  fftw_plan p; 
  //cout << " dfft " << px << " = " << d.x << " n = " << d.n << " " << d.m << " sign = " << d.sign << endl; 
  if ( d.k ==1)
  {
   if ( d.n > 1)
    p = fftw_plan_dft_2d(d.n,d.m,reinterpret_cast<fftw_complex*>(d.x),reinterpret_cast<fftw_complex*> (px),d.sign,FFTW_ESTIMATE);
   else
    p = fftw_plan_dft_1d(d.m ,reinterpret_cast<fftw_complex*>(d.x),reinterpret_cast<fftw_complex*> (px),d.sign,FFTW_ESTIMATE);
  }
   else 
   {
       if ( d.n > 1)
           p = fftw_plan_dft_3d(d.n,d.m,d.k,reinterpret_cast<fftw_complex*>(d.x),reinterpret_cast<fftw_complex*> (px),d.sign,FFTW_ESTIMATE);
       else
           p = fftw_plan_dft_2d(d.m,d.k,reinterpret_cast<fftw_complex*>(d.x),reinterpret_cast<fftw_complex*> (px),d.sign,FFTW_ESTIMATE);
       
   }
 // cout << " ---" ;
  fftw_execute(p);
 // cout << " ---" ;
  fftw_destroy_plan(p);
 // cout << " ---" ;
  return  x;
}

class Init { public:
  Init();
};
LOADINIT(Init);
Init::Init(){
  cout << " lood: init dfft " << endl;
  Dcl_Type<DFFT_1d2dor3d >();
  Global.Add("dfft","(", new OneOperator2_<DFFT_1d2dor3d,KN<Complex>*,long >(dfft ));
  Global.Add("dfft","(", new OneOperator3_<DFFT_1d2dor3d,KN<Complex>*,long,long >(dfft ));
  Global.Add("dfft","(", new OneOperator4_<DFFT_1d2dor3d,KN<Complex>*,long,long, long >(dfft ));
  Global.Add("dfft","(", new OneOperator2_<DFFT_1d2dor3d,KNM<Complex>*,long >(dfft ));
  TheOperators->Add("=", new OneOperator2_<KN<Complex>*,KN<Complex>*,DFFT_1d2dor3d>(dfft_eq));
  // TheOperators->Add("=", new OneOperator2_<KNM<Complex>*,KNM<Complex>*,DFFT_1d2dor3d>(dfft_eq));

}
