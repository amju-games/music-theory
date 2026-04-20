//
//  ViewController.mm
//  Amjulib
//
//  Created by Juliet Colman on 29/07/2013.
//  Copyright (c) 2013 Juliet Colman. All rights reserved.
//

#import "ViewController.h"

#include <AmjuGL.h>
#include <Screen.h>
#include <AmjuGL-OpenGLES.2.h>
#include <EventPoller.h>
#include <Game.h>
#include <StartUp.h>
#include "iOSKeyboard.h"

@interface ViewController () {
}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController

static ViewController* s_theVc = NULL;

+ (ViewController*) GetVC
{
  return s_theVc;
}

- (void)dealloc
{
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [_context release];
    [_effect release];
    [super dealloc];
}

- (void)viewDidLoad
{
    s_theVc = self;
  
    [super viewDidLoad];
    
    self.preferredFramesPerSecond = 60; // oh yeah
  
    // Enable multi-touch on the main view
    self.view.multipleTouchEnabled = YES;
  
    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2] autorelease];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
    
    // j.c. Initialise iOS-specific text edit boxes and keyboard
    Amju::iOSTextSetViewController(self);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
  
    Amju::TheEventPoller::Instance()->SetImpl(new Amju::EventPollerImplGeneric);
    
    Amju::AmjuGL::SetImpl(new Amju::AmjuGLOpenGLES2);
    
    std::cout << "Set AmjuGL impl...\n";
    
    Amju::AmjuGL::Init();
    // Set app-specific clear colour
    Amju::AmjuGL::SetClearColour(Amju::Colour(0.95f, 0.95f, 0.95f, 1.0f));
  
    Amju::StartUpBeforeCreateWindow();
  
    // Set the screen size
    float s = self.view.contentScaleFactor;
    int w = s * [[UIScreen mainScreen] bounds].size.width;
    int h = s * [[UIScreen mainScreen] bounds].size.height;
    Amju::Screen::SetSize(w, h);
  
    Amju::StartUpAfterCreateWindow();
  
    // We must Update once before Draw, so the game state is set.
    Amju::TheGame::Instance()->Update();
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

#pragma mark - GLKView and GLKViewController delegate methods

static void QueueEvent(Amju::Event* e)
{
  Amju::TheEventPoller::Instance()->GetImpl()->QueueEvent(e);
}

- (void)update
{
  Amju::TheGame::Instance()->Update();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
  Amju::TheGame::Instance()->Draw();
  Amju::AmjuGL::Flip();
}

void PopulateMBEvent(Amju::MouseButtonEvent* mbe, int x, int y)
{
  float scrX2 = float(Amju::Screen::X() / 2);
  float scrY2 = float(Amju::Screen::Y() / 2);

  mbe->x = (float)x / scrX2 - 1.0f;
  mbe->y = 1.0f - (float)y / scrY2;
}

void PopulateCursorEvent(Amju::CursorEvent* ce, int x, int y)
{
  Amju::MouseButtonEvent mbe;
  PopulateMBEvent(&mbe, x, y);
  ce->controller = 0;
  ce->x = mbe.x;
  ce->y = mbe.y;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
  float s = self.view.contentScaleFactor;
  
  for (UITouch* touch in touches)
  {
    CGPoint touchPoint = [touch locationInView:self.view];

    // Touch down events are treated as left mouse button down events
    Amju::MouseButtonEvent* mbe = new Amju::MouseButtonEvent;
    mbe->button = Amju::AMJU_BUTTON_MOUSE_LEFT;
    mbe->isDown = true;
    PopulateMBEvent(mbe, touchPoint.x * s, touchPoint.y * s);
    QueueEvent(mbe);
    
    // Why do we want a cursor event as well as a mouse down event?
    // Ohh, maybe it is for dx and dy
    Amju::CursorEvent* ce = new Amju::CursorEvent;
    PopulateCursorEvent(ce, touchPoint.x * s, touchPoint.y * s);
    QueueEvent(ce);
  }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
  float s = self.view.contentScaleFactor;

  for (UITouch* touch in touches)
  {
    CGPoint touchPoint = [touch locationInView:self.view];

    // Touch up events are treated as left mouse button up events
    Amju::MouseButtonEvent* mbe = new Amju::MouseButtonEvent;
    mbe->button = Amju::AMJU_BUTTON_MOUSE_LEFT;
    mbe->isDown = false;
    PopulateMBEvent(mbe, touchPoint.x * s, touchPoint.y * s);
    QueueEvent(mbe);
  }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
  float s = self.view.contentScaleFactor;
  
  for (UITouch* touch in touches)
  {
    CGPoint touchPoint = [touch locationInView:self.view];
    Amju::CursorEvent* ce = new Amju::CursorEvent;
    PopulateCursorEvent(ce, touchPoint.x * s, touchPoint.y * s);
    QueueEvent(ce);
  }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
  // This could be e.g. a phone call interrupts the game. Cancel all touches,
  //  which I think for me just means send touch up events.
  [self touchesEnded:touches withEvent:event];
}

// Defer system gestures to the second swipe
- (UIRectEdge)preferredScreenEdgesDeferringSystemGestures
{
  // Return .bottom to prioritize your game's input at the bottom edge
  // Return bottom and sides: music keys at edge of screen are affected.
  return UIRectEdgeLeft | UIRectEdgeBottom | UIRectEdgeRight;
}

// Auto-hide the Home Indicator (the horizontal bar)
- (BOOL)prefersHomeIndicatorAutoHidden {
  return YES;
}

// Landscape only, but we want to auto rotate to support Left and Right.
- (BOOL)shouldAutorotate {
  return YES;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
  // This allows both Landscape Left and Landscape Right
  return UIInterfaceOrientationMaskLandscape;
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation {
  return UIInterfaceOrientationLandscapeLeft;
}

// Want to hide status bar, but Info.plist setting is being ignored
- (BOOL)prefersStatusBarHidden {
  return YES;
}

@end
