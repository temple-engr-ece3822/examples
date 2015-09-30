// file: $isip/class/dstr/HashTable/HashTable.h
// version: $Id: HashTable.h 7572 2001-12-28 15:22:06Z alphonso $
//

// make sure definitions are made only once
//
#ifndef ISIP_HASH_TABLE
#define ISIP_HASH_TABLE

// isip include files
//
#ifndef ISIP_VECTOR
#include <Vector.h>
#endif

#ifndef ISIP_SINGLE_LINKED_LIST
#include <SingleLinkedList.h>
#endif

#ifndef ISIP_PAIR
#include <Pair.h>
#endif

#ifndef ISIP_LONG
#include <Long.h>
#endif

#ifndef ISIP_STRING
#include <String.h>
#endif

#ifndef ISIP_FLOAT
#include <Float.h>
#endif

#ifndef ISIP_CONSOLE
#include <Console.h>
#endif

// forward class definitions
//
template<class THashable, class TObject> class HashTableDiagnose;

// HashTable: this class implements a hash table using a vector
// of single linked lists.
//
// note that internally the List objects will always be in SYSTEM
// mode. The only difference for USER and SYSTEM mode will be
// controlled manually via the Node class inside the HashNode.
//
template <class THashable, class TObject>
class HashTable : public DstrBase {
  
  //---------------------------------------------------------------------------
  //
  // public constants
  //
  //---------------------------------------------------------------------------
public:
  
  // define the class name
  //
  static const String CLASS_NAME;
  
  //----------------------------------------
  //
  // i/o related constants
  //
  //----------------------------------------  
  
  static const String DEF_PARAM;
  static const String PARAM_NODES;
  static const String PARAM_LOAD_FACTOR;
  
  //----------------------------------------
  //
  // default values and arguments
  //
  //----------------------------------------

  // default values
  //
  static const float32 DEF_LOAD_FACTOR = 0.75;
  static const int32 DEF_CAPACITY = 128;
  
  // default arguments to methods
  //

  //----------------------------------------
  //
  // other constants
  //
  //----------------------------------------

  // when to compress the capacity
  //
  static const float32 LOAD_LOWER_BOUND = 0.10;

  //----------------------------------------
  //
  // error codes
  //
  //----------------------------------------  
  
  static const int32 ERR = 40800;
  
  //---------------------------------------------------------------------------
  //
  // protected data
  //
  //---------------------------------------------------------------------------
protected:

  // define the hash node object
  //
  typedef Pair< THashable, Node<TObject> > HashNode;
  typedef HashTable<THashable, TObject> Type;
  
  // define the storage element for the hash entries:
  //  the internal structure of the HashTable is a Vector where each entry in
  //  the vector is a linked list. The list elements will hold the (key, value)
  //  pairs for the hash lookup.
  //
  Vector< SingleLinkedList<HashNode> > table_d;

  // define the load factor:
  //  the load factor determines how full the hash table is allowed to
  //  get before its capacity is increased. if the number of elements
  //  grows larger than the load factor multiplied by the current capacity
  //  then the capacity is increased and the table rehashed.
  //
  Float load_factor_d;

  // define the total number of items in the hash table
  //
  Long num_items_d;

  // the allocation mode
  //
  ALLOCATION alloc_d;

  // declare the debugging parameters
  //
  static Integral::DEBUG debug_level_d;

  // define the memory manager
  //
  static MemoryManager mgr_d;  
  
  //---------------------------------------------------------------------------
  //
  // required public methods
  //
  //---------------------------------------------------------------------------
public:
  
  // static methods
  //  the diagnose method is moved outside the class header file and
  //  defined in the HashTableDiagnose.h in order to avoid issues
  //  with preprocessing of the diagnose code.
  //
  static const String& name();
  
  // method: setDebug
  //
  static bool8 setDebug(Integral::DEBUG debug_level) {
    debug_level_d = debug_level;
    return true;
  }

  // other debug methods
  //
  bool8 debug(const unichar* message) const;

  // method: destructor
  // 
  ~HashTable() {
    clear(Integral::RESET);
  }

  // default constructor
  //
  HashTable(ALLOCATION alloc = DEF_ALLOCATION,
	    int32 initial_capacity = DEF_CAPACITY,
	    float32 load_factor = DEF_LOAD_FACTOR);

  // method: copy constructor
  //
  HashTable(const HashTable<THashable, TObject>& arg) {
    assign(arg);
  }
  
  // assign methods
  //
  bool8 assign(const HashTable<THashable, TObject>& arg);

  // method: operator=
  //
  HashTable<THashable, TObject>&
  operator=(const HashTable<THashable, TObject>& arg) {
    assign(arg);
    return *this;
  }

  // equality methods:
  //
  bool8 eq(const HashTable<THashable, TObject>& compare_htable) const;

  // other i/o methods
  //
  int32 sofSize() const;
  
  // method: read
  //
  bool8 read(Sof& sof, int32 tag) {
    return read(sof, tag, name());
  }

  // method: write
  //
  bool8 write(Sof& sof, int32 tag) const {
    return write(sof, tag, name());
  }

  // other i/o methods
  //
  bool8 read(Sof& sof, int32 tag, const String& name);
  bool8 write(Sof& sof, int32 tag, const String& name) const;

  bool8 readData(Sof& sof, const String& pname = DEF_PARAM, int32 size =
		   SofParser::FULL_OBJECT, bool8 param = true,
                   bool8 nested = false);
  bool8 writeData(Sof& sof, const String& pname = DEF_PARAM) const;
   
  // method: new
  //
  void* operator new(size_t size) {
    return mgr_d.get();
  }

  // method: new[]
  //
  void* operator new[](size_t size) {
    return mgr_d.getBlock(size);
  }
  
  // method: delete
  //
  void operator delete(void* ptr) {
    mgr_d.release(ptr);
  }

  // method: delete[]
  //
  void operator delete[](void* ptr) {
    mgr_d.releaseBlock(ptr);
  }

  // method: setGrowSize
  //
  static bool8 setGrowSize(int32 grow_size) {
    return mgr_d.setGrow(grow_size);
  }

  // other memory management methods
  //
  bool8 clear(Integral::CMODE cmode = Integral::DEF_CMODE);
  
  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  extension to required methods
  //
  //---------------------------------------------------------------------------

  // method: ne
  //
  bool8 ne(const HashTable<THashable, TObject>& compare_htable) const {
    return (!eq(compare_htable));
  }

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  hash table manipulation methods
  //
  //---------------------------------------------------------------------------
  
  // data access methods
  //
  TObject* get(const THashable& key);
  const TObject* get(const THashable& key) const;
  
  // insert methods
  //
  bool8 insert(const THashable& key, TObject* value);

  // remove methods
  //
  bool8 remove(const THashable& key, TObject*& value);
  bool8 remove(const THashable& key);

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  hash table data access methods
  //
  //---------------------------------------------------------------------------
    
  // methods to get all keys/values
  //
  bool8 keys(Vector<THashable>& keys_list) const;
  bool8 values(Vector<TObject>& items_list) const;

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  hash table property methods
  //
  //---------------------------------------------------------------------------
  
  // method: getCapacity
  //
  int32 getCapacity() const {
    return table_d.length();
  }

  // method: setCapacity
  //
  bool8 setCapacity(int32 capacity) {
    return rehash(capacity);
  }

  // method: setLoadFactor
  //
  bool8 setLoadFactor(float32 factor) {
    if (factor < (2 * LOAD_LOWER_BOUND)) {
      return Error::handle(name(), L"setLoadFactor", Error::ARG,
			   __FILE__, __LINE__);
    }
    load_factor_d = factor;

    // possibly rehash the table
    //
    if ((int32)num_items_d > (((float32)load_factor_d) * getCapacity())) {
      return rehash((int32)(load_factor_d / (float32)num_items_d * (float32)2));
    }
    return true;
  }

  // method: getNumItems
  //
  int32 getNumItems() const {
    return num_items_d;
  }

  // method: isEmpty
  //
  bool8 isEmpty() const {
    return ((int32)num_items_d == 0);
  }

  // method: containsKey
  //
  bool8 containsKey(const THashable& key) const {
    int32 index = 0;
    return const_cast<Type*>(this)->findKey(index, key);
  }

  // other methods to determine the occupancy of the HashTable
  //
  bool8 containsValue(const TObject* item) const;

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  hash table memory allocation methods
  //
  //---------------------------------------------------------------------------
  
  // method: getAllocationMode
  //
  ALLOCATION getAllocationMode() const {
    return alloc_d;
  }

  // method: setAllocationMode
  //  
  bool8 setAllocationMode(ALLOCATION alloc) {
    alloc_d = alloc;
    return true;
  }
  
  //---------------------------------------------------------------------------
  //
  // private methods
  //
  //---------------------------------------------------------------------------
private:

  // methods to determine if the specified object is contained in the HashTable
  // it outputs the vector index and positions the SingleLinkedList accordingly
  //
  bool8 findKey(int32& index, const THashable& key);

  // rehashing method
  //
  bool8 rehash(int32 new_capacity);

  // method: getCurrKey
  //
  const THashable& getCurrKey(int32 index) const {
    return table_d(index).getCurr()->first();
  }

  // method: getCurrKey
  //
  THashable& getCurrKey(int32 index) {
    return table_d(index).getCurr()->first();
  }

  // method: getCurrObject
  //
  const TObject* getCurrObject(int32 index) const {
    return table_d(index).getCurr()->second().getItem();
  }

  // method: getCurrObject
  //
  TObject* getCurrObject(int32 index) {
    return table_d(index).getCurr()->second().getItem();
  }

  // method: setCurrObject
  //
  bool8 setCurrObject(int32 index, TObject* arg) {
    return table_d(index).getCurr()->second().setItem(arg);
  }

  // methods to transorm to and from a single list
  //
  bool8 getList(SingleLinkedList<HashNode>& arg);
  bool8 assignFromList(SingleLinkedList<HashNode>& arg);

  // friend class
  //
  template <class THashable_diagnose, class TObject_diagnose> 
  friend class HashTableDiagnose;
};

//-----------------------------------------------------------------------------
//
// we define non-integral constants at the end of class definition for
// templates (for non-templates these are defined in the default constructor)
//      
//-----------------------------------------------------------------------------

// constants: required constants such as the class name
//
template <class THashable, class TObject>
const String HashTable<THashable, TObject>::CLASS_NAME(L"HashTable");

template <class THashable, class TObject>
const String HashTable<THashable, TObject>::DEF_PARAM(L"");

template <class THashable, class TObject>
const String HashTable<THashable, TObject>::PARAM_NODES(L"table");

template <class THashable, class TObject>
const String HashTable<THashable, TObject>::PARAM_LOAD_FACTOR(L"load_factor");

// static instantiations: debug level
//
template <class THashable, class TObject>
Integral::DEBUG HashTable<THashable, TObject>::debug_level_d = Integral::NONE;

// static instantiations: the memory manager
//
template <class THashable, class TObject>
MemoryManager HashTable<THashable, TObject>::mgr_d(sizeof(HashTable<THashable, TObject>), CLASS_NAME);

// below are all the methods for the HashTable template class
//      

// ---------------------------------------------------------------------
//
// required static methods
//
//----------------------------------------------------------------------

// method: name
//
// arguments: none
//
// return: a static String& containing the class name
//
// this method returns the class name
//
template<class THashable, class TObject>
const String& HashTable<THashable, TObject>::name() {

  // create the static name string for this class and return it
  //
  static String cname(CLASS_NAME);
  cname.clear(Integral::RESET);
  cname.concat(CLASS_NAME);
  cname.concat(L"<");
  cname.concat(THashable::name());
  cname.concat(L",");
  cname.concat(TObject::name());
  cname.concat(L">");
  
  // return the name
  //
  return cname;
}

// ---------------------------------------------------------------------
//
// required debug methods
//
//----------------------------------------------------------------------

// method: debug
//
// arguments:
//  const unichar* message: (input) information message
//
// return: a bool8 value indicating status
//
// this method dumps the contents of an object to the console
// 
template<class THashable, class TObject>
bool8 HashTable<THashable, TObject>::debug(const unichar* message_a) const {
  
  // declare local variables
  //
  String output;
  String value;

  // print out the number of items
  //
  value.assign((int32)num_items_d);
  output.debugStr(name(), message_a, L"num_items_d", value);
  Console::put(output);
  
  // print out the load factor
  //
  value.assign((float32)load_factor_d);
  output.debugStr(name(), message_a, L"load_factor_d", value);
  Console::put(output);
  
  // print out the capacity
  //
  value.assign((int32)getCapacity());
  output.debugStr(name(), message_a, L"capacity_d", value);
  Console::put(output);
  
  // print out the memory allocation mode
  //
  output.debugStr(name(), message_a, L"alloc_d",
		  NameMap::ALLOCATION_MAP((int32)alloc_d));
  Console::put(output);

  // get the list
  //
  SingleLinkedList<HashNode> temp_list(USER);
  const_cast< HashTable<THashable, TObject>* >(this)->getList(temp_list);
  
  Console::increaseIndention();
  
  // loop over each element and print its value
  //
  for (bool8 more = temp_list.gotoFirst(); more;
       more = temp_list.gotoNext()) {
    value.assign(temp_list.getCurr()->first().hash(getCapacity()));
    temp_list.getCurr()->first().debug((unichar*)value);
    Console::increaseIndention();
    temp_list.getCurr()->second().getItem()->debug((unichar*)value);
    Console::decreaseIndention();
  }

  Console::decreaseIndention();
  
  // exit gracefully
  // 
  return true;
}

//------------------------------------------------------------------------
//
// required destructor/constructor(s)
//
//-----------------------------------------------------------------------

// method: default constructor
//
// arguments:
//  ALLOCATION alloc: (input) the flag to specify whether or not the item
//                    memory is allocated by the node
//  int32 init_capacity: (input) the initial capacity of the hash table
//  float32 load_factor: (input) the load factor of the hash table
//
// return: none
//
// this is the default constructor for the HashTable class
//
template<class THashable, class TObject>
HashTable<THashable, TObject>::HashTable(ALLOCATION alloc_a,
					 int32 init_capacity_a,
					 float32 load_factor_a) {
  
  // initialize the number of elements
  //
  num_items_d = (int32)0;

  // initialize the load factor
  //
  setLoadFactor(load_factor_a);
  
  // initialize memory allocation flag
  //
  alloc_d = alloc_a;
  
  // initialize the capacity
  //
  setCapacity(init_capacity_a);
}

//------------------------------------------------------------------------
//
// required assign methods
//
//-------------------------------------------------------------------------

// method: assign
//
// arguments:
//  const HashTable<THashable, TObject>& arg: (input) table to copy
//
// return: a bool8 value indicating status
//
// this method copies the contents of the input to this hash table
//
template<class THashable, class TObject>
bool8 HashTable<THashable, TObject>::assign(const HashTable<THashable,
					      TObject>& arg_a) {

  // clear this table
  //
  clear(Integral::RESET);

  // copy the allocation mode and the load factor
  //
  alloc_d = arg_a.alloc_d;
  load_factor_d = arg_a.load_factor_d;
  
  Vector<THashable> keys;
  arg_a.keys(keys);

  int32 len = keys.length();

  for (int32 i = 0; i < len; i++) {
    insert(keys(i), const_cast<Type*>(&arg_a)->get(keys(i)));
  }

  // exit gracefully
  //
  return true;
}

//------------------------------------------------------------------------
//
// required i/o methods
//
//------------------------------------------------------------------------

// method: sofSize
//
// arguments: none
//
// return: size of object as written to disk via the i/o methods
//
// this method determines the size of the object on disk
//
template<class THashable, class TObject>
int32 HashTable<THashable, TObject>::sofSize() const{

  // declare temporary variables
  //
  int32 tmp_size = 0;
  
  // count the size of the load_factor_d
  //
  tmp_size = load_factor_d.sofSize();
  
  // get the capacity of the hash table
  //
  int32 capacity = getCapacity();
  
  // loop over each element and add the size of that element. this is the
  // total size of the hash table
  // 
  for (int32 i = 0; i < capacity; i++) {
    
    // add the size of this element
    //
    tmp_size += table_d(i).sofSize();
  }
  
  // return the size
  //
  return tmp_size;
}

// method: read
//
// arguments:
//  Sof& sof: (input) sof file object
//  int32 tag: (input) sof object instance tag
//  const String& name: (input) sof object instance name
//
// return: a bool8 value indicating status
//
// this method has the object read itself from an Sof file
//
template<class THashable, class TObject>
bool8 HashTable<THashable, TObject>::read(Sof& sof_a, int32 tag_a,
					    const String& name_a) {
  
  // get the instance of the object from the Sof file
  //
  if (!sof_a.find(name_a, tag_a)) {
    return false;
  }

  // read the actual data from the sof file
  //
  if (!readData(sof_a)) {
    return false;
  }

  // exit gracefully
  //
  return true;
}

// method: write
//
// arguments:
//  Sof& sof: (input) sof file object
//  int32 tag: (input) sof object instance tag
//  const String& name: (input) sof object instance name
//
// return: a bool8 value indicating status
//
// this method has the object write itself to an Sof file
//
template<class THashable, class TObject>
bool8 HashTable<THashable, TObject>::write(Sof& sof_a, int32 tag_a,
					     const String& name_a) const {
  
  // declare a temporary size variable
  //
  int32 obj_size = 0;

  // switch on ascii or binary mode
  //
  if (sof_a.isText()) {
    
    // set the size to be dynamic
    //
    obj_size = Sof::ANY_SIZE;
  }
  else {

    // the size of the binary data to write
    //
    obj_size = sofSize();
  }
  
  // put the object into the sof file's index
  //
  if (!sof_a.put(name_a, tag_a, obj_size)) {
    return false;
  }
  
  // exit gracefully
  //
  return writeData(sof_a);
}

// method: readData
//
// arguments:
//  Sof& sof: (input) sof file object
//  const String& pname: (input) parameter name
//  int32 size: (input) size of the object
//  bool8 param: (input) is the parameter specified?
//  bool8 nested: (input) is this nested?
//
// return: a bool8 value indicating status
//
// this method has the object read itself from an Sof file. it assumes
// that the Sof file is already positioned correctly.
//
template<class THashable, class TObject>
bool8
HashTable<THashable, TObject>::readData(Sof& sof_a, const String& pname_a,
					int32 size_a, bool8 param_a,
					bool8 nested_a) {
  
  // first cleanup the hash table
  //
  if (!clear(Integral::RESET)) {
    return Error::handle(name(), L"readData", Error::READ,
			 __FILE__, __LINE__, Error::WARNING);
  }
  
  // implicit parameter is impossible with composite objects,  but
  // nesting can be used
  //
  SofParser parser;

  parser.setDebug(debug_level_d);
  
  if (nested_a) {
    parser.setNest();
  }
  
  // load the parser
  //  
  parser.load(sof_a, size_a);
    
  // read and set the load factor
  //
  Float factor;
  if (!factor.readData(sof_a, PARAM_LOAD_FACTOR,
		       parser.getEntry(sof_a, PARAM_LOAD_FACTOR))) {
    return Error::handle(name(), L"readData", Error::READ,
			 __FILE__, __LINE__, Error::WARNING);
  }
  setLoadFactor(factor);
  
  // read the hash nodes into a temporary list
  //
  SingleLinkedList< HashNode > temp_list(SYSTEM);
  temp_list.setDebug(debug_level_d);
  if (!temp_list.readData(sof_a, PARAM_NODES,
			  parser.getEntry(sof_a, PARAM_NODES), 
			  false)) {
    return Error::handle(name(), L"readData", Error::READ,
			 __FILE__, __LINE__, Error::WARNING);
  }
  
  // according to the number of num_items and the load factor, compute
  // a suitable capacity
  //
  int32 num_items = temp_list.length();
  Double cap;
  cap = (float64)num_items / (((float64)load_factor_d) * 100);
  cap.ceil();
  int32 capacity = ((int32)(float64)cap) * 100; 
  
  // set the capacity
  //
  table_d.setLength((int32)capacity);

  // place the objects from the list onto the table
  //
  temp_list.setAllocationMode(USER);
  assignFromList(temp_list);
  
  // clean up the list
  //
  temp_list.clear(Integral::RESET);

  // exit gracefully 
  //
  return true;
}

// method: writeData
//
// arguments:
//  Sof& sof: (input) sof file object
//  const String& pname: (input) parameter name
//
// return: a bool8 value indicating status
//
// this method writes the object to the Sof file. it assumes that the
// Sof file is already positioned correctly.
//
template<class THashable, class TObject>
bool8
HashTable<THashable, TObject>::writeData(Sof& sof_a,
					 const String& pname_a) const {
  
  // local variables
  //
  SingleLinkedList<HashNode> temp_list(USER);
  
  // extract the list of nodes
  //
  const_cast<HashTable<THashable,TObject>*>(this)->getList(temp_list);

  // write the label prefix
  //
  sof_a.writeLabelPrefix(pname_a);
  
  // write the load factor
  //
  if (!load_factor_d.writeData(sof_a, PARAM_LOAD_FACTOR)) {
    return Error::handle(name(), L"write load factor", HashTable::ERR,
			 __FILE__, __LINE__);
  }
  
  // write the nodes list
  //
  if (!temp_list.writeData(sof_a, PARAM_NODES)) {
    return Error::handle(name(), L"write the nodes", HashTable::ERR,
			 __FILE__, __LINE__);
  };
  
  // write the label suffix
  //
  sof_a.writeLabelSuffix(pname_a);

  // exit gracefully
  //
  return true;
}

//-----------------------------------------------------------------------
//
// required equality methods
//
//------------------------------------------------------------------------

// method: eq
//
// arguments:
//  const HashTable<THashable, TObject>& compare_table: (input) the hash table to compare
//
// return: a bool8 value indicating status
//
// this method compares two hash tables for equivalence. two hash tables are
// equivalent if all corresponding elements are equivalent
//
template<class THashable, class TObject>
bool8
HashTable<THashable, TObject>::eq(const HashTable<THashable, TObject>&
				  compare_table_a) const {

  // two hash table can't be equal if they have different number of items
  //
  if (num_items_d != compare_table_a.num_items_d) {
    return false;
  } 
  
  // compare if the two hash tables contain the same hash nodes
  //
  for (int32 i = 0; i < getCapacity(); i++) {
    
    for (bool8 more = const_cast<Type*>(this)->table_d(i).gotoFirst();
	 more; more = const_cast<Type*>(this)->table_d(i).gotoNext()) {
      
      if (!compare_table_a.containsValue(getCurrObject(i))) {
	return false;
      }
    }
  }

  // exit gracefully: the two hash tables are the same
  // 
  return true;
}

//-------------------------------------------------------------------------
//
//  required clear method
//
//-------------------------------------------------------------------------

// method: clear
//
// arguments:
//  Integral::CMODE cmode: (input) clear mode
//  
// return: a bool8 value indicating status
//
// this method clears the contents of the hash table. for RETAIN mode,
// all keys remain but the objects are cleared. for RESET and RELEASE
// mode, the table is emptied. for FREE mode the table is emptied and
// all memory is deleted regardless of allocation mode.
//
template<class THashable, class TObject>
bool8 HashTable<THashable, TObject>::clear(Integral::CMODE cmode_a) {

  // initialize local varaibles
  //
  int32 capacity = getCapacity();

  // loop over all elements in the table
  //
  for (int32 i = 0; i < capacity; i++) {    

    // for RETAIN or FREE propogate clear to all objects
    //
    if ((cmode_a == Integral::RETAIN) || (cmode_a == Integral::FREE)) {
      for (bool8 more = table_d(i).gotoFirst(); more;
	   more = table_d(i).gotoNext()) {
	getCurrObject(i)->clear(cmode_a);
      }
    }

    // in system mode or clear::FREE delete memory
    //
    if ((alloc_d == SYSTEM) || (cmode_a == Integral::FREE)) {
      for (bool8 more = table_d(i).gotoFirst(); more;
	   more = table_d(i).gotoNext()) {
	delete getCurrObject(i);
	setCurrObject(i, (TObject*)NULL);
      }
    }
    
    // remove references to these objects if not in RETAIN mode, i.e.,
    // empty the table.
    //
    if (cmode_a != Integral::RETAIN) {
      table_d(i).clear(cmode_a);
    }
  }

  // if cmode_a is called with the RESET, set the length of the table
  // to be 0
  //  
  if (cmode_a == Integral::RESET) {
    table_d.setLength(DEF_CAPACITY);
  }

  // for RELEASE and FREE also set the capacity to be DEF_CAPACITY
  //
  else if ((cmode_a == Integral::RELEASE) || (cmode_a == Integral::FREE))  {
    table_d.setLength(DEF_CAPACITY);
    table_d.setCapacity(DEF_CAPACITY);
  }

  // reset the number of items if not in retain mode
  //
  if (cmode_a != Integral::RETAIN) {
    num_items_d = 0;
  }

  // exit gracefully
  //
  return true;
}

//------------------------------------------------------------------------
//
// class-specific public methods:
//  hash table manipulation methods
//
//------------------------------------------------------------------------

// method: get
//
// arguments:
//  const THashable& key: (input) the key used to find the item
//  
// return: the item corresponding to a given key
//
// this method gets the item corresponding to a key
//
template<class THashable, class TObject>
TObject* HashTable<THashable, TObject>::get(const THashable& key_a) {

  // declare local variables
  //
  int32 index = 0;

  // find if there is such a key is in the hash table
  //
  if (findKey(index, key_a)) {
    
    // the linked list is positioned correctly now  
    //
    return getCurrObject(index);
  }
    
  // nothing in the hash table, return null
  //
  return (TObject*)NULL;
}

// method: get
//
// arguments:
//  const THashable& key: (input) the key used to find the item
//  
// return: the item corresponding to a given key
//
// this method gets the item corresponding to a key
//
template<class THashable, class TObject>
const TObject*
HashTable<THashable, TObject>::get(const THashable& key_a) const {

  // declare local variables
  //
  int32 index = 0;

  // find if there is such a key is in the hash table
  //
  if (const_cast<Type*>(this)->findKey(index, key_a)) {

    // the linked list is positioned correctly now  
    //
    return getCurrObject(index);
  }
    
  // nothing in the hash table, return null
  //
  return (TObject*)NULL;
}

// method: insert
//
// arguments:
//  const THashable& key: (input) the key string of an item
//  TObject* item: (input) the item to put into the hash table
//  
// return: a bool8 value indicating status
//
// this method creates a hash node for the given key and value, and inserts it
// into corresponding list in this hash table.
//
template<class THashable, class TObject>
bool8 HashTable<THashable, TObject>::insert(const THashable& key_a,
					      TObject* item_a) {

  // make sure the key is unique
  //
  if (containsKey(key_a)) {
    Error::handle(name(), L"this key is already used", ERR,
		  __FILE__, __LINE__);
    return false;
  }

  // start to build the HashNode with the key
  //
  HashNode new_node;
  new_node.first().assign(key_a);

  // when in USER mode we just reference the object
  //
  if (alloc_d == USER) {
    new_node.second().setItem(item_a);
  }

  // when in SYSTEM mode we make a copy of the object
  //
  else {
    new_node.second().setItem(new TObject(*item_a));
  }

  // add this node into the table_d
  //
  int32 index = key_a.hash(getCapacity());
  table_d(index).insertLast(&new_node);
  
  // increase the number of items
  //
  num_items_d = (int32)num_items_d + 1;

  // possibly rehash the table
  //
  if ((int32)num_items_d > (((float32)load_factor_d) * getCapacity())) {
    return rehash(getCapacity() * 2);
  }


  
  // exit gracefully
  //
  return true;
}

// method: remove
//
// arguments:
//  const THashable& key: (input) the key string of an item
//  TObject*& item: (output) the item having the given key 
//  
// return: a bool8 value indicating status
//
// this method removes the node for the given key and outputs its value.
//
template<class THashable, class TObject>
bool8 HashTable<THashable, TObject>::remove(const THashable& key_a,
					      TObject*& item_a) {

  // local variables
  //
  int32 index = 0;

  // when in SYSTEM mode the item passed in should not be null
  //
  if ((alloc_d == SYSTEM) && (item_a == (TObject*)NULL)) {
    return (Error::handle(name(), L"remove", ERR, __FILE__, __LINE__));
  }

  // when in USER mode the item should be null
  //
  if ((alloc_d == USER) && (item_a != (TObject*)NULL)) {
    return (Error::handle(name(), L"remove", ERR, __FILE__, __LINE__));
  }
  
  // look up the item of the given key
  //
  if (findKey(index, key_a)) {
    
    // if allocation mode is USER, just return the pointer
    //
    if (alloc_d == USER) {
      item_a = getCurrObject(index);
    }

    // for system mode delete the item memory
    //
    else {
      item_a->assign(*getCurrObject(index));
      delete getCurrObject(index);
    }

    // remove the node from the table
    //
    table_d(index).remove();
    
    // decrease the number of items
    //
    num_items_d = (int32)num_items_d - 1;

    // decrease the capacity if:
    //   1) the number of items is less than 10% of the capacity, and
    //   2) the current capacity is greater than the default, and
    //   3) the new capacity will still be greater than the default
    //
    // this allows for an intelligent lower bound on the halfing
    //
    int32 capacity = getCapacity();
    if (((int32)num_items_d < (LOAD_LOWER_BOUND * capacity)) &&
	(capacity >= (DEF_CAPACITY * 2))) {
      return rehash(capacity / 2);
    }

    // exit gracefully
    //
    return true;
  }
    
  // exit ungracefully: item not found
  //
  return false;
}

// method: remove
//
// arguments:
//  const THashable& key: (input) the key string of an item
//  
// return: a bool8 value indicating status
//
// this method removes the node of the given key.
//
template<class THashable, class TObject>
bool8 HashTable<THashable, TObject>::remove(const THashable& key_a) {

  // local variables
  //
  int32 index = 0;

  // look up the item of the given key
  //
  if (findKey(index, key_a)) {

    // if allocation mode is SYSTEM delete the object
    //
    if (alloc_d == SYSTEM) {
      delete getCurrObject(index);
    }

    // now remove the current linked node
    //
    table_d(index).remove();
    
    // decrease the number of items
    //
    num_items_d = (int32)num_items_d - 1;
    
    // decrease the capacity if:
    //   1) the number of items is less than 10% of the capacity, and
    //   2) the current capacity is greater than the default, and
    //   3) the new capacity will still be greater than the default
    //
    // this allows for an intelligent lower bound on the halfing
    //
    int32 capacity = getCapacity();
    if (((float64)num_items_d < (LOAD_LOWER_BOUND * capacity)) &&
	(capacity >= (DEF_CAPACITY * 2))) {
      return rehash(capacity / 2);
    }

    // exit gracefully
    //
    return true;
  }

  // exit ungracefully: item not found
  //
  return false;
}

//------------------------------------------------------------------------
//
// class-specific public methods:
//  hash table data access methods
//
//------------------------------------------------------------------------

// method: keys
//
// arguments:
//  Vector<THashable>& keys_vector: (output) all the key strings
//  
// return: a bool8 value indicating status
//
// this method gets all the key strings of this hash table
//
template<class THashable, class TObject>
bool8 HashTable<THashable, TObject>::keys(Vector<THashable>& keys_vector_a)
  const {

  // clear the input vector
  //
  keys_vector_a.clear(Integral::RESET);
  keys_vector_a.setLength(num_items_d);
  
  // get the capacity of the hash table
  //
  int32 capacity = getCapacity();
  
  // loop all the elements of the hash table
  // 
  for (int32 i = 0, index = 0; i < capacity; i++) {
    
    // loop over each linked list
    //
    for (bool8 more = const_cast<Type*>(this)->table_d(i).gotoFirst();
	 more; more = const_cast<Type*>(this)->table_d(i).gotoNext()) {
      
      // get the key of the current hash node, add it into the vector,
      // and increment the index
      //
      keys_vector_a(index++).assign(getCurrKey(i));
    }
  }
  
  // exit gracefully
  //
  return true;
}

// method: values
//
// arguments:
//  Vector<TObject>& values: (output) all the values
//  
// return: a bool8 value indicating status
//
// this method gets all the values of this hash table
//
template<class THashable, class TObject>
bool8
HashTable<THashable, TObject>::values(Vector<TObject>& values_a) const {
  
  // clear the input vector and set its length
  //
  values_a.clear(Integral::RESET);
  values_a.setLength(num_items_d);
  
  // get the capacity of the hash table
  //
  int32 capacity = getCapacity();
  
  // loop all the elements of the hash table
  // 
  for (int32 i = 0, index = 0; i < capacity; i++) {
    
    // loop each linked list
    //
    for (bool8 more = const_cast<Type*>(this)->table_d(i).gotoFirst();
	 more; more = const_cast<Type*>(this)->table_d(i).gotoNext()) {
      
      // get the value of the current hash node, add it into the vector,
      // and increment the index
      //
      values_a(index++).assign(*getCurrObject(i));
    }
  }
  
  // exit gracefully
  //
  return true;
}

//------------------------------------------------------------------------
//
// class-specific public methods:
//  hash table property methods
//
//------------------------------------------------------------------------

// method: containsValue
//
// arguments:
//  const TObject* value: (input) the value to be found
//
// return: a bool8 value indicating status
//
// this method determines if a value is contained in this hash table
// note the values may be not unique in the hash table
//
template<class THashable, class TObject>
bool8
HashTable<THashable, TObject>::containsValue(const TObject* value_a) const {
  
  // get the length of table_d
  //
  int32 capacity = getCapacity();
  
  // loop each linked list
  //
  for (int32 i = 0; i < capacity; i++) {
    
    for (bool8 more = const_cast<Type*>(this)->table_d(i).gotoFirst(); more;
	 more = const_cast<Type*>(this)->table_d(i).gotoNext()) {
      
      // if value is found, return true
      //
      if (getCurrObject(i)->eq(*value_a)) {
	return true;
      }
    }
  }  

  // value not found, return false
  //
  return false;
}

// method: getList
//
// arguments:
//  SingleLinkedList<HashNode>& arg: (output) all current hashnodes
//
// return: logical error status
//
// this method obtains a list of all hashnodes by concattenating all
// the lists together.
//
template<class THashable, class TObject>
bool8
HashTable<THashable, TObject>::getList(SingleLinkedList<HashNode>& arg_a) {

  // make sure the argument and the object are in USER mode
  //
  if (arg_a.getAllocationMode() != USER) {
    return Error::handle(name(), L"getList", Error::ARG, __FILE__, __LINE__);
  } 

  // prepare the argument by clearing it
  //
  arg_a.clear(Integral::RESET);
 
  // just copy a reference
  //
  int32 capacity = table_d.length();
  for (int32 i = 0; i < capacity; i++) {
    table_d(i).setAllocationMode(USER);
    arg_a.insert(table_d(i));
    table_d(i).setAllocationMode(SYSTEM);
  }

  // exit gracefully
  //
  return true;
}

// method: assignFromList
//
// arguments:
//  SingleLinkedList<HashNode>& arg: (input) all current hashnodes
//
// return: logical error status
//
// this method obtains a list of all hashnodes by concatenating all
// the lists together.
//
template<class THashable, class TObject>
bool8 HashTable<THashable, TObject>::assignFromList(SingleLinkedList<HashNode>& arg_a) {

  // make sure the argument and the object are in USER mode
  //
  if (arg_a.getAllocationMode() != USER) {
    return Error::handle(name(), L"assignFromList", Error::ARG, __FILE__,
			 __LINE__);
  }
  
  // loop through the data
  //
  for (bool8 more = arg_a.gotoFirst();
       more; more = arg_a.gotoNext()) {

    // determine where to hash the object
    //
    int32 index = arg_a.getCurr()->first().hash(getCapacity());

    // add the pair onto the given list
    //
    table_d(index).setAllocationMode(USER);
    table_d(index).insert(arg_a.getCurr());
    table_d(index).setAllocationMode(SYSTEM);
  }

  // set the length
  //
  num_items_d = arg_a.length();

  // exit gracefully
  //
  return true;
}

// method: rehash
//
// arguments:
//  int32 new_capacity: (input) new capacity to rehash to
//
// return: a bool8 value indicating status
//
// this method increases the capacity and reorganizes this hash table, in order
// to accommodate and access its items more efficiently.
//
template<class THashable, class TObject>
bool8 HashTable<THashable, TObject>::rehash(int32 new_capacity_a) {

  // local variables
  //
  int32 capacity = getCapacity();
  SingleLinkedList< HashNode > hashnodes_list(USER);

  // concatenate all lists onto a single list
  //
  getList(hashnodes_list);

  // clear out internal storage (making sure nodes aren't deleted)
  //
  for (int32 i = 0; i < capacity; i++) {
    table_d(i).setAllocationMode(USER);
    table_d(i).clear(Integral::RESET);
    table_d(i).setAllocationMode(SYSTEM);
  }

  // set the capacity
  //
  table_d.setLength(new_capacity_a);

  // place the objects back on the table
  //
  assignFromList(hashnodes_list);

  // exit gracefully
  //
  return true;
}

//---------------------------------------------------------------------------
//
// class-specific public methods:
//  hash table memory allocation methods
//
//---------------------------------------------------------------------------
  
// method: findKey
//
// arguments:
//  int32& index: (output) the list on which the item is found
//  const THashable& key: (input) the key to be found
//
// return: a bool8 value indicating status
//
// this method determines if a key is contained in this hash table
// and positions the item with the given key accordingly.
// note this is a private method, the user doesn't need to know the
// index of the linked list in the table_d
//
template<class THashable, class TObject>
bool8 HashTable<THashable, TObject>::findKey(int32& index_a,
					       const THashable& key_a) {

  // get the key index and loop that linked list
  //
  int32 index = key_a.hash(getCapacity());
  
  for (bool8 more = table_d(index).gotoFirst();
       more; more = table_d(index).gotoNext()) {
    
    if (key_a.eq(getCurrKey(index))) {

      // key is found, exit gracefully
      //
      index_a = index;
      return true;
    }
  }

  // key not found, return false
  //
  return false;
}

// end of include file
//
# endif
