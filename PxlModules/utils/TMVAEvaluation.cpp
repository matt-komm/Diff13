#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>

#include "Variable.hpp"
#include "PxlParser.hpp"

#include "TMVA/Reader.h"

static pxl::Logger logger("TMVAEvaluation");

class CacheTree
{
    private:
        std::unordered_map<std::string,Variable*> _variables;
        
    public:
        template<class TYPE>
        void storeVariable(const std::string& name, const TYPE& value)
        {
            std::unordered_map<std::string,Variable*>::iterator elem = _variables.find(name.c_str());
            if (elem==_variables.end()) {
                VariableTmpl<TYPE>* var = bookVariable<TYPE>(name);
                var->setValue(value);
            } else {
                (*elem->second)=value;
            }
        }
        
        void resetVariables()
        {
            for (std::unordered_map<std::string,Variable*>::iterator it = _variables.begin(); it!=_variables.end(); ++it)
            {
                it->second->reset();
            }
        }
        
        void print() const
        {
            for (std::unordered_map<std::string,Variable*>::const_iterator it = _variables.cbegin(); it!=_variables.cend(); ++it)
            {
                std::cout<<it->first<<": ";
                it->second->print();
                std::cout<<std::endl;
            }
        }
        
        void storeVariable(const std::string& name, const pxl::Variant& value)
        {
            switch (value.getType())
            {
                case pxl::Variant::TYPE_BASIC3VECTOR:
                {
                    const pxl::Basic3Vector& vec = value.asBasic3Vector();
                    storeVariable<float>(name+"_Mag",vec.getMag());
                    storeVariable<float>(name+"_X",vec.getX());
                    storeVariable<float>(name+"_Y",vec.getY());
                    storeVariable<float>(name+"_Z",vec.getZ());
                    break;
                }
                case pxl::Variant::TYPE_BOOL:
                {
                    storeVariable<short>(name,value.asBool());
                    break;
                }
                case pxl::Variant::TYPE_CHAR:
                {
                    storeVariable<char>(name,value.asChar());
                    break;
                }
                case pxl::Variant::TYPE_DOUBLE:
                {
                    storeVariable<float>(name,value.asDouble());
                    break;
                }
                case pxl::Variant::TYPE_FLOAT:
                {
                    storeVariable<float>(name,value.asFloat());
                    break;
                }
                case pxl::Variant::TYPE_INT16:
                {
                    storeVariable<int>(name,value.asInt16());
                    break;
                }
                case pxl::Variant::TYPE_INT32:
                {
                    storeVariable<int>(name,value.asInt32());
                    break;
                }
                case pxl::Variant::TYPE_INT64:
                {
                    storeVariable<int>(name,value.asInt64());
                    break;
                }
                case pxl::Variant::TYPE_LORENTZVECTOR:
                {
                    const pxl::LorentzVector& vec = value.asLorentzVector();
                    storeVariable<float>(name+"_E",vec.getE());
                    storeVariable<float>(name+"_Px",vec.getPx());
                    storeVariable<float>(name+"_Py",vec.getPy());
                    storeVariable<float>(name+"_Pz",vec.getPz());
                    storeVariable<float>(name+"_Mass",vec.getMass());
                    break;
                }
                case pxl::Variant::TYPE_NONE:
                {
                    break;
                }
                case pxl::Variant::TYPE_SERIALIZABLE:
                {
                    break;
                }
                case pxl::Variant::TYPE_STRING:
                {
                    break;
                }
                case pxl::Variant::TYPE_UCHAR:
                {
                    storeVariable<int>(name,value.asUChar());
                    break;
                }
                case pxl::Variant::TYPE_UINT16:
                {
                    storeVariable<int>(name,value.asUInt16());
                    break;
                }
                case pxl::Variant::TYPE_UINT32:
                {
                    storeVariable<int>(name,value.asUInt32());
                    break;
                }
                case pxl::Variant::TYPE_UINT64:
                {
                    storeVariable<int>(name,value.asUInt64());
                    break;
                }
                case pxl::Variant::TYPE_VECTOR:
                {
                    const std::vector<pxl::Variant>& vec = value.asVector();
                    for (unsigned int i = 0; i < vec.size(); ++i)
                    {
                        storeVariable(name+"_"+std::to_string(i),vec[i]);
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        template<class TYPE>
        VariableTmpl<TYPE>* bookVariable(const std::string& name)
        {
            VariableTmpl<TYPE>* var = new VariableTmpl<TYPE>();
            _variables[name]=var;
            return var;
        }
        
        Variable* findVariable(const std::string& name)
        {
            std::unordered_map<std::string,Variable*>::iterator it = _variables.find(name); 
            if (it!=_variables.end())
            {
                return it->second;
            }
            return nullptr;
        }
        
        void fill() {}
        void write() {}
};

class TMVAEvaluation:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        
        std::vector<std::string> _files;
        std::vector<std::string> _names;
        std::vector<std::string> _selections;
        
        std::string _suffix;
        
        std::vector<float> _variableValues;
        std::vector<std::string> _methodNames;
        
        SyntaxTree<CacheTree> _syntaxTree;
        
        CacheTree _cacheTree;
        
        TMVA::Reader* _reader;
        
        
    public:
        TMVAEvaluation():
            Module()
        {
            addSink("input", "input");
            _outputSource = addSource("output","output");
            
            addOption("weights","",_files, pxl::OptionDescription::USAGE_FILE_OPEN);
            addOption("names","variables names (same order as in training)",_names);
            addOption("variables","for parsing the variables from the event",_selections);
            addOption("suffix","",_suffix);
        }

        ~TMVAEvaluation()
        {
            delete _reader;
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("TMVAEvaluation");
            return type;
        }

        // static and dynamic methods are needed
        const std::string &getType() const
        {
            return getStaticType();
        }

        bool isRunnable() const
        {
            // this module does not provide events, so return false
            return false;
        }

        void initialize() throw (std::runtime_error)
        {
        }

        void beginJob() throw (std::runtime_error)
        {
            getOption("weights",_files);
            
            getOption("names",_names);
            _variableValues.resize(_names.size());
            
            _methodNames.resize(_files.size());
            
            _reader = new TMVA::Reader("");
            for (unsigned int ivar = 0; ivar < _names.size(); ++ivar)
            {
                _reader->AddVariable(_names[ivar],&_variableValues[ivar]);
            }
            
            for (unsigned int imethod = 0; imethod < _files.size(); ++imethod)
            {
                const int pos = _files[imethod].find_first_of('_',_files[imethod].find_last_of('/')+1)+1; //skip factory name
                const int end = _files[imethod].find_first_of('.',pos+1);
                std::string methodName = std::string(_files[imethod],pos,end-pos);
                _methodNames[imethod]=methodName;
                _reader->BookMVA(methodName,_files[imethod]);
            }
            
            
            getOption("variables",_selections);
            
            for (const std::string& s: _selections)
            {
                _syntaxTree.buildTree(s);
            }
            
            getOption("suffix",_suffix);
        }

        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event*>(sink->get());
                _syntaxTree.evaluate(event,&_cacheTree);
                
                pxl::EventView* outputEventView = nullptr;
                
                std::vector<pxl::EventView*> eventViews;
                event->getObjectsOfType(eventViews);
                for (unsigned int iev = 0; iev < eventViews.size(); ++iev)
                {
                    if (eventViews[iev]->getName()=="Reconstructed")
                    {
                        outputEventView=eventViews[iev];
                        break;
                    }
                }
                
                //_cacheTree.print();
                
                for (unsigned int ivar = 0; ivar < _names.size(); ++ivar)
                {
                    Variable* var = _cacheTree.findVariable(_names[ivar]);
                    if (var)
                    {
                        _variableValues[ivar]=var->toFloat();
                    }
                    else
                    {
                        throw std::runtime_error("variable '"+_names[ivar]+"' was not parsed!");
                    }
                }
                
                if (outputEventView)
                {
                    for (unsigned int imethod = 0; imethod < _methodNames.size(); ++imethod)
                    {
                        outputEventView->setUserRecord(_methodNames[imethod]+_suffix,(float)_reader->EvaluateMVA(_methodNames[imethod])); 
                    }
                }
                
                
                _cacheTree.resetVariables();
                
                _outputSource->setTargets(event);
                return _outputSource->processTargets();
            }
            catch(std::exception &e)
            {
                throw std::runtime_error(getName()+": "+e.what());
            }
            catch(...)
            {
                throw std::runtime_error(getName()+": unknown exception");
            }

            logger(pxl::LOG_LEVEL_ERROR , "Analysed event is not an pxl::Event !");
            return false;
        }

        void shutdown() throw(std::runtime_error)
        {
        }

        void destroy() throw (std::runtime_error)
        {
            delete this;
        }
};

PXL_MODULE_INIT(TMVAEvaluation)
PXL_PLUGIN_INIT
