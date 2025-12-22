// https://gist.github.com/Lee-swifter/d9cd651b093f0d32b65a2bce47b0ad91

#include <iostream>
#include "../tinyxml2.h"

using namespace std;
using namespace tinyxml2;

void traversingXML(XMLNode* node, int depth) 
{
    if(node == nullptr)
        return;
   
    std::string indent(depth * 4, ' ');

    if(node->ToDeclaration()) {
        auto declaration = dynamic_cast<XMLDeclaration*>(node);
        cout << indent << "XML decl，value=" << declaration->Value() << endl;
    }
    if(node->ToElement()) {
        auto element = dynamic_cast<XMLElement*>(node);
        cout << indent << "XML element，name=" << element->Name() << ", value=" << element->Value() << endl;
        const XMLAttribute* attribute = element->FirstAttribute();
        while (attribute != nullptr) {
            cout << indent << "\tAttribute: " << attribute->Name() << "=" << attribute->Value() << endl;
            attribute = attribute->Next();
        }
    }
    if(node->ToText()) {
        auto text = dynamic_cast<XMLText*>(node);
        cout << indent << "XML text：" << text->Value() << endl;
    }
    if(node->ToComment()) {
        auto comment = dynamic_cast<XMLComment*>(node);
        cout << indent << "XML comment：" << comment->Value() << endl;
    }
    if(node->ToUnknown()) {
        auto unknown = dynamic_cast<XMLUnknown*>(node);
        cout << indent << "XML unknown node type!：" << unknown->Value() << endl;
    }
    if(node->ToDocument()) {
        auto document = dynamic_cast<XMLDocument*>(node);
        cout << indent << "XML document type：" << document->ErrorName() << endl;
    }
    
    if(node->NoChildren()) {
        return;
    }
    
    XMLNode* child = node->FirstChild();
    while(child != nullptr) {
        traversingXML(child, depth + 1);
        child = child->NextSibling();
    }
}

int main(int argc, const char * argv[]) {
    
    XMLDocument xmlDocument;
    XMLError error = xmlDocument.LoadFile("test1.musicxml");
    if(error != XML_SUCCESS) {
        std::cout << "读取 xml 失败：" << xmlDocument.ErrorStr() << endl;
        return EXIT_FAILURE;
    }
    
    traversingXML(&xmlDocument, 0);
    
    return EXIT_SUCCESS;
}


