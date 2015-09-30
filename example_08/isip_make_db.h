// file: $isip/util/speech/isip_make_db/isip_make_db.h
// version: $Id: isip_make_db.h 8816 2002-11-19 20:10:22Z parihar $
//
// this is the header file for the isip_make_db utility
//
// make sure the file is included only once
//
#ifndef ISIP_UTIL_MAKE_DB
#define ISIP_UTIL_MAKE_DB

// isip include files
//
#include <CommandLine.h>
#include <AudioDatabase.h>
#include <TranscriptionDatabase.h>

static const String PROG_NAME(L"isip_make_db");
static const String OPTION_DATABASE_TYPE(L"db");
static const String OPTION_AUDIO_LIST(L"audio");
static const String OPTION_TRANSCRIPTION_LIST(L"transcription");
static const String OPTION_TRANSCRIPTION_LEVEL(L"level");
static const String OPTION_DATABASE_NAME(L"name");
static const String OPTION_DATABASE_FORMAT(L"type");

static const String DATABASE_TYPE_AUDIO(L"audio");
static const String DATABASE_TYPE_TRANSCRIPTION(L"transcription");
static const String DATABASE_TYPE_BOTH(L"both");
static const String DATABASE_FORMAT_TEXT(L"text");
static const String DATABASE_FORMAT_BINARY(L"binary");

static const String OPTION_DEBUG_LEVEL(L"debug_level");
static const String OPTION_LOG_FILE(L"log_file");

static const String EMPTY(String::NULL_STRING);
static const String LOG_FILE_DEFAULT(L"-");
static const Integral::DEBUG DEBUG_LEVEL_DEFAULT=Integral::NONE;

// end of file
//
#endif
