// Copyright 2025 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _LIBHOTH_PROTOCOL_KEY_ROTATION_H_
#define _LIBHOTH_PROTOCOL_KEY_ROTATION_H_

#include <stdint.h>

#include "protocol/host_cmd.h"
#include "transports/libhoth_device.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HOTH_PRV_CMD_HAVEN_KEY_ROTATION_OP 0x004d

enum key_rotation_record_read_half {
  KEY_ROTATION_RECORD_READ_HALF_ACTIVE = 0,
  KEY_ROTATION_RECORD_READ_HALF_STAGING = 1,
  KEY_ROTATION_RECORD_READ_HALF_A = 2,
  KEY_ROTATION_RECORD_READ_HALF_B = 3,
};

enum key_rotation_record_op {
  KEY_ROTATION_RECORD_INITIATE = 0,     // Erases staging half
  KEY_ROTATION_RECORD_WRITE = 1,        // Write to staging half
  KEY_ROTATION_RECORD_COMMIT = 2,       // Commits staging half to active half
  KEY_ROTATION_RECORD_GET_VERSION = 3,  // Get current version of key
                                        // rotation record
  KEY_ROTATION_RECORD_READ = 4,  // Read from key rotation record from flash
                                 // (offset, size, A/B/Active/Staging)
  KEY_ROTATION_RECORD_GET_STATUS = 5,  // Read key rotation record version,
                                       // image family, variant,
                                       // validation method, key data,
                                       // hash data
  KEY_ROTATION_RECORD_READ_CHUNK_TYPE =
      6,  // Gets the ith chunk of given chunk_typecode and returns the
          // chunk_size starting from the chunk_offset (chunk_index,
          // chunk_offset, chunk_typecode, chunk_size)
  KEY_ROTATION_RECORD_PAYLOAD_STATUS = 7,  // Get validation method and data
};

struct hoth_request_key_rotation_record {
  uint16_t operation;      // enum key_rotation_record_op
  uint16_t packet_offset;  // Chunked read/write offset
  uint16_t packet_size;    // Chunked read/write size excluding this header
  uint16_t reserved;
} __hoth_align4;

struct hoth_request_key_rotation_record_read {
  uint32_t read_half;  // enum key_rotation_record_read_half
} __hoth_align4;

struct hoth_request_key_rotation_record_read_chunk_type {
  uint32_t chunk_typecode;  // enum key_rotation_typecode
  uint32_t chunk_index;     // Index of the chunk to read
} __hoth_align4;

struct hoth_response_key_rotation_record_version {
  uint32_t version;
} __hoth_align4;

struct hoth_response_key_rotation_payload_status {
  uint32_t validation_method;    // enum key_rotation_validation_method
  uint32_t validation_key_data;  // If validation method is embedded key or
                                 // payload key, first 32 bits of modulus of the
                                 // key used to validate the payload
  uint32_t validation_hash_data;  // If validation method is hash, first 32 bits
                                  // of cr51 hash
} __hoth_align4;

struct hoth_response_key_rotation_status {
  uint32_t version;       // Config package version, used for anti-rollback
  uint16_t image_family;  // Image family of the payload keys this record is for
  uint16_t image_family_variant;  // Variant of the image family if any.
  uint32_t validation_method;     // enum key_rotation_validation_method
  uint32_t validation_key_data;   // If validation method is embedded key or
                                 // payload key, first 32 bits of modulus of the
                                 // key used to validate the payload
  uint32_t validation_hash_data;  // If validation method is hash, first 32 bits
                                  // of cr51 hash
} __hoth_align4;

int libhoth_key_rotation_initiate(struct libhoth_device* dev);
int libhoth_key_rotation_get_version(
    struct libhoth_device* dev,
    struct hoth_response_key_rotation_record_version* record_version);
int libhoth_key_rotation_get_status(
    struct libhoth_device* dev,
    struct hoth_response_key_rotation_status* record_status);
int libhoth_key_rotation_payload_status(
    struct libhoth_device* dev,
    struct hoth_response_key_rotation_payload_status* payload_status);

#ifdef __cplusplus
}
#endif

#endif  // _LIBHOTH_PROTOCOL_KEY_ROTATION_H_
