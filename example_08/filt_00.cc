// file: $isip/class/algo/Filter/filt_00.cc
// version: $Id: filt_00.cc 8165 2002-05-31 21:57:33Z picone $
//

// isip include files
//
#include "Filter.h"

//------------------------------------------------------------------------
//
// required public methods
//
//-----------------------------------------------------------------------

// method: default constructor
//
// arguments:
//  ALGORITHM algorithm: (input) algorithm used in filter
//
// return: none
//
Filter::Filter(ALGORITHM algorithm_a) {
  
  // initialize parameters related to filtering
  //
  algorithm_d = algorithm_a;
  implementation_d = DEF_IMPLEMENTATION;
  
  setMACoeffs(DEF_MA_COEF, DEF_MA_LAG);
  setARCoeffs(DEF_AR_COEF, DEF_AR_LAG);
}

// method: copy constructor
//
// arguments:
//  const Filter& arg: (input) filter object
//
// return: none
//
Filter::Filter(const Filter& arg_a) {

  // copy the data
  //
  if (!assign(arg_a)) {
    Error::handle(name(), L"copy constructor", Error::MEM, __FILE__, __LINE__);
  }

  // initialize the filter
  //
  init();
}

// method: assign
//
// arguments:
//  const Filter& arg: (input) object to be assigned
//
// return: a bool8 value indicating status
//
// this method assigns the input object to the current object
//
bool8 Filter::assign(const Filter& arg_a) {

  // assign lags
  //
  ma_lag_d.assign(arg_a.ma_lag_d);
  ar_lag_d.assign(arg_a.ar_lag_d);

  // assign coefficients
  //
  ma_coef_d.assign(arg_a.ma_coef_d);
  ar_coef_d.assign(arg_a.ar_coef_d);

  // assign calculation buffers
  //
  ma_mem_d.assign(arg_a.ma_mem_d);
  ar_mem_d.assign(arg_a.ar_mem_d);

  // assign algorithm and implementation
  //
  algorithm_d = arg_a.algorithm_d;
  implementation_d = arg_a.implementation_d;

  // assign valid flag
  //
  is_valid_d = arg_a.is_valid_d;

  // call the base assign method
  //
  return AlgorithmBase::assign(arg_a);
}

// method: eq
//
// arguments:
//  const Filter& arg: (input) object to be compared
//
// return: a bool8 value indicating status
//
// this method checks whether the current object is identical to the
// input object
//
bool8 Filter::eq(const Filter& arg_a) const {

  // check all the coefficients
  //
  return (ar_lag_d.eq(arg_a.ar_lag_d) &&
	  ma_lag_d.eq(arg_a.ma_lag_d) &&
	  ar_coef_d.eq(arg_a.ar_coef_d) &&
	  ma_coef_d.eq(arg_a.ma_coef_d));
}

// method: clear
//
// arguments:
//  Integral::CMODE ctype: (input) clear mode
//
// return: a bool8 value indicating status
//
// this method resets the data members to the default values
//
bool8 Filter::clear(Integral::CMODE ctype_a) {

  if (ctype_a == Integral::RETAIN) {

    // keep the parameters but clear the internal memory
    //
    ma_mem_d.clear(ctype_a);
    ar_mem_d.clear(ctype_a);
  }

  else {
    
    // reset the parameters related to the Filter
    //
    ar_coef_d.clear(ctype_a);
    ma_coef_d.clear(ctype_a);
    ar_lag_d.clear(ctype_a);
    ma_lag_d.clear(ctype_a);
    ma_mem_d.clear(ctype_a);
    ar_mem_d.clear(ctype_a);
  }
  
  // call the base clear method
  //
  return AlgorithmBase::clear(ctype_a);
}

//---------------------------------------------------------------------------
//
// class-specific public methods:
//  public methods required by the AlgorithmBase interface contract
//
//---------------------------------------------------------------------------

// method: assign
//
// arguments:
//  const AlgorithmBase& arg: (input) object to be assigned
//
// return: a bool8 value indicating status
//
// this method assigns the input algorithm object to the current
// Filter object, if the input algorithm object is not a Filter
// object, it returns an error
//
bool8 Filter::assign(const AlgorithmBase& arg_a) {

  if (typeid(arg_a) == typeid(Filter)) {
    return assign((Filter&)arg_a);
  }
  else {
    return Error::handle(name(), L"assign", Error::ARG, __FILE__, __LINE__);
  }
}

// method: eq
//
// arguments:
//  const AlgorithmBase& arg: (input) object to be compared
//
// return: a bool8 value indicating status
//
// this method checks whether the current Filter object is identical
// to the input algorithm object, if the input algorithm object is not
// a Filter object, it returns an error
//
bool8 Filter::eq(const AlgorithmBase& arg_a) const {

  if (typeid(arg_a) == typeid(Filter)) {
    return eq((Filter&)arg_a);
  }
  else {
    return Error::handle(name(), L"eq", Error::ARG, __FILE__, __LINE__);
  }
}

// method: init
// 
// arguments: none
//
// return: a bool8 value indicating status
//
bool8 Filter::init() {

  // check for duplicity of ma lags
  //
  int32 ma_lag_len = ma_lag_d.length();
  for (int32 i = 0; i < ma_lag_len - 1; i++) {
    for (int32 j = i + 1; j < ma_lag_len; j++) {
      if (ma_lag_d(i) == ma_lag_d(j)) {
	return Error::handle(name(), L"init", Error::ARG, __FILE__, __LINE__);
      }
    }
  }

  // check for duplicity of ar lags
  //
  int32 ar_lag_len = ar_lag_d.length();
  for (int32 i = 0; i < ar_lag_len - 1; i++) {
    for (int32 j = i + 1; j < ar_lag_len; j++) {
      if (ar_lag_d(i) == ar_lag_d(j)) {
	return Error::handle(name(), L"init", Error::ARG, __FILE__, __LINE__);
      }
    }
  }

  // ar lags can't be in present or future
  //
  for (int32 index = 0; index < ar_lag_len; index++) {
    if ((int32)ar_lag_d(index) >= (int32)0) {
      return Error::handle(name(), L"init", ERR_AR_LAG, __FILE__, __LINE__);
    }
  }
  
  // check for degenerate cases
  //
  if (ma_coef_d.length() == 0) {
    ma_coef_d.setLength(1);
    ma_coef_d(0) = 1.0;
  }
  if (ar_coef_d.length() == 0) {
    ar_coef_d.setLength(1);
      ar_coef_d(0) = 0;
  }
  
  // calculate the delays required
  //
  Long ma_min = ma_lag_d.min();
  Long ma_max = ma_lag_d.max();

  // check for FIR case when the ar coeff. length is zero
  //
  Long ar_min = (int32)0;
  if (ar_lag_d.length() != (int32)0) {
    ar_min = ar_lag_d.min();
  }
  
  if (ma_max < (int32)0) {
    ma_max = (int32)0;
  }
  
  if (ma_min > (int32)0) {
    ma_min = (int32)0;
  }

  int32 ma_mem_len = ma_max - ma_min + 1;
  int32 ar_mem_len = ar_min.neg();

  // create space for each channel
  //
  ma_mem_d.setLength(num_channels_d);
  ar_mem_d.setLength(num_channels_d);
  
  // size the delay lines
  //
  for (int32 nchan = 0; nchan < num_channels_d; nchan++) {
    ma_mem_d(nchan).setLength(ma_mem_len);
    ma_mem_d(nchan).clear(Integral::RETAIN);

    // set the length for ar delay line only if it is non-zero
    //
    if (ar_mem_len > (int32)0) {
    ar_mem_d(nchan).setLength(ar_mem_len);
    ar_mem_d(nchan).clear(Integral::RETAIN);
    }
  }
  
  // set the initialization flag
  //
  is_valid_d = true;
  
  // exit gracefully
  //
  return true;
}

//-----------------------------------------------------------------------------
//
// we define non-integral constants in the default constructor
//      
//-----------------------------------------------------------------------------

// constants: class name
//
const String Filter::CLASS_NAME(L"Filter");

// constants: i/o related constants
//
const String Filter::DEF_PARAM(L"");
const String Filter::PARAM_ALGORITHM(L"algorithm");
const String Filter::PARAM_IMPLEMENTATION(L"implementation");
const String Filter::PARAM_MA_COEF(L"ma_coef");
const String Filter::PARAM_AR_COEF(L"ar_coef");
const String Filter::PARAM_MA_LAG(L"ma_lag");
const String Filter::PARAM_AR_LAG(L"ar_lag");
const String Filter::PARAM_CMODE(L"compute_mode");
const String Filter::PARAM_DMODE(L"data_mode");

// constants: default values of the class data
//
const VectorFloat Filter::DEF_MA_COEF(L"1");
const VectorFloat Filter::DEF_AR_COEF(L"0");
const VectorLong Filter::DEF_MA_LAG(L"0");
const VectorLong Filter::DEF_AR_LAG(L"-1");

// constants: NameMap(s) for the enumerated values
//
const NameMap Filter::ALGO_MAP(L"LTI");
const NameMap Filter::IMPL_MAP(L"CCDE");

// static instantiations: memory manager
//
MemoryManager Filter::mgr_d(sizeof(Filter), Filter::name());
