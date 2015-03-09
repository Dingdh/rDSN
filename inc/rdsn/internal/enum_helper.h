/*
 * The MIT License (MIT)

 * Copyright (c) 2015 Microsoft Corporation

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
# pragma once

# include <map>
# include <string>
# include <mutex>

namespace rdsn {

// an invalid enum value must be provided so as to be the default value when parsing failed
#define ENUM_BEGIN(type, invalid_value)    \
    static inline enum_helper_xxx<type>* RegisterEnu_##type() {\
        enum_helper_xxx<type>* helper = new enum_helper_xxx<type>(invalid_value);

#define ENUM_REG(e) helper->register_enum(#e, e);

#define ENUM_END(type) return helper; \
    } \
    inline type enum_from_string(const char* s, type invalid_value) {\
        return enum_helper_xxx<type>::instance(RegisterEnu_##type).parse(s); \
    }\
    inline const char* enum_to_string(type val)  {\
        return enum_helper_xxx<type>::instance(RegisterEnu_##type).to_string(val); \
    }

template<typename TEnum>
class enum_helper_xxx
{
private:
    struct EnumContext
    {
        std::string name;
    };

public:
    enum_helper_xxx(TEnum invalid) : _invalid(invalid) {}

    void register_enum(const char* name, TEnum v)
    {
        _nameToValue[std::string(name)] = v;

        EnumContext ctx;
        ctx.name.assign(name);
        _valueToContext[v] = ctx;
    }

    TEnum parse(const std::string& name)
    {
        auto it = _nameToValue.find(name);
        return it != _nameToValue.end() ? it->second : _invalid;
    }

    const char* to_string(TEnum v)
    {
        auto it = _valueToContext.find(v);
        if (it != _valueToContext.end())
        {
            return it->second.name.c_str();
        }
        else
        {
            return "Unknown";
        }
    }
    
    static enum_helper_xxx& instance(enum_helper_xxx<TEnum>* (*registor)())
    {
        if (_instance == nullptr)
        {
            static std::once_flag flag;
            std::call_once(flag, [&]() {
                _instance = registor();
            });
        }
        return *_instance;
    }

private:
    static enum_helper_xxx *_instance;

private:
    TEnum                        _invalid;
    std::map<TEnum, EnumContext> _valueToContext;
    std::map<std::string, TEnum> _nameToValue;
};

template<typename TEnum> enum_helper_xxx<TEnum>* enum_helper_xxx<TEnum>::_instance = 0;

} // end namespace
