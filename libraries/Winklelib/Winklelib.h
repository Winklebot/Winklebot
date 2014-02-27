typedef enum {
   OK_SPEED, ERR_BADSPEED
} RoachReturn_t;

void WinkleInit(void);
RoachReturn_t LeftMtrSpeed(char newSpeed);
RoachReturn_t RightMtrSpeed(char newSpeed);
unsigned char ReadBumpers(void);

void SET_SHARED_BYTE_TO(unsigned char);
unsigned char GET_SHARED_BYTE(void);
void SET_SHARED_WORD_TO(unsigned int);
unsigned int GET_SHARED_WORD(void);

