#ifdef AMJU_IOS

#include <EventPoller.h>
#include <EventTypes.h>
#include <GuiFactory.h>
#include <GuiTextEdit.h>
#include <Screen.h>
#include "iOSKeyboard.h"
#include "ViewController.h"

static const float BORDER_WIDTH = 6.f;
static const float CORNER_RADIUS = 10.f;

// http://stackoverflow.com/questions/7253477/how-to-display-the-iphone-ipad-keyboard-over-a-full-screen-opengl-es-app

@interface MyKeyboardView : UITextView <UIKeyInput>
@property (assign, nonatomic) Amju::GuiTextEditIos* m_guiElement;
@end

@implementation MyKeyboardView

// TODO Override behaviour ?
- (void) insertText:(NSString *)text
{
  [super insertText:text];  
  _m_guiElement->OnTextChanged();
}

- (void) deleteBackward
{
  [super deleteBackward];
  _m_guiElement->OnTextChanged();
}

- (BOOL) hasText
{
  // Return whether there's any text present
  return YES;
}

- (BOOL) canBecomeFirstResponder
{
  return YES;
}

@end

namespace Amju
{
static GuiElement* CreateTextEditiOS()
{
  return new GuiTextEditIos;
}

static GuiElement* CreateTextiOS()
{
  return new GuiTextIos;
}

static  ViewController* vc = nullptr;
  
void iOSTextSetViewController(void* vvc)
{
  vc = (ViewController*)vvc;
  
  // Register these iOS-specific types with GuiFactory
  
  // TODO
  // For text field, usually we use the standard GuiText, but in ios-only gui*.txt fies, we
  //  can choose to use the custom iOS text, to show emoticons etc.
  // TODO
//  TheGuiFactory::Instance()->Add(GuiText::NAME, CreateTextiOS);
  
  // Always use iOS-specific text edit, never Amju GuiTextEdit
  TheGuiFactory::Instance()->Add(GuiTextEdit::NAME, CreateTextEditiOS);
}
  
GuiTextIos::GuiTextIos()
{
  m_view = nullptr;
}

GuiTextIos::~GuiTextIos()
{
  // Dealloc, remove from VC
  MyKeyboardView* myView = (MyKeyboardView*)m_view;
  [myView removeFromSuperview];
////  [myView dealloc];
  m_view = nullptr;
}

void GuiTextIos::SetText(const std::string& text)
{
  UITextView* myView = (UITextView*)m_view;
  myView.text = [[NSString alloc] initWithUTF8String:text.c_str()];

}

std::string GuiTextIos::GetText() const
{
  UITextView* myView = (UITextView*)m_view;
  return std::string([myView.text UTF8String]);
}

bool GuiTextIos::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }
  
  UITextView* myView = [UITextView new];
  m_view = myView;
  
  //myView.hidden = YES;
  
  float fontSize = 14.0f; // TODO TEMP TEST - depends on screen size??
  
  // Set font
  myView.font = [UIFont fontWithName:@"ArialRoundedMTBold" size:fontSize];
  
  // Non-editable
  [myView setUserInteractionEnabled:NO];

  // TODO Respect fgcol/bgcol in options
  myView.backgroundColor = [UIColor clearColor];
  
  [vc.view addSubview:myView];
  
  float scale = 1.f;
  if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)])
  {
    scale = [[UIScreen mainScreen] scale];
  }
  
  float scrX = Screen::X() / scale;
  float scrY = Screen::Y() / scale;
  
  float w = m_size.x * 0.5f * scrX;
  float h = m_size.y * 0.5f * scrY;
  
  Vec2f pos = GetCombinedPos();
  float x = (pos.x + 1.0f) * 0.5f * scrX;
  float y = (1.0f - (pos.y + 1.0f) * 0.5f) * scrY;
  
  [myView setFrame:CGRectMake(x, y, w, h)];
  
  std::string text;
  if (!f->GetLocalisedString(&text))
  {
    f->ReportError("GUI Text: Expected localised string");
    return false;
  }
  SetText(text);
  
  std::string options;
  if (!f->GetDataLine(&options))
  {
    f->ReportError("GUI Text: Expected options string");
    return false;
  }
  // TODO use options string
  
  return true;
}
  
GuiTextEditIos::GuiTextEditIos()
{
  m_view = nullptr;
}

GuiTextEditIos::~GuiTextEditIos()
{
  // Dealloc, remove from VC
  MyKeyboardView* myView = (MyKeyboardView*)m_view;
  [myView removeFromSuperview];
////  [myView dealloc];
  m_view = nullptr;
}
  
void GuiTextEditIos::OnTextChanged()
{
  if (m_onChangeFunc)
  {
    m_onChangeFunc(this);
  }
}

bool GuiTextEditIos::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }
  
  MyKeyboardView* myView = [MyKeyboardView new];
  myView.m_guiElement = this;
  m_view = myView;
  
  myView.hidden = NO;
  
  float fontSize = 14.0f; // TODO TEMP TEST - depends on screen size??
  
  // Set font
  myView.font = [UIFont fontWithName:@"ArialRoundedMTBold" size:fontSize];
  
  // Rounded corners
  myView.layer.cornerRadius = CORNER_RADIUS;
  
  // Border colour
  myView.layer.borderColor = [UIColor blueColor].CGColor;
  myView.layer.borderWidth = BORDER_WIDTH;
  
  [vc.view addSubview:myView];
  [vc.view bringSubviewToFront:myView];

  float scale = 1.f;
  if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)])
  {
    scale = [[UIScreen mainScreen] scale];
  }

  float scrX = Screen::X() / scale;
  float scrY = Screen::Y() / scale;

  float w = m_size.x * 0.5f * scrX;
  float h = m_size.y * 0.5f * scrY;
  
  Vec2f pos = GetCombinedPos();
  float x = (pos.x + 1.0f) * 0.5f * scrX;
  float y = (1.0f - (pos.y + 1.0f) * 0.5f) * scrY;

  // Account for border size -- border is drawn inside frame.
  w += 2 * BORDER_WIDTH;
  h += 2 * BORDER_WIDTH;
  x -= BORDER_WIDTH;
  y -= BORDER_WIDTH;
  
  [myView setFrame:CGRectMake(x, y, w, h)];

  // Padding between edge and text
  myView.textContainer.lineFragmentPadding = BORDER_WIDTH * 2;
  
  std::string text;
  if (!f->GetLocalisedString(&text))
  {
    f->ReportError("GUI Text: Expected localised string");
    return false;
  }
  SetText(text);
  
  std::string options;
  if (!f->GetDataLine(&options))
  {
    f->ReportError("GUI Text: Expected options string");
    return false;
  }
  // TODO use options string
  
  return true;
}

void GuiTextEditIos::SetText(const std::string& text)
{
  MyKeyboardView* myView = (MyKeyboardView*)m_view;
  myView.text = [[NSString alloc] initWithUTF8String:text.c_str()];
}
    
std::string GuiTextEditIos::GetText() const
{
  MyKeyboardView* myView = (MyKeyboardView*)m_view;
  return std::string([myView.text UTF8String]);
}
    
void GuiTextEditIos::ShowKeyboard(bool showNotHide)
{
  MyKeyboardView* myView = (MyKeyboardView*)m_view;

  if (showNotHide)
  {
    myView.hidden = NO;
    [myView becomeFirstResponder];
  }
  else
  {
    myView.hidden = YES;
    [myView resignFirstResponder];
  }
}
} // namespace

#endif // AMJU_IOS

