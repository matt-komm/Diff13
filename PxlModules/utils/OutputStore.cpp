#include "OutputStore.hpp"

Tree::Tree(TFile* file, std::string name):
    _file(file),
    _count(0),
    _logger("Tree")
{
    _tree = new TTree(name.c_str(),name.c_str());
    _tree->SetDirectory(file);
}


void Tree::fill()
{
    ++_count;
    _tree->Fill();
}

void Tree::write()
{
    _tree->Write();
}

OutputStore::OutputStore(std::string filename):
    _logger("OutputStore")
{
    _file = new TFile(filename.c_str(),"RECREATE");
}

Tree* OutputStore::getTree(std::string treeName)
{
    std::unordered_map<std::string,Tree*>::iterator elem = _treeMap.find(treeName.c_str());
    if (elem==_treeMap.end())
    {
        _logger(pxl::LOG_LEVEL_INFO,"create new tree: ",treeName);
        _treeMap[treeName]=new Tree(_file, treeName);
        return _treeMap[treeName];
    } else {
        return elem->second;
    }
}

void OutputStore::close()
{
    _file->cd();
    for (auto it = _treeMap.begin(); it != _treeMap.end(); ++it )
    {
        it->second->write();
    }
    _file->Close();
}



