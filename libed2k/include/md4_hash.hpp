
#ifndef __LIBED2K_MD4_HASH__
#define __LIBED2K_MD4_HASH__

#include <boost/cstdint.hpp>
#include <boost/assert.hpp>
#include <string>
#include <sstream>
#include <string.h>
#include <typeinfo>

namespace libed2k{

    const size_t MD4_HASH_SIZE = 16;

    /**
      * this class simple container for hash array
     */
    class md4_hash
    {
    public:
    	typedef boost::uint8_t md4hash_container[MD4_HASH_SIZE];
    	static const md4hash_container m_emptyMD4Hash;

    	md4_hash()
    	{
    	    clear();
    	}

    	md4_hash(const std::string strHash)
    	{
    	    fromString(strHash);
    	}

    	bool operator==(const md4_hash& hash) const
        {
    	    return (memcmp(m_hash, hash.m_hash, MD4_HASH_SIZE) == 0);
        }

    	bool operator<(const md4_hash& hash) const
    	{
    	    return (memcmp(m_hash, hash.m_hash, MD4_HASH_SIZE) == -1);
    	}

    	bool operator>(const md4_hash& hash) const
    	{
    	     return (memcmp(m_hash, hash.m_hash, MD4_HASH_SIZE) == 1);
    	}

    	void clear()
    	{
    	    memset(m_hash, 0, MD4_HASH_SIZE);
    	}

    	void fromString(const std::string& strHash)
    	{
            BOOST_ASSERT(strHash.size() == MD4_HASH_SIZE*2);
    	    clear();

    	    for ( size_t i = 0; i < MD4_HASH_SIZE * 2; i++ )
    	    {
    	        unsigned char word = strHash[i];

                if ((word >= '0') && (word <= '9'))
                {
                    word -= '0';
                }
                else if ((word >= 'A') && (word <= 'F'))
                {
                    word -= 'A' - 10;
                }
                else if (word >= 'a' && word <= 'f')
                {
                    word -= 'a' - 10;
                }
                else
                {
                     throw std::bad_cast();
                }

                if (i % 2 == 0)
                {
                    m_hash[i/2] = word << 4;
                }
                else
                {
                    m_hash[i/2] += word;
                }
    	    }
    	}

    	std::string toString() const
    	{
    	    std::stringstream ss;

    	    for (size_t i = 0; i < MD4_HASH_SIZE; i++)
    	    {
    	        ss << std::uppercase << std::hex << (m_hash[i] >> 4) << (m_hash[i] & 0x0F);
    	    }

    	    return (ss.str());
    	}

    	boost::uint8_t operator[](size_t n) const
    	{
    	    BOOST_ASSERT(n < MD4_HASH_SIZE);
    	    return (m_hash[n]);
    	}
    private:
    	md4hash_container   m_hash;
    };

};



#endif