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
#include <map>
#include <functional>
#include <algorithm>

static pxl::Logger logger("RootTreeWriter");


class SyntaxNode
{
    private:
        SyntaxNode* _parent;
        std::vector<SyntaxNode*> _children;
        const std::string _field;
    public:

        SyntaxNode(const std::string& field="", SyntaxNode* parent=nullptr):
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
            SyntaxNode* foundChild = nullptr;
            for (SyntaxNode* child: _children)
            {
                if (child->getField()==childField)
                {
                    foundChild=child;
                    break;
                }
            }

            if (!foundChild)
            {
                foundChild = new SyntaxNode(childField,this);
                _children.push_back(foundChild);
            }
            if (pos!=std::string::npos)
            {
                foundChild->buildTree(s.substr(pos+2));
            }
        }
        
        template<class TYPE>
        void evaluateChildren(const TYPE* object, Tree* tree, const std::string& prefix)
        {
            for (SyntaxNode* child: _children)
            {
                child->evaluate(object,tree,prefix);
            }
        }
        
        void evaluate(const pxl::Event* event, Tree* tree, const std::string& prefix)
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
        
        void evaluate(const pxl::EventView* eventView, Tree* tree, const std::string& prefix)
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

        void evaluate(const pxl::Particle* particle, Tree* tree, const std::string& prefix)
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
                {"Pz",[](const pxl::Particle* particle){ return particle->getPz();}}
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

        void parseUserRecords(const pxl::UserRecords* ur, Tree* tree, const std::string& prefix)
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


class SyntaxTree
{
    public:
        std::vector<SyntaxNode*> _children;
    public:

        SyntaxTree()
        {
        }
        
        void evaluate(pxl::Event* event, Tree* tree)
        {
            for (SyntaxNode* child: _children)
            {
                child->evaluate(event,tree,"");
            }
        }

        inline void print() const
        {
            for (SyntaxNode* child: _children)
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
            SyntaxNode* foundChild = nullptr;
            for (SyntaxNode* child: _children)
            {
                if (child->getField()==childField)
                {
                    foundChild=child;
                    break;
                }
            }

            if (!foundChild)
            {
                foundChild = new SyntaxNode(childField);
                _children.push_back(foundChild);
            }
            if (pos!=std::string::npos)
            {
                foundChild->buildTree(s.substr(pos+2));
            }
        }
};

/*
#include <boost/spirit/include/qi.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/spirit/home/phoenix/object/construct.hpp>

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template<typename Iterator>
class AccessorGrammar: 
    public qi::grammar<Iterator, AccessorTree(), ascii::space_type>
{
    public:
        AccessorGrammar():
            AccessorGrammar::base_type(accessorTree)
        {
            identifier =
                +(qi::char_("a-zA-Z0-9\\*")[qi::_val+=qi::_1]);
            fields = 
                identifier[phoenix::push_back(qi::_val,qi::_1)] >> *(qi::lit(",") >> identifier[phoenix::push_back(qi::_val,qi::_1)]);
            
            accessor = 
                (qi::lit("EventView") >> qi::lit("(") >> fields[qi::_val=phoenix::construct<Accessor>(Accessor::EVENTVIEW,qi::_1)] >> qi::lit(")")) |
                (qi::lit("Particle") >> qi::lit("(") >> fields[qi::_val=phoenix::construct<Accessor>(Accessor::PARTICLE,qi::_1)] >> qi::lit(")")) |
                (qi::lit("UserRecord") >> qi::lit("(") >> fields[qi::_val=phoenix::construct<Accessor>(Accessor::USERRECORD,qi::_1)] >> qi::lit(")")) |
                (qi::lit("Kinematic") >> qi::lit("(") >> fields[qi::_val=phoenix::construct<Accessor>(Accessor::KINEMATIC,qi::_1)] >> qi::lit(")"));
            accessorTree = accessor[qi::_val+=qi::_1] >> *(qi::lit("->") >> accessor[qi::_val+=qi::_1]);
        }

        qi::rule<Iterator, std::string(), ascii::space_type> identifier;
        qi::rule<Iterator, std::vector<std::string>(), ascii::space_type> fields;
        qi::rule<Iterator, Accessor(), ascii::space_type> accessor;
        qi::rule<Iterator, AccessorTree(), ascii::space_type> accessorTree;

};
*/



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
            for (const std::string& s: _selections)
            {
                _syntaxTree->buildTree(s);
            }
            //_syntaxTree->print();
        }

        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event *> (sink->get());
                if (event)
                {
                    Tree* tree = _store->getTree(event->getUserRecord("ProcessName"));
                    //tree->storeVariable("event_number",(int)event->getUserRecord("Event number").asUInt64());
                    _syntaxTree->evaluate(event,tree);
                    /*
                    std::vector<pxl::EventView*> eventViews;
                    event->getObjectsOfType(eventViews);
                    for (unsigned int iev = 0; iev < eventViews.size(); ++iev)
                    {
                        if (eventViews[iev]->getName()!="Reconstructed")
                        {
                            continue;
                        }
                        std::vector<pxl::Particle*> particles;
                        eventViews[iev]->getObjectsOfType(particles);
                        unsigned int njet = 1;
                        for (unsigned int iparticle = 0; iparticle < particles.size(); ++iparticle)
                        {
                            if (particles[iparticle]->getName()!="SelectedJet")
                            {
                                continue;
                            }
                            tree->storeVariable("jet_pt_"+std::to_string(njet),particles[iparticle]->getPt());
                            ++njet;
                        }
                    }
                    */
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
