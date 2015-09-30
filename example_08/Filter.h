// file: $isip/class/algo/Filter/Filter.h
// version: $Id: Filter.h 8376 2002-07-14 16:48:44Z gao $
//

// make sure definitions are only made once
//
#ifndef ISIP_FILTER
#define ISIP_FILTER

#ifndef ISIP_ALGORITHM_BASE
#include <AlgorithmBase.h>
#endif

#ifndef ISIP_VECTOR_FLOAT
#include <VectorFloat.h>
#endif

#ifndef ISIP_VECTOR_LONG
#include <VectorLong.h>
#endif

#ifndef ISIP_VECTOR
#include <Vector.h>
#endif

#ifndef ISIP_CIRCULAR_DELAY_LINE
#include <CircularDelayLine.h>
#endif

#ifndef ISIP_FLOAT
#include <Float.h>
#endif

#ifndef ISIP_MEMORY_MANAGER
#include <MemoryManager.h>
#endif

// Filter: a digital filtering class used to generate signals
// filtered by a variety of techniques including linear constant
// coefficient difference equations.
//
class Filter : public AlgorithmBase {
  
  //--------------------------------------------------------------------------
  //
  // public constants
  //
  //--------------------------------------------------------------------------
public:
  
  // define the class name
  //
  static const String CLASS_NAME;

  //----------------------------------------
  //
  // other important constants
  //
  //----------------------------------------
    
  // define algorithm choices
  //
  enum ALGORITHM { LTI = 0, DEF_ALGORITHM = LTI };
  
  // define implementation choices
  //
  enum IMPLEMENTATION { CCDE = 0, DEF_IMPLEMENTATION = CCDE };
  
  // define static NameMap objects
  //
  static const NameMap ALGO_MAP;
  static const NameMap IMPL_MAP;

  //----------------------------------------
  //
  // i/o related constants
  //
  //----------------------------------------

  static const String DEF_PARAM;
  static const String PARAM_ALGORITHM;  
  static const String PARAM_IMPLEMENTATION;
  static const String PARAM_MA_COEF;
  static const String PARAM_AR_COEF;
  static const String PARAM_MA_LAG;
  static const String PARAM_AR_LAG;
  static const String PARAM_CMODE;
  static const String PARAM_DMODE;  

  //----------------------------------------
  //
  // default values and arguments
  //
  //----------------------------------------

  // define default value(s) of the class data
  //
  static const VectorFloat DEF_MA_COEF;
  static const VectorFloat DEF_AR_COEF;
  static const VectorLong DEF_MA_LAG;
  static const VectorLong DEF_AR_LAG;

  // define default argument(s)
  //
  static const AlgorithmData::COEF_TYPE DEF_COEF_TYPE = AlgorithmData::GENERIC;
  static const int32 DEF_CHANNEL_INDEX = 0;
  
  //----------------------------------------
  //
  // error codes
  //
  //----------------------------------------  

  static const int32 ERR = 70600;
  static const int32 ERR_AR_LAG = 70601;
  static const int32 ERR_DATA = 70603;
  
  //---------------------------------------------------------------------------
  //
  // protected data
  //
  //---------------------------------------------------------------------------
protected:

  // algorithm name
  //
  ALGORITHM algorithm_d;
  
  // implementation type
  //
  IMPLEMENTATION implementation_d;

  // parameters related to the algorithm specification
  //
  VectorFloat ma_coef_d;
  VectorFloat ar_coef_d;
  VectorLong ma_lag_d;
  VectorLong ar_lag_d;
  
  // circular buffers for ma and ar filters
  //
  Vector< CircularDelayLine<Float> > ma_mem_d;
  Vector< CircularDelayLine<Float> > ar_mem_d;
  
  // static memory manager
  //
  static MemoryManager mgr_d;

  //---------------------------------------------------------------------------
  //
  // required public methods
  //
  //---------------------------------------------------------------------------
public:
  
  // method: name
  //
  static const String& name() {
    return CLASS_NAME;
  }

  // other static methods
  //
  static bool8 diagnose(Integral::DEBUG debug_level);
  
  // method: setDebug
  //  these methods are inherited from the AlgorithmBase class

  // other debug methods:
  //
  bool8 debug(const unichar* msg) const;

  // method: destructor
  //
  ~Filter() {}

  // method: default constructor
  //
  Filter(ALGORITHM algorithm = DEF_ALGORITHM);

  // method: copy constructor
  //
  Filter(const Filter& arg);

  // assign methods
  //
  bool8 assign(const Filter& arg);
  
  // method: operator=
  //
  Filter& operator= (const Filter& arg) {
    assign(arg);
    return *this;
  }
  
  // i/o methods				      	      	     
  //
  int32 sofSize() const;
  
  bool8 read(Sof& sof, int32 tag, const String& name = CLASS_NAME);
  
  bool8 write(Sof& sof, int32 tag, const String& name = CLASS_NAME) const;

  bool8 readData(Sof& sof, const String& pname = DEF_PARAM,
                   int32 size = SofParser::FULL_OBJECT,
                   bool8 param = true,
                   bool8 nested = false);

  bool8 writeData(Sof& sof, const String& pname = DEF_PARAM) const;

  // equality methods
  //
  bool8 eq(const Filter& arg) const;

  // memory management methods
  //
  // method: new
  //
  static void* operator new(size_t size) {
    return mgr_d.get();
  }

  // method: new[]
  //
  static void* operator new[](size_t size) {
    return mgr_d.getBlock(size);
  }

  // method: delete
  //
  static void operator delete(void* ptr) {
    mgr_d.release(ptr);
  }

  // method: delete[]
  //
  static void operator delete[](void* ptr) {
    mgr_d.releaseBlock(ptr);
  }

  // method: setGrowSize
  //
  static bool8 setGrowSize(int32 grow_size) {
    return mgr_d.setGrow(grow_size);
  }

  // other memory management methods
  //
  bool8 clear(Integral::CMODE ctype = Integral::DEF_CMODE);

  //---------------------------------------------------------------------------
  //
  // class-specific public methods
  //  set methods
  //
  //---------------------------------------------------------------------------

  // method: setAlgorithm
  //
  bool8 setAlgorithm(ALGORITHM algorithm) {
    algorithm_d = algorithm;
    return true;
  }

  // method: setImplementation
  //
  bool8 setImplementation(IMPLEMENTATION implementation) {
    implementation_d = implementation;
    return true;  
  }  

  // set methods for MA coefficients
  //
  bool8 setMACoeffs(float32 val, int32 lag);
  bool8 setMACoeffs(const VectorFloat& ma_coef);
  bool8 setMACoeffs(const VectorFloat& ma_coef,
		      const VectorLong& ma_lag);

  // set methods for AR coefficients
  //
  bool8 setARCoeffs(float32 val, int32 lag);
  bool8 setARCoeffs(const VectorFloat& ar_coef);
  bool8 setARCoeffs(const VectorFloat& ar_coef,
		      const VectorLong& ar_lag);

  // other set methods
  //
  bool8 setCoeffs(const VectorFloat& ma_coef,
		    const VectorFloat& ar_coef);
  bool8 setCoeffs(const VectorFloat& ma_coef,
		    const VectorLong& ma_lag,
		    const VectorFloat& ar_coef,
		    const VectorLong& ar_lag);

  //---------------------------------------------------------------------------
  //
  // class-specific public methods
  //  get methods
  //
  //---------------------------------------------------------------------------

  // method: getAlgorithm
  //
  ALGORITHM getAlgorithm() const {
    return algorithm_d;
  }
  
  // method: getImplementation
  //
  IMPLEMENTATION getImplementation() const {
    return implementation_d;
  }  

  // method: getMACoeffs
  //
  const VectorFloat& getMACoeffs() const {
    return ma_coef_d;
  }

  // method: getARCoeffs
  //
  const VectorFloat& getARCoeffs() const {
    return ar_coef_d;
  }
  
  // method: getMALags
  //
  const VectorLong& getMALags() const {
    return ma_lag_d;
  }
  
  // method: getARLags
  //
  const VectorLong& getARLags() const {
    return ar_lag_d;
  }

  // other get methods
  //
  int32 getLeadingNumSamp() const;
  int32 getTrailingNumSamp() const;

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  computation methods
  //
  //---------------------------------------------------------------------------

  // compute methods
  //
  bool8 compute(VectorFloat& output, const VectorFloat& input,
		  AlgorithmData::COEF_TYPE input_coef_type = DEF_COEF_TYPE,
		  int32 index = DEF_CHANNEL_INDEX);
  bool8 compute(VectorComplexFloat& output, const VectorComplexFloat& input,
		  AlgorithmData::COEF_TYPE input_coef_type = DEF_COEF_TYPE,
		  int32 index = DEF_CHANNEL_INDEX);

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  public methods required by the AlgorithmBase interface contract
  //
  //---------------------------------------------------------------------------

  // assign methods
  //
  bool8 assign(const AlgorithmBase& arg);

  // equality methods
  //
  bool8 eq(const AlgorithmBase& arg) const;

  // method: className
  //
  const String& className() const {
    return CLASS_NAME;
  }

  // initialization method
  //
  bool8 init();

  // apply methods
  //
  bool8 apply(Vector<AlgorithmData>& output,
		const Vector< CircularBuffer<AlgorithmData> >& input);

  // configuration method dealing with multi-frame computations
  //
  int32 getLeadingPad() const;

  // configuration method dealing with multi-frame computations
  //
  int32 getTrailingPad() const;

  // method: getOutputDataFormat
  //
  DMODE getOutputDataFormat() const {
    return SAMPLE_BASED;
  }

  // method to set the parser
  //
  bool8 setParser(SofParser* parser);

  //---------------------------------------------------------------------------
  //
  // private methods
  //
  //---------------------------------------------------------------------------
private:

  // common i/o methods
  //
  bool8 readDataCommon(Sof& sof, const String& pname,
			 int32 size, bool8 param, bool8 nested);
  bool8 writeDataCommon(Sof& sof, const String& pname) const;

  // algorithm-specific computation methods: LTI/CCDE
  //
  bool8 computeLtiCcdeFrameInternal(VectorFloat& output,
				      const VectorFloat& input,
				      int32 index = DEF_CHANNEL_INDEX);

  bool8 computeLtiCcdeCrossFrame(VectorFloat& output,
				   const VectorFloat& input,
				   int32 index = DEF_CHANNEL_INDEX);  

  // methods to prepare data for filtering
  //
  bool8 computeBuffer(VectorFloat& output,
			const CircularBuffer<AlgorithmData>& input,
			AlgorithmData::COEF_TYPE coef_type,
			int32 channel_index);
};

// end of include file
//
#endif
