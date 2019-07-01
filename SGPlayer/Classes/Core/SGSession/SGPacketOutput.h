//
//  SGURLSource.h
//  SGPlayer
//
//  Created by Single on 2018/1/16.
//  Copyright © 2018年 single. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SGDemuxable.h"

@protocol SGPacketOutputDelegate;

/**
 *
 */
typedef NS_ENUM(NSUInteger, SGPacketOutputState) {
    SGPacketOutputStateNone     = 0,
    SGPacketOutputStateOpening  = 1,
    SGPacketOutputStateOpened   = 2,
    SGPacketOutputStateReading  = 3,
    SGPacketOutputStatePaused   = 4,
    SGPacketOutputStateSeeking  = 5,
    SGPacketOutputStateFinished = 6,
    SGPacketOutputStateClosed   = 7,
    SGPacketOutputStateFailed   = 8,
};

@interface SGPacketOutput : NSObject

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/**
 *
 */
- (instancetype)initWithDemuxable:(id<SGDemuxable>)demuxable;

/**
 *
 */
@property (nonatomic, weak) id<SGPacketOutputDelegate> delegate;

/**
 *
 */
@property (nonatomic, readonly) SGPacketOutputState state;

/**
 *
 */
@property (nonatomic, copy, readonly) NSError *error;

/**
 *
 */
@property (nonatomic, copy, readonly) NSArray<SGTrack *> *tracks;

/**
 *
 */
@property (nonatomic, copy, readonly) NSDictionary *metadata;

/**
 *
 */
@property (nonatomic, readonly) CMTime duration;

/**
 *
 */
- (BOOL)open;

/**
 *
 */
- (BOOL)close;

/**
 *
 */
- (BOOL)pause;

/**
 *
 */
- (BOOL)resume;

/**
 *
 */
- (BOOL)seekable;

/**
 *
 */
- (BOOL)seekToTime:(CMTime)time result:(SGSeekResult)result;

@end

@protocol SGPacketOutputDelegate <NSObject>

/**
 *
 */
- (void)packetOutput:(SGPacketOutput *)packetOutput didChangeState:(SGPacketOutputState)state;

/**
 *
 */
- (void)packetOutput:(SGPacketOutput *)packetOutput didOutputPacket:(SGPacket *)packet;

@end
