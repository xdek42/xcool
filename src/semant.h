#ifndef SEMANT_H
#define SEMANT_H
#include "AstTree.h"
#include <memory>

namespace xcool {
   class SemantChecker;
    //the inheritance graph
    class InherGraph {
        friend SemantChecker;
        public:
            static std::shared_ptr<InherGraph> find_node(std::shared_ptr<InherGraph>, std::string);
            static void print_graph(std::shared_ptr<InherGraph> root);
        private:
            //the index of class saved in this node
            std::unique_ptr<xcool::ast::Class> node;
            std::shared_ptr<InherGraph> parent;
            std::vector<std::shared_ptr<InherGraph>> son_list;
    };
    
    // the semant checker
    class SemantChecker {
        public:
            void install_base_class(std::shared_ptr<InherGraph>);
            std::shared_ptr<InherGraph> build_graph(xcool::ast::Program &);
            void checkCyclic(std::shared_ptr<InherGraph> root);
            void semant_check(std::shared_ptr<InherGraph>);

    };
}







#endif