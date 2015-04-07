#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include "OutputStore.hpp"

#include <vector>
#include <string>
#include <iostream>

static pxl::Logger logger("RootTreeWriter");


class SyntaxTree
{
    private:
        SyntaxTree* _parent;
        
        std::map<std::string,SyntaxTree*> _children;
        
        const std::string _field;
    public:
        SyntaxTree(const std::string& field="", SyntaxTree* parent=nullptr):
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
            for (std::map<std::string,SyntaxTree*>::const_iterator it = _children.cbegin(); it != _children.cend(); ++it)
            {
                it->second->print(ident+1);
            }
        }
        
        void buildTree(const std::string& s)
        {
            std::string::size_type pos = s.find(".");
            std::string childField = "";
            if (pos!=std::string::npos)
            {
                childField = s.substr(0,pos);
            }
            else
            {
                childField=s;
            }
            auto it = _children.find(childField);
            if (it==_children.end())
            {
                SyntaxTree* parser = new SyntaxTree(childField,this);
                _children[childField]=parser;
            }
            if (pos!=std::string::npos)
            {
                _children[childField]->buildTree(s.substr(pos+1));
            }
        }
        
        template<class TYPE>
        void evaluateChildren(TYPE* obj, Tree* tree, const std::string& prefix)
        {
            for (std::pair<std::string,SyntaxTree*> child: _children)
            {
                child.second->evaluate(obj,tree, prefix+"__"+_field);
            }
        }
        
        void evaluate(pxl::Event* event, Tree* tree, const std::string& prefix)
        {
            std::vector<pxl::EventView*> eventViews;
            event->getObjectsOfType(eventViews);
            unsigned int multiplicity = 1;
            for (pxl::EventView* eventView: eventViews)
            {
                if (eventView->getName()==_field)
                {
                    evaluateChildren(eventView,tree,prefix+"_"+std::to_string(multiplicity));
                    ++multiplicity;
                }
            }
        }
        
        void evaluate(pxl::EventView* eventView, Tree* tree, const std::string& prefix)
        {
            std::vector<pxl::Particle*> particles;
            eventView->getObjectsOfType(particles);
            unsigned int multiplicity = 1;
            for (pxl::Particle* particle: particles)
            {
                if (particle->getName()==_field)
                {
                    evaluateChildren(particle,tree,prefix+"_"+std::to_string(multiplicity));
                    ++multiplicity;
                }
            }
        }
        
        void evaluate(pxl::Particle* particle, Tree* tree, const std::string& prefix)
        {
            if (_field=="pt")
            {
                tree->storeVariable(prefix+"__pt",particle->getPt());
            }
            
        }
        
};

class RootTreeWriter:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        
        std::string _outputFileName;
        
        OutputStore* _store;
        
        std::vector<std::string> _selections;
        
        SyntaxTree* _syntaxTree;
        
    public:
        RootTreeWriter():
            Module(),
            _store(nullptr),
            _syntaxTree(nullptr)
        {
            addSink("input", "input");
            _outputSource = addSource("output", "output");
            
            addOption("root file","",_outputFileName,pxl::OptionDescription::USAGE_FILE_SAVE);
            
            addOption("variables","",_selections);
        }

        ~RootTreeWriter()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("RootTreeWriter");
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
            getOption("root file",_outputFileName);
            _store = new OutputStore(_outputFileName);
            _syntaxTree = new SyntaxTree();
            
            getOption("variables",_selections);
            for (const std::string& selection: _selections)
            {
                _syntaxTree->buildTree(selection);
            }
            _syntaxTree->print();
        }

        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event *> (sink->get());
                if (event)
                {
                    Tree* tree = _store->getTree(event->getUserRecord("Process"));

                    //tree->storeVariable("event_number",(int)event->getUserRecord("Event number").asUInt64());
                    _syntaxTree->evaluateChildren(event,tree,"");
                    tree->fill();
                    
                     _outputSource->setTargets(event);
                    return _outputSource->processTargets();
                }
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
        
        void endJob()
        {
            if (_store)
            {
                _store->close();
                delete _store;
            }
        }

        void shutdown() throw(std::runtime_error)
        {
        }

        void destroy() throw (std::runtime_error)
        {
            delete this;
        }
};

PXL_MODULE_INIT(RootTreeWriter)
PXL_PLUGIN_INIT
