/*
 * Copyright (c) 2020, Andreas Kling <kling@serenityos.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <AK/Function.h>
#include <AK/StringBuilder.h>
#include <LibJS/Heap/Heap.h>
#include <LibJS/Interpreter.h>
#include <LibJS/Runtime/Array.h>
#include <LibJS/Runtime/ArrayPrototype.h>
#include <LibJS/Runtime/Error.h>
#include <LibJS/Runtime/Value.h>

namespace JS {

ArrayPrototype::ArrayPrototype()
{
    put_native_function("shift", shift);
    put_native_function("pop", pop);
    put_native_function("push", push, 1);
    put_native_function("toString", to_string, 0);
    put("length", Value(0));
}

ArrayPrototype::~ArrayPrototype()
{
}

static Array* array_from(Interpreter& interpreter)
{
    auto* this_object = interpreter.this_value().to_object(interpreter.heap());
    if (!this_object)
        return {};
    if (!this_object->is_array()) {
        interpreter.throw_exception<Error>("TypeError", "Not an Array");
        return nullptr;
    }
    return static_cast<Array*>(this_object);
}

Value ArrayPrototype::push(Interpreter& interpreter)
{
    auto* this_object = interpreter.this_value().to_object(interpreter.heap());
    if (!this_object)
        return {};
    ASSERT(this_object->is_array());
    if (!interpreter.argument_count())
        return js_undefined();
    static_cast<Array*>(this_object)->push(interpreter.argument(0));
    return Value(static_cast<const Array*>(this_object)->length());
}

Value ArrayPrototype::pop(Interpreter& interpreter)
{
    auto* this_object = interpreter.this_value().to_object(interpreter.heap());
    if (!this_object)
        return {};
    ASSERT(this_object->is_array());
    return static_cast<Array*>(this_object)->pop();
}

Value ArrayPrototype::shift(Interpreter& interpreter)
{
    auto* this_object = interpreter.this_value().to_object(interpreter.heap());
    if (!this_object)
        return {};
    ASSERT(this_object->is_array());
    return static_cast<Array*>(this_object)->shift();
}

Value ArrayPrototype::to_string(Interpreter& interpreter)
{
    auto* array = array_from(interpreter);
    if (!array)
        return {};

    StringBuilder builder;
    for (size_t i = 0; i < array->elements().size(); ++i) {
        if (i != 0)
            builder.append(',');
        builder.append(array->elements()[i].to_string());
    }
    return js_string(interpreter, builder.to_string());
}

}
