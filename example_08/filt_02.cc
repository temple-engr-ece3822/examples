// file: $isip/class/algo/Filter/filt_02.cc
// version: $Id: filt_02.cc 8384 2002-07-15 18:47:02Z gao $
//

// isip include files
//
#include "Filter.h"
#include <Console.h>

// method: diagnose
//
// arguments:
//  Integral::DEBUG level: (input) debug level for diagnostics
//
// return: a bool8 value indicating status
//
bool8 Filter::diagnose(Integral::DEBUG level_a) {

  // ---------------------------------------------------------------------
  //
  // 0. preliminaries
  //
  // ---------------------------------------------------------------------

  // output the class name
  //
  if (level_a > Integral::NONE) {
    String output(L"diagnosing class ");
    output.concat(CLASS_NAME);
    output.concat(L": ");
    Console::put(output);
    Console::increaseIndention();
  } 

  // --------------------------------------------------------------------
  //
  // 1. required public methods
  //
  // --------------------------------------------------------------------

  // set indentation
  //
  if (level_a > Integral::NONE) {
    Console::put(L"testing required public methods...\n");
    Console::increaseIndention();
  }

  // test destructor/constructor(s) and memory management
  //
  Filter filt0;
  Filter filt1(filt0);

  if (!filt1.eq(filt0)) {
    return Error::handle(name(), L"eqality method", Error::TEST,
                         __FILE__, __LINE__);
  }

  // test large allocation construction and deletion
  //
  if (level_a == Integral::ALL) {
    
    Console::put(L"\ntesting large chunk memory allocation and deletion:\n");
    
    // set the memory to a strange block size so we can hopefully catch any
    // frame overrun errors
    //
    
    Filter::setGrowSize((int32)500);

    Filter* pft = new Filter();
    
    for (int32 j = 1; j <= 10; j++) {
      Filter** pfts = new Filter*[j * 10];
      
      // create the objects
      //
      for (int32 i = 0; i < j * 10; i++) {
        pfts[i] = new Filter();
      }
    
      // delete objects
      //
      for (int32 i = (j * 10) - 1; i >= 0; i--) {
        delete pfts[i];
      }
    
      delete [] pfts;
    } 
    
    delete pft;
  }
  
  // test the i/o methods
  //
  Filter filt2;
  Filter filt3;
  Filter  filt4;

  VectorFloat arcoef0;
  arcoef0.assign(L"1.0, 0.3, 0.4");
  VectorFloat macoef0;
  macoef0.assign(L"1.0, 0.5, 0.6");
  VectorLong arlag0;
  arlag0.assign(L"0, -1, -2");
  VectorLong malag0;
  malag0.assign(L"0, -1, -2");
  filt2.setCoeffs(macoef0, malag0, arcoef0, arlag0);
  
  // we need binary and text sof files
  //
  String tmp_filename0;
  Integral::makeTemp(tmp_filename0);
  String tmp_filename1;
  Integral::makeTemp(tmp_filename1);

  // open files in write mode
  //
  Sof tmp_file0;
  tmp_file0.open(tmp_filename0, File::WRITE_ONLY, File::TEXT);
  Sof tmp_file1;
  tmp_file1.open(tmp_filename1, File::WRITE_ONLY, File::BINARY);

  filt2.write(tmp_file0, (int32)0);
  filt2.write(tmp_file1, (int32)0);
  
  // close the files
  //
  tmp_file0.close();
  tmp_file1.close();
  
  // open the files in read mode
  //
  tmp_file0.open(tmp_filename0);
  tmp_file1.open(tmp_filename1);

  // read the value back
  //
  filt3.read(tmp_file0, (int32)0);
  
  if (!filt3.eq(filt2)) {
    filt3.debug(L"filt3");
    return Error::handle(name(), L"read", Error::TEST, __FILE__, __LINE__);
  }

  filt4.read(tmp_file0, (int32)0);

  if (!filt4.eq(filt2)) {
    filt4.debug(L"filt4");
    return Error::handle(name(), L"read", Error::TEST, __FILE__, __LINE__);
  }

  // close and  delete the temporary files
  //
  tmp_file0.close();
  tmp_file1.close();

  File::remove(tmp_filename0);
  File::remove(tmp_filename1);
  
  // reset indentation
  //
  if (level_a > Integral::NONE) {
    Console::decreaseIndention();
  }
  
  //--------------------------------------------------------------------------
  //
  // 2. class-specific public methods:
  //     set and get methods
  //
  //--------------------------------------------------------------------------

  // set indentation
  //
  if (level_a > Integral::NONE) {
    Console::put(L"testing class-specific public methods: set and get methods...\n");
    Console::increaseIndention();
  }

  // test the set methods
  //
  Filter filt5;
    
  filt5.setMACoeffs(macoef0);
  if (!filt5.ma_coef_d.eq(macoef0)) {
    return Error::handle(name(), L"setCoeffs", Error::TEST,
			 __FILE__, __LINE__);
  }
  
  filt5.setMACoeffs(macoef0, malag0);
  if (!filt5.ma_coef_d.eq(macoef0) || !filt5.ma_lag_d.eq(malag0)) {
    return Error::handle(name(), L"setCoeffs", Error::TEST,
			 __FILE__, __LINE__);
  }
  
  filt5.setARCoeffs(arcoef0);
  if (!filt5.ar_coef_d.eq(arcoef0)) {
    return Error::handle(name(), L" setmethod", Error::TEST,
			 __FILE__, __LINE__);
  }

  filt5.setARCoeffs(arcoef0, arlag0);
  if (!filt5.ar_coef_d.eq(arcoef0) || !filt5.ar_lag_d.eq(arlag0)) {
    return Error::handle(name(), L"setCoeffs", Error::TEST,
			 __FILE__, __LINE__);
  }
  
  filt5.setCoeffs(macoef0, malag0, arcoef0, arlag0);
  if (!filt5.ar_coef_d.eq(arcoef0) || !filt5.ar_lag_d.eq(arlag0)
      || !filt5.ma_lag_d.eq(malag0) || !filt5.ma_coef_d.eq(macoef0)) {
    return Error::handle(name(), L" setmethod", Error::TEST,
			 __FILE__, __LINE__);
  }

  // test the get methods
  //
  if (!macoef0.eq(filt5.getMACoeffs())) {
    return Error::handle(name(), L"getMACoeffs", Error::TEST,
			 __FILE__, __LINE__);
  }
  
  if (!malag0.eq(filt5.getMALags())) {
    return Error::handle(name(), L"setMALags", Error::TEST,
			 __FILE__, __LINE__);
  }

  if (!arcoef0.eq(filt5.getARCoeffs())) {
    return Error::handle(name(), L"getARCoeffs", Error::TEST,
			 __FILE__, __LINE__);
  }
  
  if (!arlag0.eq(filt5.getARLags())) {
    return Error::handle(name(), L"setARLags", Error::TEST,
			 __FILE__, __LINE__);
  }
    
  // reset indentation
  //
  if (level_a > Integral::NONE) {
    Console::decreaseIndention();
  }
  
  //--------------------------------------------------------------------------
  //
  // 3. class-specific public methods:
  //     computation methods
  //
  //--------------------------------------------------------------------------

  // set indentation
  //
  if (level_a > Integral::NONE) {
    Console::put(L"testing class-specific public methods: computation methods...\n");
    Console::increaseIndention();
  }
  
  if (level_a > Integral:: BRIEF) {
    Console::put(L"\ntesting the compute method:\n");
  }

  // test causal IIR with an input signal as 3 samples of a  ramp
  //  
  Filter filt7;
  VectorFloat ma_coef(L"1.2, 0.5, 1.1, 1.0");
  VectorLong ma_lag(L"-2, -1, -4, 0");
  VectorFloat ar_coef(L"0.5, 2.0, 0.2");
  VectorLong ar_lag(L"-2, -1, -5");
  
  filt7.setCoeffs(ma_coef, ma_lag, ar_coef, ar_lag);
  
  VectorFloat in(L"1, 2, 3");
  VectorFloat out;
  VectorFloat ans(L"1, 0.5, 3.7");
    
  filt7.compute(out, in);  
  
  if (!out.almostEqual(ans)) {
    in.debug(L"input:");
    out.debug(L"wrong result:");
    ans.debug(L"expected result:");
    filt7.debug(L"filter:");
    return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
  }

  // test causal IIR with an input signal as 3 samples of a ramp
  //
  Filter filt8;
  filt8.setCoeffs(ma_coef, ma_lag, ar_coef, ar_lag);    
  in.assign(L"4, 5, 6");
  ans.assign(L"4, -1, 13.3");
  
  filt8.compute(out, in);

  if (!out.almostEqual(ans)) {
    in.debug(L"input:");
    out.debug(L"wrong result:");
    ans.debug(L"expected result:");
    filt8.debug(L"filter:");
    return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
  }

  // test causal IIR with an input signal as 3 samples of a step function
  //
  Filter filt9;
  filt9.setCoeffs(ma_coef, ma_lag, ar_coef, ar_lag);
  in.assign(L"1, 1, 1");
  ans.assign(L"1, -0.5, 3.2");
  
  filt9.compute(out, in);

  if (!out.almostEqual(ans)) {
    in.debug(L"input:");
    out.debug(L"wrong result:");
    ans.debug(L"expected result:");
    filt9.debug(L"filter:");
    return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
  }

  // test causal IIR with an input signal as 3 samples of zeros
  //
  Filter filt10;
  filt10.setCoeffs(ma_coef, ma_lag, ar_coef, ar_lag);
  in.assign(L"0, 0, 0");
  ans.assign(L"0, 0, 0");
  
  filt10.compute(out, in);
  
  if (!out.almostEqual(ans)) {
    in.debug(L"input:");
    out.debug(L"wrong result:");
    ans.debug(L"expected result:");
    filt10.debug(L"filter:");
    return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
  }

  if (level_a > Integral:: BRIEF) {
    Console::put(L"\n \testing simple ma filter:\n");
  }

  // test causal FIR with an input signal as 3 samples of a ramp
  //
  Filter filt11;
  ma_coef.assign(L"1.0, 0.5, 1.2");
  VectorFloat ar_coef1;
  filt11.setCoeffs(ma_coef, ar_coef1);
  in.assign(L"1.0, 2.0, 3.0");
  ans.assign(L"1.0, 2.5, 5.2");

  filt11.compute(out, in);

  if (!out.almostEqual(ans)) {
    in.debug(L"input:");
    out.debug(L"wrong result:");
    ans.debug(L"expected result:");
    filt11.debug(L"filter:");    
    return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
  }  
  
  // test causal FIR with an input signal as 3 samples of zeros
  //
  Filter filt12;
  filt11.setCoeffs(ma_coef, ar_coef1);
  in.assign(L"0, 0, 0");
  ans.assign(L"0, 0, 0");
  
  filt12.compute(out, in);    

  if (!out.almostEqual(ans)) {
    in.debug(L"input:");
    out.debug(L"wrong result:");
    ans.debug(L"expected result:");
    filt12.debug(L"filter:");
    return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
  }  

  // test anti-causal IIR with an input signal as 3 samples of a  ramp
  //  
  Filter filt13;
  ma_coef.assign(L"3, 0.5, 1, 2");
  ma_lag.assign(L"-2, -1, 1, 0");
  ar_coef.assign(L"0.5, 2.0, 1");
  ar_lag.assign(L"-2, -1, -3");
  
  filt13.setCoeffs(ma_coef, ma_lag, ar_coef, ar_lag);
  
  in.assign(L"1, 2, 3, 4, 5");
  ans.assign(L"4, -0.5, 13, -9.25");
    
  filt13.compute(out, in);  
  
  if (!out.almostEqual(ans)) {
    in.debug(L"input:");
    out.debug(L"wrong result:");
    ans.debug(L"expected result:");
    filt13.debug(L"filter:");
    return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
  }

  // test anti-causal IIR with an input signal as 3 samples of a ramp
  //
  Filter filt14;
  filt14.setCoeffs(ma_coef, ma_lag, ar_coef, ar_lag);
  in.assign(L"4, 5, 6");
  ans.assign(L"13, -8");
  
  filt14.compute(out, in);
  
  if (!out.almostEqual(ans)) {
    in.debug(L"input:");
    out.debug(L"wrong result:");
    ans.debug(L"expected result:");
    filt14.debug(L"filter:");
    return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
  }
  
  // test anti-causal IIR with an input signal as 3 samples of a step function
  //
  Filter filt15;
  filt15.setCoeffs(ma_coef, ma_lag, ar_coef, ar_lag);
  in.assign(L"1, 1, 1");
  ans.assign(L"3, -2.5");
  
  filt15.compute(out, in);
  
  if (!out.almostEqual(ans)) {
    in.debug(L"input:");
    out.debug(L"wrong result:");
    ans.debug(L"expected result:");
    filt15.debug(L"filter:");
    return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
  }

  // test anti-causal IIR with an input signal as 3 samples of zeros
  //
  Filter filt16;
  filt16.setCoeffs(ma_coef, ma_lag, ar_coef, ar_lag);
  in.assign(L"0, 0, 0");
  ans.assign(L"0, 0");
  
  filt16.compute(out, in);
  
  if (!out.almostEqual(ans)) {
    in.debug(L"input:");
    out.debug(L"wrong result:");
    ans.debug(L"expected result:");
    filt16.debug(L"filter:");
    return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
  }


  filt16.clear();
  
  // set ma and ar coefficients include future sample for ma
  //
  ma_coef.assign(L"1.2, 0.5, 1.1, 1.0, 1.0");
  ma_lag.assign(L"-2, -1, -4, 0, 1");
             
  ar_coef.assign(L"0.5, 2.0, 0.2");
  ar_lag.assign(L"-2, -1, -5");
  filt16.setCoeffs(ma_coef, ma_lag, ar_coef, ar_lag);

  // declare input vector
  //
  in.assign(L"1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10");

  ans.assign(L"3, -0.5, 8.7, -4.25, 21.85, -19.675, 55.825, -71.4525, 152.742, -242.429, 438.921, -741.493, 1296.92, -2229.73, 3877.2, -6704.81, 11646.6, -20168.1, 34995.8, -60651.2, 105172, -182321, 316110, -548034, 950161, -1.64732e+06, 2.85604e+06, -4.95162e+06, 8.58486e+06, -1.48839e+07, 2.58049e+07, -4.4739e+07, 7.75659e+07, -1.34479e+08, 2.33152e+08, -4.04226e+08, 7.00824e+08, -1.21505e+09, 2.10658e+09, -3.65226e+09, 6.33209e+09, -1.09782e+10, 1.90334e+10, -3.2999e+10, 5.72117e+10, -9.91903e+10, 1.7197e+11, -2.98152e+11, 5.16919e+11");

  filt16.compute(out, in);

  if (!out.almostEqual(ans)) {
    in.debug(L"input:");
    out.debug(L"wrong result:");
    ans.debug(L"expected result:");
    filt16.debug(L"filter:");
    return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
  }
   filt16.clear();
  
  // set ma and ar coefficients include future sample for ma
  //
  ma_coef.assign(L"1.2, 0.5, 1.1, 1.0, 1.0");
  ma_lag.assign(L"-2, -1, -4, 0, 1");
             
  ar_coef.assign(L"0.5, 2.0, 0.2");
  ar_lag.assign(L"-2, -1, -5");
  filt16.setCoeffs(ma_coef, ma_lag, ar_coef, ar_lag);

  filt16.setComputeMode(AlgorithmBase::CROSS_FRAME);

  // declare input vector
  //
  in.assign(L"1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1");
  ans.assign(L"3, -0.5, 8.7, -4.25, 21.85, -19.675, 55.825, -71.4525, 152.742, -242.429");

  filt16.compute(out, in);

  if (!out.almostEqual(ans)) {
    in.debug(L"input:");
    out.debug(L"wrong result:");
    ans.debug(L"expected result:");
    filt16.debug(L"filter:");
    return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
  }

  ans.assign(L"438.921, -741.493, 1296.92, -2229.73, 3877.2, -6704.81, 11646.6, -20168.1, 34995.8, -60651.2");
  
  filt16.compute(out, in);

  if (!out.almostEqual(ans)) {
    in.debug(L"input:");
    out.debug(L"wrong result:");
    ans.debug(L"expected result:");
    filt16.debug(L"filter:");
    return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
  }
  
  // reset indentation
  //
  if (level_a > Integral::NONE) {
    Console::decreaseIndention();
  }
  
  // --------------------------------------------------------------------
  //
  // 4. print completion message
  //
  // --------------------------------------------------------------------

  // reset indentation
  //
  if (level_a > Integral::NONE) {
    Console::decreaseIndention();
  }
  
  if (level_a > Integral::NONE) {
    String output(L"diagnostics passed for class ");
    output.concat(name());
    output.concat(L"\n");
    Console::put(output);
  }

  // exit gracefully
  //
  return true;
}
