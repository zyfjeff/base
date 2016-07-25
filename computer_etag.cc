#include <iostream>
#include <string>
#include <openssl/md5.h>
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>
#include <fstream>

using namespace std;

static std::string getContentMd5(const char* data,size_t size)
{
        std::ostringstream content_streams;
        unsigned char md[16] = {'\0'};
        
        MD5_CTX ctx;
        MD5_Init(&ctx);
        MD5_Update(&ctx,data,size);
        MD5_Final(md,&ctx);
        
        for(int i = 0; i < 16;++i) {
                content_streams << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(md[i]);
        }
        return content_streams.str();
}


// Reference nginx etag generate algorithm: etag=size + "_" + last_modtifiy_time
static std::string calculateEtag(const std::string &filename)
{
        ifstream file(filename,ios::binary);
        istreambuf_iterator<char> beg(file),end;
        string filecontent(beg,end);
        file.close();
        std::cout << filecontent << std::endl;
        return getContentMd5(filecontent.c_str(),filecontent.size());
/*
      std::string ret;
      if(access(filename.c_str(),F_OK) == 0) {
        //calculate etag
        struct stat file_stat;
        if(!stat(filename.c_str(),&file_stat)) {
            
            ostringstream istr;
            istr << file_stat.st_size;
            istr << "_";
            istr << file_stat.st_mtime;
            ret = istr.str();
        }  else {
            std::cout << "calculate " << filename << "etag error" << std::endl;
        }
    }
    if (!ret.empty())
        ret = getContentMd5(ret.c_str(),ret.size());
*/
}

int main()
{
    string filename = "test.conf";
    std::cout << calculateEtag(filename) << std::endl;
}
