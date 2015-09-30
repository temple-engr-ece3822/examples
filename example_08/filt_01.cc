// file: $isip/class/algo/Filter/filt_01.cc
// version: $Id: filt_01.cc 8165 2002-05-31 21:57:33Z picone $
//

// isip include files
//
#include "Filter.h"
#include <Console.h>

// method: debug
//
// arguments:
//  const unichar* msg: (input) message to print
//
// return: a bool8 value indicating status
//
bool8 Filter::debug(const unichar* msg_a) const {

  // declare local variables
  //
  String value;
  String output;

  // output the information message
  //
  output.debugStr(name(), msg_a, L":");
  Console::put(output);
  Console::increaseIndention();
  
  // display the algorithm name
  //
  output.debugStr(name(), msg_a, L"algorithm_d",
                  ALGO_MAP.getName((int32)algorithm_d));
  Console::put(output);
  
  // display the implementation name
  //
  output.debugStr(name(), msg_a, L"implementation_d",
                  IMPL_MAP.getName((int32)implementation_d));
  Console::put(output);
  
  // display the mode name
  //
  output.debugStr(name(), msg_a, L"cmode_d",
                  CMODE_MAP.getName((int32)cmode_d));
  Console::put(output);
  
  // display the cmode name
  //
  output.debugStr(name(), msg_a, L"dmode_d",
                  DMODE_MAP.getName((int32)dmode_d));
  Console::put(output);
  
  // display the coefficients
  //
  ar_coef_d.debug(L"ar_coef_d");
  ma_coef_d.debug(L"ma_coef_d");

  // display the lags
  //
  ar_lag_d.debug(L"ar_lag_d");
  ma_lag_d.debug(L"ma_lag_d");

  // display the memory
  //
  ma_mem_d.debug(L"ma_memory");
  ar_mem_d.debug(L"ar_memory");

  // display the debug level
  //
  value.assign((int32) debug_level_d);
  output.debugStr(name(), msg_a, L"debug_level_d", value);
  Console::put(output);

  // display the base class
  //
  AlgorithmBase::debug(L"AlgorithmBase");
  
  // decrease indention
  //
  Console::decreaseIndention();
  
  // exit gracefully
  //
  return true;
}
