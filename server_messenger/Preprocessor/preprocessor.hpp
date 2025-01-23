#pragma once
#include <iomanip>
#include <iostream>
using namespace std;

#define MIN_NUMBER_LENGTH 11
#define MAX_NUMBER_LENGTH 13

#define PHONE_NUMBER_DEBUG		true    // print debugging information in class phone number
#define ACCOUNT_DEBUG			true	// print debugging information in classes account
#define SERVER_DEBUG			true	// print debugging information in class server
#define ACCOUNT_FACTORY_DEBUG	true	// print debugging information in class accounts factory
#define ERROR_DEBUG				true    // print error	   information in all classes
#define USER_MESSAGE_DEBUG		true	// print user message
#define EXCEPTIONS_DEBUG		true	// print exceotuions in all classes

#define FILL 25

//========Phone number========
#ifdef _DEBUG
#if (PHONE_NUMBER_DEBUG == true)
#define phDEBUG_LOG(debug_setup, msg) clog << "\033[38;5;250;48;5;17m" << setw(FILL) << left << setfill('.') << debug_setup << msg << "\033[0m" << endl;
#else
#define phDEBUG_LOG(debug_setup, msg)
#endif // PHONE_NUMBER_DEBUG
#else
#define phDEBUG_LOG(debug_setup, msg)
#endif // _DEBUG

//========Account========
#ifdef _DEBUG
#if (ACCOUNT_DEBUG == true)
#define acDEBUG_LOG(debug_setup, msg) clog << "\033[38;5;250;48;5;17m" << setw(FILL) << left << setfill('.') << debug_setup << msg << "\033[0m" << endl;
#else
#define acDEBUG_LOG(debug_setup, msg)
#endif // PHONE_NUMBER_DEBUG
#else
#define acDEBUG_LOG(debug_setup, msg)

#endif // _DEBUG

//========Server========
#ifdef _DEBUG
#if (SERVER_DEBUG == true)
#define svDEBUG_LOG(debug_setup, msg) clog << "\033[38;5;250;48;5;17m" << setw(FILL) << left << setfill('.') << debug_setup << msg << "\033[0m" << endl;
#else
#define svDEBUG_LOG(debug_setup, msg)
#endif // PHONE_NUMBER_DEBUG
#else
#define svDEBUG_LOG(debug_setup, msg)
#endif // _DEBUG

//========Account factory========
#ifdef _DEBUG
#if (ACCOUNT_FACTORY_DEBUG == true)
#define afDEBUG_LOG(debug_setup, msg) clog << "\033[38;5;250;48;5;17m" << setw(FILL) << left << setfill('.') << debug_setup << msg << "\033[0m" << endl;
#else
#define afDEBUG_LOG(debug_setup, msg)
#endif // PHONE_NUMBER_DEBUG
#else
#define afDEBUG_LOG(debug_setup, msg)
#endif // _DEBUG

//========Account base========
#ifdef _DEBUG
#if (ACCOUNT_FACTORY_DEBUG == true)
#define abDEBUG_LOG(debug_setup, msg) clog << "\033[38;5;250;48;5;17m" << setw(FILL) << left << setfill('.') << debug_setup << msg << "\033[0m" << endl;
#else
#define abDEBUG_LOG(debug_setup, msg)
#endif // PHONE_NUMBER_DEBUG
#else
#define abDEBUG_LOG(debug_setup, msg)
#endif // _DEBUG

//========Chat manager========
#ifdef _DEBUG
#if (ACCOUNT_FACTORY_DEBUG == true)
#define cmDEBUG_LOG(debug_setup, msg) clog << "\033[38;5;250;48;5;17m" << setw(FILL) << left << setfill('.') << debug_setup << msg << "\033[0m" << endl;
#else
#define cmDEBUG_LOG(debug_setup, msg)
#endif // PHONE_NUMBER_DEBUG
#else
#define cmDEBUG_LOG(debug_setup, msg)
#endif // _DEBUG

//========Thread pool========
#ifdef _DEBUG
#if (ACCOUNT_FACTORY_DEBUG == true)
#define tpDEBUG_LOG(debug_setup, msg) clog << "\033[38;5;250;48;5;17m" << setw(FILL) << left << setfill('.') << debug_setup << msg << "\033[0m" << endl;
#else
#define tpDEBUG_LOG(debug_setup, msg)
#endif // PHONE_NUMBER_DEBUG
#else
#define tpDEBUG_LOG(debug_setup, msg)
#endif // _DEBUG

//========Error========
#ifdef _DEBUG
#if (ERROR_DEBUG == true)
#define ERROR_LOG(debug_setup, msg) clog << "\033[38;5;253;48;5;1m" << setw(FILL) << left << setfill('.') << debug_setup << msg << "\033[0m" << endl;
#else
#define ERROR_LOG(debug_setup, msg)
#endif // PHONE_NUMBER_DEBUG
#else
#define ERROR_LOG(debug_setup, msg)
#endif // _DEBUG

//========EXCEPTIONS========
#ifdef _DEBUG
#if (EXCEPTIONS_DEBUG == true)
#define EXCEPTIONS_LOG(debug_setup, msg)  clog << "\033[38;5;253;48;5;52m" << setw(FILL) << left << setfill('.') << debug_setup << msg << "\033[0m" << endl;
#else
#define EXCEPTIONS_LOG(debug_setup, msg)
#endif // PHONE_NUMBER_DEBUG
#else
#define EXCEPTIONS_LOG(debug_setup, msg)
#endif // _DEBUG


//========User message========
#ifdef _DEBUG
#if (USER_MESSAGE_DEBUG == true)
#define USER_MESSAGE(name, msg)  clog << "\033[38;5;15;48;5;238m" << setw(FILL) << left << setfill('.') << name << msg << "\033[0m" << endl;
#else
#define USER_MESSAGE(name, msg)
#endif // PHONE_NUMBER_DEBUG
#else
#define USER_MESSAGE(name, msg)
#endif // _DEBUG