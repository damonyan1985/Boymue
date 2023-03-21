#include <iostream>
#include <map>
#include <string>
#include <cctype>

#include "xml2json.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/encodedstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/error/en.h"

static const char xml2json_text_additional_name[] = "#text";
static const char xml2json_text_additional_value[] = "text";
static const char xml2json_tag_label[] = "tag";
static const char xml2json_id_count[] = "count";
static const char xml2json_props_label[] = "props";
static const char xml2json_children_label[] = "children";
static const bool xml2json_numeric_support = false;
/* Example:
   <view>
       <button value="v">hello</button>
   </view>
 
   translate to json
   {
    "tag": "view",
    "children": [
        {
            "tag": "button",
            "props": {"value": "v"},
            "children": [
                {"tag": "#text", "text": "hello"}
            ]
        }
    ]
   }
*/
/* [End]   This part is configurable */

// Avoided any namespace pollution.
static bool xml2json_has_digits_only(const char * input, bool *hasDecimal)
{
    if (input == nullptr)
        return false;  // treat empty input as a string (probably will be an empty string)

    const char * runPtr = input;

    *hasDecimal = false;

    while (*runPtr != '\0')
    {
        if (*runPtr == '.')
        {
            if (!(*hasDecimal))
                *hasDecimal = true;
            else
                return false; // we found two dots - not a number
        }
        else if (isalpha(*runPtr))
        {
            return false;
        }
        runPtr++;
    }

    return true;
}

void xml2json_add_attributes(rapidxml::xml_node<> *xmlnode, rapidjson::Value &jsvalue, rapidjson::Document::AllocatorType& allocator)
{
    rapidxml::xml_attribute<> *myattr;
    for(myattr = xmlnode->first_attribute(); myattr; myattr = myattr->next_attribute())
    {
        rapidjson::Value jn, jv;
        jn.SetString(myattr->name(), allocator);

        if (xml2json_numeric_support == false)
        {
            jv.SetString(myattr->value(), allocator);
        }
        else
        {
            bool hasDecimal;
            if (xml2json_has_digits_only(myattr->value(), &hasDecimal) == false)
            {
                jv.SetString(myattr->value(), allocator);
            }
            else
            {
                if (hasDecimal)
                {
                    double value = std::strtod(myattr->value(),nullptr);
                    jv.SetDouble(value);
                }
                else
                {
                    long int value = std::strtol(myattr->value(), nullptr, 0);
                    jv.SetInt(value);
                }
            }
        }
        jsvalue.AddMember(jn, jv, allocator);
    }
}

void xml2json_traverse_node(rapidxml::xml_node<> *xmlnode, rapidjson::Value &jsvalue, rapidjson::Document::AllocatorType& allocator)
{
    // 处理text节点
    if (xmlnode->type() == rapidxml::node_data) {
        jsvalue.AddMember(rapidjson::StringRef(xml2json_tag_label),
                          rapidjson::StringRef(xml2json_text_additional_name), allocator);
        jsvalue.AddMember(rapidjson::StringRef(xml2json_text_additional_value),
                          rapidjson::StringRef(xmlnode->value()), allocator);
        return;
    }

    // 添加tag属性
    jsvalue.AddMember(rapidjson::StringRef(xml2json_tag_label), rapidjson::StringRef(xmlnode->name()), allocator);
    if ((xmlnode->type() == rapidxml::node_data || xmlnode->type() == rapidxml::node_cdata) && xmlnode->value())
    {
        // case: pure_text
        jsvalue.SetString(xmlnode->value(), allocator);  // then addmember("#text" , jsvalue, allocator)
    }
    else if (xmlnode->type() == rapidxml::node_element)
    {
        // 处理属性
        if (xmlnode->first_attribute())
        {
            rapidjson::Value props(rapidjson::kObjectType);
            xml2json_add_attributes(xmlnode, props, allocator);
            jsvalue.AddMember(rapidjson::StringRef(xml2json_props_label), props, allocator);
        }
        
        // 处理子元素
        if (xmlnode->first_node())
        {
            rapidjson::Value children(rapidjson::kArrayType);
                        
            for(rapidxml::xml_node<> *xmlnode_chd = xmlnode->first_node(); xmlnode_chd; xmlnode_chd = xmlnode_chd->next_sibling())
            {
                // 创建子元素
                rapidjson::Value jchild(rapidjson::kObjectType);
                
                // 添加到数组
                children.PushBack(jchild, allocator);
            }
            
            jsvalue.AddMember(rapidjson::StringRef(xml2json_children_label), children, allocator);
        }
    }
    else
    {
        std::cerr << "err data!!" << std::endl;
    }
}

std::string xml2json(const char *xml_str)
{
    rapidxml::xml_document<> xml_doc;
    xml_doc.parse<0>(const_cast<char *>(xml_str));

    rapidjson::Document js_doc;
    js_doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = js_doc.GetAllocator();
    
    for (rapidxml::xml_node<> *xmlnode_chd = xml_doc.first_node(); xmlnode_chd; xmlnode_chd = xmlnode_chd->next_sibling())
    {
        xml2json_traverse_node(xmlnode_chd, js_doc, allocator);
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    js_doc.Accept(writer);

    return buffer.GetString();
}
