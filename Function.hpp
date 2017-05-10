#ifndef ___40P3_bonus__Function__
#define ___40P3_bonus__Function__

#include <iostream>
#include <exception>

namespace cs540 {
    class BadFunctionCall : public std::exception{};
    
    template<typename ResultType,typename ... ArgumentTypes> class Base{
    public:
        virtual Base<ResultType, ArgumentTypes...>* copy(){return NULL;};
        virtual ResultType getResult(ArgumentTypes...){ResultType x;return x;};
    };
    
    template<typename ResultType, typename FunctionType, typename ... ArgumentTypes>
    class target : public Base<ResultType, ArgumentTypes...>{
    public:
        target<ResultType,FunctionType,ArgumentTypes...>(){
            
        }
        
        target<ResultType,FunctionType,ArgumentTypes...>(const FunctionType &obj) : x(obj){
            
        }
        
        virtual Base<ResultType, ArgumentTypes...>* copy(){
            return new target<ResultType,FunctionType,ArgumentTypes...>(x);
        }
        
        virtual ResultType getResult(ArgumentTypes... args){
            return x(args...);
        }
    private:
        FunctionType x;
    };
    
    template <typename> class Function{};
    
    template <typename ResultType, typename ... ArgumentTypes> class Function<ResultType(ArgumentTypes...)>{
    public:
        Function(){
            tar = NULL;
        }
        
        template <typename FunctionType> Function(FunctionType obj){
            tar = new target<ResultType,FunctionType,ArgumentTypes...>(obj);
        }
        Function(const Function &obj){
            tar = obj.tar->copy();
        }
        Function &operator=(const Function &obj){
            if(this!=&obj){
                delete tar;
                tar = obj.tar->copy();
            }
            return *this;
        }
        ~Function(){
            delete tar;
        }
        ResultType operator()(ArgumentTypes... args){
            if(*this) return tar->getResult(args...);
            else throw cs540::BadFunctionCall();
        }
        explicit operator bool() const{
            return tar!=NULL;
        }
    private:
        Base<ResultType, ArgumentTypes...>* tar;
    };
    
    template <typename ResultType, typename... ArgumentTypes>
    bool operator==(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t){
        return !f;
    }
    
    template <typename ResultType, typename... ArgumentTypes>
    bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> & f){
         return !f;
    }
    
    template <typename ResultType, typename... ArgumentTypes>
    bool operator!=(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t){
        return bool(f);
    }
    
    template <typename ResultType, typename... ArgumentTypes>
    bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> & f){
        return bool(f);
    }
}
#endif

