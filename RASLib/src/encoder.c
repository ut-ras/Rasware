#include <RASLib/inc/encoder.h>

void InitializeEncoders(int enc0, int enc1) 
{	return; }

void PresetEncoderCounts(int enc0, int enc1)
{	return; }

signed long GetEncoderCount(int index)  
{
	return 0xFFFF; 
}

void GetEncoderCounts(signed long *enc0, signed long *enc1)
{  
	*enc0 = 0xFFFF; 
	*enc1 = 0xFFFF;
	return; 
}
