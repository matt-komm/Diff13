#ifndef __PXLPARSER_H__
#define __PXLPARSER_H__

#include "pxl/hep.hh"
#include "pxl/core.hh"

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <functional>
#include <algorithm>



template<class TREE>
class SyntaxNode
{
    private:
        SyntaxNode<TREE>* _parent;
        std::vector<SyntaxNode<TREE>*> _children;
        const std::string _field;
    public:

        SyntaxNode(const std::string& field="", SyntaxNode<TREE>* parent=nullptr):
            _field(field),
            _parent(parent)
        {
        }
        
        inline const std::string& getField() const
        {
            return _field;
        }
        
        inline void print(unsigned int ident=0) const
        {
            for (unsigned int i = 0; i < ident; ++i)
            {
                std::cout<<" - ";
            }
            std::cout<<_field<<std::endl;
            for (SyntaxNode* child: _children)
            {
                child->print(ident+1);
            }
        }
        
        void buildTree(const std::string& s)
        {
            std::string::size_type pos = s.find("->");
            std::string childField = "";
            if (pos!=std::string::npos)
            {
                childField = s.substr(0,pos);
            }
            else
            {
                childField=s;
            }
            SyntaxNode<TREE>* foundChild = nullptr;
            for (SyntaxNode<TREE>* child: _children)
            {
                if (child->getField()==childField)
                {
                    foundChild=child;
                    break;
                }
            }

            if (!foundChild)
            {
                foundChild = new SyntaxNode<TREE>(childField,this);
                _children.push_back(foundChild);
            }
            if (pos!=std::string::npos)
            {
                foundChild->buildTree(s.substr(pos+2));
            }
        }
        
        template<class TYPE>
        void evaluateChildren(const TYPE* object, TREE* tree, const std::string& prefix)
        {
            for (SyntaxNode* child: _children)
            {
                child->evaluate(object,tree,prefix);
            }
        }
        
        void evaluate(const pxl::Event* event, TREE* tree, const std::string& prefix)
        {
            std::vector<pxl::EventView*> eventViews;
            event->getObjectsOfType(eventViews);
            unsigned int multiplicity = 1;
            for (pxl::EventView* eventView: eventViews)
            {
                if (eventView->getName()==_field)
                {
                    evaluateChildren(eventView,tree,prefix+getField()+"_"+std::to_string(multiplicity)+"__");
                    ++multiplicity;
                }
            }
            parseUserRecords(&event->getUserRecords(),tree,prefix);

        }
        
        void evaluate(const pxl::EventView* eventView, TREE* tree, const std::string& prefix)
        {
            std::vector<pxl::Particle*> particles;
            eventView->getObjectsOfType(particles);
            unsigned int multiplicity = 1;
            for (pxl::Particle* particle: particles)
            {
                if (particle->getName()==_field)
                {
                    evaluateChildren(particle,tree,prefix+getField()+"_"+std::to_string(multiplicity)+"__");
                    ++multiplicity;
                }
            }
            parseUserRecords(&eventView->getUserRecords(),tree,prefix);
        }

        void evaluate(const pxl::Particle* particle, TREE* tree, const std::string& prefix)
        {
            const static std::map<std::string,std::function<float(const pxl::Particle* particle)>> fct = {
                {"Pt",[](const pxl::Particle* particle){ return particle->getPt();}},
                {"Eta",[](const pxl::Particle* particle){ return particle->getEta();}},
                {"Phi",[](const pxl::Particle* particle){ return particle->getPhi();}},
                {"E",[](const pxl::Particle* particle){ return particle->getE();}},
                {"P",[](const pxl::Particle* particle){ return particle->getP();}},
                {"Mass",[](const pxl::Particle* particle){ return particle->getMass();}},
                {"Px",[](const pxl::Particle* particle){ return particle->getPx();}},
                {"Py",[](const pxl::Particle* particle){ return particle->getPy();}},
                {"Pz",[](const pxl::Particle* particle){ return particle->getPz();}},
                {"Charge",[](const pxl::Particle* particle){ return particle->getCharge();}}
            };
            if (getField()=="ALL" or getField()=="KIN")
            {
                for (auto it: fct)
                {
                    tree->storeVariable(prefix+it.first,it.second(particle));
                }
            }
            else
            {
                auto it = fct.find(getField());
                if (it!=fct.end())
                {
                    tree->storeVariable(prefix+getField(),it->second(particle));
                }
            }

            parseUserRecords(&particle->getUserRecords(),tree,prefix);
        }

        void parseUserRecords(const pxl::UserRecords* ur, TREE* tree, const std::string& prefix)
        {
            if (getField()=="ALL" or getField()=="UR")
            {
                for (auto it: *ur->getContainer())
                {
                    std::string urName=it.first;
                    std::replace(urName.begin(), urName.end(), ' ', '_');
                    std::replace(urName.begin(), urName.end(), ':', '_');
                    tree->storeVariable(prefix+urName,it.second);
                }
            }
        }
};

template<class TREE>
class SyntaxTree
{
    public:
        std::vector<SyntaxNode<TREE>*> _children;
    public:

        SyntaxTree()
        {
        }
        
        void evaluate(pxl::Event* event, TREE* tree)
        {
            for (SyntaxNode<TREE>* child: _children)
            {
                child->evaluate(event,tree,"");
            }
        }

        inline void print() const
        {
            for (SyntaxNode<TREE>* child: _children)
            {
                child->print();
            }
        }
       

        void buildTree(const std::string& s)
        {
            std::string::size_type pos = s.find("->");
            std::string childField = "";
            if (pos!=std::string::npos)
            {
                childField = s.substr(0,pos);
            }
            else
            {
                childField=s;
            }
            SyntaxNode<TREE>* foundChild = nullptr;
            for (SyntaxNode<TREE>* child: _children)
            {
                if (child->getField()==childField)
                {
                    foundChild=child;
                    break;
                }
            }

            if (!foundChild)
            {
                foundChild = new SyntaxNode<TREE>(childField);
                _children.push_back(foundChild);
            }
            if (pos!=std::string::npos)
            {
                foundChild->buildTree(s.substr(pos+2));
            }
        }
};

#endif
