#include "jsmn.h"
#include "time.h"
#include "json_protocol.h"
#include "uart.h"
#include "rasstring.h"

//
// Internal buffers for keeping track of publishers and subscribers
//
static tPub* pubPtrBuff[MAX_PUBLISHERS];
static tSub* subPtrBuff[MAX_SUBSCRIBERS];

static unsigned int pubCount = 0,
                    subCount = 0;

//
// Input/output message buffers and jsmn stuff
//
static char inMsgBuff[MAX_IN_MSG_SIZE],
            outMsgBuff[MAX_OUT_MSG_SIZE];
static jsmn_parser parser;
static jsmntok_t tokens[MAX_IN_TOKENS];

//
// Constants for keeping track of tokens in messages
//
static const int elemExtra = 6, // for the comma (or beginning bracket), the colon, and four quotes
                 msgExtra = 3; // for the closing bracket, ending newline, and null terminator

//
// Change these two functions to change how messages are sent
//
static void printError(char* msg, int error, char* extra) {
    if (!extra) {
        Printf("{\"error\":\"%s\",\"code\":%d}\n", msg, error);
    } else {
        Printf("{\"error\":\"%s\",\"code\":%d,\"extra:\":'%s'}\n", msg, error, extra);
    }
}

static void printMessage(char* msg) {
    Printf("%s", msg);
}

// Returns success
static int checkAndCopyKey(char *jsonkey, char *key, unsigned int *keyLenPtr) {
    unsigned int keyLen = Strnlen(jsonkey, MAX_KEY_LEN);

    if (keyLen >= MAX_KEY_LEN) {
        printError("key exceeds max length", 0, 0);
        return false;
    }

    Strcpy(key, jsonkey);
    key[keyLen] = 0; // null terminator
    *keyLenPtr = keyLen;

    return true;
}

int InitializePublisher(tPub *pubPtr, char *jsonkey, void *data, char* (*handler)(void*)) {
    int success = false;

    if (pubCount >= MAX_PUBLISHERS) {
        printError("publisher limit already reached", 0, 0);
        return false;
    }

    success = checkAndCopyKey(
        jsonkey,
        pubPtr->key,
        &(pubPtr->keylen)
        );

    if (!success) {
        return false;
    }

    pubPtr->data = data;
    pubPtr->handler = handler;
    
    pubPtrBuff[pubCount] = pubPtr;
    pubCount += 1;
    
    return true;
}

int InitializeSubscriber(tSub *subPtr, char *jsonkey, void *data, void (*handler)(void*,char*)) {
    int success = false;
 
    if (subCount >= MAX_SUBSCRIBERS) {
        printError("subscriber limit already reached", 0, 0);
        return false;
    }

    success = checkAndCopyKey(
        jsonkey,
        subPtr->key,
        &(subPtr->keylen)
        );

    if (!success) {
        return false;
    }

    subPtr->data = data;
    subPtr->handler = handler;

    subPtrBuff[subCount] = subPtr;
    subCount += 1;
    
    return true;
}

// Calls all publisher handlers to put together a message, then sends it
static void createAndPublishMessage(void *data) {
    int i,
        totalLen = 0;
    char *msgPtr = outMsgBuff,
         *format1 = "{\"%s\":\"%s\"",
         *format2 = ",\"%s\":\"%s\"",
         *format = format1;

    for (i = 0; i < pubCount; i++) {
        char *value = (pubPtrBuff[i]->handler)(pubPtrBuff[i]->data);

        unsigned int valuelen = Strnlen(value, MAX_VAL_LEN),
                     len = pubPtrBuff[i]->keylen + valuelen + elemExtra; 

        if (valuelen >= MAX_VAL_LEN) {
            printError("data value string returned by publisher exceeds maximum length", 0, 0);
            return;
        }

        if (MAX_OUT_MSG_SIZE < totalLen + len + msgExtra) {
            printError("message exceeds buffer size", 0, 0);
            return;
        }

        SPrintf(
            msgPtr,
            format,
            pubPtrBuff[i]->key,
            (pubPtrBuff[i]->handler)(pubPtrBuff[i]->data)
            );

        format = format2;
        totalLen += len;
        msgPtr += len;
    }

    if (pubCount == 0) {
        SPrintf(msgPtr, "{}\n");
    } else {
        SPrintf(msgPtr, "}\n");
    }

    printMessage(outMsgBuff);
}

void BeginPublishing(float period) {
    CallEvery(createAndPublishMessage, 0, period);
}

// Uses recursion to traverse JSON messages and pick out top-leve keys.
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
            if (0 == Strcmp(subPtrBuff[i]->key, &inMsgBuff[tokens[index].start])) {
                (subPtrBuff[i]->handler)(
                    subPtrBuff[i]->data, 
                    &inMsgBuff[tokens[index + 1].start]
                    );
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

// we need this because Gets seems to be printing out the characters received, and we don't want that
static int ReadLine(char *line, int max_len) {
    int i = 0;
    char ch = 0;
    
    while (i < max_len && ch != '\n') {
        ch = Getc();
        line[i] = ch;
        i += 1;
    }
    
    line[max_len] = 0;
    return i;
}

void BeginSubscribing(float secsBetweenReads) {
    while (1) {
        int numBytes = ReadLine(inMsgBuff, sizeof(inMsgBuff));
        inMsgBuff[MAX_IN_MSG_SIZE - 1] = 0; // just in case Gets doesn't set the null terminator

        if (numBytes > 0) {
            int error;

            jsmn_init(&parser);
            error = jsmn_parse(&parser, inMsgBuff, tokens, MAX_IN_TOKENS);

            if (error == JSMN_SUCCESS) {
                // recursively run thru the tokens, comparing with subscriber keys,
                //  and calling subscriber handlers when finding matching keys
                walkJSONMsg(0, 0);
            } else {
                printError("jsmn parse error", error, inMsgBuff);
            }
        } else {
            printError("failed to read line properly", 0, 0);
        }

        Wait(secsBetweenReads);
    }
}
