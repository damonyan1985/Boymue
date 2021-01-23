// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.23

#include "Document.h"
#include "expat.h"

namespace boymue {
static void XMLCALL OnStartElement(void* userData, const char* name, const char** atts) {

}

static void XMLCALL OnEndElement(void* userData, const char* name)
{
}

static void XMLCALL OnCharacters(void* userData, const char* text, int len)
{
}

void Document::initDocument(const std::string& content) {
    XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetUserData(parser, this);
    XML_SetElementHandler(parser, &OnStartElement, &OnEndElement);
    {
        XML_SetCharacterDataHandler(parser, &OnCharacters);
    }

    XML_Parse(parser, content.c_str(), content.length(), 0);
}


}