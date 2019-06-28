//
//  SGPlayerItem+Internal.h
//  SGPlayer iOS
//
//  Created by Single on 2018/10/25.
//  Copyright © 2018 single. All rights reserved.
//

#import "SGPlayerItem.h"
#import "SGAudioDescriptor.h"
#import "SGDemuxerOptions.h"
#import "SGDecoderOptions.h"
#import "SGCapacity.h"
#import "SGFrame.h"

@protocol SGPlayerItemDelegate;

/**
 *
 */
typedef NS_ENUM(int, SGPlayerItemState) {
    SGPlayerItemStateNone,
    SGPlayerItemStateOpening,
    SGPlayerItemStateOpened,
    SGPlayerItemStateReading,
    SGPlayerItemStateSeeking,
    SGPlayerItemStateFinished,
    SGPlayerItemStateClosed,
    SGPlayerItemStateFailed,
};

@interface SGPlayerItem ()

/**
 *
 */
@property (nonatomic, copy) SGDemuxerOptions *demuxerOptions;

/**
 *
 */
@property (nonatomic, copy) SGDecoderOptions *decoderOptions;

/**
 *
 */
@property (nonatomic, weak) id<SGPlayerItemDelegate> delegate;

/**
 *
 */
@property (nonatomic, readonly) SGPlayerItemState state;

/**
 *
 */
- (BOOL)open;

/**
 *
 */
- (BOOL)start;

/**
 *
 */
- (BOOL)close;

/**
 *
 */
- (BOOL)seekable;

/**
 *
 */
- (BOOL)seekToTime:(CMTime)time result:(SGSeekResult)result;

/**
 *
 */
- (SGCapacity)capacityWithType:(SGMediaType)type;

/**
 *
 */
- (BOOL)isAvailable:(SGMediaType)type;

/**
 *
 */
- (BOOL)isFinished:(SGMediaType)type;

/**
 *
 */
@property (nonatomic, copy) SGAudioDescriptor *audioDescriptor;

/**
 *
 */
- (__kindof SGFrame *)copyAudioFrame:(SGTimeReader)timeReader;
- (__kindof SGFrame *)copyVideoFrame:(SGTimeReader)timeReader;

@end

@protocol SGPlayerItemDelegate <NSObject>

/**
 *
 */
- (void)playerItem:(SGPlayerItem *)playerItem didChangeState:(SGPlayerItemState)state;

/**
 *
 */
- (void)playerItem:(SGPlayerItem *)playerItem didChangeCapacity:(SGCapacity)capacity type:(SGMediaType)type;

@end
