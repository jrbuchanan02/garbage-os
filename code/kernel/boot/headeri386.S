// bootboot stuff


#define HEADER_MAGIC        0xE85250D6
#define HEADER_ARCH         0
#define HEADER_LENGTH       header_end - header_start
#define HEADER_CHECK        -(HEADER_ARCH + HEADER_LENGTH + HEADER_MAGIC)

#define TAG_TERMINATION_TYPE    0
#define TAG_TERMINATION_FLAG    0
#define TAG_TERMINATION_SIZE    8

#define INFORMATION_REQUEST_TYPE    1
#define INFORMATION_REQUEST_FLAG    0
#define INFORMATION_REQUEST_SIZE    into_request_end - info_request_start
#define INFORMATION_REQUEST_MEM_INF 4
#define INFORMATION_REQUEST_MEM_MAP 6
#define INFORMATION_REQUEST_FB_INFO 8


.section header
.align 8

header_start:
    .int HEADER_MAGIC
    .int HEADER_ARCH
    .int HEADER_LENGTH
    .int HEADER_CHECK
header_end:
info_request_start:
    .short INFORMATION_REQUEST_TYPE
    .short INFORMATION_REQUEST_FLAG
    .int INFORMATION_REQUEST_SIZE
    //.short INFORMATION_REQUEST_MEM_INF
    .short INFORMATION_REQUEST_MEM_MAP
    //.short INFORMATION_REQUEST_FB_INFO
into_request_end:
    .short  TAG_TERMINATION_TYPE
    .short  TAG_TERMINATION_FLAG
    .int    TAG_TERMINATION_SIZE