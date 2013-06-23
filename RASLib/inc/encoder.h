#define ENCODER_0	1
#define ENCODER_1	0

void InitializeEncoders(int enc0, int enc1); 

void PresetEncoderCounts(int enc0, int enc1);

signed long GetEncoderCount(int index);  

void GetEncoderCounts(signed long *enc0, signed long *enc1); 
