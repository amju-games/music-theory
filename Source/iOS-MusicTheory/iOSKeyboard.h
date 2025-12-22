#pragma once

#ifdef AMJU_IOS

#include <GuiTextEdit.h>

namespace Amju
{
// Call from view controller, passing self.
void iOSTextSetViewController(void*);
  
class GuiTextIos : public GuiElement, public IGuiText
{
public:
  GuiTextIos();
  ~GuiTextIos();
  
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  void SetText(const std::string& text) override;
  std::string GetText() const override;
  virtual void Draw() override {}
  virtual bool Load(File*) override;
  
private:
  void* m_view; // The iOS UIView
};

class GuiTextEditIos : public GuiElement, public IGuiTextEdit
{
public:
  GuiTextEditIos();
  ~GuiTextEditIos();
  
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  void ShowKeyboard(bool showNotHide);
  
  void SetText(const std::string& text) override;
  std::string GetText() const override;
  
  virtual void Draw() override {}
  virtual bool Load(File*) override;
  
  void OnTextChanged();

private:
  void* m_view; // The iOS UIView
};
}

#endif // AMJU_IOS


