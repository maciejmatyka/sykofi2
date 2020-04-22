
const int DX=2;		// picture may be smaller in demo!
const int DY=2;

const int PICTURE_WIDTH =  640;// 1280;  // import picture resolution
const int PICTURE_HEIGHT =  360;// 720;

const int NX = PICTURE_WIDTH/DX;		// solver grid resolution
const int NY = PICTURE_HEIGHT/DX;
char* filetobuf(char *file); // (copied from Wikipedia)
