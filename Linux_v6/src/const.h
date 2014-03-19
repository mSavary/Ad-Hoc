// MESSAGE TYPE
#define HELLO_MESSAGE 1
#define TC_MESSAGE 2
#define MID_MESSAGE 3
#define HNA_MESSAGE 4

// LINK TYPE for linkcode
#define UNSPEC_LINK 0
#define ASYM_LINK 1
#define SYM_LINK 2
#define LOST_LINK 3

// NEIGHBOR TYPE
#define NOT_NEIGH 0
#define SYM_NEIGH 1
#define MPR_NEIGH 2

// LINK CODE VALUES //todo mettre les valeurs exacte
#define LINK_CODE_MPR 01
#define LINK_CODE_NGHB 10

// LINK Hysteresis
#define HYST_THRESHOLD_HIGH 0.8
#define HYST_THRESHOLD_LOW 0.3
#define HYST_THRESHOLD_SCALING 0.5

// WILLINGNESS
#define WILL_NEVER 0
#define WILL_LOW 1
#define WILL_DEFAULT 3
#define WILL_HIGH 6
#define WILL_ALWAYS 7

// EMISSION INTERVALS
#define HELLO_INTERVAL 2
#define REFRESH_INTERVAL 2
#define TC_INTERVAL 5
#define MID_INTERVAL 5
#define HNA_INTERVAL 5

// HOLDING TIME
#define NEIGHB_HOLD_TIME 3*REFRESH_INTERVAL
#define TOP_HOLD_TIME 3*TC_INTERVAL
#define DUP_HOLD_TIME 30
#define MID_HOLD_TIME 3*MID_INTERVAL
#define HNA_HOLD_TIME 3*HNA_INTERVAL

// Miscellaneous Constants
#define TC_REDUNDANCY 0
#define MPR_COVERAGE 1
#define MAX_JITTER HELLO_INTERVAL/4

//IPv6
#define EN_TETE_IPv6 0x2014

//Listener
#define PORT 698
#define BUFF_SIZE 1280 // size in bytes
// Semaphore et mutex
#define MAX_LENGTH 50
#define MIN_LENGTH 0

// Semaphore et mutex
#define MAX_LENGTH 50
#define MIN_LENGTH 0

//node
#define HELLO_INTERVAL 2
#define TC_INTERVAL 5

//controller
#define HELLO_TYPE 1
#define TC_TYPE 2

//route
#define ADD 1
#define DEL 2
#define UPD 3
#define NONE 0
