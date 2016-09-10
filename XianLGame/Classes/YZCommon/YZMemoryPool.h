#ifndef MemoryPool_h__
#define MemoryPool_h__

#include <vector>
#include <iostream>
#include <mutex>
  
using namespace std;  
  
/* 
    在内存池中分配固定大小的内存块 
 
    该类的目的是加速内存分配速度，并且减少因重复分配相同 
    内存时产生的内存碎片，比如在服务器应用程序中。 
*/  
namespace YZ {

class YZMemPool
{  
public:  
  
    //创建大小为blockSize的内存块，内存池数目为预分配的数目preAlloc   
    YZMemPool(std::size_t blockSize, int preAlloc = 0, int maxAlloc = 0);  
  
    ~YZMemPool();  
  
    //获取一个内存块。如果内存池中没有足够的内存块，则会自动分配新的内存块   
    //如果分配的内存块数目达到了最大值，则会返回一个异常   
    void* get();  
  
    //释放当前内存块，将其插入内存池   
    void release(void* ptr);  
  
    //返回内存块大小   
    std::size_t blockSize() const;  
  
    //返回内存池中内存块数目   
    int allocated() const;  
  
    //返回内存池中可用的内存块数目   
    int available() const;  
  
private:  
    YZMemPool();  
    YZMemPool(const YZMemPool&);  
    YZMemPool& operator = (const YZMemPool&);  
  
    enum  
    {  
        BLOCK_RESERVE = 32  
    };  
  
    typedef std::vector<char*> BlockVec;  
  
    std::size_t m_blockSize;  
    int         m_maxAlloc;  
    int         m_allocated;  
    BlockVec    m_blocks;
	std::mutex	_dataMutex;
};  
  
inline std::size_t YZMemPool::blockSize() const  
{  
    return m_blockSize;  
}  
  
  
inline int YZMemPool::allocated() const  
{  
    return m_allocated;  
}  
  
  
inline int YZMemPool::available() const  
{  
    return (int) m_blocks.size();  
}  
  
}

#endif // MemoryPool_h__
