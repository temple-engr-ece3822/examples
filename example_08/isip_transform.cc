// file: $isip/util/speech/isip_transform/isip_transform.cc
// version: $Id: isip_transform.cc 10626 2006-08-24 16:05:40Z dom5 $
//

// isip include files
//
#include "isip_transform.h"

// utility: isip_transform: 
//
// this is a general data transformation utility. it can be used for
// feature extraction, feature translation, or audio conversions.
//
int main(int argc, const char** argv) {

  // declare primary objects
  //
  Sdb sdb;
  Sdb sdb_params;
  CommandLine cmdl(sdb);
  Vector<String> vector_param;
  Vector<FrontEnd> vector_fe;
  
  // declare local variables used to handle command line options
  //    
  DebugLevel debug_level;
  Filename lfile;
  Filename pfile;
  File::TYPE type;
  DebugLevel verbosity;

  // declare miscellaneous local variables
  //
  String cmdl_value;
  Filename input_file;

  //---------------------------------------------------------------------------
  //
  // configure and parse the command line:
  //  add only the options that are not included by default.
  // 
  //---------------------------------------------------------------------------

  // set up messages
  //
  cmdl.setUsage(
#include "usage_message.text"
                );
  cmdl.setHelp(
#include "help_message.text"
               );
  
  cmdl.setIdent("$Revision: 10626 $",
		"$Name: $",
		"$Date: 2006-08-24 11:05:40 -0500 (Thu, 24 Aug 2006) $");
  
  // set command line options
  //
  cmdl.addOptionParam(debug_level, CommandLine::DEF_PARAM_DEBUG);
  cmdl.addOptionParam(lfile, CommandLine::DEF_PARAM_LFILE,
		      CommandLine::DEF_LFILE_NAME);
  cmdl.addOptionParam(pfile, CommandLine::DEF_PARAM_PFILE,
		      CommandLine::DEF_EMPTY);
  cmdl.addOptionParam(type, CommandLine::DEF_PARAM_TYPE);
  cmdl.addOptionParam(verbosity, CommandLine::DEF_PARAM_VERBOSITY);
  
  // parse the command line
  //
  cmdl.parse(argc,argv);

  // check for required options and arguments
  //
  if ((pfile.length() == (int32)0) ||
      (!sdb.gotoFirst())) {
    cmdl.printUsage();
  }

  // configure the console
  //
  if (lfile.ne(CommandLine::DEF_LFILE_NAME)) {
    Console::open(lfile);
  }
  
  // echo the command line
  //
  if (verbosity > Integral::BRIEF) {
    cmdl.printCommandLine();
    cmdl.printVersion();
  }
  
  //---------------------------------------------------------------------------
  //
  // detect multiple recipes:
  //  transforms allows each input file to be processed through
  //  multiple recipes.
  //
  //---------------------------------------------------------------------------
  
  // determine if the recipe file is a file list
  //
  if (Sdb::isSdb(pfile)) {
    sdb_params.append(pfile, true);
  }
  else {
    sdb_params.append(pfile);
  }

  //---------------------------------------------------------------------------
  //
  // configure a vector of front ends
  //
  //---------------------------------------------------------------------------
  
  // fetch the number of front-ends and allocate space
  //
  int32 num_fe = sdb_params.length();
  if (verbosity >= Integral::BRIEF) {
    String output;
    output.assign(L"total recipes ");
    output.concat(num_fe);
    Console::putNoWrap(output);
  }

  vector_param.setLength(num_fe);
  vector_fe.setLength(num_fe);

  // initialize Sdb processing of the parameter files
  //
  if (!sdb_params.gotoFirst()) {
    Error::handle(PROG_NAME, L"main", Sdb::ERR, __FILE__, __LINE__);
  }

  // configure one front end at a time
  //
  for (int32 i = 0; i < num_fe; i++) {
    
    // get the next name
    //
    sdb_params.getName(pfile);
    vector_param(i).assign(pfile);
    
    if (verbosity >= Integral::BRIEF) {
      String msg(L"loading parameter file: ");
      msg.concat(pfile);
      Console::put(msg);
    }
    
    if (!vector_fe(i).load(pfile)) {
      String msg(L"unable to open parameter file (\n");
      msg.concat(pfile);
      msg.concat(L")");
      Console::put(msg);
      Error::handle(PROG_NAME, L"main",
		    Error::NO_PARAM_FILE, __FILE__, __LINE__);
    }
    
    // advance sdb
    //
    sdb_params.gotoNext();
    
    // set the debug level and verbosity
    //
    vector_fe(i).setDebug(debug_level);
    vector_fe(i).setVerbosity(verbosity);
      
    //-------------------------------------------------------------------------
    //
    // override any parameter file parameters that were set
    // by command line options.
    //
    //-------------------------------------------------------------------------

    if (cmdl.getValue(cmdl_value, CommandLine::DEF_PARAM_DIRECTORY)) {
      vector_fe(i).setOutputDirectory(cmdl_value);
    }
    
    if (cmdl.getValue(cmdl_value, CommandLine::DEF_PARAM_EXTENSION)) {
      vector_fe(i).setOutputExtension(cmdl_value);
    }
    
    if (cmdl.getValue(cmdl_value, CommandLine::DEF_PARAM_PRESERVE)) {
      int32 preserve;
      cmdl_value.get(preserve);
      vector_fe(i).setOutputPreserve(preserve);
    }
    
    if (cmdl.getValue(cmdl_value, CommandLine::DEF_PARAM_SUFFIX)) {
      vector_fe(i).setOutputSuffix(cmdl_value);
    }
    
    if (cmdl.getValue(cmdl_value, CommandLine::DEF_PARAM_TYPE)) {
      vector_fe(i).setOutputType(type);
    }
  }
  
  //---------------------------------------------------------------------------
  //
  // main processing loop
  //
  //---------------------------------------------------------------------------

  // increase the indention
  //
  if (verbosity >= Integral::BRIEF) {
    Console::increaseIndention();
  }

  // loop over all files: count the number of files processed
  //
  int32 num_files = 0;
  int32 num_processed = 0;
  sdb.gotoFirst();

  do {

    // fetch the next file
    //
    sdb.getName(input_file);
    num_files++;
    
    // display a status message
    //
    if (verbosity >= Integral::BRIEF) {
      String output(L"processing file ");
      output.concat(num_files);
      output.concat(L": ");
      output.concat(input_file);
      Console::putNoWrap(output);
    }
    
    // increase the indention
    //
    if (verbosity >= Integral::BRIEF) {
      Console::increaseIndention();
    }
    
    // loop over all front ends
    //
    for (int32 j = 0; j < num_fe; j++) {
      
      // display a status message
      //
      if (verbosity >= Integral::BRIEF) {
	String output(L"processing pfile ");
	output.concat(j+1);
	output.concat(L": ");
	output.concat(vector_param(j));
	Console::putNoWrap(output);
      }
      
      // process the data
      //
      if (!vector_fe(j).run(input_file)) {
	Error::handle(PROG_NAME, L"main", FrontEnd::ERR, __FILE__, __LINE__);
      }      
    }
    
    // increment the number of files successfully processed
    //
    num_processed++;

    // decrease the indention
    //
    if (verbosity >= Integral::BRIEF) {
      Console::decreaseIndention();
    }    
  } while (sdb.gotoNext());
  
  // decrease the indention and report results
  //
  if (verbosity >= Integral::BRIEF) {
    Console::decreaseIndention();
    cmdl.printFinalReport(num_processed, num_files);
  }
  
  // close the log file
  //
  if (lfile.ne(CommandLine::DEF_LFILE_NAME)) {
    Console::close();
  }
  
  // exit gracefully
  //
  Integral::exit();
}
