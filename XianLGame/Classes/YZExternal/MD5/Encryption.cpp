
#include <string>
#include <vector>
#include "base/CCData.h"
#include "xxtea.h"
NS_CC_BEGIN
Data decryptData(Data data)
{
	char key[] = { 'G', '9', 'w', '0', 'B', 'A', 'Q', 'E', 'F', 'A', 'A', 'S', 'C', 'A', 'l', '8', 'w', 'g', 'g' };
	std::string _xxteaKey=std::string(key);
	if (_xxteaKey.empty())
	{
		return data;
	}

	unsigned char tag = *(data.getBytes());
	if (tag == 0xff)
	{
		xxtea_long dLength = 0;
		unsigned char* dData = xxtea_decrypt(data.getBytes() + 1, data.getSize() - 1, (unsigned char*)_xxteaKey.c_str(), xxtea_long(strlen(_xxteaKey.c_str())), &dLength);
		data.copy(dData, dLength);
		free(dData);
	}

	return data;
}
NS_CC_END
