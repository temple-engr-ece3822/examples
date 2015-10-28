// file: $(AUTOEEG)/util/gen_feats/gen_feats.cc
//
// this is the driver program for feature extraction.
//

// modified:
//  20140223 (JP): upgraded to the new Edf feature file I/O
//  20140125 (JP): revamped the parameter file parsing; made the
//                 front end implementations more modular
//  20131120 (JP): initial version
//

// local include files
//
#include <Fe.h>

// fe: feature extraction
//
// This is a driver program that reads EDF files and generates
// feature files.
//
int main(int argc, const char** argv) {

  // declare local variables
  //
  long status = 0;

  // create an EDF object
  //
  Edf edf(Edf::LEVEL_NONE);

  // create a front end object
  //
  Fe fe(Edf::LEVEL_NONE);

  // make sure there are enough arguments and that the first
  // argument is preceded by "-p"
  //
  if ((argc <= 2) || ((strcmp(argv[1], "-p") != 0))) {
    fprintf(stdout, "usage: gen_feats -p pfile x1.list x2.list ...\n");
    exit(1);
  }

  // load the parameter file
  //
  if (!fe.load_parameters((char*)argv[2])) {
    fprintf(stdout, "**> error parsing the parameter file (%s)\n", argv[2]);
    exit(1);
  }

  // allow the directory to be overridden:
  //  check explicitly for an additional "-d" argument
  //
  long arg_ind = 3;
  if (strcmp(argv[3], "-d") == 0) {
    arg_ind = 5;
    fe.set_output_directory((char*)argv[4]);
  }

  // propagate the debug level
  //
  edf.set_debug(fe.get_debug());

  // display an informational message
  //
  fprintf(stdout, "beginning argument processing...\n");

  // main processing loop: loop over all input filenames
  //
  long num_files_att = 0;
  long num_files_proc = 0;
  char feat_fname[Edf::MAX_LSTR_LENGTH];

  for (int i=arg_ind; i<argc; i++) {

    // if it is an edf file, process it
    //
    if (edf.is_edf((char*)argv[i])) {

      // display a status message
      //
      num_files_att++;
      fprintf(stdout, "  %6ld: %s\n", num_files_att, (char*)argv[i]);

      // compute features
      //
      if (fe.compute(feat_fname, (char*)argv[i])) {
	num_files_proc++;
	fprintf(stdout, "          %s\n", feat_fname);
      }
      else {
	fprintf(stdout, "  **> gen_feats: error generating features\n");
      }
    }

    // else: treat it as a file list
    //
    else {

      // dsiplay an informational message
      //
      fprintf(stdout, " opening list %s...\n", (char*)argv[i]);

      // open the list
      //
      FILE* fp = fopen(argv[i], "r");
      if (fp == (FILE*)NULL) {
	fprintf(stdout, " **> gen_feats: error opening file list (%s)\n",
		argv[i]);
	exit(1);
      }

      // loop over all files
      //
      char edf_fname[Edf::MAX_LSTR_LENGTH];
      while (fscanf(fp, "%s", edf_fname) == 1) {

	// display a status message
	//
	num_files_att++;
	fprintf(stdout, "  %6ld: %s\n", num_files_att, edf_fname);

	// compute features
	//
	if (fe.compute(feat_fname, edf_fname)) {
	  num_files_proc++;
	  fprintf(stdout, "          %s\n", feat_fname);
	}
	else {
	  fprintf(stdout, "  **> gen_feats: error generating features\n");
	}
      }

      // close the list
      //
      fclose(fp);
    }
  }    

  // display the results
  //
  fprintf(stdout, "processed %ld out of %ld files successfully\n",
	  num_files_proc, num_files_att);
  
  // exit gracefully
  //
  exit(status);
}
