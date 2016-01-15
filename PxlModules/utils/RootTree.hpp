#ifndef __ROOTTREE_H__
#define __ROOTTREE_H__

#include <pxl/core.hh>

#include <TTree.h>
#include <TFile.h>
#include <TObject.h>
#include <TBranch.h>

#include <unordered_map>
#include <string>
#include <iostream>

#include "Variable.hpp"

class RootTree
{
    private:
        unsigned int _count;
        std::unordered_map<std::string,Variable*> _variables;
        TTree* _tree;
        TFile* _file;
        pxl::Logger _logger;
    public:
        RootTree(TFile* file, std::string name);
        
        template<class TYPE>
        void storeVariable(const std::string& name, const TYPE& value)
        {
            std::unordered_map<std::string,Variable*>::iterator elem = _variables.find(name.c_str());
            if (elem==_variables.end()) {
                _logger(pxl::LOG_LEVEL_DEBUG ,"store new variable '",name,"' in tree '",_tree->GetName(),"' with ",_count," empty entries");
                VariableTmpl<TYPE>* var = bookVariable<TYPE>(name);
                var->setValue(value);
            } else {
                (*elem->second)=value;
            }
        }
        
        void resetVariables();
        
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
            TBranch* branch = _tree->Branch(name.c_str(),(TYPE*)var->getAddress());
            _logger(pxl::LOG_LEVEL_DEBUG ,"fill new variable '",name,"' in tree '",_tree->GetName(),"' with ",_count," empty entries");
            for (unsigned int cnt=0;cnt<_count; ++cnt)
            {
                branch->Fill();
            }
            return var;
        }
        
        void fill();
        void write();
};

#endif

