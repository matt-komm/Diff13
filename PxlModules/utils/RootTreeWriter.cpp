#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include <boost/spirit/include/qi.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/spirit/home/phoenix/object/construct.hpp>

#include "OutputStore.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

static pxl::Logger logger("RootTreeWriter");

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

class Accessor
{
    public:
        enum Type {EVENTVIEW,PARTICLE,USERRECORD,KINEMATIC};

        Type _type;
        std::vector<std::string> _fields;

        Accessor():
            _type(EVENTVIEW)
        {
        }
        
        Accessor(const Accessor& accessor) = default;

        Accessor(Type type, std::vector<std::string> fields):
            _type(type),
            _fields(fields)
        {
        }
    
        template<class TYPE>
        void process(const TYPE& object)
        {
        }
        
        void process(const pxl::Event& event)
        {
        }
        
        std::string toString() const
        {
            std::stringstream ss;
            ss<<"(";
            switch (_type)
            {
                case EVENTVIEW:
                    ss<<"eventview";
                    break;
                    
                case PARTICLE:
                    ss<<"particle";
                    break;
                    
                case USERRECORD:
                    ss<<"userrecord";
                    break;
            }
            ss<<",[";
            for (auto s: _fields)
            {
                ss<<s<<",";
            }
            ss<<"])";
            return ss.str();
        }
        
        virtual ~Accessor()
        {
        }
};


class AccessorChain
{
    private:
        std::vector<Accessor> _chain;
    public:
        AccessorChain()
        {
        }
        
        AccessorChain& operator+=(const Accessor& accessor)
        {
            _chain.push_back(accessor);
        }
        
        template<class TYPE>
        void access(const TYPE& type, OutputStore& store, std::string prefix)
        {
            
        } 
        
        std::string toString() const
        {
            std::stringstream ss;
            for (unsigned int i = 0; i < _chain.size(); ++i)
            {
                ss<<"->"<<_chain[i].toString();
            }
            return ss.str();
        }
};

template<typename Iterator>
class AccessorGrammar: 
    public qi::grammar<Iterator, AccessorChain(), ascii::space_type>
{
    public:
        AccessorGrammar():
            AccessorGrammar::base_type(accessorChain)
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
            accessorChain = accessor[qi::_val+=qi::_1] >> *(qi::lit("->") >> accessor[qi::_val+=qi::_1]);
            
        }

        qi::rule<Iterator, std::string(), ascii::space_type> identifier;
        qi::rule<Iterator, std::vector<std::string>(), ascii::space_type> fields;
        qi::rule<Iterator, Accessor(), ascii::space_type> accessor;
        qi::rule<Iterator, AccessorChain(), ascii::space_type> accessorChain;
};


class RootTreeWriter:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        
        std::string _outputFileName;
        
        OutputStore* _store;
        
        std::vector<std::string> _selections;
        
        
    public:
        RootTreeWriter():
            Module(),
            _store(nullptr)
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
            getOption("variables",_selections);

            AccessorGrammar<std::string::const_iterator> grammar;
            AccessorChain result;
            
            for (const std::string& selection: _selections)
            {
                //std::cout<<selection<<std::endl;
                std::string::const_iterator iter = selection.begin();
                std::string::const_iterator end = selection.end();
                bool success = qi::phrase_parse(iter, end, grammar, ascii::space,result);
                
                if (!success or iter!=end)
                {
                    std::cout<<"ERROR: "<<std::string(selection.begin(),iter)<<" ^^^ "<<std::string(iter,selection.end())<<std::endl;
                }
                std::cout<<result.toString()<<std::endl;
            }
        }

        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event *> (sink->get());
                if (event)
                {
                    Tree* tree = _store->getTree(event->getUserRecord("ProcessName"));

                    tree->storeVariable("event_number",(int)event->getUserRecord("Event number").asUInt64());
                    //_syntaxTree->evaluateChildren(event,tree,"");
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
