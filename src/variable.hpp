#define VARIABLE_H

#ifndef UTIL_H
#include "util.hpp"
#endif

namespace CS {

union Value {
    int i;
    double d;
    void* p;

    Value(int ii): i(ii) {}
    Value(double dd): d(dd) {}
    Value(void* pp): p(pp) {}
    private:
        Value();
};

class Variable {
    public:

    /* constructor family */

    private:
        Variable();

    public:

    Variable(Value& vv, int type_id_):
        v(vv), type_id(type_id_), self(*this) {
        }

    Variable(double d):
        v(d), type_id(GetId("double")), self(*this) {
        }

    Variable(int i):
        v(i), type_id(GetId("int")), self(*this) {
        }

    Variable(void* p):
        v(p), type_id(GetId("pointer")), self(*this) {
        }

    Variable(const Variable& variable):
        v(variable.v), type_id(variable.type_id), self(*this) {
        }

    Variable operator = (const Variable& rhs) {
        this->v = rhs.v;
        this->type_id = rhs.type_id;
        return *this;
    }

    /* constructor family end */

    int GetInt() const {
        return v.i;
    }

    double GetDouble() const {
        return v.d;
    }

    void* GetPointer() const {
        return v.p;
    }

    double GetAny() const {
        if (type_id == 1) return v.i;
        else if (type_id == 2) return v.d;
        assert(false);
    }

    void IllegalOperation(const char* message) const {
        printf("invalid operation: %s\n", message);
        exit(5);
    }

    Variable operator + (const Variable& rhs) {
        if (this->type_id == 1 && rhs.type_id == 1) {
            return Variable(GetInt() + rhs.GetInt());
        } else if (this->type_id <= 2 && rhs.type_id <= 2) {
            return Variable(GetAny() + rhs.GetAny());
        } else {
            IllegalOperation("plus a number and a pointer");
        }
    }

    Variable operator - (const Variable& rhs) {
        if (this->type_id == 1 && rhs.type_id == 1) {
            return Variable(GetInt() - rhs.GetAny());
        } else if (this->type_id <= 2 && rhs.type_id <= 2) {
            return Variable(GetAny() - rhs.GetAny());
        } else {
            IllegalOperation("minus a number and a pointer");
        }
    }

    Variable operator * (const Variable& rhs) {
        if (this->type_id == 1 && rhs.type_id == 1) {
            return Variable(GetAny() * rhs.GetAny());
        } else if (this->type_id <= 2 && rhs.type_id <= 2) {
            return Variable(GetAny() * rhs.GetAny());
        } else {
            IllegalOperation("multiply a number and a pointer");
        }

    }

    Variable operator / (const Variable& rhs) {
        if (this->type_id == 1 && rhs.type_id == 1) {
            return Variable(GetAny() / rhs.GetAny());
        } else if (this->type_id <= 2 && rhs.type_id <= 2) {
            return Variable(GetAny() / rhs.GetAny());
        } else {
            IllegalOperation("devide a number and a pointer");
        }

    }

    string to_string() {
        switch (type_id) {
            case 1:
                return std::to_string(v.i);
            case 2:
                return std::to_string(v.d);
            case 3:
                //return std::to_string(static_cast<int>(v.p));
            default:
                return string();
        }
    }
    
    private:
        Value v;
        int type_id;
        Variable& self;
};

}
