//
//  MediaRecorderUtils.m
//  FirstProgarm
//
//  Created by ndoo on 15/7/29.
//  Copyright (c) 2015年 ndoo. All rights reserved.
//

#import "MediaRecorderUtils.h"
#import "VoiceConvert/VoiceConverter.h"

@implementation MediaRecorderUtils

static id _owner;
static id _amrPath;
static id _wavPath;
static id _uploadPath;
static id _downloadPath;
static UInt64 _startRecordTimer;

+ (void)init:(NSString *)filePath AndFileName:(NSString *)fileName
{
    if(_owner == nil)
    {
        _amrPath =  [[filePath stringByAppendingString:fileName ]stringByAppendingString:@".arm"];
        _wavPath =  [[filePath stringByAppendingString:fileName ]stringByAppendingString:@".wav"];
        
        [_amrPath retain];
        [_wavPath retain];
    }
    
    _amrPath =  [[filePath stringByAppendingString:fileName ]stringByAppendingString:@".arm"];
    _wavPath =  [[filePath stringByAppendingString:fileName ]stringByAppendingString:@".wav"];
    NSLog(@"AMR Path:%@",_amrPath);
    NSLog(@"WAV Path:%@",_wavPath);
    [[MediaRecorderUtils alloc] audio];
}

+ (id)getAmrPath
{
    return _amrPath;
}

+ (id)getWavPath
{
    return _wavPath;
}

+ (id)getUploadPath
{
    return _uploadPath;
}

+ (id)getDownloadPath
{
    return _downloadPath;
}

+ (void)startRecord
{
    NSLog(@"开始录音:%@",_wavPath);
    _startRecordTimer = [[NSDate date] timeIntervalSince1970]*1000;
    [_owner _startRecord];
}

+ (void)stopRecord:(NSString *)uploadPath
{
    _uploadPath = uploadPath;
    UInt64 temp1 = [[NSDate date] timeIntervalSince1970]*1000;
    UInt64 temp2 = (temp1 - _startRecordTimer) / 1000;
    long v = ceil(temp2);
    NSString *str1 = [NSString stringWithFormat:@"%ld",v];
    _uploadPath = [_uploadPath stringByAppendingString:@"&duration="];
    _uploadPath = [_uploadPath stringByAppendingString:str1];
    [_owner _stopRecord];
    //[VoiceConverter wavToAmr:_wavPath amrSavePath:_amrPath];
}

+ (void)startPlay:(NSString *)downloadPath
{
    _downloadPath = downloadPath;
    NSLog(@"downloadPath:%@",_downloadPath);
}

+ (void)stopPlay
{
    [_owner _stopPlay];
}

+ (void)readyPlay
{
    [_owner _startPlay];
}

+ (BOOL) canRecord{
    return [_owner canRecord];
}

- (void)audio
{
    //录音设置
    NSMutableDictionary *recordSetting = [[NSMutableDictionary alloc]init];    //设置录式  AVFormatIDKey==kAudioFormatLinearPCM
    //设置录音采样率(Hz) 如：AVSampleRateKey==8000/44100/96000（影响音频的质量）
    [recordSetting setValue:[NSNumber numberWithFloat:4570] forKey:AVSampleRateKey];
    //录音通道数  1 或 2
    [recordSetting setValue:[NSNumber numberWithInt:1] forKey:AVNumberOfChannelsKey];
    //线性采样位数  8、16、24、32
    [recordSetting setValue:[NSNumber numberWithInt:16] forKey:AVLinearPCMBitDepthKey];
    //录音的质量
    [recordSetting setValue:[NSNumber numberWithInt:AVAudioQualityMedium ] forKey:AVEncoderAudioQualityKey];
    NSURL *url = [NSURL fileURLWithPath:_wavPath];
    NSError *error;
    //初始化
    recorder = [[AVAudioRecorder alloc] initWithURL:url settings:recordSetting error:&error];

    _owner = self;
    [_owner retain];
}

///新增api,获取录音权限. 返回值,YES为无拒绝,NO为拒绝录音.

- (BOOL)canRecord
{
    __block BOOL bCanRecord = YES;
        AVAudioSession *audioSession = [AVAudioSession sharedInstance];
        if ([audioSession respondsToSelector:@selector(requestRecordPermission:)])
        {
            [audioSession performSelector:@selector(requestRecordPermission:) withObject:^(BOOL granted) {
                if (granted)
                {
                    bCanRecord = YES;
                }
                else
                {
                    [[[UIAlertView alloc] initWithTitle:nil
                                                message:[NSString stringWithFormat:@"需要访问您的麦克风。\n请启用麦克风-设置/隐私/麦克风"]
                                               delegate:nil
                                      cancelButtonTitle:@"好"
                                      otherButtonTitles:nil] show];
                }
            }];
        
    }
    
    return bCanRecord;
}

- (void)_startRecord
{
    //创建录音文件，准备录音
    if([_owner canRecord] == YES)
    {
        if ([recorder prepareToRecord])
        {
            AVAudioSession *audioSession = [AVAudioSession sharedInstance];
            [audioSession setCategory:AVAudioSessionCategoryPlayAndRecord error:nil];
            [audioSession setActive:YES error:nil];
            
            //开始
            [recorder record];
        }
    }

}

- (void)_stopRecord
{
    AVAudioSession *audioSession = [AVAudioSession sharedInstance];
    [audioSession setCategory:AVAudioSessionCategoryPlayback error:nil];
    [audioSession setActive:YES error:nil];
    [recorder stop];
    NSLog(@"停止录音:");
}

- (void)_startPlay
{
    if (self.avPlay.playing)
    {
        [self.avPlay stop];
        return;
    }
    NSURL *url = [NSURL fileURLWithPath:_wavPath];
    NSLog(@"播放录音:%@",url);
    AVAudioPlayer *player = [[AVAudioPlayer alloc]initWithContentsOfURL:url error:nil];
    self.avPlay = player;
    [self.avPlay play];
}

- (void)_stopPlay
{
    [self.avPlay stop];
}

@end
