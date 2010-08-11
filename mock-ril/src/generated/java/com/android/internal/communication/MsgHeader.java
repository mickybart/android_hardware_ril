// Generated by the protocol buffer compiler.  DO NOT EDIT!

package com.android.internal.communication;

public  final class MsgHeader extends
    com.google.protobuf.micro.MessageMicro {
  public MsgHeader() {}
  
  // required uint32 cmd = 1;
  public static final int CMD_FIELD_NUMBER = 1;
  private boolean hasCmd;
  private int cmd_ = 0;
  public int getCmd() { return cmd_; }
  public boolean hasCmd() { return hasCmd; }
  public MsgHeader setCmd(int value) {
    hasCmd = true;
    cmd_ = value;
    return this;
  }
  public MsgHeader clearCmd() {
    hasCmd = false;
    cmd_ = 0;
    return this;
  }
  
  // required uint32 length_data = 2;
  public static final int LENGTH_DATA_FIELD_NUMBER = 2;
  private boolean hasLengthData;
  private int lengthData_ = 0;
  public int getLengthData() { return lengthData_; }
  public boolean hasLengthData() { return hasLengthData; }
  public MsgHeader setLengthData(int value) {
    hasLengthData = true;
    lengthData_ = value;
    return this;
  }
  public MsgHeader clearLengthData() {
    hasLengthData = false;
    lengthData_ = 0;
    return this;
  }
  
  // optional uint32 status = 3;
  public static final int STATUS_FIELD_NUMBER = 3;
  private boolean hasStatus;
  private int status_ = 0;
  public int getStatus() { return status_; }
  public boolean hasStatus() { return hasStatus; }
  public MsgHeader setStatus(int value) {
    hasStatus = true;
    status_ = value;
    return this;
  }
  public MsgHeader clearStatus() {
    hasStatus = false;
    status_ = 0;
    return this;
  }
  
  // optional uint64 token = 4;
  public static final int TOKEN_FIELD_NUMBER = 4;
  private boolean hasToken;
  private long token_ = 0L;
  public long getToken() { return token_; }
  public boolean hasToken() { return hasToken; }
  public MsgHeader setToken(long value) {
    hasToken = true;
    token_ = value;
    return this;
  }
  public MsgHeader clearToken() {
    hasToken = false;
    token_ = 0L;
    return this;
  }
  
  public final MsgHeader clear() {
    clearCmd();
    clearLengthData();
    clearStatus();
    clearToken();
    cachedSize = -1;
    return this;
  }
  
  public final boolean isInitialized() {
    if (!hasCmd) return false;
    if (!hasLengthData) return false;
    return true;
  }
  
  public void writeTo(com.google.protobuf.micro.CodedOutputStreamMicro output)
                      throws java.io.IOException {
    if (hasCmd()) {
      output.writeUInt32(1, getCmd());
    }
    if (hasLengthData()) {
      output.writeUInt32(2, getLengthData());
    }
    if (hasStatus()) {
      output.writeUInt32(3, getStatus());
    }
    if (hasToken()) {
      output.writeUInt64(4, getToken());
    }
  }
  
  private int cachedSize = -1;
  public int getCachedSize() {
    if (cachedSize < 0) {
      // getSerializedSize sets cachedSize
      getSerializedSize();
    }
    return cachedSize;
  }
  
  public int getSerializedSize() {
    int size = 0;
    if (hasCmd()) {
      size += com.google.protobuf.micro.CodedOutputStreamMicro
        .computeUInt32Size(1, getCmd());
    }
    if (hasLengthData()) {
      size += com.google.protobuf.micro.CodedOutputStreamMicro
        .computeUInt32Size(2, getLengthData());
    }
    if (hasStatus()) {
      size += com.google.protobuf.micro.CodedOutputStreamMicro
        .computeUInt32Size(3, getStatus());
    }
    if (hasToken()) {
      size += com.google.protobuf.micro.CodedOutputStreamMicro
        .computeUInt64Size(4, getToken());
    }
    cachedSize = size;
    return size;
  }
  
  public MsgHeader mergeFrom(
      com.google.protobuf.micro.CodedInputStreamMicro input)
      throws java.io.IOException {
    while (true) {
      int tag = input.readTag();
      switch (tag) {
        case 0:
          return this;
        default: {
          if (!parseUnknownField(input, tag)) {
            return this;
          }
          break;
        }
        case 8: {
          setCmd(input.readUInt32());
          break;
        }
        case 16: {
          setLengthData(input.readUInt32());
          break;
        }
        case 24: {
          setStatus(input.readUInt32());
          break;
        }
        case 32: {
          setToken(input.readUInt64());
          break;
        }
      }
    }
  }
  
  public static MsgHeader parseFrom(byte[] data)
      throws com.google.protobuf.micro.InvalidProtocolBufferMicroException {
    return (MsgHeader) (new MsgHeader().mergeFrom(data));
  }
  
  public static MsgHeader parseFrom(
          com.google.protobuf.micro.CodedInputStreamMicro input)
      throws java.io.IOException {
    return (MsgHeader) (new MsgHeader().mergeFrom(input));
  }
  
}
