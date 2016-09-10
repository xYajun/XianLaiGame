//
//  MediaRecorderUtils.h
//  FirstProgarm
//
//  Created by ndoo on 15/7/29.
//  Copyright (c) 2015年 ndoo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface MediaRecorderUtils : NSObject
{
    AVAudioRecorder *recorder;
    
}

@property (retain, nonatomic) AVAudioPlayer *avPlay;

+ (id)getAmrPath;
+ (id)getWavPath;
+ (id)getUploadPath;
+ (id)getDownloadPath;
+ (void)init:(NSString *)filePath AndFileName:(NSString *)fileName;
+ (void)startRecord;
+ (void)stopRecord:(NSString *)uploadPath;
+ (void)startPlay:(NSString *)downloadPath;
+ (void)stopPlay;
+ (void)readyPlay;
+ (BOOL)canRecord;

@end
