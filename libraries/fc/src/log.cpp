#include <fc/log.hpp>
#include <fc/unique_lock.hpp>
#include <boost/thread/mutex.hpp>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#ifndef WIN32
#include <unistd.h>
#endif

#include <fc/filesystem.hpp>
#include <iostream>

namespace fc {
  const char* thread_name();
  void*       thread_ptr();

  const char* short_name( const char* file_name ) { 
    const char* end = file_name + strlen(file_name);
    --end;
    while( end >= file_name ) {
      if( *end == '/' || *end == '\\' ) {
        return end + 1;
      }
      --end;
    }
    return file_name;
  } 

  #ifdef WIN32 
    #define isatty _isatty
    #define fileno _fileno
  #endif // WIN32 

  void log( const char* color, const char* file_name, size_t line_num, 
            const char* method_name, const char* format, ... ) {
    #ifndef WIN32
    if(isatty(fileno(stderr)))
      fprintf( stderr, "\r%s",color);
    #endif
    fc::unique_lock<boost::mutex> lock(log_mutex());
 //   fc::string sname = fc::path(file_name).filename().generic_string();
    fprintf( stderr, "%-15s %-15s %-5d %-15s ", thread_name(), short_name(file_name), int(line_num), method_name );
    va_list args;
    va_start(args,format);
    vfprintf( stderr, format, args );
    va_end(args);
    #ifndef WIN32
    if (isatty(fileno(stderr)))
      fprintf( stderr, "%s", CONSOLE_DEFAULT );
    #endif
    fprintf( stderr, "\n" );
    fflush( stderr );
    return;
  }

  /** used to add extra fields to be printed (thread,fiber,time,etc) */
  void add_log_field( void (*f)( ) ) {
  }

  void remove_log_field( void (*f)( ) ) {
  }
}
