#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/device/file.hpp>
 
#include <iostream>
#include <sstream>

int main()
{
	try{
		
		std::stringstream ss_comp;
		ss_comp << "hellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohello";
		boost::iostreams::filtering_ostream out;
		out.push(boost::iostreams::gzip_compressor());
		out.push(boost::iostreams::file_sink("test.txt"));		//压缩到文件中
		boost::iostreams::copy(ss_comp, out);
 
		std::stringstream ss_decomp;
		boost::iostreams::filtering_istream in;
		in.push(boost::iostreams::gzip_decompressor());
		in.push(boost::iostreams::file_source("test.txt"));		//从文件中解压
		boost::iostreams::copy(in, ss_decomp);
		std::cout << "decompressor data:" << ss_decomp.str() << std::endl;
	}
	catch(std::exception& e)
	{
		std::cout << "exception:" << e.what() << std::endl;
	}
	catch(...)
	{
		std::cout << "unknown exception." << std::endl;
	}
	return 0;
}
