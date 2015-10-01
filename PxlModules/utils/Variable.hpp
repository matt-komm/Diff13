#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <iostream>

class Variable
{
    public:
        virtual ~Variable()
        {
        }
        
        virtual void* getAddress() = 0;
        virtual void reset() = 0;
        virtual bool isDirty() = 0;
        virtual unsigned int getSize() const = 0;
        virtual void print() const = 0;
        
        template<class TYPE>
        Variable* operator=(const TYPE& value);
        template<class TYPE> TYPE get() const;
};

template<class TYPE>
class VariableTmpl:
    public Variable
{
    private:
        TYPE* _value;
        bool _isDirty;
    public:
        VariableTmpl(TYPE value=std::numeric_limits<TYPE>::lowest()):
            _value(new TYPE(value)),
            _isDirty(true)
        {
        }
        
        virtual void* getAddress()
        {
            return _value;
        }
        
        virtual bool isDirty()
        {
            return _isDirty;
        }
        
        virtual void reset()
        {
            *_value=std::numeric_limits<TYPE>::lowest();
            _isDirty=true;
        }
        
        virtual unsigned int getSize() const
        {
            return sizeof(TYPE);
        }
        
        virtual void print() const
        {
            std::cout<<*_value<<std::endl;
        }
        
        void setValue(const TYPE& value)
        {
            _isDirty=false;
            *_value=value;
        }
        
        TYPE getValue()
        {
            return *_value;
        }
        
        ~VariableTmpl()
        {
            delete _value;
        }
};

template<class TYPE>
Variable* Variable::operator=(const TYPE& value)
{
    VariableTmpl<TYPE>* var = dynamic_cast<VariableTmpl<TYPE>*>(this);
    if (var)
    {
        var->setValue(value);
        return this;
    }
    throw "Error - variable and value type do not match";
    return this;
}

template<class TYPE> TYPE Variable::get() const
{
    VariableTmpl<TYPE>* var = dynamic_cast<VariableTmpl<TYPE>*>(this);
    if (var)
    {
        return var->getValue();
    }
    throw "Error - variable and return type do not match";
    return 0;
}

#endif

