#include <fc/interprocess/mmap_struct.hpp>
#include <fc/filesystem.hpp>
#include <fc/io/fstream.hpp>
#include <string.h>

namespace fc
{
   namespace detail
   {
      void mmap_struct_base::open( const fc::path& file, size_t s, bool create )
      {
         if( !fc::exists( file ) || fc::file_size(file) != s )
         {
            fc::ofstream out( file );
            char buffer[1024];
            memset( buffer, 0, sizeof(buffer) );

            size_t bytes_left = s;
            while( bytes_left > 0 )
            {
               size_t to_write = std::min<size_t>(bytes_left, sizeof(buffer) );
               out.write( buffer, to_write );
               bytes_left -= to_write;
            }
         }
         _file_mapping.reset( new fc::file_mapping( file.generic_string().c_str(), fc::read_write ) );
         _mapped_region.reset( new fc::mapped_region( *_file_mapping, fc::read_write, 0, s ) );
      }
   } // namespace fc

} // namespace fc
