/*
 * checksum.h
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#ifndef SRC_CHECKSUM_H_
#define SRC_CHECKSUM_H_

#include <string.h>
#define M 0x00010000
typedef char BYTE;

int checksum(BYTE* input[], unsigned block_size);

#endif /* SRC_CHECKSUM_H_ */
