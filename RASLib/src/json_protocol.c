#include <string.h>
#include <stdio.h>
#include "jsmn.h"
#include "time.h"
#include "data_protocol.h"

struct {
    char *key;
    void *data;
    char* (*handler)(void*);
    unsigned int keylen;
} typedef tPub;

struct {
    char *key;
    void *data;
    void (*handler)(void*,char*);
    unsigned int keylen;
} typedef tSub;

static tPub pubBuff[MAX_PUBLISHERS];
static tSub subBuff[MAX_SUBSCRIBERS];
static char keyBuff[KEY_BUFF_SIZE];

static unsigned int pubCount = 0,
                    subCount = 0,
                    keyBuffCount = 0;

static char inMsgBuff[MAX_IN_MSG_SIZE],
            outMsgBuff[MAX_OUT_MSG_SIZE];
static jsmn_parser parser;
static jsmntok_t tokens[MAX_IN_TOKENS];

static const int elemExtra = 6, // 6 for the comma (or beginning bracket), the colon, and four quotes
                 msgExtra = 3; // 3 for the closing bracket, ending newline, and null terminator

// This is so it'll be easy to change how messages are sent
static void printError(char* msg, int error) {
    printf("{\"error\":\"%s\",\"code\":%d}\n", msg, error);
}

static void printMessage(char* msg) {
    printf("%s", msg);
}

static int checkAndCopyKey(char *jsonkey, char **keyPtr, unsigned int *keyLenPtr) {
    unsigned int keyLen = strnlen(jsonkey, MAX_KEY_LEN); 
    char *keyCopy = keyBuff + keyBuffCount;
    
    if (keyLen >= MAX_KEY_LEN) {
        printError("key exceeds max length", 0);
        return 0;
    }
    
    if (keyBuffCount + keyLen >= KEY_BUFF_SIZE) {
        printError("key cannot fit in buffer", 0);
        return 0;
    }
    
    strcpy(keyCopy, jsonkey);
    keyBuffCount += keyLen + 1; // 1 for the null terminator

    *keyPtr = keyCopy;
    *keyLenPtr = keyLen;

    return 1;
}

// Adds a handler that will be called to create a string value for the given key 
// (this will deepcopy jsonkey to an internal buffer)
void AddPublisher(char *jsonkey, void *data, char* (*handler)(void*)) {
    char *keyCopy;
    int success = 0;
    
    if (pubCount >= MAX_PUBLISHERS) {
        printError("publisher limit already reached", 0);
        return;
    }
    
    success = checkAndCopyKey(
        jsonkey, 
        &pubBuff[pubCount].key, 
        &pubBuff[pubCount].keylen
        );

    if (!success) {
        return;
    }

    pubBuff[pubCount].data = data;
    pubBuff[pubCount].handler = handler;

    pubCount += 1;
}

// Add a handler to be called whenever the given key is found in received messages
// (this will deepcopy jsonkey to an internal buffer)
void AddSubscriber(char *jsonkey, void *data, void (*handler)(void*,char*)) {
    char *keyCopy;   
    int success = 0;
 
    if (subCount >= MAX_SUBSCRIBERS) {
        printError("subscriber limit already reached", 0);
        return;
    }
    
    success = checkAndCopyKey(
        jsonkey, 
        &subBuff[subCount].key, 
        &subBuff[subCount].keylen 
        );

    if (!success) {
        return;
    }
    
    subBuff[subCount].data = data;
    subBuff[subCount].handler = handler;

    subCount += 1;
}

static void createAndPublishMessage(void *data) {
    int i,  
        totalLen = 0;
    char *msgPtr = outMsgBuff,
         *format1 = "{\"%s\":\"%s\"",
         *format2 = ",\"%s\":\"%s\"",
         *format = format1;

    for (i = 0; i < pubCount; i++) {
        char *key = pubBuff[i].key,
             *value = pubBuff[i].handler(pubBuff[i].data);
        unsigned int valuelen = strnlen(value, MAX_VAL_LEN),
                     len = pubBuff[i].keylen + valuelen + elemExtra; 

        if (valuelen >= MAX_VAL_LEN) {
            printError("data value string returned by publisher exceeds maximum length", 0);
            return;
        }

        if (MAX_OUT_MSG_SIZE < totalLen + len + msgExtra) {
            printError("message exceeds buffer size", 0);
            return;
        }

        sprintf(
            msgPtr, 
            format, 
            pubBuff[i].key, 
            pubBuff[i].handler(pubBuff[i].data)
            );

        format = format2;
        totalLen += len;
        msgPtr += len;
    }

    if (pubCount == 0) {
        sprintf(msgPtr, "{}\n");
    } else {
        sprintf(msgPtr, "}\n");
    }

    printMessage(outMsgBuff); 
}

// Begins calling all pub handlers and encoding data every 'period' seconds
void BeginPublishing(float period) {
    CallEvery(createAndPublishMessage, 0, period);
}

// this uses recursion to traverse JSON messages and pick out top-leve keys.
// TODO: using recursion on a microcontroller is probably not a good idea because of
//  limited stack space. this should probably be rewritten iteratively, using a stack
//  that is statically allocated in file scope.
static int walkJSONMsg(int index, int isKey) {
    int i, total = 0;

    if (isKey) {
        // replace ending quotes with null terminators
        inMsgBuff[tokens[index].end] = 0;
        inMsgBuff[tokens[index + 1].end] = 0;

        for (i = 0; i < subCount; i++) {
            if (0 == strcmp(subBuff[i].key, &inMsgBuff[tokens[index].start])) {
                subBuff[i].handler(subBuff[i].data, &inMsgBuff[tokens[index + 1].start]);
            }
        }
    }

    for (i = 0; i < tokens[index].size; i++) {
        total += walkJSONMsg(
            index + 1 + i + total,
            index == 0 && i%2 == 0
            );
    }

    return total + tokens[index].size;
}

// Begins parsing messages and calling sub handlers in a blocking loop
void BeginSubscribing(float secsBetweenReads) {
    while (1) {
        char *res = gets(inMsgBuff); // TODO: this isn't safe--we need something like UARTgets(char* b, int max_size)
        inMsgBuff[MAX_IN_MSG_SIZE - 1] = 0; // because gets isn't safe

        if (res) {
            jsmn_init(&parser);
            int error = jsmn_parse(&parser, inMsgBuff, tokens, MAX_IN_TOKENS);

            if (error == JSMN_SUCCESS) {
                // recursively run thru the tokens, compare with subscriber keys
                walkJSONMsg(0, 0);
            } else {
                printError("jsmn parse error", error);
            }
        } else {
            printError("failed to read line properly", 0);
        }

        Wait(secsBetweenReads);
    }
