#ifndef SEMANT_H
#define SEMANT_H
#include "AstTree.h"
#include <memory>


namespace xcool {
   
    //the inheritance graph
    class InherGraph {
        public:
            //the index of class saved in this node
            std::unique_ptr<xcool::ast::Class> node;
            std::shared_ptr<InherGraph> parent;
            std::vector<std::shared_ptr<InherGraph>> son_list;
    };
    
    // the semant checker
    class SemantChecker {
        public:
            std::shared_ptr<InherGraph> buildInherGraph(xcool::ast::Program &);
            void checkCyclic(std::shared_ptr<InherGraph> root);
            void printGraph(std::shared_ptr<InherGraph> root);
    };
}







#endif