//
//  SGPacket.m
//  SGPlayer
//
//  Created by Single on 2018/1/22.
//  Copyright © 2018年 single. All rights reserved.
//

#import "SGPacket.h"
#import "SGPacket+Internal.h"

@interface SGPacket ()

{
    UInt64 _size;
    NSLock *_lock;
    AVPacket *_core;
    SGTrack *_track;
    CMTime _duration;
    CMTime _timeStamp;
    UInt64 _lockingCount;
    CMTime _decodeTimeStamp;
    SGCodecDescription *_codecDescription;
}

@end

@implementation SGPacket

- (instancetype)init
{
    if (self = [super init]) {
        self->_lock = [[NSLock alloc] init];
        self->_core = av_packet_alloc();
        [self clear];
    }
    return self;
}

- (void)dealloc
{
    NSAssert(self->_lockingCount == 0, @"SGPacket, Invalid locking count");
    [self clear];
    if (self->_core) {
        av_packet_free(&self->_core);
        self->_core = nil;
    }
}

#pragma mark - Setter & Getter

- (void *)coreptr
{
    return self->_core;
}

- (SGTrack *)track
{
    return self->_track;
}

- (UInt64)size
{
    return self->_size;
}

- (CMTime)duration
{
    return self->_duration;
}

- (CMTime)timeStamp
{
    return self->_timeStamp;
}

- (CMTime)decodeTimeStamp
{
    return self->_decodeTimeStamp;
}

- (AVPacket *)core
{
    return self->_core;
}

- (void)setCodecDescription:(SGCodecDescription *)codecDescription
{
    self->_codecDescription = codecDescription;
}

- (SGCodecDescription *)codecDescription
{
    return self->_codecDescription;
}

#pragma mark - Item

- (void)lock
{
    [self->_lock lock];
    self->_lockingCount += 1;
    [self->_lock unlock];
}

- (void)unlock
{
    [self->_lock lock];
    self->_lockingCount -= 1;
    [self->_lock unlock];
    if (self->_lockingCount == 0) {
        self->_lockingCount = 0;
        [[SGObjectPool sharedPool] comeback:self];
    }
}

- (void)clear
{
    if (self->_core) {
        av_packet_unref(self->_core);
    }
    self->_size = 0;
    self->_track = nil;
    self->_duration = kCMTimeZero;
    self->_timeStamp = kCMTimeZero;
    self->_decodeTimeStamp = kCMTimeZero;
    self->_codecDescription = nil;
}

#pragma mark - Control

- (void)fill
{
    AVPacket *pkt = self->_core;
    AVRational timebase = self->_codecDescription.timebase;
    SGCodecDescription *cd = self->_codecDescription;
    if (pkt->pts == AV_NOPTS_VALUE) {
        pkt->pts = pkt->dts;
    }
    self->_size = pkt->size;
    self->_track = cd.track;
    self->_duration = CMTimeMake(pkt->duration * timebase.num, timebase.den);
    self->_timeStamp = CMTimeMake(pkt->pts * timebase.num, timebase.den);
    self->_decodeTimeStamp = CMTimeMake(pkt->dts * timebase.num, timebase.den);
    for (SGTimeLayout *obj in cd.timeLayouts) {
        self->_duration = [obj convertDuration:self->_duration];
        self->_timeStamp = [obj convertTimeStamp:self->_timeStamp];
        self->_decodeTimeStamp = [obj convertTimeStamp:self->_decodeTimeStamp];
    }
}

@end
