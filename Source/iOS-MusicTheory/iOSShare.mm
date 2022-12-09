#import <UIKit/UIKit.h>
#include "iOSShare.h"
#import "ViewController.h"

namespace Amju
{
static UIImage* lastImage = nullptr;
  
void iOSShareTextAndScreenshot(unsigned char* data, const std::string& text, int w, int h)
{
  lastImage = nullptr;
  if (data)
  {
    // Convert screenshot data to UIImage
    // http://stackoverflow.com/questions/1352864/how-to-get-uiimage-from-eaglview
    
    int dataLen = w * h * 3; // 3 bytes per pixel, TODO pass in?
    // make data provider with data.
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, data, dataLen, NULL);
    // prep the ingredients
    int bitsPerComponent = 8;
    int bitsPerPixel = 24; //32;
    int bytesPerRow = 3 * w; // * s;
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    // make the cgimage
    CGImageRef imageRef = CGImageCreate(w, h, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
    // then make the uiimage from that
    lastImage = [[ UIImage imageWithCGImage:imageRef scale:1 orientation:UIImageOrientationUp ] retain];
    
    // This works, but no need as user can choose to save image from iOS share menu
    //UIImageWriteToSavedPhotosAlbum(lastImage, nil, nil, nil);
    
    // Don't delete screenshot data - it isn't copied apparently.
    
    // Leak these to be on the safe side.
//    CGImageRelease( imageRef );
//    CGDataProviderRelease(provider);
//    CGColorSpaceRelease(colorSpaceRef);
    
  }
  
  NSMutableArray *activityItems = [NSMutableArray new];
  [activityItems addObject:[NSString stringWithUTF8String:text.c_str()]];
  if (lastImage)
  {
    [activityItems addObject:lastImage];
  }
  
  UIActivityViewController *activityViewControntroller =
    [[UIActivityViewController alloc] initWithActivityItems:activityItems applicationActivities:nil];
  
  ViewController* vc = [ViewController GetVC];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
  {
      activityViewControntroller.popoverPresentationController.sourceView = vc.view;
      activityViewControntroller.popoverPresentationController.sourceRect =
        CGRectMake(vc.view.bounds.size.width/2, vc.view.bounds.size.height/4, 0, 0);
  }
  [vc presentViewController:activityViewControntroller animated:true completion:nil];
}
}

