// file: $isip/util/speech/isip_make_db/isip_make_db.cc
// version: $Id: isip_make_db.cc 10615 2006-08-24 16:01:56Z dom5 $
//
// isip include files
//
#include "isip_make_db.h"

// isip_database_creator: utility that creates the
// audio/transcription/both database
//
int main(int argc, const char** argv) {

  // setup the command line
  //
  CommandLine cmdl;  
  cmdl.setUsage(
#include "usage_message.text"
                );
  cmdl.setHelp(
#include "help_message.text"
               );

  cmdl.setIdent("$Revision: 10615 $",
		"$Name: $",
		"$Date: 2006-08-24 11:01:56 -0500 (Thu, 24 Aug 2006) $");
  
  // add a command line option for the database type
  //
  String database_type;
  cmdl.addOptionParam(database_type, OPTION_DATABASE_TYPE,
		      DATABASE_TYPE_AUDIO);      

  // add a command line option for the audio list file
  //
  Filename audio_list;
  cmdl.addOptionParam(audio_list, OPTION_AUDIO_LIST, (Filename)EMPTY);

  // add a command line option for the transcription file
  //
  Filename transcription_list;
  cmdl.addOptionParam(transcription_list, OPTION_TRANSCRIPTION_LIST, (Filename)EMPTY);    

  // add a command line option for the transcription level
  //
  String transcription_level;
  cmdl.addOptionParam(transcription_level, OPTION_TRANSCRIPTION_LEVEL, EMPTY);

  // add a command line option for the database name
  //
  String database_name;
  cmdl.addOptionParam(database_name, OPTION_DATABASE_NAME, EMPTY);      

  // add a command line option for format of output database file
  //
  String database_format;
  cmdl.addOptionParam(database_format, OPTION_DATABASE_FORMAT,
		      DATABASE_FORMAT_TEXT);
  
  // add a command line option for the debug level
  //
  DebugLevel debug_level;
  cmdl.addOptionParam(debug_level, OPTION_DEBUG_LEVEL,
		      DEBUG_LEVEL_DEFAULT);

  // add a command line option for the log file
  //
  Filename log_file;
  cmdl.addOptionParam(log_file, OPTION_LOG_FILE, (Filename)LOG_FILE_DEFAULT);

  // parse the command line
  //
  if (!cmdl.parse(argc, argv)) {
    cmdl.printUsage();
  };

  // open the log file
  //
  if (log_file.ne(L"-")) {
    Console::open(log_file);
    Console::setLineWrap(File::NO_WRAP);
  }

  // verify that the user has specified the format of the database
  //
  if (!((database_format.eq(DATABASE_FORMAT_TEXT))
	||(database_format.eq(DATABASE_FORMAT_BINARY)))) {
    cmdl.printUsage();
  };
  
  // set the file format
  //
  File::TYPE file_format = File::TEXT;
  if (database_format.eq(DATABASE_FORMAT_BINARY)) {
    file_format = File::BINARY;
  }

  // verify that the user has specified the name of the database
  //
  if (database_name.eq(EMPTY)) {
    cmdl.printUsage();
  };
  
  // get number of unused arguments (this might include either
  // audio_list.text or trans.text or both
  //
  long num_arg = (long)cmdl.numArguments();
  
  // verify that the user has specified a database type
  //
  if (!database_type.eq(EMPTY)) {
    database_type.toLower();
    
    // database type: audio
    //
    if (database_type.eq(DATABASE_TYPE_AUDIO)) {

      // verify that the audio list file has been specified
      //
      if(audio_list.eq((Filename)EMPTY)) {
	cmdl.printUsage();
      };

      // declare the audio database object
      //
      AudioDatabase audio_database;

      // set the debug level
      //
      audio_database.setDebug(debug_level);

      // set the name of the audio database
      //
      audio_database.setDataBaseName(database_name);
	
      // set the debug level
      //
      audio_database.setDebug(debug_level);
      
      // generate the audio database
      //
      Vector<Filename> tmp_vec;
      audio_database.load(audio_list, tmp_vec);

      // verify that the output audio database file has been specified as the
      // argument without an option
      //      
      String audio_arg;
      cmdl.getArgument(audio_arg, 0);
      Filename output_audio_file(audio_arg);      
      if((audio_arg.eq(EMPTY)) ||( num_arg != 1)) {
	cmdl.printUsage();
      };
      
      // write the audio database to a file
      //
      Sof tmp_sof;
      tmp_sof.open(output_audio_file, File::WRITE_ONLY, file_format);
      audio_database.store(tmp_sof, (long)0, tmp_vec);
      tmp_sof.close();
    }
    
    // database type: transcription
    //
    else if (database_type.eq(DATABASE_TYPE_TRANSCRIPTION)) {

      // verify that the user has specified the level of the transcription
      //
      if (transcription_level.eq(EMPTY)) {
	cmdl.printUsage();
      };
      
      // verify that the transcription file has been specified
      //      
      if(transcription_list.eq((Filename)EMPTY)) {
	cmdl.printUsage();
      };
      
      // declare the transcription database object
      //
      TranscriptionDatabase trans_database;

      // set the name of the transcription database
      //
      trans_database.setDataBaseName(database_name);

      // set the debug level
      //
      trans_database.setDebug(debug_level);

      // verify that the output audio database file has been specified as the
      // argument without an option
      //      
      String trans_arg;
      cmdl.getArgument(trans_arg, 0);
      Filename output_trans_file(trans_arg);      
      if((trans_arg.eq(EMPTY)) ||( num_arg != 1)) {
	cmdl.printUsage();
      };

      // read the transcriptions and write the transcription database
      // to a file
      //
      Sof tmp_sof;
      tmp_sof.open(output_trans_file, File::WRITE_ONLY, file_format);
      trans_database.store(transcription_list, database_name,
			   transcription_level, tmp_sof, (long)0);
      tmp_sof.close();
    }

    // database type: both
    //
    else if (database_type.eq(DATABASE_TYPE_BOTH)) {

      // verify that the user has specified the level of the transcription
      //
      if (transcription_level.eq(EMPTY)) {
	cmdl.printUsage();
      };
      
      // verify that the audio list file and the transcription list
      // file has been specified
      //
      if((audio_list.eq((Filename)EMPTY)) ||
	 (transcription_list.eq((Filename)EMPTY))) {
	cmdl.printUsage();
      };

      // declare the audio and transcription database objects
      //
      AudioDatabase audio_database;
      TranscriptionDatabase trans_database;
      
      // set the debug level
      //
      audio_database.setDebug(debug_level);
      trans_database.setDebug(debug_level);

      // set the name of the audio databases
      //
      audio_database.setDataBaseName(database_name);
      trans_database.setDataBaseName(database_name);
	      
      // generate the audio and transcription databases
      //
      Vector<Filename> tmp_vec;
      audio_database.load(audio_list, tmp_vec);

      // verify that the output audio database file and the output
      // transcription dabase file has been specified as the argument
      // without an option
      //      
      String audio_arg;
      String trans_arg;
      cmdl.getArgument(audio_arg, 0);
      cmdl.getArgument(trans_arg, 1);
      Filename output_audio_file(audio_arg);
      Filename output_trans_file(trans_arg);
      if((audio_arg.eq(EMPTY)) || (trans_arg.eq(EMPTY)) || ( num_arg != 2)) {
	cmdl.printUsage();
      }

      // write the audio database to a file
      //
      Sof tmp_sof;
      tmp_sof.open(output_audio_file, File::WRITE_ONLY, file_format);
      audio_database.store(tmp_sof, (long)0, tmp_vec);
      tmp_sof.close();
      
      // read the transcriptions and write the transcription database
      // to a file
      //
      tmp_sof.open(output_trans_file, File::WRITE_ONLY, file_format);
      trans_database.store(transcription_list, database_name,
			   transcription_level, tmp_sof, (long)0);
      tmp_sof.close();
    }
    
    // database type: unknown
    //    
    else {
      cmdl.printUsage();
    }
  }

  else {
    cmdl.printUsage();
  }
  
  // exit gracefully
  //
  return Integral::exit();
}
