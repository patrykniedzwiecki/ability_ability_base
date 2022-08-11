/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "array_wrapper.h"
#include <cstdint>
#include "bool_wrapper.h"
#include "zchar_wrapper.h"
#include "byte_wrapper.h"
#include "double_wrapper.h"
#include "float_wrapper.h"
#include "int_wrapper.h"
#include "long_wrapper.h"
#include "short_wrapper.h"
#include "string_wrapper.h"
#include "want_params_wrapper.h"

namespace OHOS {
namespace AAFwk {
namespace {
const int MAX_SIZE = 1024;
};
IINTERFACE_IMPL_1(Array, Object, IArray);

Array::Array(long size, const InterfaceID &id) : size_(size), typeId_(id)
{
    for (long i = 0; i < size; i++) {
        values_.push_back(nullptr);
    }
}

ErrCode Array::Get(long index, /* [in] */
    sptr<IInterface> &value)   /* [out] */
{
    if (index < 0 || index >= size_) {
        value = nullptr;
        return ERR_INVALID_VALUE;
    }
    value = values_[index];
    return ERR_OK;
}

ErrCode Array::GetLength(long &size) /* [out] */
{
    VALIDATE_NOT_NULL(&size);

    size = size_;
    return ERR_OK;
}

ErrCode Array::GetType(InterfaceID &id) /* [out] */
{
    VALIDATE_NOT_NULL(&id);

    id = typeId_;
    return ERR_OK;
}

ErrCode Array::Set(long index, /* [in] */
    IInterface *value)         /* [in] */
{
    if (index < 0 || index >= size_) {
        return ERR_INVALID_VALUE;
    }
    values_[index] = value;
    return ERR_OK;
}

bool Array::Equals(IObject &other) /* [in] */
{
    Array *otherObj = static_cast<Array *>(IArray::Query(&other));
    if (otherObj == nullptr) {
        return false;
    }

    if (this == otherObj) {
        return true;
    }

    if (otherObj->size_ != size_ || otherObj->typeId_ != typeId_) {
        return false;
    }

    if (size_ < 0 || size_ > MAX_SIZE) {
        return false;
    }

    for (long i = 0; i < size_; i++) {
        if (!Object::Equals(*(values_[i].GetRefPtr()), *(otherObj->values_[i].GetRefPtr()))) {
            return false;
        }
    }
    return true;
}

std::string Array::ToString()
{
    std::string result;
    if (typeId_ == g_IID_IString) {
        result += String::SIGNATURE;
    } else if (typeId_ == g_IID_IBoolean) {
        result += Boolean::SIGNATURE;
    } else if (typeId_ == g_IID_IByte) {
        result += Byte::SIGNATURE;
    } else if (typeId_ == g_IID_IShort) {
        result += Short::SIGNATURE;
    } else if (typeId_ == g_IID_IInteger) {
        result += Integer::SIGNATURE;
    } else if (typeId_ == g_IID_ILong) {
        result += Long::SIGNATURE;
    } else if (typeId_ == g_IID_IFloat) {
        result += Float::SIGNATURE;
    } else if (typeId_ == g_IID_IDouble) {
        result += Double::SIGNATURE;
    } else if (typeId_ == g_IID_IArray) {
        result += Array::SIGNATURE;
    } else if (typeId_ == g_IID_IChar) {
        result += Char::SIGNATURE;
    } else if (typeId_ == g_IID_IWantParams) {
        result += WantParamWrapper::SIGNATURE;
    } else {
        result += "";
    }

    result += std::to_string(size_) + "{";
    for (long i = 0; i < size_; i++) {
        result += Object::ToString(*(values_[i].GetRefPtr()));
        if (i < size_ - 1) {
            result += ",";
        }
    }
    result += "}";
    return result;
}

sptr<IArray> Array::ParseString(const std::string &values, long size)
{
    sptr<IArray> array = new (std::nothrow) Array(size, g_IID_IString);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return String::Parse(str); };
        ParseElement(array, func, values, size);
    }
    return array;
}

sptr<IArray> Array::ParseBoolean(const std::string &values, long size)
{
    sptr<IArray> array = new (std::nothrow) Array(size, g_IID_IBoolean);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Boolean::Parse(str); };
        ParseElement(array, func, values, size);
    }
    return array;
}

sptr<IArray> Array::ParseByte(const std::string &values, long size)
{
    sptr<IArray> array = new (std::nothrow) Array(size, g_IID_IByte);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Byte::Parse(str); };
        ParseElement(array, func, values, size);
    }
    return array;
}

sptr<IArray> Array::ParseShort(const std::string &values, long size)
{
    sptr<IArray> array = new (std::nothrow) Array(size, g_IID_IShort);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Short::Parse(str); };
        ParseElement(array, func, values, size);
    }
    return array;
}

sptr<IArray> Array::ParseInteger(const std::string &values, long size)
{
    sptr<IArray> array = new (std::nothrow) Array(size, g_IID_IInteger);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Integer::Parse(str); };
        ParseElement(array, func, values, size);
    }
    return array;
}

sptr<IArray> Array::ParseLong(const std::string &values, long size)
{
    sptr<IArray> array = new (std::nothrow) Array(size, g_IID_ILong);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Long::Parse(str); };
        ParseElement(array, func, values, size);
    }
    return array;
}

sptr<IArray> Array::ParseFloat(const std::string &values, long size)
{
    sptr<IArray> array = new (std::nothrow) Array(size, g_IID_IFloat);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Float::Parse(str); };
        ParseElement(array, func, values, size);
    }
    return array;
}

sptr<IArray> Array::ParseDouble(const std::string &values, long size)
{
    sptr<IArray> array = new (std::nothrow) Array(size, g_IID_IDouble);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Double::Parse(str); };
        ParseElement(array, func, values, size);
    }
    return array;
}
sptr<IArray> Array::ParseChar(const std::string &values, long size)
{
    sptr<IArray> array = new (std::nothrow) Array(size, g_IID_IChar);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Char::Parse(str); };
        ParseElement(array, func, values, size);
    }
    return array;
}
sptr<IArray> Array::ParseArray(const std::string &values, long size)
{
    sptr<IArray> array = new (std::nothrow) Array(size, g_IID_IArray);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return Array::Parse(str); };
        ParseElement(array, func, values, size);
    }
    return array;
}

sptr<IArray> Array::ParseWantParams(const std::string &values, long size)
{
    sptr<IArray> array = new (std::nothrow) Array(size, g_IID_IWantParams);
    if (array != nullptr) {
        auto func = [](const std::string &str) -> sptr<IInterface> { return WantParamWrapper::Parse(str); };
        ParseElement(array, func, values, size);
    }
    return array;
}

sptr<IArray> Array::Parse(const std::string &arrayStr) /* [in] */
{
    char signature = arrayStr[0];
    if (signature != String::SIGNATURE && signature != Boolean::SIGNATURE && signature != Byte::SIGNATURE &&
        signature != Short::SIGNATURE && signature != Integer::SIGNATURE && signature != Long::SIGNATURE &&
        signature != Float::SIGNATURE && signature != Double::SIGNATURE && signature != Array::SIGNATURE &&
        signature != Char::SIGNATURE && signature != WantParamWrapper::SIGNATURE) {
        return nullptr;
    }

    std::size_t idx = arrayStr.find("{");
    if (idx == std::string::npos) {
        return nullptr;
    }
    if (arrayStr[arrayStr.length() - 1] != '}') {
        return nullptr;
    }

    long size = std::stol(arrayStr.substr(1, idx - 1));
    idx += 1;
    std::string values = arrayStr.substr(idx, arrayStr.length() - 1 - idx);

    switch (signature) {
        case Char::SIGNATURE:
            return ParseChar(values, size);
        case String::SIGNATURE:
            return ParseString(values, size);
        case Boolean::SIGNATURE:
            return ParseBoolean(values, size);
        case Byte::SIGNATURE:
            return ParseByte(values, size);
        case Short::SIGNATURE:
            return ParseShort(values, size);
        case Integer::SIGNATURE:
            return ParseInteger(values, size);
        case Long::SIGNATURE:
            return ParseLong(values, size);
        case Float::SIGNATURE:
            return ParseFloat(values, size);
        case Double::SIGNATURE:
            return ParseDouble(values, size);
        case Array::SIGNATURE:
            return ParseArray(values, size);
        case WantParamWrapper::SIGNATURE:
            return ParseWantParams(values, size);
        default:
            break;
    }
    return nullptr;
}

void Array::ParseElement(IArray *array,                  /* [in] */
    std::function<sptr<IInterface>(std::string &)> func, /* [in] */
    const std::string &values,                           /* [in] */
    long size)                                           /* [in] */
{
    if (array == nullptr) {
        return;
    }

    std::size_t beginIdx = 0;
    std::size_t endIdx;
    for (long i = 0; i < size; i++) {
        std::string valueStr;
        if (i < size - 1) {
            endIdx = values.find(",", beginIdx);
            if (endIdx == std::string::npos) {
                valueStr = values.substr(beginIdx);
            } else {
                valueStr = values.substr(beginIdx, endIdx - beginIdx);
                beginIdx = endIdx + 1;
            }
        } else {
            valueStr = values.substr(beginIdx, values.length() - beginIdx);
        }
        array->Set(i, func(valueStr));
    }
}

bool Array::IsBooleanArray(IArray *array) /* [in] */
{
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IBoolean;
}

bool Array::IsCharArray(IArray *array) /* [in] */
{
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IChar;
}

bool Array::IsByteArray(IArray *array) /* [in] */
{
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IByte;
}

bool Array::IsShortArray(IArray *array) /* [in] */
{
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IShort;
}

bool Array::IsIntegerArray(IArray *array) /* [in] */
{
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IInteger;
}

bool Array::IsLongArray(IArray *array) /* [in] */
{
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_ILong;
}

bool Array::IsFloatArray(IArray *array) /* [in] */
{
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IFloat;
}

bool Array::IsDoubleArray(IArray *array) /* [in] */
{
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IDouble;
}

bool Array::IsStringArray(IArray *array) /* [in] */
{
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IString;
}

bool Array::IsWantParamsArray(IArray *array) /* [in] */
{
    InterfaceID typeId;
    array->GetType(typeId);
    return typeId == g_IID_IWantParams;
}

void Array::ForEach(IArray *array,          /* [in] */
    std::function<void(IInterface *)> func) /* [in] */
{
    long size;
    array->GetLength(size);
    for (long i = 0; i < size; i++) {
        sptr<IInterface> object;
        array->Get(i, object);
        func(object);
    }
}
}  // namespace AAFwk
}  // namespace OHOS