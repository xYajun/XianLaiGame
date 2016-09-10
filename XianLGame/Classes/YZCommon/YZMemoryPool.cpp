#include "YZMemoryPool.h"   
#include "YZLog.h"
  
namespace YZ {

YZMemPool::YZMemPool(std::size_t blockSize, int preAlloc, int maxAlloc)
	: m_blockSize(blockSize)
	, m_maxAlloc(maxAlloc)
	, m_allocated(preAlloc)  
{  
    if ( preAlloc < 0 || maxAlloc == 0 || maxAlloc < preAlloc )  
    {  
		YZLOG_DEBUG("YZMemPool::CMemPool parameter error. \n"); 
    }  
  
    int r = BLOCK_RESERVE;  
    if (preAlloc > r)  
        r = preAlloc;  
    if (maxAlloc > 0 && maxAlloc < r)  
        r = maxAlloc;  
    m_blocks.reserve(r);  
    for (int i = 0; i < preAlloc; ++i)  
    {  
        m_blocks.push_back(new char[m_blockSize]);  
    }  
}  
  
YZMemPool::~YZMemPool()  
{  
    for (BlockVec::iterator it = m_blocks.begin(); it != m_blocks.end(); ++it)  
    {  
        delete [] *it;  
    }  
}  

void* YZMemPool::get()
{  
	std::lock_guard <std::mutex> autoLock(_dataMutex);
    if (m_blocks.empty())  
    {  
        if (m_maxAlloc == 0 || m_allocated < m_maxAlloc)  
        {  
            ++m_allocated;  
            return new char[m_blockSize];  
        }  
        else  
        {  
            cout<<"YZMemPool::get YZMemPool exhausted."<<endl;  
            return (void *)NULL;  
        }  
    }  
    else
    {  
        char* ptr = m_blocks.back();  
        m_blocks.pop_back();
		//YZLOG_WARNING("YZMemPool Size  >> %d", m_blocks.size());
        return ptr;  
    }  
}  
  
void YZMemPool::release(void* ptr)
{  
    std::lock_guard <std::mutex> autoLock(_dataMutex);
    m_blocks.push_back(reinterpret_cast<char*>(ptr));
	//YZLOG_WARNING("YZMemPool Size  >> %d", m_blocks.size());
}   

}